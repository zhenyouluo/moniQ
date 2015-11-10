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
      qWarning() << "Database credentials not set. Point your web browser to this host to set them.";
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

void Database::addHost(QString host, QString ipAddress)
{
  if (connected)
  {
    QSqlQuery query;
    query.exec("DELETE FROM hosts WHERE ipv4='" + ipAddress + "'");
    query.exec("INSERT INTO hosts (hostname, ipv4) VALUES ('" + host + "','" + ipAddress + "')");
    ObjectInstances::commandServer.broadCast("HOSTLIST UPDATED");
  }
}

bool Database::moniqCredentialsSet()
{
  if (!connected)
  {
    return false;
  }
  QSqlQuery query("SELECT * FROM users");
  return (query.size() > 0);
}

bool Database::checkCredentials(QString user, QString password)
{
  if (!connected)
  {
    return false;
  }
  QSqlQuery query("SELECT * FROM users WHERE user='" + user + "' AND password = '" + password + "'");
  return (query.size() > 0);
}
