#include <QDebug>
#include <QSqlQuery>

#include "database.h"
#include "objectinstances.h"

Database::Database(QObject *parent) : QObject(parent)
{
  connected = false;
}

void Database::start(bool warnUser)
{
  QString database_user = QString(ObjectInstances::databaseCredentials.value("database_user", "").toByteArray());
  QString database_password = QString(ObjectInstances::databaseCredentials.value("database_password", "").toByteArray());

  if ((database_user == "") || (database_password == ""))
  {
    if (warnUser)
    {
      qWarning() << "Database credentials not set. Point you web browser to port 6901 at this host to set them.";
    }
    return;
  }
  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
  db.setHostName("localhost");
  db.setDatabaseName("moniQ");
  db.setUserName(database_user);
  db.setPassword(database_password);
  connected = db.open();
}

void Database::addHost(QString ipAddress)
{
  // if connected

  QSqlQuery query;
  query.exec("INSERT INTO hosts (name) VALUES ('" + ipAddress + "')");
}
