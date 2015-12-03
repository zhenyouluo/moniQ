#include <QTextStream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>

#include "pinglin.h"


Pinglin::Pinglin()
{
  valid = true;
}

bool Pinglin::isValid()
{
  return valid;
}

u_short in_cksum(u_short* addr, int len)
{
  register int nleft = len;
  register u_short *w = addr;
  register u_short answer;
  register int sum = 0;

  while( nleft > 1 )
  {
    sum += *w++;
    nleft -= 2;
  }

  if( nleft == 1 )
  {
    u_short	u = 0;

    *(u_char *)(&u) = *(u_char *)w ;
    sum += u;
  }

  /*
   * add back carry outs from top 16 bits to low 16 bits
   */
  sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
  sum += (sum >> 16);			/* add carry */
  answer = ~sum;				/* truncate to 16 bits */
  return (answer);
}

int Pinglin::ping(char *ip)
{
  QTextStream cout(stdout);
  cout << "pinglin ping" << endl;
  struct protoent *proto;
  int s;			/* Socket file descriptor */
  int datalen = 64-8;
  u_char packet[4096];

  struct sockaddr whereto;
  struct sockaddr from;
  struct sockaddr_in *to = (struct sockaddr_in *) &whereto;
  bzero((char *)&whereto, sizeof(struct sockaddr) );
  to->sin_family = AF_INET;
  to->sin_addr.s_addr = inet_addr(ip);

  if ((proto = getprotobyname("icmp")) == NULL)
  {
    return -1;
  }

  if ((s = socket(AF_INET, SOCK_RAW, proto->p_proto)) < 0)
  {
    return -2;
  }

  static u_char outpack[4096];
  register struct icmp *icp = (struct icmp *) outpack;
  int i, cc;
  register u_char *datap = &outpack[8+sizeof(struct timeval)];

  icp->icmp_type = ICMP_ECHO;
  icp->icmp_code = 0;
  icp->icmp_cksum = 0;
  icp->icmp_seq = 0;
  int id = rand() % 32000;
  icp->icmp_id = id;

  cc = datalen+8;			/* skips ICMP portion */

  for (i = 8; i < datalen; i++)
  {
    /* skip 8 for time */
    *datap++ = i;
  }

  /* Compute ICMP checksum here */
  icp->icmp_cksum = in_cksum( (u_short*) icp, cc );

  /* cc = sendto(s, msg, len, flags, to, tolen) */
  i = sendto(s, outpack, cc, MSG_DONTWAIT, &whereto, sizeof(struct sockaddr));
  if (i < 0)
  {
    return -3;
  }
  if (i != cc)
  {
    return -4;
  }
  sleep(5);
  int len = sizeof(packet);
  int fromlen = sizeof(from);
  while ((cc = recvfrom(s, packet, len, MSG_DONTWAIT, &from, (socklen_t*) &fromlen)) > 0)
  {
    struct ip *ip;
    register struct icmp *icp;
    int hlen;
    ip = (struct ip *) packet;
    hlen = ip->ip_hl << 2;
    if (cc < hlen + ICMP_MINLEN)
    {
      return -5;
    }
    cc -= hlen;
    icp = (struct icmp *)(packet + hlen);
    if ((icp->icmp_type == ICMP_ECHOREPLY) && (id == icp->icmp_id))
    {
      return 0;
    }
       // cout << "cc:" << cc << "fromlen" << fromlen << endl;
       // cout << "id_out: " << id << "id_in: " << icp->icmp_id << endl;
       // cout << "type: " << icp->icmp_type << "echo: " << ICMP_ECHOREPLY << endl;
  }
  return -6;
}

