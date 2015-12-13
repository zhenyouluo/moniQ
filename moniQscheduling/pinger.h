#ifndef PINGER_H
#define PINGER_H

#include <QString>
#include <QObject>
#include <QPluginLoader>

#define ICMP_PACKET_SIZE 32

class Pinger : public QObject
{
  Q_OBJECT

private:

public:

public slots:
  void ping(QString ip_address);

signals:
   void sendPingResult(int result);
};

#endif // PINGER_H
