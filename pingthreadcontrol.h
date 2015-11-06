#ifndef PINGTHREADCONTROL_H
#define PINGTHREADCONTROL_H

#include <QObject>
#include <QPluginLoader>
#include <QTimer>
#include <QThread>

#include "pingerinterface.h"
#include "pinger.h"
#include "ipv4_address.h"

class PingThreadControl : public QObject
{
  Q_OBJECT

private:
  bool thread_available;
  bool monitoringPing;
  QString ipAddress;
  QThread thread;
  Pinger* pinger;


  int failureCounter;

public:
  explicit PingThreadControl(QObject *parent = 0);
  void startPing(QString ip_address, bool monitoring);
  bool isAvailable();
  void connectPinger();

signals:
  void initPing(QString ip_address);

public slots:
  void processPingResult(int result);

};

#endif // PINGTHREADCONTROL_H
