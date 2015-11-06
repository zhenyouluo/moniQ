#ifndef PINGWIN_H
#define PINGWIN_H

#include <QObject>

#include <winsock2.h>

#include "pingwin_global.h"
#include "../pingerinterface.h"

#define WIN32_LEAN_AND_MEAN
#define ICMP_PACKET_SIZE 32

class PINGWINSHARED_EXPORT Pingwin: public QObject, public PingerInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID PingerInterface_iid)
  Q_INTERFACES(PingerInterface)

private:
  HANDLE hIcmpFile;
  bool valid;
  LPVOID ReplyBuffer = NULL;
  DWORD ReplySize = 0;

public:
  Pingwin();
  ~Pingwin();
//  int get_ping_result();
  bool isValid();
  int ping(char *);

};

#endif // PINGWIN_H
