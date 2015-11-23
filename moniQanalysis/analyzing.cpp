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
      int missed_pings = 0;
      QString current_state = "normal";

      if (result != 0)
      {
        missed_pings = prev_missed_pings + 1;

        if (missed_pings > hostsWarningLevels.value(ipv4))
        {
          current_state = "warning";
        }
        if (missed_pings > hostsCriticalLevels.value(ipv4))
        {
          current_state = "critical";
        }
      }

      hostMissedPings.insert(ipv4, missed_pings);
      // update missed pings in database
      ObjectInstances2::database.updateMissedPings(ipv4, missed_pings);

      if (prev_state != current_state)
      {
        hostStates.insert(ipv4, current_state);
        // new state to database
        ObjectInstances2::database.updateState(ipv4, current_state);
      }
    }
  }
}
