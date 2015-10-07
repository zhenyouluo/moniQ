#include <QUdpSocket>
#include <QDebug>
#include <QCoreApplication>
#include "networkreader.h"
#include "ipv4_address.h"

NetworkReader::NetworkReader(QObject *parent) : QObject(parent)
{
  udpSocket = new QUdpSocket(this);
  if (udpSocket->bind(QHostAddress::Any, 6901))
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
    if (datagram.size() < 2)
    {
      udpSocket->writeDatagram(QByteArray(1, 1), sender, senderPort);  // response 1: command to short
      qDebug() << "command to short";
      return;
    }
    quint16 command_id = datagram[0] * 256 + datagram[1];

    // parse arguments
    int pos = 2;
    QStringList arguments;
    while (pos < datagram.size())
    {
      quint8 arglen = datagram[pos];
      QByteArray arg = datagram.mid(pos+1, arglen);
      qDebug() << arglen << arg;
      if (arg.size() < arglen)
      {
        udpSocket->writeDatagram(QByteArray(1, 2), sender, senderPort);  // response 2: argument not fully read
        qDebug() << "argument not fully read";
        return;
      }
      arguments.append(arg);
      pos = pos + arglen + 1;
    }

    switch (command_id)
    {
      case 1: // ipv4 discoverer: first argument is starting ip address, second argument is ending ip address
      {
        if (arguments.length() < 2)
        {
          udpSocket->writeDatagram(QByteArray(1, 3), sender, senderPort);  // response 3: not enough arguments
          return;
        }
        Ipv4_Address start_address(arguments[0]);
        if (!start_address.isValid())
        {
          udpSocket->writeDatagram(QByteArray(1, 11), sender, senderPort);  // response 11: starting address not valid
          return;
        }
        Ipv4_Address end_address(arguments[1]);
        if (!end_address.isValid())
        {
          udpSocket->writeDatagram(QByteArray(1, 12), sender, senderPort);  // response 12: ending address not valid
          return;
        }
        quint32 start_ip_int = start_address.toInt();
        quint32 end_ip_int = end_address.toInt();
        if (start_ip_int > end_ip_int)
        {
          udpSocket->writeDatagram(QByteArray(1, 13), sender, senderPort);  // response 13: starting address not before ending address
          return;
        }
      }
    }

  }
}


