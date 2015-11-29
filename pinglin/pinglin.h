#ifndef PINGLIN_H
#define PINGLIN_H

#include <QObject>

#include "pinglin_global.h"
#include "../moniQscheduling/pingerinterface.h"

class PINGLINSHARED_EXPORT Pinglin: public QObject, public PingerInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID PingerInterface_iid)
  Q_INTERFACES(PingerInterface)

private:
  bool valid;

public:
  Pinglin();
  bool isValid();
  int ping(char *);

};

#endif // PINGLIN_H
