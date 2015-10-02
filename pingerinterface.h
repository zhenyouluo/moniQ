#ifndef PINGERINTERFACE_H
#define PINGERINTERFACE_H

#include <QString>

#define PingerInterface_iid "nl.pikestjelp.moniQ.PingerInterface"

class PingerInterface
{
public:
  virtual ~PingerInterface() {}
  virtual bool ping(QString) = 0;
};

Q_DECLARE_INTERFACE(PingerInterface, PingerInterface_iid)

#endif // PINGERINTERFACE_H
