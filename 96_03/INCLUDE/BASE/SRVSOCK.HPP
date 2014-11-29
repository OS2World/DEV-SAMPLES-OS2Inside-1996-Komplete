/*
	CT.NET++ "ComeTogether"
	OS/2 V3.0 , WIN NT 3.51 , WIN 95

	INCLUDE\BASE\SRVSOCK.HPP
*/

#ifndef _SRVSOCK_HPP_
#define _SRVSOCK_HPP_

#include <BASE\CLISOCK.HPP>

class ServerSocket : public Socket {
public:
				ServerSocket();
virtual		~ServerSocket();

	int		bind();
	int		bind(pClientSocket client);
	int		listen(int clients = 1);
	int		accept();
	int		accept(pClientSocket client);

};

#endif	// _SRVSOCK.HPP

