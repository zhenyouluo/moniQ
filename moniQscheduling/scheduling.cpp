#include <QObject>
#include <QTextStream>
#include <QDateTime>
#include <QTimer>

#include "scheduling.h"
#include "objectinstances2.h"
#include "stdinthread.h"
#include "../database.h"

Scheduling::Scheduling(QObject *parent) : QObject(parent)
{
}

void Scheduling::start()
{
  StdinThread *t = new StdinThread();
  connect(t, &StdinThread::incomingData, this, &Scheduling::processStdin);
  connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
  t->start();

  hostsUpCheckIntervals = ObjectInstances2::database.getHostsCheckIntervals(true);
  hostsDownCheckIntervals = ObjectInstances2::database.getHostsCheckIntervals(false);

  // timestamp: utc seconds since epoch
  qint64 now = QDateTime::currentMSecsSinceEpoch() / 1000;

  // create initial schedule
  QHashIterator<QString, int> i(hostsUpCheckIntervals);
  while (i.hasNext())
  {
    i.next();
    int interval = i.value();

    if (interval > 0)
    {
      // check random within interval
      int sec_to_next_check = 1 + (qrand() % interval);

      pingSchedule.insert(now + sec_to_next_check, i.key());
    }
  }
}

void Scheduling::addHostToSchedule(QString ipv4, bool up)
{
  qint64 now = QDateTime::currentMSecsSinceEpoch() / 1000;
  int interval;
  if (up)
  {
    interval = hostsUpCheckIntervals.value(ipv4, -1);
  }
  else
  {
    interval = hostsDownCheckIntervals.value(ipv4, -1);
  }
  QTextStream cout(stdout);

  cout << "intreval: " << interval << endl;
  if (interval != -1)
  {
    pingSchedule.insert(now + interval, ipv4);
  }
}

void Scheduling::scheduleNextSeconds()
{
  // timestamp: utc seconds since epoch
  qint64 now = QDateTime::currentMSecsSinceEpoch() / 1000;

  // count checks in the next 5 seconds
  int counter = 0;
  while ((counter < pingSchedule.size()) && (pingSchedule.keys().value(counter) < (quint64) now+5))
  {
    counter++;
  }
  if (counter > 0)
  {
    int step_in_msec = 5000 / counter;
    for (int i = 0; i < counter; i++)
    {
      QTimer::singleShot(step_in_msec * i, this, SLOT(startPing()));
    }
  }
  QTimer::singleShot(5000, this, SLOT(scheduleNextSeconds()));
}

void Scheduling::startPing()
{
  if (!pingSchedule.isEmpty())
  {
    QString ipv4 = pingSchedule.take(pingSchedule.firstKey());
    ObjectInstances2::pingScheduler.schedulePing(ipv4, true);
  }
}


void Scheduling::processStdin(QString message)
{
  QTextStream cout(stdout);

  cout << "on sched read from stdin: " << message << endl;
}
