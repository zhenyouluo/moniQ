#include <QCoreApplication>
#include <QDebug>
#include <QObject>
#include <QProcess>

#include "networkdiscoverer.h"
#include "objectinstances.h"

int main(int argc, char *argv[])
{
  qRegisterMetaType<QString>("QString");
  QCoreApplication a(argc, argv);

  ObjectInstances::commandServer.start();

  ObjectInstances::database.start(true);

  ObjectInstances::processController.startProcesses();
  ObjectInstances::processController.messageScheduler("aise\r\n");

  QTimer::singleShot(0, &ObjectInstances::pingScheduler, SLOT(connectPingers()));

  int returncode = a.exec();
  if (returncode == -1)
  {
    ObjectInstances::processController.endProcesses();
    QProcess* proc = new QProcess();
    proc->start(QCoreApplication::applicationFilePath());
  }
  return returncode;
}
