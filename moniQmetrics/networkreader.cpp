#include <QDebug>
#include <QCoreApplication>
#include <QDateTime>

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
    if (datagram.size() == 0)
    {
      QByteArray bytes;
      bytes.append(1);
      bytes.append((char) 0);
      bytes.append(100);
      bytes.append(2);
      bytes.append((char) 0);
      bytes.append(100);
      bytes.append(3);
      bytes.append((char) 0);
      bytes.append(100);
      bytes.append(4);
      bytes.append((char) 0);
      bytes.append(100);
      udpSocket->writeDatagram(bytes, sender, senderPort);
    }
    else
    {
      int i = 0;
      while (i < datagram.size())
      {
        // get time stamp (8 bytes)
        if (i+8 < datagram.size())
        {
          quint64 ts = 0;
          for (int j = 0; j < 8; j++)
          {
            qDebug() << datagram.at(i+j) << " " << (int) datagram.at(i+j) << " " << (unsigned char) datagram.at(i+j);

            ts = (ts * 256) + (unsigned char) datagram.at(i+j);
            qDebug() << "ts: " << ts;
          }
          QDateTime date = QDateTime::fromMSecsSinceEpoch(ts * 1000);
          qDebug() << ts;
          qDebug() << date.toString("dd.MM.yyyy	hh:mm:ss");
          i = i + 8;
          if (i+1 < datagram.size())
          {
            // first byte determines data type
            switch (datagram.at(i))
            {
              case 1:
              {
                qDebug() << "Disks";
                i++;
                int nr_of_bytes = processDiskData(datagram, i);
                i = i + nr_of_bytes;
                break;
              }
              case 2:
              {
                qDebug() << "Cpu";
                i++;
                if (i+2 < datagram.size())
                {
                  int perc1000 = (unsigned char) datagram.at(i) * 256 + (unsigned char) datagram.at(i+1);
                  qDebug() << perc1000;
                  i = i + 2;
                }
                else
                {
                  i = datagram.size();
                }
                break;
              }
              case 3:
              {
                qDebug() << "Mem";
                i++;
                if (i+2 < datagram.size())
                {
                  int perc1000 = (unsigned char) datagram.at(i) * 256 + (unsigned char) datagram.at(i+1);
                  qDebug() << perc1000;
                  i = i + 2;
                }
                else
                {
                  i = datagram.size();
                }
                break;
              }
            }
          }
          else
          {
            i = datagram.size();
          }
        }
        else
        {
          i = datagram.size();
        }
      }
    }
  }
}

int NetworkReader::processDiskData(QByteArray datagram, int offset)
{
  int i = offset;
  if (i < datagram.size())
  {
    if (i < datagram.size())
    {
      int namesize = datagram.at(i);
      i++;
      if (i+namesize < datagram.size())
      {
        qDebug() << datagram.mid(i, namesize);
        i=i+namesize;
        if (i+2 < datagram.size())
        {
          int perc1000 = (unsigned char) datagram.at(i) * 256 + (unsigned char) datagram.at(i+1);
          qDebug() << perc1000;
          i = i + 2;
        }
      }
    }
  }
  return i-offset;
}
