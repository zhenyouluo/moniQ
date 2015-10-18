#ifndef PINGER_H
#define PINGER_H

#include <QString>
#include <QRunnable>
#include <QObject>
#include <QPluginLoader>

#include "ipv4_address.h"
#include "pingerinterface.h"

class Pinger : public QObject, public QRunnable
{
  Q_OBJECT

private:
  Ipv4_Address ipAddress;
  QPluginLoader* pluginLoader;
  QObject* pingerPlugin;
  PingerInterface* pingerInterface;

public:
  Pinger(Ipv4_Address ip_address);
  void run();

public slots:
  void stopEventLoop();

signals:
   void sendPingResult(int result);
};

#endif // PINGER_H
