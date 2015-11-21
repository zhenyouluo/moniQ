#ifndef SCHEDULING_H
#define SCHEDULING_H

#include <QObject>
#include <QMultiMap>
#include <QHash>

class Scheduling : public QObject
{
  Q_OBJECT

private:
  QMultiMap<quint64, QString> pingSchedule;
  QHash<QString, int> hostsUpCheckIntervals;
  QHash<QString, int> hostsDownCheckIntervals;

public:
  explicit Scheduling(QObject *parent = 0);
  void start();
  void addHostToSchedule(QString ipv4, bool up);

signals:

public slots:
  void processStdin(QString string);
  void scheduleNextSeconds();
  void startPing();
};

#endif // SCHEDULING_H
