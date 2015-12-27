#ifndef OBJECTINSTANCES_H
#define OBJECTINSTANCES_H

#include <QObject>
#include <QSettings>

#include "../database.h"
#include "networkreader.h"

class ObjectInstances : public QObject
{
  Q_OBJECT
public:
  explicit ObjectInstances(QObject *parent = 0);
  static Database database;
  static QSettings databaseCredentials;
  static NetworkReader* networkreader;
signals:

public slots:
};

#endif // OBJECTINSTANCES_H
