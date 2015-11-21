#include <QDebug>
#include <QPluginLoader>
#include <QThread>

#include "pinger.h"
#include "pingerinterface.h"

Pinger::Pinger()
{
  pingerPlugin = 0;
}

void Pinger::ping(QString ip_address)
{
  if (!pingerPlugin)
  {
    pluginLoader = new QPluginLoader("pingwin");
    pingerPlugin = pluginLoader->instance();
    if (!pingerPlugin)
    {
      qDebug() << "Could not instantiate pinger plugin";
      emit sendPingResult(-1);
      return;
    }
    pingerInterface = qobject_cast<PingerInterface *>(pingerPlugin);
    if (!pingerInterface)
    {
      qDebug() << "Could not type cast pinger plugin";
      emit sendPingResult(-2);
      return;
    }
    if (!pingerInterface->isValid())
    {
      emit sendPingResult(-3);
      return;
    }
  }
  qDebug() << "before ping"<< ip_address.toLatin1().data();
  int result = pingerInterface->ping(ip_address.toLatin1().data());
  emit sendPingResult(result);
}
