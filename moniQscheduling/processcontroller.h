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
  void messageAnalyzer(QString message);

private:
  QProcess* analyzingProcess;

signals:
  void endProcess();

public slots:
  void dataFromAnalyzer();

};

#endif // PROCESSCONTROLLER_H
