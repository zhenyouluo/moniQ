#ifndef NETWORKDISCOVERER_H
#define NETWORKDISCOVERER_H

#include <QObject>

#include "ipv4_address.h"
#include "pingthreadcontrol.h"

#define MAX_CONCURRENT_PINGS 20
#define PING_CHECK_INTERVAL 1000 // in msec

class NetworkDiscoverer : public QObject
{
  Q_OBJECT

private:
  quint32 endIp;
  quint32 curr_ip;
  PingThreadControl* pingThreadControls[MAX_CONCURRENT_PINGS];

public:
  explicit NetworkDiscoverer(QObject *parent = 0);
  void pingIpv4Range(Ipv4_Address* start_ip, Ipv4_Address* end_ip);

signals:

public slots:
    void getPingResult();
};

#endif // NETWORKDISCOVERER_H
