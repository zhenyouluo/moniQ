#ifndef RESOLVER_H
#define RESOLVER_H

#include <QObject>
#include <QWebSocket>
#include <QHostInfo>

class Resolver : public QObject
{
  Q_OBJECT
public:
  explicit Resolver(QObject *parent = 0);
  void host2Ip(QString host, QWebSocket *pclient);
  void ip2Host(QString ipv4, QWebSocket *pclient);

private:
  QWebSocket *pClient;
  bool toHost;

signals:

public slots:
  void processHostInfo(QHostInfo hostinfo);
};

#endif // RESOLVER_H
