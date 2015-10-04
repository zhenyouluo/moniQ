#ifndef NETWORKDISCOVERER_H
#define NETWORKDISCOVERER_H

#include <QObject>

#define MAX_NETWORKDISCOVERER_THREADS 10

class NetworkDiscoverer : public QObject
{
  Q_OBJECT
public:
  explicit NetworkDiscoverer(QObject *parent = 0);

signals:

public slots:
    void processPingResult(QString ip_address, int result);
};

#endif // NETWORKDISCOVERER_H
