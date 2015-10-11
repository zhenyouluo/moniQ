#ifndef COMMANDSERVER_H
#define COMMANDSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class CommandServer : public QObject
{
    Q_OBJECT
public:
    explicit CommandServer(QObject *parent = Q_NULLPTR);
    virtual ~CommandServer();
    void start();
private Q_SLOTS:
    void onNewConnection();
    void processMessage(QString message);
    void socketDisconnected();
    QString respondToCommand(QString command, QStringList arguments);

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
};

#endif //COMMANDSERVER_H
