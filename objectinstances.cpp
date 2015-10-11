#include <QSettings>

#include "objectinstances.h"

ObjectInstances::ObjectInstances(QObject *parent) : QObject(parent)
{

}

// declare needed instances
NetworkDiscoverer ObjectInstances::networkDiscoverer;
CommandServer ObjectInstances::commandServer;
ChatServer* ObjectInstances::server= new ChatServer(1234);
QSettings ObjectInstances::databaseCredentials(QSettings::SystemScope, "tools4all", "moniQ");
