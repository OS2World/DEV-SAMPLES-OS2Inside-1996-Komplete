/*
   netdb.h

   Copyright (c) 1980,1983,1988 Regents of the University of California.
   @(#)netdb.h     5.9 (Berkeley) 4/5/88
*/

#ifndef __NETDB_32H
#define __NETDB_32H

#include <stdio.h>
#include <string.h>

struct servent;
typedef servent*  pservent;

#include <sys\socket.h>
#include <netinet\in.h>

#define _MAXALIASES     35
#define _MAXADDRS       35
#define _MAXLINELEN     1024
#define _HOSTBUFSIZE    (BUFSIZ + 1)

/*
 * Structures returned by network
 * data base library.  All addresses
 * are supplied in host order, and
 * returned in network order (suitable
 * for use in system calls).
 */
class hostent;
typedef hostent*  phostent;

class hostent {
public:
				hostent();
				~hostent();

				operator PCHAR();
	phostent	Set(phostent p);

	phostent	gethostbyaddr(PCHAR addr,
					int x, int net = AF_INET);
	phostent	gethostbyname(PCHAR addr);

	PCHAR    h_name;        /* official name of host */
	PCHAR*   h_aliases;    /* alias list */
	DTYP1		h_addrtype;     /* host address type */
	DTYP1		h_length;       /* length of address */
	PCHAR*   h_addr_list;  /* list of addresses from name server */
#define h_addr  h_addr_list[0]  /* address, for backward compatiblity */
};

/*
 * Assumption here is that a network number
 * fits in 32 bits -- probably a poor one.
 */
struct netent {
   PCHAR    n_name;        /* official name of net */
   PCHAR*   n_aliases;    /* alias list */
	DTYP1    n_addrtype;     /* net address type */
	ULONG    n_net;          /* network # */
};
typedef netent*  pnetent;

struct servent {
	PCHAR    s_name;        /* official service name */
	PCHAR*   s_aliases;    /* alias list */
	DTYP1    s_port;         /* port # */
	PCHAR    s_proto;       /* protocol to use */
};
typedef servent*  pservent;

struct protoent {
	PCHAR    p_name;        /* official protocol name */
	PCHAR*   p_aliases;    /* alias list */
	DTYP1    p_proto;        /* protocol # */
};
typedef protoent*  pprotoent;

/*
 * After a successful call to gethostbyname_r()/gethostbyaddr_r(), the
 * structure hostent_data will contain the data to which pointers in
 * the hostent structure will point to.
 */

struct hostent_data {
	in_addr     host_addr;              /* host address pointer */
	PCHAR       h_addr_ptrs[_MAXADDRS + 1];    /* host address         */
	char        hostaddr[_MAXADDRS];
	char        hostbuf[_HOSTBUFSIZE + 1];      /* host data            */
	PCHAR       host_aliases[_MAXALIASES];
	PCHAR       host_addrs[2];
	FILE*       hostf;
	int         stayopen;                       /* AIX addon            */
	ULONG       host_addresses[_MAXADDRS];/* As per defect 48367. */
};                                                /*    Actual Addresses. */
typedef hostent_data*   phostent_data;

struct servent_data {          /* should be considered opaque */
   FILE*       serv_fp;
   char        line[_MAXLINELEN];
   PCHAR       serv_aliases[_MAXALIASES];
   int         _serv_stayopen;
};
typedef servent_data*   pservent_data;

extern "C" {
int SYSCALL       gethostbyname_r(PCHAR, phostent, phostent_data);
int SYSCALL       gethostbyaddr_r(PCHAR, int, int, phostent, phostent_data);
int SYSCALL       getservbyname_r(PCHAR, PCHAR, pservent, pservent_data);
int SYSCALL       gethostname(PCHAR, int );
phostent SYSCALL  gethostbyname(PCHAR);
phostent SYSCALL  _gethtbyname(PCHAR);
phostent SYSCALL  gethostbyaddr(PCHAR, int, int);
phostent SYSCALL  _gethtbyaddr(PCHAR, int, int);
pnetent SYSCALL   getnetbyname(PCHAR);
pnetent SYSCALL   getnetbyaddr(ULONG, int);
pservent SYSCALL  getservbyname( PCHAR, PCHAR);
pservent SYSCALL  getservbyport( int, PCHAR);
pservent SYSCALL  getservent();
pprotoent SYSCALL getprotobyname(PCHAR);
pprotoent SYSCALL getprotobynumber(int);
void SYSCALL      sethostent(int);
phostent SYSCALL  gethostent();
void SYSCALL      endhostent();
void SYSCALL      setnetent(int);
pnetent SYSCALL   getnetent();
void SYSCALL      endnetent();
void SYSCALL      setprotoent(int);
pprotoent SYSCALL getprotoent();
void SYSCALL      endprotoent();
void SYSCALL      setservent(int);
pservent SYSCALL	getservent();
void SYSCALL      endservent();

}	// extern "C"

/*
 * Error return codes from gethostbyname() and gethostbyaddr()
 * (left in extern int h_errno).
 */

extern int h_errno;

#define HOST_NOT_FOUND  1 /* Authoritative Answer Host not found */
#define TRY_AGAIN       2 /* Non-Authoritive Host not found, or SERVERFAIL */
#define NO_RECOVERY     3 /* Non recoverable errors, FORMERR, REFUSED, NOTIMP */
#define NO_DATA         4 /* Valid name, no data record of requested type */
#define NO_ADDRESS      NO_DATA         /* no address, look for MX record */

#endif // __NETDB_32H

