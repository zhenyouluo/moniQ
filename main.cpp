#include <QCoreApplication>
#include <QtSql/qsqldatabase.h>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QDebug>

#include <QObject>
#include "networkdiscoverer.h"
#include "ipv4_address.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  Ipv4_Address* ip4 = new Ipv4_Address(QString("215.214.213.212"));
  quint32 qu = ip4->toInt();
  qDebug() << qu;
  ip4 = new Ipv4_Address(qu);
  qDebug() << ip4->toString();
  return 0;
  NetworkDiscoverer* networkdiscoverer = new NetworkDiscoverer();


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
