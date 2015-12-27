#include <QSettings>

#include "objectinstances.h"

ObjectInstances::ObjectInstances(QObject *parent) : QObject(parent)
{

}

// declare needed instances
Database ObjectInstances::database;
QSettings ObjectInstances::databaseCredentials(QSettings::SystemScope, "tools4all", "moniQ");
NetworkReader* ObjectInstances::networkreader;
//Scheduling ObjectInstances::scheduler;



