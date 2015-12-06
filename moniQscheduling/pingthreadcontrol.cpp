#include <QThread>
#include <QDebug>

#include "pingthreadcontrol.h"
#include "pinger.h"
#include "objectinstances2.h"

PingThreadControl::PingThreadControl(QObject *parent) : QObject(parent)
{
  thread_available = true;
  pinger = new Pinger();
  pinger->moveToThread(&thread);
}

void PingThreadControl::connectPinger()
{
  connect(this, &PingThreadControl::initPing, pinger, &Pinger::ping);
  connect(pinger, &Pinger::sendPingResult, this, &PingThreadControl::processPingResult);
  thread.start();
}

void PingThreadControl::startPing(QString ip_address, bool monitoring, bool pingnow)
{
  ipAddress = ip_address;
  monitoringPing = monitoring;
  pingNow = pingnow;
  failureCounter = 0;
  thread_available = false;
  emit initPing(ip_address);
}

void PingThreadControl::processPingResult(int result)
{
  if ((result == 0) || (failureCounter > 4))
  {
    thread_available = true;
    ObjectInstances2::pingScheduler.processPingResult(ipAddress, result, monitoringPing, pingNow);
    return;
  }
  failureCounter++;
  emit initPing(ipAddress);
}

bool PingThreadControl::isAvailable()
{
  return thread_available;
}
