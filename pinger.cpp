#include <QDebug>
#include <QPluginLoader>
#include <QThread>

#include "pinger.h"
#include "pingerinterface.h"

Pinger::Pinger(Ipv4_Address ip_address)
{
  ipAddress = ip_address;
}

void Pinger::run()
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
  int result = pingerInterface->ping(ipAddress.toString().toLatin1().data());
  if (result != 0)
  {
    emit sendPingResult(result);
    return;
  }

  QThread::sleep(2);
  int result = pingerInterface->get_ping_result();
  emit sendPingResult(result);
}

void stopEventLoop()
{
  QThread::currentThread()->exit();
}
