#ifndef OBJECTINSTANCES_H
#define OBJECTINSTANCES_H

#include <QObject>
#include <QSettings>

#include "networkdiscoverer.h"
#include "commandserver.h"
#include "chatserver.h"

// Class containing static instances of all objects for which we need exactly one instance

class ObjectInstances : public QObject
{
  Q_OBJECT
public:
  explicit ObjectInstances(QObject *parent = 0);
  static NetworkDiscoverer networkDiscoverer;
  static CommandServer commandServer;
  static ChatServer* server;
  static QSettings databaseCredentials;
signals:

public slots:
};




#endif // OBJECTINSTANCES_H
