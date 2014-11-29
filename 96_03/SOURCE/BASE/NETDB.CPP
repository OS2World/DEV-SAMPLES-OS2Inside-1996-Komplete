/*
	CT.NET++ "ComeTogether"
	OS/2 V3.0 , WIN NT 3.51 , WIN 95

	SOURCE\BASE\NETDB.CPP
*/

#include <netdb.h>

hostent::hostent() {
}

hostent::~hostent() {
}

hostent::operator PCHAR() {
	return h_name;
}

phostent
hostent::Set(phostent p) {
	h_name = p->h_name;
	h_aliases = p->h_aliases;
	h_addrtype = p->h_addrtype;
	h_length = p->h_length;
	h_addr_list = p->h_addr_list;
	return this;
}

phostent
hostent::gethostbyaddr(PCHAR addr, int x, int net) {
	return Set(::gethostbyaddr(addr, x, net));
}

phostent
hostent::gethostbyname(PCHAR name) {
	return Set(::gethostbyname(name));
}

