#ifndef PINGER_H
#define PINGER_H

#include <QString>
#include <QObject>
#include <QPluginLoader>

#include "ipv4_address.h"
#include "pingerinterface.h"

class Pinger : public QObject
{
  Q_OBJECT

private:
  Ipv4_Address* ipAddress;
  int threadIndex;
  QPluginLoader* pluginLoader;
  QObject* pingerPlugin;
  PingerInterface* pingerInterface;

public:
  Pinger();

public slots:
  void ping(QString ip_address);

signals:
   void sendPingResult(int result);
};

#endif // PINGER_H
