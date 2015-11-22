#ifndef STDINTHREAD_H
#define STDINTHREAD_H

#include <QThread>
#include <QTextStream>
class StdinThread : public QThread
{
  Q_OBJECT

signals:
  void incomingData(QString string);

public:
  StdinThread();
  void run(void)
  {
    while (true)
    {
      QTextStream stream(stdin);
      QString line;
      line = stream.readLine();
      emit incomingData(line);
    }
  }
};
#endif // STDINTHREAD_H
