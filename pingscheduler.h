#ifndef PINGSCHEDULER_H
#define PINGSCHEDULER_H

#include <QObject>
#include <QQueue>

#include "pingthreadcontrol.h"

#define MAX_CONCURRENT_PINGS 20

class PingScheduler : public QObject
{
  Q_OBJECT
public:
  explicit PingScheduler(QObject *parent = 0);
  int getFreeThread();
  void schedulePing(QString ip_address, bool monitoring);
  void processPingResult(QString ipAddress, int result, bool monitoring);


private:
  QQueue<QString> discover_queue;
  QQueue<QString> monitor_queue;
  PingThreadControl pingthreadcontrols[MAX_CONCURRENT_PINGS];

signals:
  void sendPingResult(QString ip_address, int result);

public slots:
  void connectPingers();
};

#endif // PINGSCHEDULER_H
