/*
	sys\socket.h

	Copyright (c) 1982, 1985, 1986 Regents of the University of California.
	@(#)socket.h    7.2 (Berkeley) 12/30/87
*/

#ifndef __SOCKET_32H
#define __SOCKET_32H

#include <sys\types.h>

/*
 * Address families.
 */
#define AF_UNSPEC       0		// unspecified
#define AF_UNIX         1     // local to host (pipes, portals)
#define AF_INET         2     // internetwork: UDP, TCP, etc.
#define AF_IMPLINK      3     // arpanet imp addresses
#define AF_PUP          4     // pup protocols: e.g. BSP
#define AF_CHAOS        5     // mit CHAOS protocols
#define AF_NS           6     // XEROX NS protocols
#define AF_NBS          7     // nbs protocols
#define AF_ECMA         8     // european computer manufacturers
#define AF_DATAKIT      9     // datakit protocols
#define AF_CCITT        10    // CCITT protocols, X.25 etc
#define AF_SNA          11    // IBM SNA
#define AF_DECnet       12    // DECnet
#define AF_DLI          13    // Direct data link interface
#define AF_LAT          14    // LAT
#define AF_HYLINK       15    // NSC Hyperchannel
#define AF_APPLETALK    16    // Apple Talk
#define AF_NETBIOS		17		// NetBIOS-style addresses
#define AF_VOICEVIEW		18		// VoiceView
#define AF_MAX          19

#include <netdb.h>

#define INVALID_SOCKET	(int)(~0)
#define SOCKET_ERROR		(-1)
/*
 * Definitions related to sockets: types, address families, options.
 */

/*
 * Types
 */
#define SOCK_STREAM     1               /* stream socket */
#define SOCK_DGRAM      2               /* datagram socket */
#define SOCK_RAW        3               /* raw-protocol interface */
#define SOCK_RDM        4               /* reliably-delivered message */
#define SOCK_SEQPACKET  5               /* sequenced packet stream */

/*
 * Option flags per-socket.
 */
#define SO_DEBUG        0x0001          /* turn on debugging info recording */
#define SO_ACCEPTCONN   0x0002          /* socket has had listen() */
#define SO_REUSEADDR    0x0004          /* allow local address reuse */
#define SO_KEEPALIVE    0x0008          /* keep connections alive */
#define SO_DONTROUTE    0x0010          /* just use interface addresses */
#define SO_BROADCAST    0x0020          /* permit sending of broadcast msgs */
#define SO_USELOOPBACK  0x0040          /* bypass hardware when possible */
#define SO_LINGER       0x0080          /* linger on close if data present */
#define SO_OOBINLINE    0x0100          /* leave received OOB data in line */

/*
 * Additional options, not kept in so_options.
 */
#define SO_SNDBUF       0x1001          /* send buffer size */
#define SO_RCVBUF       0x1002          /* receive buffer size */
#define SO_SNDLOWAT     0x1003          /* send low-water mark */
#define SO_RCVLOWAT     0x1004          /* receive low-water mark */
#define SO_SNDTIMEO     0x1005          /* send timeout */
#define SO_RCVTIMEO     0x1006          /* receive timeout */
#define SO_ERROR        0x1007          /* get error status and clear */
#define SO_TYPE         0x1008          /* get socket type */

/*
 * Structure used for manipulating linger option.
 */
struct linger {
	DTYP1	l_onoff;                /* option on/off */
	DTYP1	l_linger;               /* linger time */
};

/*
 * Level number for (get/set)sockopt() to apply to socket itself.
 */
#define SOL_SOCKET      0xffff          /* options for socket level */

/*
 * Structure used by kernel to store most
 * addresses.
 */
struct sockaddr {
	USHORT	sa_family;              /* address family */
	char		sa_data[14];            /* up to 14 bytes of direct address */
};
typedef sockaddr* psockaddr;

/*
 * Structure used by kernel to pass protocol
 * information in raw sockets.
 */
struct sockproto {
	USHORT	sp_family;              /* address family */
	USHORT	sp_protocol;            /* protocol */
};

/*
 * Protocol families, same as address families for now.
 */
#define PF_UNSPEC       AF_UNSPEC
#define PF_UNIX         AF_UNIX
#define PF_INET         AF_INET
#define PF_IMPLINK      AF_IMPLINK
#define PF_PUP          AF_PUP
#define PF_CHAOS        AF_CHAOS
#define PF_NS           AF_NS
#define PF_NBS          AF_NBS
#define PF_ECMA         AF_ECMA
#define PF_DATAKIT      AF_DATAKIT
#define PF_CCITT        AF_CCITT
#define PF_SNA          AF_SNA
#define PF_DECnet       AF_DECnet
#define PF_DLI          AF_DLI
#define PF_LAT          AF_LAT
#define PF_HYLINK       AF_HYLINK
#define PF_APPLETALK    AF_APPLETALK

#define PF_MAX          AF_MAX

/*
 * Maximum queue length specifiable by listen.
 */
#define SOMAXCONN       5

struct iovec {
	PCHAR    iov_base;
	int      iov_len;
};
typedef iovec* piovec;

struct uio {
	piovec   uio_iov;
	int      uio_iovcnt;
	off_t    uio_offset;
	int      uio_segflg;
	int      uio_resid;
};
typedef uio*   pio;

/*
 * Message header for recvmsg and sendmsg calls.
 */
struct msghdr {
	PCHAR    msg_name;               /* optional address */
	int      msg_namelen;            /* size of address */
	piovec   msg_iov;             /* scatter/gather array */
	int      msg_iovlen;             /* # elements in msg_iov */
	PCHAR    msg_accrights;          /* access rights sent/received */
	int      msg_accrightslen;
};
typedef msghdr*   pmsghdr;

enum    uio_rw { UIO_READ, UIO_WRITE };
#define FREAD  1
#define FWRITE 2

#define MSG_OOB         0x1             /* process out-of-band data */
#define MSG_PEEK        0x2             /* peek at incoming message */
#define MSG_DONTROUTE   0x4             /* send without using routing tables */

#define MSG_MAXIOVLEN   16

// *** Deklaration API-Funktionen ***
extern "C" {

int SYSCALL    accept(int, psockaddr, PINT);
int SYSCALL    bind(int, psockaddr, int);
int SYSCALL    connect(int, psockaddr, int);
int SYSCALL    getpeername(int, psockaddr, PINT);
int SYSCALL    getsockname(int, psockaddr, PINT);
int SYSCALL    getsockopt(int, int, int, PCHAR, PINT);
//pservent SYSCALL	getservbyname(PCHAR, PCHAR);
ULONG SYSCALL	inet_addr(const PCHAR);
int SYSCALL    listen(int, int);
int SYSCALL    recv(int, PCHAR, int, int);
int SYSCALL    recvfrom(int, PCHAR, int, int, psockaddr, PINT);
#ifndef BSD_SELECT
int SYSCALL    select(PINT, int, int, int, long);
#endif
int SYSCALL    send(int, PCHAR, int, int);
int SYSCALL    sendto(int, PCHAR, int, int, psockaddr, int);
int SYSCALL    shutdown(int, int);
int SYSCALL    socket(int, int, int);

#ifdef __OS2__

// *** Deklaration API-Funktionen OS/2 ***

int SYSCALL    gethostid();
//int SYSCALL    ioctl(int, int, PCHAR, int);
int SYSCALL    recvmsg(int, pmsghdr, int);
int SYSCALL    sendmsg(int, pmsghdr, int);
int SYSCALL    setsockopt(int, int, int, PCHAR, int);
int SYSCALL    sock_init();
int SYSCALL    sock_errno();
void SYSCALL   psock_errno(PCHAR);
int SYSCALL    soclose(int);
int SYSCALL    soabort(int);
int SYSCALL    so_cancel(int);
int SYSCALL    readv(int, piovec, int);
int SYSCALL    writev(int, piovec, int);
int SYSCALL    getinetversion(PCHAR);

#else

// *** Deklaration API-Funktionen WIN 32 ***

int SYSCALL		closesocket(int);
int SYSCALL    ioctlsocket(int, long, ULONG);
ULONG SYSCALL	htonl(ULONG);
USHORT SYSCALL	htons(USHORT);
ULONG	SYSCALL	ntohl(ULONG);
USHORT SYSCALL	ntohs(USHORT);

#define WSADESCRIPTION_LEN      256
#define WSASYS_STATUS_LEN       128

struct WSAData {
	USHORT   wVersion;
	USHORT   wHighVersion;
	char     szDescription[WSADESCRIPTION_LEN+1];
	char     szSystemStatus[WSASYS_STATUS_LEN+1];
	USHORT	iMaxSockets;
	USHORT	iMaxUdpDg;
	PCHAR		lpVendorInfo;
};
typedef WSAData*	pWSAData;

int SYSCALL WSAStartup(USHORT wVersionRequired, pWSAData lpWSAData);
int SYSCALL WSACleanup();
int SYSCALL WSAGetLastError();
int SYSCALL WSAAsyncSelect(int, HWND, UINT, long);
}

// flags used with WSAAsyncSelect()
#define FD_READ		0x01
#define FD_WRITE		0x02
#define FD_OOB			0x04
#define FD_ACCEPT		0x08
#define FD_CONNECT	0x10
#define FD_CLOSE		0x20

#endif	// __OS2__
}

#endif /* __SOCKET_32H */

