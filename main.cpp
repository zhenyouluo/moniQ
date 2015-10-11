#include <QCoreApplication>
#include <QDebug>
#include <QObject>

#include "networkdiscoverer.h"
#include "objectinstances.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  ObjectInstances::commandServer.start();

  ObjectInstances::database.start(true);





  return a.exec();

}
