/*
	CT.NET++ "ComeTogether"
	OS/2 V3.0 , WIN NT 3.51 , WIN 95

	SOURCE\BASE\SRVSOCK.CPP
*/

#include <BASE\SRVSOCK.HPP>

ServerSocket::ServerSocket() :
	Socket() {
}

ServerSocket::~ServerSocket() {
}

int
ServerSocket::bind() {
	int x = ::bind(s, (psockaddr)&sockadd,
		sizeof(sockaddr_in));
	if (x) throw SocketException(
		SocketException::ebind, GetErrorCode() );
	return x;
}

int
ServerSocket::bind(pClientSocket pclient) {
	int x = ::bind(s, pclient->Get_sockaddr(),
		sizeof(sockaddr_in));
	if (x) throw SocketException(
		SocketException::ebind, GetErrorCode() );
	return x;
}

int
ServerSocket::listen(int clients) {
	int x = ::listen(s, clients);
	if (x) throw SocketException(
		SocketException::elisten, GetErrorCode() );
	return x;
}

int
ServerSocket::accept() {
int len = sizeof(sockaddr_in);
	return s = ::accept(s, (psockaddr)&sockadd,
		(int near*)&len);
}

int
ServerSocket::accept(pClientSocket pclient) {
int len = sizeof(sockaddr_in);
	return pclient->Set_socket(::accept(s,
		pclient->Get_sockaddr(), (int near*)&len));
}
