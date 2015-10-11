#ifndef NETWORKDISCOVERER_H
#define NETWORKDISCOVERER_H

#include <QObject>

#include "ipv4_address.h"

#define MAX_NETWORKDISCOVERER_THREADS 10

class NetworkDiscoverer : public QObject
{
  Q_OBJECT

private:
  quint32 startIp;
  quint32 endIp;
  quint32 curr_ip;

public:
  explicit NetworkDiscoverer(QObject *parent = 0);
  void pingIpv4Range(Ipv4_Address* start_ip, Ipv4_Address* end_ip);

signals:

public slots:
    void processPingResult(QString ip_address, int result);
};

#endif // NETWORKDISCOVERER_H
