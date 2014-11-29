/*
	CT.NET++ "ComeTogether"
	OS/2 V3.0 , WIN NT 3.51 , WIN 95

	INCLUDE\BASE\CLISOCK.HPP
*/

#ifndef _CLISOCK_HPP_
#define _CLISOCK_HPP_

class ClientSocket;
typedef ClientSocket*	pClientSocket;
#include <BASE\SOCKET.HPP>

class ClientSocket : public Socket {
public:
				ClientSocket();
virtual		~ClientSocket();

	int		connect();
	int		connect(pSocket server);

};

#endif	// _CLISOCK.HPP


