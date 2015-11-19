#ifndef OBJECTINSTANCES2_H
#define OBJECTINSTANCES2_H

#include <QObject>
#include <QSettings>

#include "../database.h"

class ObjectInstances2 : public QObject
{
  Q_OBJECT
public:
  explicit ObjectInstances2(QObject *parent = 0);
  static Database database;
  static QSettings databaseCredentials;

signals:

public slots:
};

#endif // OBJECTINSTANCES2_H
