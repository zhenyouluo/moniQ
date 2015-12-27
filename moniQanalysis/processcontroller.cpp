#include <QTextStream>
#include <QCoreApplication>

#include "processcontroller.h"

ProcessController::ProcessController(QObject *parent) : QObject(parent)
{
}

bool ProcessController::startProcesses()
{
  metricsProcess = new QProcess();
  connect(this, &ProcessController::endProcess, metricsProcess, &QProcess::kill);
  connect(metricsProcess, &QProcess::readyReadStandardOutput, this, &ProcessController::dataFromMetrics);

#ifdef QT_DEBUG
#ifdef Q_OS_WIN
  metricsProcess->start("C:\\Users\\Aise\\Documents\\build-moniQ-msvc2013_64-Debug\\moniQmetrics\\debug\\moniQmetrics.exe");
#endif
#ifdef Q_OS_LINUX
  metricsProcess->start("/home/aise/build-moniQ-Qt_5_5_1_gcc_64-Debug/moniQmetrics/moniQmetrics");
#endif
#else
  metricsProcess->start("\"" + QCoreApplication::applicationDirPath() + "\\moniQmetrics.exe\"");
#endif
  return metricsProcess->waitForStarted();
}

void ProcessController::endProcesses()
{
  emit endProcess();
}

void ProcessController::dataFromMetrics()
{
  while(metricsProcess->canReadLine())
  {
    QString message = QString(metricsProcess->readLine());
    message = message.trimmed();
    QTextStream out(stdout);
    out << "met:" << message << endl;
  }
}

void ProcessController::messageMetrics(QString message)
{
  QTextStream cout(stdout);
  cout << "m:" << message << endl;
  metricsProcess->write((message + "\n").toLatin1().data());
}

