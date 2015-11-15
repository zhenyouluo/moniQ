#include <QSettings>

#include "objectinstances.h"

ObjectInstances::ObjectInstances(QObject *parent) : QObject(parent)
{

}

// declare needed instances
CommandServer ObjectInstances::commandServer;
Database ObjectInstances::database;
QSettings ObjectInstances::databaseCredentials(QSettings::SystemScope, "tools4all", "moniQ");
PingScheduler ObjectInstances::pingScheduler;
ProcessController ObjectInstances::processController;
