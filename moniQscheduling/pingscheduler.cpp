#include <QCoreApplication>
#include <QDebug>

#include "pingscheduler.h"
#include "objectinstances2.h"

PingScheduler::PingScheduler(QObject *parent) : QObject(parent)
{
  QCoreApplication::addLibraryPath("C:\\Users\\Aise\\Documents\\build-moniQ-msvc2013_64-Debug\\pingwin\\debug");
}

int PingScheduler::getFreeThread()
{
  for (int i = 0; i < MAX_CONCURRENT_PINGS; i++)
  {
    if (pingthreadcontrols[i].isAvailable())
    {
      return i;
    }
  }
  return -1;
}

void PingScheduler::connectPingers()
{
  for (int i = 0; i < MAX_CONCURRENT_PINGS; i++)
  {
    pingthreadcontrols[i].connectPinger();
  }
}

void PingScheduler::schedulePing(QString ip_address, bool monitoring)
{
  int freeThread = getFreeThread();
  if (freeThread >= 0)
  {
    pingthreadcontrols[freeThread].startPing(ip_address, monitoring);
  }
  else
  {
    if (monitoring)
    {
      monitor_queue.enqueue(ip_address);
    }
    else
    {
      discover_queue.enqueue(ip_address);
    }
  }
}

void PingScheduler::processPingResult(QString ipAddress, int result, bool monitoring)
{
  QTextStream cout(stdout);

  if (monitoring)
  {
    if (result == 0)
    {
      // reschedule according to up schedule
      ObjectInstances2::scheduler.addHostToSchedule(ipAddress, true);
    }
    else
    {
      // reschedule according to down schedule
      ObjectInstances2::scheduler.addHostToSchedule(ipAddress, false);
    }
    cout << "PINGRESULT:" << ipAddress << ";" << result << monitoring << endl;
    ObjectInstances2::processController.messageAnalyzer("PINGRESULT:" + ipAddress + ";" + QString::number(result));
  }
  else
  {
    QTextStream cout(stdout);
    cout << "PINGRESULT:" << ipAddress << ";" << result << endl;
    //emit sendPingResult(ipAddress, result);
  }
  int free_thread = getFreeThread();
  if (free_thread >= 0)
  {
    if (!monitor_queue.isEmpty())
    {
      QString ip_address =  monitor_queue.dequeue();
      pingthreadcontrols[free_thread].startPing(ip_address, true);
    }
    else
    {
      if (!discover_queue.isEmpty())
      {
        QString ip_address =  discover_queue.dequeue();
        pingthreadcontrols[free_thread].startPing(ip_address, false);
      }
    }
  }
}
