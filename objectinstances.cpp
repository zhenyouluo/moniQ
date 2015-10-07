#include "objectinstances.h"

ObjectInstances::ObjectInstances(QObject *parent) : QObject(parent)
{

}

// create needed instances
NetworkDiscoverer* ObjectInstances::networkDiscoverer = new NetworkDiscoverer();
CommandServer ObjectInstances::commandServer;
ChatServer* ObjectInstances::server= new ChatServer(1234);
