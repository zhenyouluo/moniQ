#ifndef PINGTHREADCONTROL_H
#define PINGTHREADCONTROL_H

#include <QObject>
#include <QPluginLoader>
#include <QTimer>

#include "pingerinterface.h"
#include "pinger.h"
#include "ipv4_address.h"

class PingThreadControl : public QObject
{
  Q_OBJECT

private:
  int threadIndex;
  Ipv4_Address ipAddress;
  int failureCounter;
  QTimer *timer;
  Pinger* pinger;

public:
  explicit PingThreadControl(QObject *parent = 0);

signals:
  void killPinger();

public slots:
  void timeOut();
  void processPingResult(int result);
};

#endif // PINGTHREADCONTROL_H
