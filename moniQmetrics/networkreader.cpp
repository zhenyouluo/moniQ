#include <QDebug>
#include <QCoreApplication>

#include "networkreader.h"

NetworkReader::NetworkReader(QObject *parent) : QObject(parent)
{
  udpSocket = new QUdpSocket(this);
  if (udpSocket->bind(QHostAddress::Any, 6900))
  {
    qDebug() << "bound";
  }
  else
  {
    qDebug() << "not bound";
    QCoreApplication::exit(1);
  }
  QObject::connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

void NetworkReader::readPendingDatagrams()
{
  qDebug() << "incoming";
  while (udpSocket->hasPendingDatagrams())
  {
    QByteArray datagram;
    datagram.resize(udpSocket->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;

    udpSocket->readDatagram(datagram.data(), datagram.size(),
                            &sender, &senderPort);
    qDebug() << datagram;
    udpSocket->writeDatagram(QByteArray(1, 1), sender, senderPort);
  }
}


