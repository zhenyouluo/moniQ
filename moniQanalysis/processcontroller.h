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
  void messageMetrics(QString message);

private:
  QProcess* metricsProcess;

signals:
  void endProcess();

public slots:
  void dataFromMetrics();

};

#endif // PROCESSCONTROLLER_H
