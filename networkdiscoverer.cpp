#include <QMetaType>
#include <QCoreApplication>
#include <QDebug>
#include <QThreadPool>

#include "networkdiscoverer.h"
#include "ipv4_address.h"
#include "pinger.h"
#include "objectinstances.h"

NetworkDiscoverer::NetworkDiscoverer(QObject *parent) : QObject(parent)
{
  QCoreApplication::addLibraryPath("C:\\Users\\Aise\\Documents\\build-moniQ-msvc2013_64-Debug\\pingwin\\debug");
  qRegisterMetaType<QString>("QString&");
}

void NetworkDiscoverer::pingIpv4Range(Ipv4_Address* start_ip, Ipv4_Address* end_ip)
{
  startIp = start_ip->toInt();
  endIp = end_ip->toInt();

  curr_ip = startIp;

  // ping is blocking, so use mutiple threads
  QThreadPool::globalInstance()->setMaxThreadCount(MAX_NETWORKDISCOVERER_THREADS);
  while ((QThreadPool::globalInstance()->activeThreadCount() < QThreadPool::globalInstance()->maxThreadCount()) &&
         (curr_ip <= endIp))
  {
    Pinger* pinger = new Pinger(Ipv4_Address(curr_ip).toString());
    QObject::connect(pinger, &Pinger::sendPingResult, this, &NetworkDiscoverer::processPingResult);
    QThreadPool::globalInstance()->start(pinger);
    curr_ip++;
  }
}

 void NetworkDiscoverer::processPingResult(QString ip_address, int result)
{
  if (result < 0)
  {
    // loading pinger plugin failed, try for same ip address again
    Pinger* pinger = new Pinger(ip_address);
    QObject::connect(pinger, &Pinger::sendPingResult, this, &NetworkDiscoverer::processPingResult);
    QThreadPool::globalInstance()->start(pinger);
  }
  if (result == 0)
  {
    // add to database
    ObjectInstances::database.addHost(ip_address);
    qDebug() << ip_address << ":" << result;
  }
  else
  {
    // log result
    qDebug() << ip_address << ":" << result;
  }
  if (curr_ip <= endIp)
  {
    Pinger* pinger = new Pinger(Ipv4_Address(curr_ip).toString());
    QObject::connect(pinger, &Pinger::sendPingResult, this, &NetworkDiscoverer::processPingResult);
    QThreadPool::globalInstance()->start(pinger);
    curr_ip++;
  }
}
