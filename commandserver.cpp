#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include <QtCore/QDebug>
#include <QCoreApplication>

#include "commandserver.h"
#include "objectinstances.h"
#include "ipv4_address.h"
#include "resolver.h"

QT_USE_NAMESPACE

CommandServer::CommandServer(QObject *parent) :
  QObject(parent),
  m_pWebSocketServer(Q_NULLPTR),
  m_clients()
{
}

void CommandServer::start()
{
  m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Command Server"),
                                            QWebSocketServer::NonSecureMode,
                                            this);
  if (m_pWebSocketServer->listen(QHostAddress::Any, 6901))
  {
    qDebug() << "Command Server listening on port 6901";
    connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &CommandServer::onNewConnection);
  }
}

CommandServer::~CommandServer()
{
  qDebug() << "Command Server stopped listening on port 6901";
  m_pWebSocketServer->close();
  qDeleteAll(m_clients.begin(), m_clients.end());
}

void CommandServer::onNewConnection()
{
  qDebug() << "new connection";
  QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

  connect(pSocket, &QWebSocket::textMessageReceived, this, &CommandServer::processMessage);
  connect(pSocket, &QWebSocket::disconnected, this, &CommandServer::socketDisconnected);

  m_clients << pSocket;
}

void CommandServer::processMessage(QString message)
{
  qDebug() << message;
  QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());
  Q_FOREACH (QWebSocket *pClient, m_clients)
  {
    if (pClient == pSender) // only reply to the sender
    {
      int pos = message.indexOf(",");
      QString command;
      QStringList arguments;
      if (pos == -1)
      {
        command = message;
      }
      else
      {
        command = message.left(pos);
        while (pos+1 < message.size())
        {
          int pos2 = message.indexOf(",", pos+1);
          if (pos2 != -1)
          {
            pos = pos + 1;
            QString arglenstr = message.mid(pos, pos2-pos);
            bool ok;
            int arglen = arglenstr.toInt(&ok);
            if (!ok)
            {
              pClient->sendTextMessage("ERROR: Could not convert argument to number.");
              return;
            }
            if (arglen < 1 || arglen > 500)
            {
              pClient->sendTextMessage("ERROR: Invalid argument length.");
              return;
            }
            QString arg = message.mid(pos2+1, arglen);
            if (arg.length() != arglen)
            {
              pClient->sendTextMessage("ERROR: Argument shorter then expected.");
              return;
            }
            arguments.append(arg);
            pos = pos2 + arglen + 1;
            if (pos < message.size() && message[pos] != ',')
            {
              pClient->sendTextMessage("ERROR: Argument longer then expected.");
              return;
            }
          }
          else
          {
            pos = message.size(); // stop searching for arguments
          }
        }
      }
      //
      // process commands which are allowed without being logged on to moniQ
      //
      if (command == "ARE_DATABASE_CREDENTIALS_SETUP")
      {
        QString database_user = QString(ObjectInstances::databaseCredentials.value("database_user", "").toByteArray());
        if (database_user != "")
        {
          pClient->sendTextMessage("YES");
        }
        else
        {
          pClient->sendTextMessage("NO");
        }
        return;
      }
      if (command == "ARE_MONIQ_CREDENTIALS_SETUP")
      {
        if (ObjectInstances::database.moniqCredentialsSet())
        {
          pClient->sendTextMessage("YES");
        }
        else
        {
          pClient->sendTextMessage("NO");
        }
        return;
      }
      if (command == "SET_DATABASE_CREDENTIALS")
      {
        QString database_user = QString(ObjectInstances::databaseCredentials.value("database_user", "").toByteArray());
        if (database_user != "")
        {
          pClient->sendTextMessage("ERROR: Database credentials already setup.");
          return;
        }
        if (arguments.count() < 2)
        {
          pClient->sendTextMessage("ERROR: Expecting 2 arguments: database user and password.");
          return;
        }
        ObjectInstances::databaseCredentials.setValue("database_user", arguments[0]);
        ObjectInstances::databaseCredentials.setValue("database_password", arguments[1]);
        ObjectInstances::database.start(arguments[0], arguments[1]);
        pClient->sendTextMessage("OK");
      }
      if (command == "LOGON")
      {
        if (ObjectInstances::database.moniqCredentialsSet())
        {
          if (arguments.count() < 3)
          {
            pClient->sendTextMessage("ERROR: Expecting 3 arguments: session id, user and password.");
            return;
          }
          if (!ObjectInstances::database.checkCredentials(arguments[1], arguments[2]))
          {
            pClient->sendTextMessage("ERROR: Wrong username or password.");
            return;
          }
          else
          {
            sessions << arguments[0];
            pClient->sendTextMessage("OK");
          }
        }
        else
        {
          if (arguments.count() < 1)
          {
            pClient->sendTextMessage("ERROR: Expecting 1 argument: session id.");
            return;
          }
          sessions << arguments[0];
        }
        return;
      }

      //
      // All other commands require being logged on: first argument must be valid session id
      //
      if (arguments.count() < 1)
      {
        pClient->sendTextMessage("ERROR: Expecting 1 argument: session id.");
        return;
      }
      if (!sessions.contains(arguments[0]))
      {
        pClient->sendTextMessage("ERROR: No valid session id.");
        return;
      }


      QString response = respondToCommand(pClient, command, arguments);
      if (response != "")
      {
        pClient->sendTextMessage(response);
      }
      qDebug() << arguments;
    }
    else
    {
      qDebug() << "other sender";
    }
  }
}

QString CommandServer::respondToCommand(QWebSocket *pClient, QString command, QStringList arguments)
{
  if (command == "CHECKSESSION")
  {
    return "";
  }
  if (command == "GET_DATABASE_CREDENTIALS")
  {
    return "DATABASE_CREDENTIALS:" +
           QString(ObjectInstances::databaseCredentials.value("database_user", "").toByteArray()) +
           ";" +
           QString(ObjectInstances::databaseCredentials.value("database_password", "").toByteArray());
  }
  if (command == "NETWORKDISCOVER")
  {
    if (arguments.length() < 3)
    {
      return "ERROR: Please supply starting and ending IP address.";
    }
    Ipv4_Address* start_address = new Ipv4_Address(arguments[1]);
    if (!start_address->isValid())
    {
      return "ERROR: Starting IP address not valid.";
    }
    Ipv4_Address* end_address = new Ipv4_Address(arguments[2]);
    if (!end_address->isValid())
    {
      return "ERROR: Ending IP address not valid.";
    }
    quint32 start_ip_int = start_address->toInt();
    quint32 end_ip_int = end_address->toInt();
    if (start_ip_int > end_ip_int)
    {
      return "ERROR: Starting IP address must be smaller then ending IP address.";
    }
    NetworkDiscoverer* networkDiscoverer = new NetworkDiscoverer(pClient, this);
    networkDiscoverer->pingIpv4Range(start_address, end_address);

    return "OK";
  }
  if (command == "ADDHOST")
  {
    if (arguments.length() < 3)
    {
      return "ERROR: Please supply hostname and IP address.";
    }
    Ipv4_Address* ip_address = new Ipv4_Address(arguments[2]);
    if (!ip_address->isValid())
    {
      return "ERROR: IP address not valid.";
    }
    ObjectInstances::database.addHost(arguments[1], arguments[2]);
    ObjectInstances::processController.messageScheduler("ADDHOST:" + arguments[1] + ";" + arguments[2]);
    ObjectInstances::commandServer.broadCast("HOSTLIST UPDATED");
    return "";
  }
  if (command == "EDITHOST")
  {
    if (arguments.length() < 4)
    {
      return "ERROR: Please supply hostname and IP address.";
    }
    Ipv4_Address* ip_address = new Ipv4_Address(arguments[2]);
    if (!ip_address->isValid())
    {
      return "ERROR: IP address not valid.";
    }
    ObjectInstances::database.updateHost(arguments[1], arguments[2], arguments[3]);
    ObjectInstances::processController.messageScheduler("HOSTUPDATED");
    ObjectInstances::commandServer.broadCast("HOSTLIST UPDATED");
    return "HOST UPDATED";
  }
  if (command == "ADDFULLHOST")
  {
    if (arguments.length() < 4)
    {
      return "ERROR: Please supply hostname, IP address and host template.";
    }
    Ipv4_Address* ip_address = new Ipv4_Address(arguments[2]);
    if (!ip_address->isValid())
    {
      return "ERROR: IP address not valid.";
    }
    ObjectInstances::database.addFullHost(arguments[1], arguments[2], arguments[3]);
    ObjectInstances::processController.messageScheduler("ADDHOST:" + arguments[1] + ";" + arguments[2]);
    ObjectInstances::commandServer.broadCast("HOSTLIST UPDATED");
    return "HOST ADDED";
  }
  if (command == "DELETEHOST")
  {
    if (arguments.length() < 2)
    {
      return "ERROR: Please supply hostname.";
    }
    ObjectInstances::database.deleteHost(arguments[1]);
    ObjectInstances::processController.messageScheduler("HOSTDELETED");
    ObjectInstances::commandServer.broadCast("HOSTLIST UPDATED");
    return "";
  }
  if (command == "DELETEHOSTTEMPLATE")
  {
    if (arguments.length() < 2)
    {
      return "ERROR: Please supply template name.";
    }
    if (ObjectInstances::database.deleteHostTemplate(arguments[1]))
    {
      ObjectInstances::commandServer.broadCast("HOSTTEMPLATELIST UPDATED");
      return "";
    }
    return "ERROR: Template " + arguments[1] + " is still in use.";
  }

  if (command == "ADDHOSTTEMPLATE")
  {
    if (arguments.length() < 6)
    {
      return "ERROR: Please supply template name, 2 intervals and 2 levels.";
    }
    if (ObjectInstances::database.addHostTemplate(arguments[1], arguments[2], arguments[3], arguments[4], arguments[5]))
    {
      ObjectInstances::commandServer.broadCast("TEMPLATELIST UPDATED");
      return "TEMPLATE ADDED";
    }
    return "ERROR: Template " + arguments[1] + " already exists.";
  }
  if (command == "EDITHOSTTEMPLATE")
  {
    if (arguments.length() < 6)
    {
      return "ERROR: Please supply template name, 2 intervals and 2 levels.";
    }
    ObjectInstances::database.editHostTemplate(arguments[1], arguments[2], arguments[3], arguments[4], arguments[5]);
    ObjectInstances::processController.messageScheduler("TEMPLATE UPDATED");
    ObjectInstances::commandServer.broadCast("TEMPLATELIST UPDATED");
    return "TEMPLATE UPDATED";
  }
  if (command == "HOST2IP")
  {
    if (arguments.length() < 2)
    {
      return "ERROR: Please supply hostname.";
    }
    Resolver* resolver = new Resolver(this);
    resolver->host2Ip(arguments[1], pClient);
    return "";
  }
  if (command == "IP2HOST")
  {
    if (arguments.length() < 2)
    {
      return "ERROR: Please supply ip address.";
    }
    Resolver* resolver = new Resolver(this);
    resolver->ip2Host(arguments[1], pClient);
    return "";
  }
  if (command == "PINGNOW")
  {
    if (arguments.length() < 2)
    {
      return "ERROR: Please supply IP address.";
    }
    Ipv4_Address* ip_address = new Ipv4_Address(arguments[1]);
    if (!ip_address->isValid())
    {
      return "ERROR: IP address not valid.";
    }
    ObjectInstances::processController.messageScheduler("PINGNOW:"+arguments[1]);
    return "";
  }
  if (command == "RESTART")
  {
    QCoreApplication::exit(-1);
    return "moniQ restarting";
  }
  return "UNKNOWN COMMAND";
}

void CommandServer::broadCast(QString message)
{
  Q_FOREACH (QWebSocket* pClient, m_clients)
  {
    pClient->sendTextMessage(message);
  }
}

void CommandServer::socketDisconnected()
{
  QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
  if (pClient)
  {
    m_clients.removeAll(pClient);
    pClient->deleteLater();
  }
}

