#ifndef TCPSCANNER_H
#define TCPSCANNER_H

#include <QObject>
#include <QAbstractSocket>
#include <QTcpSocket>

class TcpScanner : public QObject
{
  Q_OBJECT
private:
  QTcpSocket* tcpSocket;

public:
  explicit TcpScanner(QObject *parent = 0);

signals:

private slots:

    void displayError(QAbstractSocket::SocketError socketError);
};

#endif // TCPSCANNER_H
