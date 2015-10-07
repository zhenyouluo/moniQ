#include <QMetaType>
#include <QCoreApplication>
#include <QDebug>
#include <QThreadPool>

#include "networkdiscoverer.h"

#include "pinger.h"

NetworkDiscoverer::NetworkDiscoverer(QObject *parent) : QObject(parent)
{
  QCoreApplication::addLibraryPath("C:\\Users\\Aise\\Documents\\build-moniQ-msvc2013_64-Debug\\debug");
  qRegisterMetaType<QString>("QString&");
}

bool NetworkDiscoverer::pingIPs()
{
  // ping is blocking, so use mutiple threads
  QThreadPool::globalInstance()->setMaxThreadCount(MAX_NETWORKDISCOVERER_THREADS);
  while (QThreadPool::globalInstance()->activeThreadCount() < QThreadPool::globalInstance()->maxThreadCount())
  {
    Pinger* pinger = new Pinger("192.168");
    QObject::connect(pinger, &Pinger::sendPingResult, this, &NetworkDiscoverer::processPingResult);
    QThreadPool::globalInstance()->start(pinger);
  }
  return true;
}

 void NetworkDiscoverer::processPingResult(QString ip_address, int result)
{
   Pinger* pinger = new Pinger("192.168");
   QObject::connect(pinger, &Pinger::sendPingResult, this, &NetworkDiscoverer::processPingResult);
   QThreadPool::globalInstance()->start(pinger);
}
