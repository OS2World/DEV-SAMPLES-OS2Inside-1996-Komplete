/*
	CT.NET++ "ComeTogether"
	OS/2 V3.0 , WIN NT 3.51 , WIN 95

	SOURCE\BASE\CLISOCK.CPP
*/

#include <BASE\CLISOCK.HPP>

ClientSocket::ClientSocket() :
	Socket() {
}

ClientSocket::~ClientSocket() {
}

int
ClientSocket::connect() {
	return connect(this);
}

int
ClientSocket::connect(pSocket pserver) {
	int x = ::connect(s, pserver->Get_sockaddr(),
		sizeof(sockaddr_in));
	if (x) throw SocketException(
		SocketException::econnect, GetErrorCode() );
	return x;
}
