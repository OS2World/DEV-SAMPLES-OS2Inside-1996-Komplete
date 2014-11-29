/*
   utils.h
*/

#ifndef __UTILS_32H
#define __UTILS_32H

extern "C" {
#include <sys\types.h>

#ifdef __OS2__
ULONG SYSCALL	lswap(ULONG);
USHORT SYSCALL	bswap(USHORT);
int SYSCALL		rexec(PCHAR*, int, PCHAR, PCHAR, PCHAR, PINT);
/* int SYSCALL getpid(void); */
}

/* Definition for bswap */
#define htonl(x)   (lswap(x))
#define ntohl(x)   (lswap(x))
#define htons(x)   (bswap(x))
#define ntohs(x)   (bswap(x))

#endif	// __OS2__

#ifndef X11
#define bzero(x,y) memset((x),'\0',(y))
#define bcopy(x,y,z) memcpy((y),(x),(z))
#define bcmp(x,y,z)  memcmp((y),(x),(z))
#endif /* X11 */
#define ovbcopy(x,y,z) bcopy((x),(y),(z))
#define copyout(x,y,z) memcpy((y),(x),(z))
#define strcasecmp(x,y) strcmpi((x),(y))
#define strncasecmp(x,y,z) strnicmp(x,y,z)

#ifndef __WIN32__
	#define sleep(x) DosSleep(((long)(x))*1000L)
#endif

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#define imin(x,y) MIN((x),(y))
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif
#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif

#define timercmp(t1,t2,op) (((t1)->tv_sec op (t2)->tv_sec) || \
									(((t1)->tv_sec == (t2)->tv_sec) \
									&& ((t1)->tv_usec op (t2)->tv_usec)))
#define random() ((ULONG)rand())
#define srandom(x) srand(x)

#endif /* __UTILS_32H */
