//using namespace std;

#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <QDebug>
#include "pingwin.h"


Pingwin::Pingwin()
{
  valid = false;
  hIcmpFile = IcmpCreateFile();
  if (hIcmpFile == INVALID_HANDLE_VALUE)
  {
    qDebug() << "Unable to open handle. IcmpCreatefile returned error: " << GetLastError();
    return;
  }

  ReplySize = sizeof(ICMP_ECHO_REPLY) + ICMP_PACKET_SIZE;
  ReplyBuffer = (VOID*) malloc(ReplySize);
  if (ReplyBuffer == NULL)
  {
    qDebug() << "Unable to allocate memory";
    return;
  }
  valid = true;
}

Pingwin::~Pingwin()
{
  //TODO remove file and mem
}

bool Pingwin::isValid()
{
  return valid;
}

int Pingwin::ping(char *ip)
{
  char SendData[ICMP_PACKET_SIZE] = "Data Buffer";
  DWORD dwRetVal = IcmpSendEcho(hIcmpFile, inet_addr(ip), SendData, sizeof(SendData), NULL, ReplyBuffer, ReplySize, 1000);
  if (dwRetVal != 0)
  {
    PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
    struct in_addr ReplyAddr;
    ReplyAddr.S_un.S_addr = pEchoReply->Address;
    qDebug() << "Sent icmp message to " << ip;
    qDebug() << "Received icmp message responses:" << dwRetVal;
    qDebug() << "Status = " << pEchoReply->Status;
    return pEchoReply->Status;
  }
  else
  {
    qDebug() << "Call to IcmpSendEcho failed. IcmpSendEcho returned error: " << GetLastError();
    return -1;
  }
  return 0;
}
