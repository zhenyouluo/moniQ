#ifndef IPV4_ADDRESS_H
#define IPV4_ADDRESS_H

#include <QObject>

class Ipv4_Address : public QObject
{
  Q_OBJECT

private:
  quint8 bytes[4]; // 4 bytes, left (0) to right (3)
  bool validAddress;

public:
  explicit Ipv4_Address(QObject *parent = 0);
  Ipv4_Address(QString address_string, QObject *parent = 0);
  Ipv4_Address(quint32 address_int, QObject *parent = 0);
  bool isValid();
  QString toString();
  quint32 toInt();

signals:

public slots:
};

#endif // IPV4_ADDRESS_H
