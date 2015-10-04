//using namespace std;

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <QDebug>
#include "pingwin.h"

#define DEFAULT_PACKET_SIZE 32
#define DEFAULT_TTL 30
#define MAX_PING_DATA_SIZE 1024
#define MAX_PING_PACKET_SIZE (MAX_PING_DATA_SIZE + sizeof(IPHeader))

int allocate_buffers(ICMPHeader*& send_buf, IPHeader*& recv_buf,
        int packet_size);

Pingwin::Pingwin()
{
}

int Pingwin::ping(char *ip)
{
  int seq_no = 0;
      ICMPHeader* send_buf = 0;
      IPHeader* recv_buf = 0;
      // Figure out how big to make the ping packet
          int packet_size = DEFAULT_PACKET_SIZE;
          int ttl = DEFAULT_TTL;
          packet_size = max(sizeof(ICMPHeader),
                      min(MAX_PING_DATA_SIZE, (unsigned int)packet_size));

              // Start Winsock up
              WSAData wsaData;
              if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0) {
                  qDebug() << "Failed to find Winsock 2.1 or better.";
                  return 1;
              }

              // Set up for pinging
              SOCKET sd;
              sockaddr_in dest, source;
              if (setup_for_ping(ip, ttl, sd, dest) < 0) {
                  goto cleanup;
              }
              if (allocate_buffers(send_buf, recv_buf, packet_size) < 0) {
                  goto cleanup;
              }
              init_ping_packet(send_buf, packet_size, seq_no);

              // Send the ping and receive the reply
              if (send_ping(sd, dest, send_buf, packet_size) >= 0) {
                  while (1) {
                      // Receive replies until we either get a successful read,
                      // or a fatal error occurs.
                      if (recv_ping(sd, source, recv_buf, MAX_PING_PACKET_SIZE) <
                              0) {
                          // Pull the sequence number out of the ICMP header.  If
                          // it's bad, we just complain, but otherwise we take
                          // off, because the read failed for some reason.
                          unsigned short header_len = recv_buf->h_len * 4;
                          ICMPHeader* icmphdr = (ICMPHeader*)
                                  ((char*)recv_buf + header_len);
                          if (icmphdr->seq != seq_no) {
                              qDebug() << "bad sequence number!";
                              continue;
                          }
                          else {
                              break;
                          }
                      }
                      int result = decode_reply(recv_buf, packet_size, &source);
                      if (result == 0) {
                          return 0;
                      }
                      if (result == -1) {
                          return 2;
                      }
                  }
              }

          cleanup:
              delete[]send_buf;
              delete[]recv_buf;
              WSACleanup();

  return false;
}

//////////////////////////// setup_for_ping ////////////////////////////
// Creates the Winsock structures necessary for sending and recieving
// ping packets.  host can be either a dotted-quad IP address, or a
// host name.  ttl is the time to live (a.k.a. number of hops) for the
// packet.  The other two parameters are outputs from the function.
// Returns < 0 for failure.

int Pingwin::setup_for_ping(char* host, int ttl, SOCKET& sd, sockaddr_in& dest)
{
    // Create the socket
    sd = WSASocket(AF_INET, SOCK_RAW, IPPROTO_ICMP, 0, 0, 0);
    if (sd == INVALID_SOCKET) {
        qDebug() << "Failed to create raw socket: " << WSAGetLastError();
        return -1;
    }

    if (setsockopt(sd, IPPROTO_IP, IP_TTL, (const char*)&ttl,
            sizeof(ttl)) == SOCKET_ERROR) {
        qDebug() << "TTL setsockopt failed: " << WSAGetLastError();
        return -1;
    }

    // Initialize the destination host info block
    memset(&dest, 0, sizeof(dest));

    // Turn first passed parameter into an IP address to ping
    unsigned int addr = inet_addr(host);
    if (addr != INADDR_NONE) {
        // It was a dotted quad number, so save result
        dest.sin_addr.s_addr = addr;
        dest.sin_family = AF_INET;
    }
    else {
        // Not in dotted quad form, so try and look it up
        hostent* hp = gethostbyname(host);
        if (hp != 0) {
            // Found an address for that host, so save it
            memcpy(&(dest.sin_addr), hp->h_addr, hp->h_length);
            dest.sin_family = hp->h_addrtype;
        }
        else {
            // Not a recognized hostname either!
            qDebug() << "Failed to resolve " << host;
            return -1;
        }
    }

    return 0;
}

USHORT Pingwin::ip_checksum(USHORT* buffer, int size)
{
    unsigned long cksum = 0;

    // Sum all the words together, adding the final byte if size is odd
    while (size > 1) {
        cksum += *buffer++;
        size -= sizeof(USHORT);
    }
    if (size) {
        cksum += *(UCHAR*)buffer;
    }

    // Do a little shuffling
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);

    // Return the bitwise complement of the resulting mishmash
    return (USHORT)(~cksum);
}

/////////////////////////// init_ping_packet ///////////////////////////
// Fill in the fields and data area of an ICMP packet, making it
// packet_size bytes by padding it with a byte pattern, and giving it
// the given sequence number.  That completes the packet, so we also
// calculate the checksum for the packet and place it in the appropriate
// field.

void Pingwin::init_ping_packet(ICMPHeader* icmp_hdr, int packet_size, int seq_no)
{
    // Set up the packet's fields
    icmp_hdr->type = ICMP_ECHO_REQUEST;
    icmp_hdr->code = 0;
    icmp_hdr->checksum = 0;
    icmp_hdr->id = (USHORT)GetCurrentProcessId();
    icmp_hdr->seq = seq_no;
    icmp_hdr->timestamp = GetTickCount();

    // "You're dead meat now, packet!"
    const unsigned long int deadmeat = 0xDEADBEEF;
    char* datapart = (char*)icmp_hdr + sizeof(ICMPHeader);
    int bytes_left = packet_size - sizeof(ICMPHeader);
    while (bytes_left > 0) {
        memcpy(datapart, &deadmeat, min(int(sizeof(deadmeat)),
                bytes_left));
        bytes_left -= sizeof(deadmeat);
        datapart += sizeof(deadmeat);
    }

    // Calculate a checksum on the result
    icmp_hdr->checksum = ip_checksum((USHORT*)icmp_hdr, packet_size);
}

/////////////////////////////// send_ping //////////////////////////////
// Send an ICMP echo ("ping") packet to host dest by way of sd with
// packet_size bytes.  packet_size is the total size of the ping packet
// to send, including the ICMP header and the payload area; it is not
// checked for sanity, so make sure that it's at least
// sizeof(ICMPHeader) bytes, and that send_buf points to at least
// packet_size bytes.  Returns < 0 for failure.

int Pingwin::send_ping(SOCKET sd, const sockaddr_in& dest, ICMPHeader* send_buf,
        int packet_size)
{
    // Send the ping packet in send_buf as-is
    qDebug() << "Sending " << packet_size << " bytes to " <<
            inet_ntoa(dest.sin_addr) << "..." << flush;
    int bwrote = sendto(sd, (char*)send_buf, packet_size, 0,
            (sockaddr*)&dest, sizeof(dest));
    if (bwrote == SOCKET_ERROR) {
        qDebug() << "send failed: " << WSAGetLastError();
        return -1;
    }
    else if (bwrote < packet_size) {
        qDebug() << "sent " << bwrote << " bytes..." << flush;
    }

    return 0;
}


/////////////////////////////// recv_ping //////////////////////////////
// Receive a ping reply on sd into recv_buf, and stores address info
// for sender in source.  On failure, returns < 0, 0 otherwise.
//
// Note that recv_buf must be larger than send_buf (passed to send_ping)
// because the incoming packet has the IP header attached.  It can also
// have IP options set, so it is not sufficient to make it
// sizeof(send_buf) + sizeof(IPHeader).  We suggest just making it
// fairly large and not worrying about wasting space.

int Pingwin::recv_ping(SOCKET sd, sockaddr_in& source, IPHeader* recv_buf,
        int packet_size)
{
    // Wait for the ping reply
    int fromlen = sizeof(source);
    int bread = recvfrom(sd, (char*)recv_buf,
            packet_size + sizeof(IPHeader), 0,
            (sockaddr*)&source, &fromlen);
    if (bread == SOCKET_ERROR) {
        qDebug() << "read failed: ";
        if (WSAGetLastError() == WSAEMSGSIZE) {
            qDebug() << "buffer too small";
        }
        else {
            qDebug() << "error #" << WSAGetLastError();
        }
        return -1;
    }

    return 0;
}


///////////////////////////// decode_reply /////////////////////////////
// Decode and output details about an ICMP reply packet.  Returns -1
// on failure, -2 on "try again" and 0 on success.

int Pingwin::decode_reply(IPHeader* reply, int bytes, sockaddr_in* from)
{
    // Skip ahead to the ICMP header within the IP packet
    unsigned short header_len = reply->h_len * 4;
    ICMPHeader* icmphdr = (ICMPHeader*)((char*)reply + header_len);

    // Make sure the reply is sane
    if (bytes < header_len + ICMP_MIN) {
        qDebug() << "too few bytes from " << inet_ntoa(from->sin_addr);
        return -1;
    }
    else if (icmphdr->type != ICMP_ECHO_REPLY) {
        if (icmphdr->type != ICMP_TTL_EXPIRE) {
            if (icmphdr->type == ICMP_DEST_UNREACH) {
                qDebug() << "Destination unreachable";
            }
            else {
                qDebug() << "Unknown ICMP packet type " << int(icmphdr->type) <<
                        " received";
            }
            return -1;
        }
        // If "TTL expired", fall through.  Next test will fail if we
        // try it, so we need a way past it.
    }
    else if (icmphdr->id != (USHORT)GetCurrentProcessId()) {
        // Must be a reply for another pinger running locally, so just
        // ignore it.
        return -2;
    }

    // Figure out how far the packet travelled
    int nHops = int(256 - reply->ttl);
    if (nHops == 192) {
        // TTL came back 64, so ping was probably to a host on the
        // LAN -- call it a single hop.
        nHops = 1;
    }
    else if (nHops == 128) {
        // Probably localhost
        nHops = 0;
    }

    // Okay, we ran the gamut, so the packet must be legal -- dump it
    qDebug() << bytes << " bytes from " <<
            inet_ntoa(from->sin_addr) << ", icmp_seq " <<
            icmphdr->seq << ", ";
    if (icmphdr->type == ICMP_TTL_EXPIRE) {
        qDebug() << "TTL expired.";
    }
    else {
        qDebug() << nHops << " hop" << (nHops == 1 ? "" : "s");
        qDebug() << ", time: " << (GetTickCount() - icmphdr->timestamp) <<
                " ms.";
    }

    return 0;
}
/////////////////////////// allocate_buffers ///////////////////////////
// Allocates send and receive buffers.  Returns < 0 for failure.

int allocate_buffers(ICMPHeader*& send_buf, IPHeader*& recv_buf,
        int packet_size)
{
    // First the send buffer
    send_buf = (ICMPHeader*)new char[packet_size];
    if (send_buf == 0) {
        qDebug() << "Failed to allocate output buffer.";
        return -1;
    }

    // And then the receive buffer
    recv_buf = (IPHeader*)new char[MAX_PING_PACKET_SIZE];
    if (recv_buf == 0) {
        qDebug() << "Failed to allocate output buffer.";
        return -1;
    }

    return 0;
}
