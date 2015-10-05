#ifndef OBJECTINSTANCES_H
#define OBJECTINSTANCES_H

#include <QObject>

class ObjectInstances : public QObject
{
  Q_OBJECT
public:
  explicit ObjectInstances(QObject *parent = 0);

signals:

public slots:
};

#endif // OBJECTINSTANCES_H
