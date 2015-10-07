#ifndef OBJECTINSTANCES_H
#define OBJECTINSTANCES_H

#include <QObject>

#include "networkdiscoverer.h"
#include "commandserver.h"
#include "chatserver.h"

class ObjectInstances : public QObject
{
  Q_OBJECT
public:
  explicit ObjectInstances(QObject *parent = 0);
  static NetworkDiscoverer* networkDiscoverer;
  static CommandServer commandServer;
  static ChatServer* server;

signals:

public slots:
};




#endif // OBJECTINSTANCES_H
