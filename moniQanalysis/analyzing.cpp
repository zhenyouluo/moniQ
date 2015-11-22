#include "stdinthread.h"
#include "../database.h"
#include "objectinstances2.h"
#include "analyzing.h"

Analyzing::Analyzing(QObject *parent) : QObject(parent)
{

}

void Analyzing::start()
{
  StdinThread *t = new StdinThread();
  connect(t, &StdinThread::incomingData, this, &Analyzing::processStdin);
  connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
  t->start();

  hostsWarningLevels = ObjectInstances2::database.getHostsWarninglevels();
  hostsCriticalLevels = ObjectInstances2::database.getHostsCriticalLevels();
  hostStates = ObjectInstances2::database.getHostsStates();
  hostMissedPings = ObjectInstances2::database.getHostsMissedPings();
}

void Analyzing::processStdin(QString message)
{
  QTextStream cout(stdout);

  cout << "on ana read from stdin: " << message << endl;

  if (message.left(11) == "PINGRESULT:")
  {
    QString data = message.mid(11);
    QStringList parts = data.split(";");
    if (parts.size() > 1)
    {
      QString ipv4 = parts.value(0);
      int result = parts.value(1).toInt();
      int prev_missed_pings = hostMissedPings.value(ipv4);
      QString prev_state = hostStates.value(ipv4);

      if (result == 0)
      {
        if (prev_missed_pings > 0)
        {
          hostMissedPings.insert(ipv4, 0);
          // update database
        }
        if (prev_state != "normal")
        {
          hostStates.insert(ipv4, "normal");
          // update database (2x)
        }
        else
        {
          // expand history state in database
        }
      }
      else
      {
        int missed_pings = prev_missed_pings + 1;
        // update database

        QString current_state = "normal";
        if (missed_pings > hostsWarningLevels.value(ipv4))
        {
          current_state = "warning";
        }
        if (missed_pings > hostsCriticalLevels.value(ipv4))
        {
          current_state = "critical";
        }
        hostStates.insert(ipv4, current_state);
        if (prev_state == current_state)
        {
          // expand history state in database
        }
        else
        {
          // update database (2x)
        }
      }
    }
  }
}
