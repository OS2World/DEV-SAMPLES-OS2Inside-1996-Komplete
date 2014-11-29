/*
	CT.NET

	netinet\in.h
*/

#ifndef __IN_H
#define __IN_H

extern "C" {
#include <sys\types.h>

/*
 * Constants and structures defined by the internet system,
 * Per RFC 790, September 1981.
 */

/*
 * Protocols
 */
#define IPPROTO_IP              0               /* dummy for IP */
#define IPPROTO_ICMP            1               /* control message protocol */
#define IPPROTO_GGP             3               /* gateway^2 (deprecated) */
#define IPPROTO_TCP             6               /* tcp */
#define IPPROTO_EGP             8               /* exterior gateway protocol */
#define IPPROTO_PUP             12              /* pup */
#define IPPROTO_UDP             17              /* user datagram protocol */
#define IPPROTO_IDP             22              /* xns idp */

#define IPPROTO_RAW             255             /* raw IP packet */
#define IPPROTO_MAX             256


/*
 * Ports < IPPORT_RESERVED are reserved for
 * privileged processes (e.g. root).
 * Ports > IPPORT_USERRESERVED are reserved
 * for servers, not necessarily privileged.
 */
#define IPPORT_RESERVED         1024
#define IPPORT_USERRESERVED     5000

/*
 * Link numbers
 */
#define IMPLINK_IP              155
#define IMPLINK_LOWEXPER        156
#define IMPLINK_HIGHEXPER       158

/*
 * Internet address (a structure for historical reasons)
 */
class in_addr {
public:
			in_addr();
			in_addr(UCHAR i1, UCHAR i2, UCHAR i3, UCHAR i4);
			in_addr(ULONG addr);
			~in_addr();

			void		Set_s_addr(UCHAR i1, UCHAR i2, UCHAR i3, UCHAR i4);
			int		Set_s_addr(PCHAR addrtext);
			ULONG		Set_s_addr(ULONG addr);
			ULONG		Get_s_addr();
			void		Get_inet_addr(PCHAR adress);

			operator ULONG();
			operator = (ULONG addr);

protected:
	union {
		struct { UCHAR s_b1, s_b2, s_b3, s_b4; } S_un_b;
		struct { USHORT s_w1, s_w2; } Sun_w;
		ULONG	s_addr;
	} S_un;
};
typedef in_addr*	pin_addr;
/*
 * Definitions of bits in internet address integers.
 * On subnets, the decomposition of addresses to host and net parts
 * is done according to subnet mask, not the masks here.
 */
#define IN_CLASSA(i)            (((long)(i) & 0x80000000L) == 0)
#define IN_CLASSA_NET           0xff000000L
#define IN_CLASSA_NSHIFT        24
#define IN_CLASSA_HOST          0x00ffffffL
#define IN_CLASSA_MAX           128

#define IN_CLASSB(i)            (((long)(i) & 0xc0000000L) == 0x80000000L)
#define IN_CLASSB_NET           0xffff0000L
#define IN_CLASSB_NSHIFT        16
#define IN_CLASSB_HOST          0x0000ffffL
#define IN_CLASSB_MAX           65536L

#define IN_CLASSC(i)            (((long)(i) & 0xe0000000L) == 0xc0000000L)
#define IN_CLASSC_NET           0xffffff00L
#define IN_CLASSC_NSHIFT        8
#define IN_CLASSC_HOST          0x000000ffL

#define IN_CLASSD(i)            (((long)(i) & 0xf0000000L) == 0xe0000000L)
#define IN_MULTICAST(i)         IN_CLASSD(i)

#define IN_EXPERIMENTAL(i)      (((long)(i) & 0xe0000000L) == 0xe0000000L)
#define IN_BADCLASS(i)          (((long)(i) & 0xf0000000L) == 0xf0000000L)

#define INADDR_ANY              (ULONG)0x00000000L
#define INADDR_BROADCAST        (ULONG)0xffffffffL     /* must be masked */
#ifndef KERNEL
#define INADDR_NONE             0xffffffffL             /* -1 return */
#endif

#define IN_LOOPBACKNET          127                     /* official! */

/*
 * Socket address, internet style.
 */
struct sockaddr_in {
	short    sin_family;
	USHORT   sin_port;
	in_addr  sin_addr;
	char     sin_zero[8];
};
typedef sockaddr_in*	psockaddr_in;

class Sockaddr_in {
public:
				Sockaddr_in();
				~Sockaddr_in();

				operator sockaddr_in();
				operator psockaddr_in();

	USHORT	Setin_port(USHORT port);
	USHORT	Getin_port();
	ULONG		Set_sin_addr(ULONG addr = INADDR_ANY);
	ULONG		Set_sin_addr(PCHAR target);
	ULONG		Get_sin_addr();
	PCHAR		Get_in_addr();
	short		Set_sin_family(short family = AF_INET);
	short		Get_sin_family();
	void		Get_inet_addr(PCHAR adress);

protected:
	sockaddr_in	data;
};
typedef Sockaddr_in*	pSockaddr_in;

/*
 * Options for use with [gs]etsockopt at the IP level.
 */
#define IP_OPTIONS      1               /* set/get IP per-packet options */

in_addr SYSCALL   inet_makeaddr(ULONG, ULONG);
ULONG SYSCALL     inet_network(PCHAR);
PCHAR SYSCALL     inet_ntoa(in_addr);
ULONG SYSCALL     inet_lnaof(in_addr);
ULONG SYSCALL     inet_netof(in_addr);

}
#endif	// __IN_H

