#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <QDebug>
#include <QTextStream>

#include "pinger.h"

void Pinger::ping(QString ip_address)
{
  QTextStream cout(stdout);
  //int result = pingerInterface->ping();
  //emit sendPingResult(result);

  HANDLE hIcmpFile = IcmpCreateFile();
  if (hIcmpFile == INVALID_HANDLE_VALUE)
  {
    emit sendPingResult(-1);
    return;
  }

  DWORD ReplySize = sizeof(ICMP_ECHO_REPLY) + ICMP_PACKET_SIZE;
  LPVOID ReplyBuffer = (VOID*) malloc(ReplySize);
  if (ReplyBuffer == NULL)
  {
    emit sendPingResult(-2);
    return;
  }

  char SendData[ICMP_PACKET_SIZE] = "Data Buffer";
  DWORD dwRetVal = IcmpSendEcho(hIcmpFile, inet_addr(ip_address.toLatin1().data()), SendData, sizeof(SendData), NULL, ReplyBuffer, ReplySize, 1000);
  if (dwRetVal != 0)
  {
    PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
    struct in_addr ReplyAddr;
    ReplyAddr.S_un.S_addr = pEchoReply->Address;
    cout << "Status = " << pEchoReply->Status << endl;
    if (pEchoReply->Status == 0)
    {
      emit sendPingResult(0);
    }
    else
    {
      emit sendPingResult(-3);
    }
  }
  else
  {
    emit sendPingResult(-4);
  }
}
