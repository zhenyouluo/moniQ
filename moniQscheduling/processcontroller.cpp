#include <QDebug>
#include <QTextStream>
#include <QCoreApplication>

#include "processcontroller.h"

ProcessController::ProcessController(QObject *parent) : QObject(parent)
{
}

bool ProcessController::startProcesses()
{
  analyzingProcess = new QProcess();
  connect(this, &ProcessController::endProcess, analyzingProcess, &QProcess::kill);
  connect(analyzingProcess, &QProcess::readyReadStandardOutput, this, &ProcessController::dataFromAnalyzer);

#ifdef QT_DEBUG
  analyzingProcess->start("C:\\Users\\Aise\\Documents\\build-moniQ-msvc2013_64-Debug\\moniQanalysis\\debug\\moniQanalysis.exe");
#else
  analyzingProcess->start("\"" + QCoreApplication::applicationDirPath() + "\\moniQanalysis.exe\"");
#endif
  return analyzingProcess->waitForStarted();
}

void ProcessController::endProcesses()
{
  emit endProcess();
}

void ProcessController::dataFromAnalyzer()
{
  while(analyzingProcess->canReadLine())
  {
    QString message = QString(analyzingProcess->readLine());
    message = message.trimmed();
    QTextStream out(stdout);
    out << "a:" << message << endl;
  }
}

void ProcessController::messageAnalyzer(QString message)
{
  QTextStream cout(stdout);
  cout << "m:" << message << endl;
  analyzingProcess->write((message + "\n").toLatin1().data());
}


