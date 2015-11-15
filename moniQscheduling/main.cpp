#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>

#include <scheduling.h>

int main(int argc, char *argv[])
{

  QCoreApplication a(argc, argv);
  QTextStream cout(stdout);

  //cout << "testje2" << endl;

  Scheduling* scheduling = new Scheduling();
  cout << "testje3" << endl;
  return a.exec();
}
