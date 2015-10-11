#include <QSettings>

#include "objectinstances.h"

ObjectInstances::ObjectInstances(QObject *parent) : QObject(parent)
{

}

// declare needed instances
NetworkDiscoverer ObjectInstances::networkDiscoverer;
CommandServer ObjectInstances::commandServer;
Database ObjectInstances::database;
QSettings ObjectInstances::databaseCredentials(QSettings::SystemScope, "tools4all", "moniQ");
