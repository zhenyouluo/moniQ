#ifndef PINGERINTERFACE_H
#define PINGERINTERFACE_H

#include <QString>

#define PingerInterface_iid "nl.tools4all.moniQ.PingerInterface"

class PingerInterface
{
public:
  virtual ~PingerInterface() {}
  virtual int ping(char * ip_address) = 0;
  virtual int get_ping_result() = 0;
  virtual bool isValid() = 0;
};

Q_DECLARE_INTERFACE(PingerInterface, PingerInterface_iid)

#endif // PINGERINTERFACE_H
