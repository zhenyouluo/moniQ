#ifndef NETWORKREADER_H
#define NETWORKREADER_H

#include <QObject>
#include <QUdpSocket>

class NetworkReader : public QObject
{
  Q_OBJECT
public:
  explicit NetworkReader(QObject *parent = 0);
  int processDiskData(QByteArray datagram, int offset);

signals:

private:
  QUdpSocket* udpSocket;

public slots:
  void readPendingDatagrams();
};

#endif // NETWORKREADER_H
