//using namespace std;

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <QDebug>
#include "pingwin.h"


Pingwin::Pingwin()
{
  valid = false;
  max_ping_packet_size = MAX_PING_DATA_SIZE + sizeof(IPHeader);

  // Start Winsock up
  if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
  {
    qDebug() << "Failed to find Winsock 2.1 or better.";
    return;
  }
  // Create the socket
  sd = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, 0, 0, 0);

  if (sd == INVALID_SOCKET)
  {
    qDebug() << "Failed to create raw socket: " << WSAGetLastError();
    return;
  }

  ttl = DEFAULT_TTL;
  if (setsockopt(sd, IPPROTO_IP, IP_TTL, (const char*)&ttl, sizeof(ttl)) == SOCKET_ERROR)
  {
    qDebug() << "TTL setsockopt failed: " << WSAGetLastError();
    return;
  }

  valid = true;
}

Pingwin::~Pingwin()
{
  WSACleanup();
}

bool Pingwin::isValid()
{
  return valid;
}

int Pingwin::ping(char *ip)
{
  // Initialize the destination host info block
  memset(&dest, 0, sizeof(dest));

  unsigned int addr = inet_addr(ip);
  if (addr != INADDR_NONE)
  {
    // It was a dotted quad number, so save result
    dest.sin_addr.s_addr = addr;
    dest.sin_family = AF_INET;
  }
  else
  {
    return 1;
  }

  ICMPHeader* icmp_hdr = (ICMPHeader*) send_buf;

  // Set up the packet's fields
  icmp_hdr->type = ICMP_ECHO_REQUEST;
  icmp_hdr->code = 0;
  icmp_hdr->checksum = 0;
  icmp_hdr->id = (USHORT) index;
  icmp_hdr->seq = 0;
  icmp_hdr->timestamp = GetTickCount();

  // "You're dead meat now, packet!"
  const unsigned long int deadmeat = 0xDEADBEEF;
  char* datapart = (char*)icmp_hdr + sizeof(ICMPHeader);
  int bytes_left = PACKET_SIZE - sizeof(ICMPHeader);
  while (bytes_left > 0)
  {
    memcpy(datapart, &deadmeat, min(int(sizeof(deadmeat)), bytes_left));
    bytes_left -= sizeof(deadmeat);
    datapart += sizeof(deadmeat);
  }

  // Calculate a checksum on the result
  icmp_hdr->checksum = ip_checksum((USHORT*)icmp_hdr, PACKET_SIZE);

  // Send the ping
  int bwrote = sendto(sd, (char*) send_buf, PACKET_SIZE, 0, (sockaddr*) &dest, sizeof(dest));
  if (bwrote == SOCKET_ERROR)
  {
    qDebug() << "send failed: " << WSAGetLastError();
    return 2;
  }
  return 0;
}

bool Pingwin::get_ping_result(int* index, int* result)
{
  int fromlen = sizeof(source);
  qDebug() << fromlen;
  if (sd[*index] == INVALID_SOCKET)
  {
    qDebug() << "invalid";
  }
  int bread = recvfrom(sd[*index], (char*)recv_buf, MAX_PING_PACKET_SIZE, 0, (sockaddr*) &source, &fromlen);

  qDebug() << "read bytes: " << bread << inet_ntoa(source.sin_addr);
  if (bread == SOCKET_ERROR)
  {
    if (WSAGetLastError() == WSAEMSGSIZE)
    {
      qDebug() << "buffer too small";
    }
    else
    {
      qDebug() << "error #" << WSAGetLastError();
    }
    //return false;
  }

  // Pull the sequence number out of the ICMP header.  If
  // it's bad, we just complain, but otherwise we take
  // off, because the read failed for some reason.
  unsigned short header_len = ((IPHeader*) recv_buf)->h_len * 4;
  ICMPHeader* icmphdr = (ICMPHeader*) ((char*)recv_buf + header_len);
  if (icmphdr->seq != 0)
  {
    qDebug() << "bad sequence number!";
    return false;
  }

  // Make sure the reply is sane
  if (PACKET_SIZE < header_len + ICMP_MIN)
  {
     qDebug() << "too few bytes from " << inet_ntoa(source.sin_addr);
     return false;
  }
  *index = icmphdr->id;
  qDebug() << "type" << icmphdr->type;
  if (icmphdr->type == ICMP_ECHO_REPLY)
  {
    *result = 0;
  }
  else
  {
    *result = -1;
  }
  return true;
}

USHORT Pingwin::ip_checksum(USHORT* buffer, int size)
{
  unsigned long cksum = 0;

  // Sum all the words together, adding the final byte if size is odd
  while (size > 1)
  {
    cksum += *buffer++;
    size -= sizeof(USHORT);
  }
  if (size)
  {
    cksum += *(UCHAR*) buffer;
  }

  // Do a little shuffling
  cksum = (cksum >> 16) + (cksum & 0xffff);
  cksum += (cksum >> 16);

  // Return the bitwise complement of the resulting mishmash
  return (USHORT) (~cksum);
}

