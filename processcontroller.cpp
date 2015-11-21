#include <QDebug>

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
    qDebug() << "hier: "<< schedulingProcess->readLine();
  }
}

void ProcessController::messageScheduler(QString message)
{
  schedulingProcess->write(message.toLatin1().data());
}
