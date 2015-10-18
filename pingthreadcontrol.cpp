#include <QThreadPool>
#include <QTime>

#include "pingthreadcontrol.h"
#include "pinger.h"
#include "objectinstances.h"

PingThreadControl::PingThreadControl(QObject *parent) : QObject(parent)
{
  timer = new Timer();
  timer->setSingleShot(true);
  timer->setInterval(20000);
}

void PingThreadControl::startPing(Ipv4_Address ip_address, int index)
{
  failureCounter = 0;
  threadIndex = index;
  ipAddress = ip_address;

  pinger = new Pinger(ipAddress);
  pinger->setAutoDelete(-1);

  connect(timer, SIGNAL(timeout()), this, PingThreadControl::timeOut();
  connect(pinger, Pinger::sendPingResult, this, PingThreadControl::processPingResult);
  connect(this, PingThreadControl::killPinger, pinger, Pinger::stopEventLoop);

  QThreadPool::globalInstance()->start(pinger);
  timer->start();
}

void PingThreadControl::timeOut()
{
  if (failureCounter > 4)
  {
    ObjectInstances::networkDiscoverer.processPingResult(ipAddress, -1, threadIndex);
    return;
  }
  failureCounter++;
  emit killPinger();

  pinger = new Pinger(ipAddress);
  pinger->setAutoDelete(-1);

  connect(timer, SIGNAL(timeout()), this, PingThreadControl::timeOut();
  connect(pinger, Pinger::sendPingResult, this, PingThreadControl::processPingResult);
  connect(this, PingThreadControl::killPinger, pinger, Pinger::stopEventLoop);

  QThreadPool::globalInstance()->start(pinger);
  timer->start();
}

void PingThreadControl::processPingResult(int result)
{
  if ((result == 0) || (failureCounter > 4))
  {
    ObjectInstances::networkDiscoverer.processPingResult(ipAddress, result, threadIndex);
    return;
  }
  failureCounter++;
  timer->stop();
  emit killPinger();

  pinger = new Pinger(ipAddress);
  pinger->setAutoDelete(-1);

  connect(timer, SIGNAL(timeout()), this, PingThreadControl::timeOut();
  connect(pinger, Pinger::sendPingResult, this, PingThreadControl::processPingResult);
  connect(this, PingThreadControl::killPinger, pinger, Pinger::stopEventLoop);

  QThreadPool::globalInstance()->start(pinger);
  timer->start();
}
