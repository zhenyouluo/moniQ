#ifndef ANALYZING_H
#define ANALYZING_H

#include <QObject>
#include <QHash>

class Analyzing : public QObject
{
  Q_OBJECT

private:
  QHash<QString, int> hostsWarningLevels;
  QHash<QString, int> hostsCriticalLevels;
  QHash<QString, QString> hostStates;
  QHash<QString, int> hostMissedPings;
  QHash<QString, QString> hostNames;

public:
  explicit Analyzing(QObject *parent = 0);
  void start();

signals:

public slots:
  void processStdin(QString string);
};

#endif // ANALYZING_H
