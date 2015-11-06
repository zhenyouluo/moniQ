#include <QCoreApplication>
#include <QDebug>
#include <QObject>

#include "networkdiscoverer.h"
#include "objectinstances.h"

int main(int argc, char *argv[])
{
  qRegisterMetaType<QString>("QString");
  QCoreApplication a(argc, argv);

  ObjectInstances::commandServer.start();

  ObjectInstances::database.start(true);


  QTimer::singleShot(0, &ObjectInstances::pingScheduler, SLOT(connectPingers()));


  return a.exec();

}
