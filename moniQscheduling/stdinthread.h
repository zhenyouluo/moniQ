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
    QTextStream stream(stdin);
    QString line;
    while (true)
    {
      line = stream.readLine();
      QTextStream cout(stdout);

      cout << "stdin: " << line << endl;
      emit incomingData(line);
    }
  }
};
#endif // STDINTHREAD_H

