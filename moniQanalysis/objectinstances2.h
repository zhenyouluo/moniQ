#ifndef OBJECTINSTANCES2_H
#define OBJECTINSTANCES2_H

#include <QObject>
#include <QSettings>

#include "../database.h"
#include "analyzing.h"
#include "processcontroller.h"

class ObjectInstances2 : public QObject
{
  Q_OBJECT
public:
  explicit ObjectInstances2(QObject *parent = 0);
  static Database database;
  static QSettings databaseCredentials;
  static Analyzing analyzer;
  static ProcessController processController;

signals:

public slots:
};

#endif // OBJECTINSTANCES2_H
