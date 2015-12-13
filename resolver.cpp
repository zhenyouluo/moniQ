

#include "resolver.h"

Resolver::Resolver(QObject *parent) : QObject(parent)
{

}

void Resolver::host2Ip(QString host, QWebSocket *pclient)
{
  toHost = false;
  pClient = pclient;
  QHostInfo::lookupHost(host, this, SLOT(processHostInfo(QHostInfo)));
}

void Resolver::ip2Host(QString ipv4, QWebSocket *pclient)
{
  toHost = true;
  pClient = pclient;
  QHostInfo::lookupHost(ipv4, this, SLOT(processHostInfo(QHostInfo)));
}

void Resolver::processHostInfo(QHostInfo hostinfo)
{
  QString hostname;
  QString ipv4;
  if (toHost)
  {
    if (hostinfo.error() == QHostInfo::NoError)
    {
      hostname = hostinfo.hostName();
    }
    else
    {
      hostname = hostinfo.addresses().at(0).toString();
    }
    pClient->sendTextMessage("RESOLVED HOST:" + hostinfo.addresses().at(0).toString() + ";" + hostname);
  }
  else
  {
    if (hostinfo.error() == QHostInfo::NoError)
    {
      ipv4 = hostinfo.addresses().at(0).toString();
    }
    else
    {
      ipv4 = "?";
    }
    pClient->sendTextMessage("RESOLVED IP:" + ipv4 + ";" + hostinfo.hostName());
  }
  this->disconnect();
  this->deleteLater();
}
