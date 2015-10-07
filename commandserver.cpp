#include "commandserver.h"
#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include <QtCore/QDebug>

QT_USE_NAMESPACE

CommandServer::CommandServer(QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(Q_NULLPTR),
    m_clients()
{
}

void CommandServer::start()
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Chat Server"),
                                              QWebSocketServer::NonSecureMode,
                                              this);
    if (m_pWebSocketServer->listen(QHostAddress::Any, 6901))
    {
        qDebug() << "Chat Server listening on port" << 6901;
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
  QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());
  Q_FOREACH (QWebSocket *pClient, m_clients)
  {
    if (pClient == pSender) // only reply to the sender
    {
      int pos = message.indexOf(",");
      QString command;
      if (pos == -1)
      {
        command = message;
      }
      else
      {
        command = message.left(pos);
        QStringList arguments;
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
        pClient->sendTextMessage(command);
        qDebug() << arguments;
      }
    }
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

