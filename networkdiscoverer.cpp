#include <QMetaType>
#include <QCoreApplication>
#include <QDebug>
#include <QThreadPool>


#include "networkdiscoverer.h"
#include "ipv4_address.h"
#include "pinger.h"
#include "objectinstances.h"
#include "pingerinterface.h"

NetworkDiscoverer::NetworkDiscoverer(QObject *parent) : QObject(parent)
{
  QCoreApplication::addLibraryPath("C:\\Users\\Aise\\Documents\\build-moniQ-msvc2013_64-Debug\\pingwin\\debug");
}

void NetworkDiscoverer::pingIpv4Range(Ipv4_Address* start_ip, Ipv4_Address* end_ip)
{
  QThreadPool::setMaxThreadCount(MAX_CONCURRENT_PINGS);

  for (int i = 0; i < MAX_CONCURRENT_PINGS; i++)
  {
    pingThreadControls[i] = new PingThreadControl();
  }

  curr_ip = start_ip->toInt();
  endIp = end_ip->toInt();

  // start pinging
  int index = 0;
  while ((index < MAX_CONCURRENT_PINGS) && (curr_ip <= endIp))
  {
    pingThreadControls[index]->startPing(Ipv4_Address(curr_ip), index);
    curr_ip++;
    index++;
  }
}

void NetworkDiscoverer::processPingResult(Ipv4_Address ip_address, int result, int index)
{
  if (result == 0)
  {
    // add to database
    ObjectInstances::database.addHost(ip_address.toString());
    qDebug() << ip_address.toString() << ":" << result;
  }
  else
  {
    // log result
    qDebug() << ip_address.toString() << ":" << result;
  }

  if (curr_ip <= endIp)
  {
    pingThreadControls[index]->startPing(Ipv4_Address(curr_ip), index);
    curr_ip++;
  }
  else
  {
    pingThreadControls[index]->deleteLater();
  }
}
