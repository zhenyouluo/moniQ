#include <QCoreApplication>
#include <QTextStream>

#include "objectinstances.h"
#include "networkreader.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  QTextStream cout(stdout);
 cout << "OUT:Metrics process started" << endl;
 QString database_user = QString(ObjectInstances::databaseCredentials.value("database_user", "").toByteArray());
  QString database_password = QString(ObjectInstances::databaseCredentials.value("database_password", "").toByteArray());

  ObjectInstances::database.start(database_user, database_password);
  ObjectInstances::networkreader = new NetworkReader();

  return a.exec();
}
