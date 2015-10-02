#include "tcpscanner.h"
#include <QDebug>
#include <QTcpSocket>
#include <QAbstractSocket>

TcpScanner::TcpScanner(QObject *parent) : QObject(parent)
{
  tcpSocket = new QTcpSocket(this);
  connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
              this, SLOT(displayError(QAbstractSocket::SocketError)));
  tcpSocket->connectToHost("192.168.1.141", 1);
  qDebug() << tcpSocket->errorString();
}

void TcpScanner::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
      qDebug() << "RemoteHostClosedError";
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "HostNotFoundError";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "ConnectionRefusedError";
        break;
    default:
        qDebug() << tcpSocket->errorString();
    }


}
