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

public:
  Pingwin();
  int ping(char *);
  int setup_for_ping(char* host, int ttl, SOCKET& sd, sockaddr_in& dest);
  USHORT ip_checksum(USHORT* buffer, int size);
  void init_ping_packet(ICMPHeader* icmp_hdr, int packet_size, int seq_no);
  int decode_reply(IPHeader* reply, int bytes, sockaddr_in* from);
  int recv_ping(SOCKET sd, sockaddr_in& source, IPHeader* recv_buf,int packet_size);
  int send_ping(SOCKET sd, const sockaddr_in& dest, ICMPHeader* send_buf,int packet_size);
};

#endif // PINGWIN_H
