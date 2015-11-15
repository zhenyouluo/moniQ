#ifndef SCHEDULING_H
#define SCHEDULING_H

#include <QObject>

class Scheduling : public QObject
{
  Q_OBJECT
public:
  explicit Scheduling(QObject *parent = 0);

signals:

public slots:
  void processStdin(QString string);
};

#endif // SCHEDULING_H
