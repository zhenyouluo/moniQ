#ifndef NETWORKDISCOVERER_H
#define NETWORKDISCOVERER_H

#include <QObject>
#include <QWebSocket>

#include "ipv4_address.h"
#include "pingthreadcontrol.h"
#include "pingscheduler.h"

#define PING_CHECK_INTERVAL 1000 // in msec

class NetworkDiscoverer : public QObject
{
  Q_OBJECT

private:
  QWebSocket* pClient;
  PingThreadControl* pingThreadControls[MAX_CONCURRENT_PINGS];
  QSet<QString> waitingForAnswer;
  quint32 endIp;
  quint32 startIp;

public:
  explicit NetworkDiscoverer(QWebSocket* pclient, QObject *parent = 0);
  void pingIpv4Range(Ipv4_Address* start_ip, Ipv4_Address* end_ip);

signals:

public slots:
  void processPingResult(QString ip_address, int result);
};

#endif // NETWORKDISCOVERER_H
