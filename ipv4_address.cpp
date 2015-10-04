
#include <QDebug>
#include "ipv4_address.h"

Ipv4_Address::Ipv4_Address(QString address_string, QObject *parent) : QObject(parent)
{
  validAddress = false;
  QStringList byte_list = address_string.split(".");
  if (byte_list.length() != 4)
  {
    return;
  }
  for (int i = 0; i < byte_list.size(); ++i)
  {
    bool ok;
    int byte = byte_list[i].toInt(&ok);
    if (!ok)
    {
      return;
    }
    if (byte < 0 || byte > 255)
    {
      return;
    }
    bytes[i] = (quint8) byte;
  }
  validAddress = true;
}

Ipv4_Address::Ipv4_Address(quint32 address_int, QObject *parent) : QObject(parent)
{
  quint32 temp_int = address_int;
  quint32 divider = 256*256*256;
  for (int i = 0; i < 4; ++i)
  {
    bytes[i] = (quint8) (temp_int / divider);
    temp_int = temp_int - (bytes[i] * divider);
    divider = divider / 256;
  }
  validAddress = true;
}

QString Ipv4_Address::toString()
{
  QString tmpstr = QString::number(bytes[0]);
  for (int i = 1; i < 4; ++i)
  {
    tmpstr += ".";
    tmpstr += QString::number(bytes[i]);
  }
  return tmpstr;
}

quint32 Ipv4_Address::toInt()
{
  quint32 tmp = bytes[0];
  for (int i = 1; i < 4; ++i)
  {
    tmp = tmp * 256;
    tmp = tmp + bytes[i];
  }
  return tmp;
}

bool Ipv4_Address::isValid()
{
  return validAddress;
}
