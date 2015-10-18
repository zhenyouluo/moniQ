#ifndef PINGWIN_H
#define PINGWIN_H
#include <QObject>
#include <QtPlugin>
#include <QString>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "pingwin_global.h"
#include "../pingerinterface.h"
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>

// ICMP packet types
#define ICMP_ECHO_REPLY 0
#define ICMP_DEST_UNREACH 3
#define ICMP_TTL_EXPIRE 11
#define ICMP_ECHO_REQUEST 8

// Minimum ICMP packet size, in bytes
#define ICMP_MIN 8

#define PACKET_SIZE 32
#define DEFAULT_TTL 30
#define MAX_PING_DATA_SIZE 1024
#define MAX_PING_PACKET_SIZE 1036

// The following two structures need to be packed tightly, but unlike
// Borland C++, Microsoft C++ does not do this by default.
#pragma pack(1)

// The IP header
struct IPHeader {
    BYTE h_len:4;           // Length of the header in dwords
    BYTE version:4;         // Version of IP
    BYTE tos;               // Type of service
    USHORT total_len;       // Length of the packet in dwords
    USHORT ident;           // unique identifier
    USHORT flags;           // Flags
    BYTE ttl;               // Time to live
    BYTE proto;             // Protocol number (TCP, UDP etc)
    USHORT checksum;        // IP checksum
    ULONG source_ip;
    ULONG dest_ip;
};

// ICMP header
struct ICMPHeader {
    BYTE type;          // ICMP packet type
    BYTE code;          // Type sub code
    USHORT checksum;
    USHORT id;
    USHORT seq;
    ULONG timestamp;    // not part of ICMP, but we need it
};


#pragma pack()

class PINGWINSHARED_EXPORT Pingwin: public QObject, public PingerInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID PingerInterface_iid)
  Q_INTERFACES(PingerInterface)

private:
  WSAData wsaData;
  bool valid;
  SOCKET sd;
  sockaddr_in dest, source;
  int ttl;
  int max_ping_packet_size;
  char send_buf[PACKET_SIZE];
  char recv_buf[MAX_PING_PACKET_SIZE];

public:
  Pingwin();
  ~Pingwin();
  int get_ping_result();
  bool isValid();
  int ping(char *);
  USHORT ip_checksum(USHORT* buffer, int size);
};

#endif // PINGWIN_H
