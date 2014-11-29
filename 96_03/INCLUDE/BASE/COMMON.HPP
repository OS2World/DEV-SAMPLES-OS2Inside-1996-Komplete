/*
	CT.NET++ "ComeTogether"
	OS/2 V3.0 , WIN NT 3.51 , WIN 95

	INCLUDE\BASE\COMMON.HPP
*/

#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#define LIBVERSION	"1.1"

#ifdef __OS2__
	#define INCL_DOSSEMAPHORES
	#define INCL_BASE
	#define INCL_DOSPROCESS
	#include <os2.h>

	#define APINAME					"OS/2 Warp"
	#ifdef __BORLANDC__
		#define COMPILERNAME       "BC++ OS/2"
	#else
		#define COMPILERNAME       "IBM VAC++"
	#endif
#else
	#define _WINSOCKAPI_
	#include <windows.h>

	#define APINAME					"Windows NT"
	#define COMPILERNAME          "BC++ 4.52"
#endif

#ifdef __BORLANDC__
	#define  __EXPORT__		_export
	#define STARTTHREAD(function, stacksize, arglist)       \
		_beginthread(function, stacksize, arglist)
	#ifdef __WIN32__
		#define _THREADLINK_   _USERENTRY
	#else
		#define _THREADLINK_
	#endif
#else
	#define  __EXPORT__  	_Export
	#define STARTTHREAD(function, stacksize, arglist)       \
		_beginthread(function, NULL, stacksize, arglist)
	#define _THREADLINK_   _Optlink
#endif

#ifdef __OS2__

	#define SLEEP			::DosSleep
	typedef	int	HANDLE;
	#define	SETTHREADPRIORITY(handle, value)				\
					::DosSetPriority(PRTYS_THREAD,			\
						PRTYC_NOCHANGE, value, handle)

#else

	#define SLEEP	Sleep
	#define	SETTHREADPRIORITY(handle, value)				\
					::SetThreadPriority(handle, value)

#endif

#define O_WRONLY		0x0001
#define O_CREAT		0x0100
#define O_EXCL			0x0400
#define S_IREAD		0x0100
#define S_IWRITE		0x0080
#define SH_DENYRW 0x0010

#ifdef __IBMCPP__
	#define		GET_DRIVE	::_getdrive
	#define		SOPEN			::_sopen
	#define		CLOSE			::_close
#else
	#define		GET_DRIVE	::getdisk
	#define		SOPEN			::sopen
	#define		CLOSE			::close
#endif

#endif	// _COMMON_HPP_

#pragma hdrstop

