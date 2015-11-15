#ifndef OBJECTINSTANCES_H
#define OBJECTINSTANCES_H

#include <QObject>
#include <QSettings>

#include "networkdiscoverer.h"
#include "commandserver.h"
#include "database.h"
#include "pingscheduler.h"
#include "processcontroller.h"

// Class containing static instances of all objects for which we need exactly one instance

class ObjectInstances : public QObject
{
  Q_OBJECT
public:
  explicit ObjectInstances(QObject *parent = 0);
  static CommandServer commandServer;
  static Database database;
  static QSettings databaseCredentials;
  static PingScheduler pingScheduler;
  static ProcessController processController;
signals:

public slots:
};




#endif // OBJECTINSTANCES_H
