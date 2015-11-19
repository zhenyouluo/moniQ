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
  QString database_user = QString(ObjectInstances::databaseCredentials.value("database_user", "").toByteArray());
  QString database_password = QString(ObjectInstances::databaseCredentials.value("database_password", "").toByteArray());

  if ((database_user == "") || (database_password == ""))
  {
    qWarning() << "Database credentials not set. Point your web browser to this host to set them.";
  }
  ObjectInstances::database.start(database_user, database_password);

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
