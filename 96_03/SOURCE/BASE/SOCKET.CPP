/*
	CT.NET++ "ComeTogether"
	OS/2 V3.0 , WIN NT 3.51 , WIN 95

	SOURCE\BASE\SOCKET.CPP
*/

#include <BASE\SOCKET.HPP>

// *** class SocketException ********************

Socket::SocketException::SocketException(excepttyp typ,
		int errorvalue) {

	etyp = typ;
	value = errorvalue;
}

// *** class Socket *****************************

Socket::Socket() {
	phost = NULL;
	s = 0;
}

Socket::~Socket() {
	close();
}

int
Socket::close() {
int ok;
	if (s) {
		#ifdef __OS2__
			ok = ::soclose(s);
		#else
			ok = ::closesocket(s);
		#endif
		s = 0;
	}
	return ok;
}

int
Socket::sysinit(USHORT version) {
#ifdef __OS2__
	return ::sock_init();
#else
	WSAData	wsadata;
	return ::WSAStartup(version, &wsadata);
#endif
}

int
Socket::sysfree() {
#ifdef __OS2__
#else
	return ::WSACleanup();
#endif
}

int
Socket::Get_socket() {
	return s;
}

int
Socket::Set_socket(int sock) {
	return s = sock;
}

phostent
Socket::Get_phost() {
	return phost;
}

phostent
Socket::Sethostent(phostent ph) {
	return phost = ph;
}

int
Socket::Gethostname(PCHAR hostname, UINT size) {
	return ::gethostname(hostname, size);
}

phostent
Socket::Gethostbyname(PCHAR hostname) {
	phostent p = ::gethostbyname(hostname);
	if (!p) throw SocketException(
		SocketException::egethostbyname,
		GetErrorCode() );
	return Sethostent(p);
}

phostent
Socket::Gethostbyaddr(PCHAR hostaddress, int type) {
	return Sethostent(::gethostbyaddr(
		hostaddress, 4, type));
}

pservent
Socket::Getservbyname(PCHAR name, PCHAR proto) {
	return ::getservbyname(name, proto);
}

void
Socket::Setin_port(USHORT port) {
	sockadd.Setin_port(port);
}

USHORT
Socket::Getin_port() {
	return sockadd.Getin_port();
}

ULONG
Socket::Set_sin_addr() {
	return (phost) ?
		sockadd.Set_sin_addr(Getaddr_list())
		: NULL;
}

ULONG
Socket::Set_sin_addr(ULONG addr) {
	return sockadd.Set_sin_addr(addr);
}

BOOL
Socket::Set_address(USHORT port, PCHAR target) {
	if ((!Gethostbyname(target)) ||  (!Set_sin_addr())) return TRUE;
	Setin_port(port);
	return FALSE;
}

ULONG
Socket::Set_sin_addr(PCHAR target) {
	return sockadd.Set_sin_addr(target);
}

ULONG
Socket::Get_sin_addr() {
	return sockadd.Get_sin_addr();
}

short
Socket::Set_sin_family(short family) {
	return sockadd.Set_sin_family(family);
}

short
Socket::Get_sin_family() {
	return sockadd.Get_sin_family();
}

ULONG
Socket::Getaddr_list(UINT index) {
	return *((ULONG *)phost->h_addr_list[index]);
}

void
Socket::Get_inet_addr(PCHAR adress) {
	sockadd.Get_inet_addr(adress);
}

int
Socket::socket(int net, int typ, int y) {
	int x = ::socket(net, typ, y);
	if (x == -1) throw SocketException(
		SocketException::esocket, GetErrorCode() );
	return s = x;
}

Socket::operator =(int handle) {
	return s = handle;
}

Socket::operator int() {
	return s;
}

Socket::operator Sockaddr_in() {
	return sockadd;
}

Socket::operator pSockaddr_in() {
	return &sockadd;
}

psockaddr
Socket::Get_sockaddr() {
	return (psockaddr)&sockadd;
}

int
Socket::send(PCHAR buffer, int size, int options) {
	int x = ::send(s, buffer, size, options);
	if (!x) throw SocketException(
		SocketException::esend, GetErrorCode() );
	return x;
}

int
Socket::recv(PCHAR buffer, int size, int options) {
	int x = ::recv(s, buffer, size, options);
	if (!x) throw SocketException(
		SocketException::erecv, GetErrorCode() );
	return x;
}

int
Socket::GetErrorCode() {
	#ifdef __OS2__
		return ::sock_errno();
	#else
		return ::WSAGetLastError();
	#endif
}
