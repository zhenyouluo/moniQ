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
  explicit Ipv4_Address(QString address_string, QObject *parent = 0);
  bool isValid();

signals:

public slots:
};

#endif // IPV4_ADDRESS_H
