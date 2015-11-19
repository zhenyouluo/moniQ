#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>

#include "database.h"

Database::Database(QObject *parent) : QObject(parent)
{
  connected = false;
}

void Database::start(QString user, QString password)
{
  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
  db.setHostName("localhost");
  db.setDatabaseName("moniQ");
  db.setUserName(user);
  db.setPassword(password);
  connected = db.open();
}

void Database::addHost(QString host, QString ipAddress)
{
  if (connected)
  {
    QSqlQuery query;
    query.exec("DELETE FROM hosts WHERE ipv4='" + ipAddress + "'");
    query.exec("INSERT INTO hosts (hostname, ipv4) VALUES ('" + host + "','" + ipAddress + "')");
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

QHash<QString, int> Database::getHostsUpCheckIntervals()
{
  QHash<QString, int> intervals;
  if (connected)
  {
    QSqlQuery query("SELECT * FROM hosts;");
    int ipv4field = query.record().indexOf("ipv4");
    int templatefield = query.record().indexOf("template");
    while (query.next())
    {
      QString hosttemplate = query.value(templatefield).toString();
      QString ipv4 = query.value(ipv4field).toString();
      QSqlQuery query2("SELECT * FROM host_templates WHERE name = '" + hosttemplate + "';");
      int intervalfield = query2.record().indexOf("check_interval_on_up");
      while (query2.next())
      {
        intervals.insert(ipv4, query2.value(intervalfield).toInt());
      }
    }
  }
  return intervals;
}
