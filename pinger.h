#ifndef PINGER_H
#define PINGER_H

#include <QString>
#include <QRunnable>
#include <QObject>

class Pinger : public QObject, public QRunnable
{
  Q_OBJECT

private:
  QString ipAddress;

public:
  Pinger(QString ip_address);
  void run();

signals:
   void sendPingResult(QString ip_address, int result);
};

#endif // PINGER_H
