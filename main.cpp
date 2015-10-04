#include <QCoreApplication>
#include <QtSql/qsqldatabase.h>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QPluginLoader>
#include <QObject>
#include "tcpscanner.h"
#include "pingerinterface.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  QCoreApplication::addLibraryPath("C:\\Users\\Aise\\Documents\\build-moniQ-msvc2013_64-Debug\\debug");


  QPluginLoader pl("pingwin");
  QObject *qp = pl.instance();
  if (qp)
  {
    qDebug() << "qp true ";
  }
  else
  {
    qDebug() << "qp is false ";
  }
  PingerInterface* pi = qobject_cast<PingerInterface *>(qp);
  if (pi)
  {
    qDebug() << "true "<< pi->ping("192.168.1.150");
  }
  else
  {
    qDebug() << "is false ";
  }

  TcpScanner* tcpscanner = new TcpScanner(&a);

  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
     db.setHostName("localhost");
     db.setDatabaseName("moniQ");
     db.setUserName("root");
     db.setPassword("frysia");
     bool ok = db.open();
     QSqlQuery query;
             bool ok2 = query.exec("SELECT * FROM hosts;");

             while (query.next()) {
                 QString name = query.value(0).toString();
                 qDebug() << "name:" << name;
             }
             qDebug() << "klaar";
  return a.exec();

}
