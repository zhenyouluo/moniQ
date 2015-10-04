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

bool Ipv4_Address::isValid()
{
  return validAddress;
}
