/*
	CT.NET++ "ComeTogether"
	OS/2 V3.0 , WIN NT 3.51 , WIN 95

	INCLUDE\BASE\SOCKET.HPP
*/

#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_

#ifdef __WIN32__
	#define _WINSOCKAPI_
	#include <windows.h>
#endif

#pragma hdrstop

extern "C" {

#include <sys\socket.h>
#include <netinet\in.h>
#include <netdb.h>

}

// *** Class Socket ***************************************
class Socket;
typedef Socket* pSocket;

class Socket {
public:

	// *** Class SocketException ***************************
	class SocketException {
	public:
		enum	excepttyp {
					esocket, elisten, ebind,
					esend, erecv,
					econnect, egethostbyname
		};

		SocketException(excepttyp typ, int errorvalue);

		excepttyp	etyp;
		int			value;
};

				Socket();
virtual		~Socket();

virtual int	sysinit(USHORT version);
virtual int sysfree();

	int		Set_socket(int sock);
	int		Get_socket();
	int		close();
	int		Gethostname(PCHAR hostname, UINT size);
	phostent	Gethostbyaddr(PCHAR hostaddress, int type = PF_INET);
	phostent	Gethostbyname(PCHAR hostname);
	phostent	Get_phost();
	phostent	Sethostent(phostent phost);
	pservent	Getservbyname(PCHAR, PCHAR);
	USHORT		Getin_port();
	void		Setin_port(USHORT port);
	ULONG		Set_sin_addr();
	ULONG		Set_sin_addr(ULONG addr);
	ULONG		Set_sin_addr(PCHAR target);
	BOOL		Set_address(USHORT port, PCHAR target);
	ULONG		Get_sin_addr();
	short		Set_sin_family(short family = AF_INET);
	short		Get_sin_family();

	int		socket(int net = AF_INET, int typ = SOCK_STREAM,
					int x = 0);
	int		send(PCHAR buffer, int size, int options = 0);
	int		recv(PCHAR buffer, int size, int options = 0);
	ULONG		Getaddr_list(UINT index = 0);
	int		GetErrorCode();
	void		Get_inet_addr(PCHAR adress);
	psockaddr	Get_sockaddr();
	int		setsockopt(int, int, PCHAR, int);
	int		recvfrom(PCHAR, int, int, psockaddr, PINT);
	int		sendto(PCHAR, int, int, psockaddr, int);

	operator =(int handle);
	operator int();
	operator Sockaddr_in();
	operator pSockaddr_in();

protected:
	Sockaddr_in	sockadd;
	phostent	phost;
	int		s;
};

#endif	// _SOCKET_HPP_

