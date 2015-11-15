#include <QObject>
#include <QTextStream>

#include "scheduling.h"
#include "stdinthread.h"

Scheduling::Scheduling(QObject *parent) : QObject(parent)
{
  StdinThread *t = new StdinThread();
  connect(t, &StdinThread::incomingData, this, &Scheduling::processStdin);
  connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
  t->start();
}

void Scheduling::processStdin(QString message)
{
  QTextStream cout(stdout);

  cout << "on sched read from stdin: " << message << endl;
}
