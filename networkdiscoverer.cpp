#include <QDebug>
#include <QThreadPool>
#include <QHostInfo>


#include "networkdiscoverer.h"
#include "ipv4_address.h"
#include "objectinstances.h"

NetworkDiscoverer::NetworkDiscoverer(QWebSocket* pclient, QObject *parent) : QObject(parent)
{
  pClient = pclient;
  QObject::connect(&ObjectInstances::processController, &ProcessController::sendPingResult, this, &NetworkDiscoverer::processPingResult);
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
    ObjectInstances::processController.messageScheduler("PING:" + Ipv4_Address(curr_ip).toString());
    curr_ip++;
  }
}

// socket
void NetworkDiscoverer::processPingResult(QString ip_address, int result)
{
  if (waitingForAnswer.remove(ip_address))
  {
    qDebug() << "removed";
    // ip adress removed, so it was on our waiting list

    if (result == 0)
    {
      // host responded, try to find hostname
      QHostInfo::lookupHost(ip_address, this, SLOT(publishDiscoveredHost(QHostInfo)));
    }
    else
    {
      if (waitingForAnswer.size() == 0)
      {
        this->disconnect();
        this->deleteLater();
      }
    }
    int nr_of_addresses = 1 + endIp - startIp;
    float progress = (nr_of_addresses - waitingForAnswer.size()) / (float) nr_of_addresses;
    pClient->sendTextMessage("DISCOVERPROGRESS:"+QString::number(progress));
  }
  // log result
  qDebug() << ip_address << ":" << result;
}

void NetworkDiscoverer::publishDiscoveredHost(QHostInfo hostinfo)
{
  QString hostname;
  if (hostinfo.error() == QHostInfo::NoError)
  {
    hostname = hostinfo.hostName();
  }
  else
  {
    hostname = hostinfo.addresses().at(0).toIPv4Address();
  }
  pClient->sendTextMessage("DISCOVERED IPV4:" + hostname + ";" + hostinfo.addresses().at(0).toString());
  if (waitingForAnswer.size() == 0)
  {
    this->disconnect();
    this->deleteLater();
  }
}
