#include <QDebug>
#include <QTextStream>

#include "processcontroller.h"

ProcessController::ProcessController(QObject *parent) : QObject(parent)
{
}

bool ProcessController::startProcesses()
{
  schedulingProcess = new QProcess();
  connect(this, &ProcessController::endProcess, schedulingProcess, &QProcess::kill);
  connect(schedulingProcess, &QProcess::readyReadStandardOutput, this, &ProcessController::dataFromScheduler);
  schedulingProcess->start("C:\\Users\\Aise\\Documents\\build-moniQ-msvc2013_64-Debug\\moniQscheduling\\debug\\moniQscheduling.exe");
  return schedulingProcess->waitForStarted();
}

void ProcessController::endProcesses()
{
  emit endProcess();
}

void ProcessController::dataFromScheduler()
{
  while(schedulingProcess->canReadLine())
  {
    QString message = QString(schedulingProcess->readLine());
    message = message.trimmed();
    if (message.left(4) == "OUT:")
    {
      QTextStream out(stdout);
      out << message.remove(0,4) << endl;
    }
    else
    {
      qDebug() << message;
    }
  }
}

void ProcessController::messageScheduler(QString message)
{
  schedulingProcess->write(message.toLatin1().data());
}
