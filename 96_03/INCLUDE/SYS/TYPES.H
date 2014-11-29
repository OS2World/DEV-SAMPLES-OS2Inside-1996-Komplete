/*
	sys\types.h
*/

#ifndef __TYPES_32H
#define __TYPES_32H

#ifdef __OS2__

	#ifdef __BORLANDC__
		#define  SYSCALL	__syscall
	#else
		#define  SYSCALL  _System
	#endif

	#define near
	typedef int*            PINT;
	typedef unsigned long   BOOL;
	typedef	int			DTYP1;

#else

	#define _WINSOCKAPI_
	#include <windows.h>
//	#ifdef __WIN32__
		#define	SYSCALL	PASCAL FAR
//	#else
//		#define	SYSCALL	_pascal far
//	#endif

	typedef	short			DTYP1;

#endif

#define INET
typedef unsigned long   ULONG;
typedef unsigned short  USHORT;
typedef unsigned int    UINT;
typedef unsigned char   UCHAR;
typedef unsigned char*  PUCHAR;
typedef char*           PCHAR;

#ifndef __off_t
	#define __off_t
	typedef long off_t;
#endif /* off_t */

typedef char *caddr_t;
typedef long daddr_t;
#define NIL ((char*) 0)

#define PZERO 0
// #define BSD 43
#include <errno.h>
#include <nerrno.h>
//#include <sys\time.h>
#include <utils.h>

#define MAXHOSTNAMELEN 120
#ifndef MAXPATHLEN
	#define MAXPATHLEN 80
#endif
#define MAXSOCKETS 2048

/*
#ifndef X11
	#define index(string,c) strchr((string),(c))
#endif
*/

#define TRUE 1
#define FALSE 0
#define SIGALRM 0

#endif // __TYPES_32H

