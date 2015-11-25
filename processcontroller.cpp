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
    if (message.left(11) == "PINGRESULT:")
    {
      QStringList parts = message.remove(0,11).split(";");
      if (parts.size() > 1)
      {
        emit sendPingResult(parts[0], parts[1].toInt());
      }
    }

  }
}

void ProcessController::messageScheduler(QString message)
{
  qDebug() << "write" << message;
  schedulingProcess->write((message + "\n").toLatin1().data());
}
