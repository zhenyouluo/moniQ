#include <QDebug>
#include <QPluginLoader>

#include "pinger.h"
#include "pingerinterface.h"

Pinger::Pinger(QString ip_address)
{
  ipAddress = ip_address;
}

void Pinger::run()
{
  //qDebug() << ipAddress;
  QPluginLoader pl("pingwin");
  QObject *qp = pl.instance();
  if (!qp)
  {
    qDebug() << "Could not instantiate pinger plugin";
    emit sendPingResult(ipAddress, -1);
    return;
  }
  PingerInterface* pi = qobject_cast<PingerInterface *>(qp);
  if (!pi)
  {
    qDebug() << "Could not type cast pinger plugin";
    emit sendPingResult(ipAddress, -2);
    return;
  }
  int result = pi->ping(ipAddress.toLatin1().data());
  emit sendPingResult(ipAddress, result);
}
