#include <QSettings>

#include "objectinstances2.h"

ObjectInstances2::ObjectInstances2(QObject *parent) : QObject(parent)
{

}

// declare needed instances
Database ObjectInstances2::database;
QSettings ObjectInstances2::databaseCredentials(QSettings::SystemScope, "tools4all", "moniQ");
