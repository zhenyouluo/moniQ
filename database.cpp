#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>

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
    query.exec("SELECT * FROM hosts WHERE hostname = '" + host + "';");
    if (query.next())
    {
      // host already exists with a different ip address
      query.exec("UPDATE `hosts` SET `ipv4`='" + ipAddress + "' WHERE hostname = '" + host + "';");
    }
    else
    {
      query.exec("INSERT INTO hosts (hostname, ipv4) VALUES ('" + host + "','" + ipAddress + "')");
      query.exec("INSERT INTO host_state_history (hostname, state, time) VALUES ('" + host + "','normal','" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "')");
    }
  }
}

void Database::addFullHost(QString host, QString ipAddress, QString tmpl)
{
  if (connected)
  {
    QSqlQuery query;
    query.exec("DELETE FROM hosts WHERE ipv4='" + ipAddress + "'");
    query.exec("SELECT * FROM hosts WHERE hostname = '" + host + "';");
    if (query.next())
    {
      // host already exists with a different ip address
      query.exec("UPDATE `hosts` SET `ipv4`='" + ipAddress + "',`template`='" + tmpl + "' WHERE hostname = '" + host + "';");
    }
    else
    {
      query.exec("INSERT INTO hosts (hostname, ipv4, template) VALUES ('" + host + "','" + ipAddress + "','" + tmpl + "')");
      query.exec("INSERT INTO host_state_history (hostname, state, time) VALUES ('" + host + "','normal','" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "')");
    }
  }
}

bool Database::addHostTemplate(QString tmpl, QString up_interval, QString down_interval, QString warning_level, QString critical_level)
{
  if (connected)
  {
    QSqlQuery query;
    query.exec("SELECT * FROM host_templates WHERE name = '" + tmpl + "'");
    if (query.next())
    {
      // template already exists
      return false;
    }
    query.exec("INSERT INTO host_templates (`name`, `check_interval_on_up`, `check_interval_on_down`, `missed_pings_warning_level`, `missed_pings_critical_level`) VALUES ('" + tmpl + "','" + up_interval + "','" + down_interval + "','" + warning_level + "','" + critical_level + "')");
  }
  return true;
}

void Database::editHostTemplate(QString tmpl, QString up_interval, QString down_interval, QString warning_level, QString critical_level)
{
  if (connected)
  {
    QSqlQuery query;
    query.exec("UPDATE host_templates SET `check_interval_on_up`=" + up_interval + ", `check_interval_on_down`=" + down_interval + ", `missed_pings_warning_level`=" + warning_level + ", `missed_pings_critical_level`=" + critical_level + " WHERE name='" + tmpl + "';");
  }
}

void Database::updateHost(QString host, QString ipAddress, QString tmpl)
{
  if (connected)
  {
    QSqlQuery query;
    query.exec("DELETE FROM hosts WHERE ipv4='" + ipAddress + "' AND hostname != '" + host + "'");
    query.exec("UPDATE `hosts` SET `ipv4`='" + ipAddress + "',`template`='" + tmpl + "' WHERE hostname = '" + host + "';");
  }
}

bool Database::deleteHostTemplate(QString tmpl)
{
  if (connected)
  {
    QSqlQuery query;
    query.exec("SELECT * FROM hosts WHERE template = '" + tmpl + "'");
    if (query.next())
    {
      // template in use
      return false;
    }
    query.exec("DELETE FROM host_templates WHERE name='" + tmpl + "'");
  }
  return true;
}

void Database::deleteHost(QString host)
{
  if (connected)
  {
    QSqlQuery query;
    query.exec("DELETE FROM hosts WHERE hostname='" + host + "'");
    query.exec("DELETE FROM host_state_history WHERE hostname='" + host + "'");
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

QHash<QString, int> Database::getHostsCheckIntervals(bool up)
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
      int intervalfield;
      if (up)
      {
        intervalfield = query2.record().indexOf("check_interval_on_up");
      }
      else
      {
        intervalfield = query2.record().indexOf("check_interval_on_down");
      }
      while (query2.next())
      {
        intervals.insert(ipv4, query2.value(intervalfield).toInt());
      }
    }
  }
  return intervals;
}

QHash<QString, QString> Database::getHostnames()
{
  QHash<QString, QString> hostnames;
  if (connected)
  {
    QSqlQuery query("SELECT * FROM hosts;");
    int ipv4field = query.record().indexOf("ipv4");
    int namefield = query.record().indexOf("hostname");
    while (query.next())
    {
      hostnames.insert(query.value(ipv4field).toString(), query.value(namefield).toString());
    }
  }
  return hostnames;
}

QHash<QString, int> Database::getHostsWarninglevels()
{
  QHash<QString, int> levels;
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
      int field = query2.record().indexOf("missed_pings_warning_level");
      while (query2.next())
      {
        levels.insert(ipv4, query2.value(field).toInt());
      }
    }
  }
  return levels;
}

QHash<QString, int> Database::getHostsCriticalLevels()
{
  QHash<QString, int> levels;
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
      int field = query2.record().indexOf("missed_pings_critical_level");
      while (query2.next())
      {
        levels.insert(ipv4, query2.value(field).toInt());
      }
    }
  }
  return levels;
}

QHash<QString, QString> Database::getHostsStates()
{
  QHash<QString, QString> states;
  if (connected)
  {
    QSqlQuery query("SELECT * FROM hosts;");
    int ipv4field = query.record().indexOf("ipv4");
    int statefield = query.record().indexOf("state");
    while (query.next())
    {
      states.insert(query.value(ipv4field).toString(), query.value(statefield).toString());
    }
  }
  return states;
}

QHash<QString, int> Database::getHostsMissedPings()
{
  QHash<QString, int> missedpings;
  if (connected)
  {
    QSqlQuery query("SELECT * FROM hosts;");
    int ipv4field = query.record().indexOf("ipv4");
    int mpfield = query.record().indexOf("missed_pings");
    while (query.next())
    {
      missedpings.insert(query.value(ipv4field).toString(), query.value(mpfield).toInt());
    }
  }
  return missedpings;
}

void Database::updateMissedPings(QString ipv4, int missed_pings)
{
  QSqlQuery query("UPDATE `hosts` SET `missed_pings`=" + QString::number(missed_pings) + " WHERE ipv4='" + ipv4 + "';");
}

void Database::updateState(QString host, QString current_state)
{
  QSqlQuery query("UPDATE `hosts` SET `state`='" + current_state + "' WHERE hostname='" + host + "';");
  QSqlQuery query2("INSERT INTO `host_state_history`(`hostname`, `state`, `time`) VALUES ('" + host + "','" + current_state + "','" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "')");
}
