#include <QDebug>
#include <QThreadPool>
#include <float.h>


#include "networkdiscoverer.h"
#include "ipv4_address.h"
#include "pinger.h"
#include "objectinstances.h"
#include "pingerinterface.h"

NetworkDiscoverer::NetworkDiscoverer(QWebSocket* pclient, QObject *parent) : QObject(parent)
{
  pClient = pclient;
  QObject::connect(&ObjectInstances::pingScheduler, &PingScheduler::sendPingResult, this, &NetworkDiscoverer::processPingResult);
}

void NetworkDiscoverer::pingIpv4Range(Ipv4_Address* start_ip, Ipv4_Address* end_ip)
{

  endIp = end_ip->toInt();
  startIp = start_ip->toInt();
  quint32 curr_ip = start_ip->toInt();

  // start pinging
  while (curr_ip <= endIp)
  {
    waitingForAnswer.insert(Ipv4_Address(curr_ip).toString());
    ObjectInstances::pingScheduler.schedulePing(Ipv4_Address(curr_ip).toString(), false);
    curr_ip++;
  }
}

// socket
void NetworkDiscoverer::processPingResult(QString ip_address, int result)
{
  if (result == 0)
  {
    pClient->sendTextMessage("DISCOVERED IPV4:"+ip_address);
  }
  waitingForAnswer.remove(ip_address);
  int nr_of_addresses = 1 + endIp - startIp;
  float progress = (nr_of_addresses - waitingForAnswer.size()) / (float) nr_of_addresses;
  pClient->sendTextMessage("DISCOVERPROGRESS:"+QString::number(progress));
  if (waitingForAnswer.size() == 0)
  {
    this->disconnect();
    this->deleteLater();
  }
  // log result
  qDebug() << ip_address << ":" << result;
}
