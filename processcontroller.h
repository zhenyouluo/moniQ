#ifndef PROCESSCONTROLLER_H
#define PROCESSCONTROLLER_H

#include <QObject>
#include <QProcess>

class ProcessController : public QObject
{
  Q_OBJECT
public:
  explicit ProcessController(QObject *parent = 0);
  bool startProcesses();
  void endProcesses();
  void messageScheduler(QString message);

private:
  QProcess* schedulingProcess;

signals:
  void endProcess();

public slots:
  void dataFromScheduler();

};

#endif // PROCESSCONTROLLER_H
