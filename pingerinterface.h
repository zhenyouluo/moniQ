#ifndef PINGERINTERFACE_H
#define PINGERINTERFACE_H

#include <QString>

#define PingerInterface_iid "nl.pikestjelp.moniQ.PingerInterface"

class PingerInterface
{
public:
  virtual ~PingerInterface() {}
  virtual int ping(char *) = 0;
};

Q_DECLARE_INTERFACE(PingerInterface, PingerInterface_iid)

#endif // PINGERINTERFACE_H
