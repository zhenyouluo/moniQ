#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include <QtCore/QDebug>

#include "commandserver.h"
#include "objectinstances.h"
#include "ipv4_address.h"

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
    connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
            this, &CommandServer::onNewConnection);
  }
}

CommandServer::~CommandServer()
{
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
              pClient->sendTextMessage("ERROR: Invalid argument lenght.");
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
      QString response = respondToCommand(command, arguments);
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

QString CommandServer::respondToCommand(QString command, QStringList arguments)
{
  if (command == "GET_DATABASE_USER")
  {
    QString database_user = QString(ObjectInstances::databaseCredentials.value("database_user", "").toByteArray());
    if (database_user == "")
    {
      return "DATABASE_USER_NOT_SETUP";
    }
    return QString("DATABASE_USER:").append(database_user);
  }
  if (command == "GET_DATABASE_PASSWORD")
  {
    QString database_password = QString(ObjectInstances::databaseCredentials.value("database_password", "").toByteArray());
    if (database_password == "")
    {
      return "DATABASE_PASSWORD_NOT_SETUP";
    }
    return QString("DATABASE_PASSWORD:").append(database_password);
  }
  if (command == "SET_DATABASE_USER")
  {
    ObjectInstances::databaseCredentials.setValue("database_user", arguments[0]);

    return "";
  }
  if (command == "SET_DATABASE_PASSWORD")
  {
    ObjectInstances::databaseCredentials.setValue("database_password", arguments[0]);

    return "";
  }
  if (command == "NETWORKDISCOVER")
  {
    if (arguments.length() < 2)
    {
      return "ERROR: Please supply starting and ending IP address.";
    }
    Ipv4_Address* start_address = new Ipv4_Address(arguments[0]);
    if (!start_address->isValid())
    {
      return "ERROR: Starting IP address not valid.";
    }
    Ipv4_Address* end_address = new Ipv4_Address(arguments[1]);
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
    ObjectInstances::networkDiscoverer.pingIpv4Range(start_address, end_address);

    return "";
  }
  return "UNKNOWN COMMAND";
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

