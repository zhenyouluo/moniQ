#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql/QSqlDatabase>

class Database : public QObject
{
  Q_OBJECT

private:
   QSqlDatabase db;
   bool connected;

public:
  explicit Database(QObject *parent = 0);
  void start(bool warnUser);
  void addHost(QString ipAddress);

signals:

public slots:
};

#endif // DATABASE_H
