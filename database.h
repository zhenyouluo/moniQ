#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QHash>
#include <QtSql/QSqlDatabase>

class Database : public QObject
{
  Q_OBJECT

private:
   QSqlDatabase db;
   bool connected;

public:
  explicit Database(QObject *parent = 0);
  void start(QString user, QString password);
  void addHost(QString host, QString ipAddress);
  bool moniqCredentialsSet();
  bool checkCredentials(QString user, QString password);
  QHash<QString, int> getHostsUpCheckIntervals();
signals:

public slots:
};

#endif // DATABASE_H
