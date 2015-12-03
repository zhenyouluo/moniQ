#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include <QTime>
#include <QTimer>

#include <scheduling.h>
#include "objectinstances2.h"

int main(int argc, char *argv[])
{
  qRegisterMetaType<QString>("QString");
  qsrand(QTime::currentTime().msec());

  QCoreApplication a(argc, argv);
  QTextStream cout(stdout);
  QString database_user = QString(ObjectInstances2::databaseCredentials.value("database_user", "").toByteArray());
  QString database_password = QString(ObjectInstances2::databaseCredentials.value("database_password", "").toByteArray());

  ObjectInstances2::database.start(database_user, database_password);

  cout << "OUT:Scheduling process started2" << endl;
  ObjectInstances2::scheduler.start();
cout << "OUT:start processes" << endl;
  ObjectInstances2::processController.startProcesses();
cout << "OUT:start processes" << endl;
  QTimer::singleShot(0, &ObjectInstances2::pingScheduler, SLOT(connectPingers()));
  QTimer::singleShot(0, &ObjectInstances2::scheduler, SLOT(scheduleNextSeconds()));
  return a.exec();
}
