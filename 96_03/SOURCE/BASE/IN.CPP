/*
		\SOURCE\IN.CPP
*/

#include <stdio.h>

#include <sys\socket.h>
#include <netinet\in.h>

in_addr::in_addr() {
	S_un.s_addr = 0;
}

in_addr::in_addr(UCHAR i1, UCHAR i2, UCHAR i3, UCHAR i4) {
	Set_s_addr(i1, i2, i3, i4);
}

in_addr::in_addr(ULONG addr) {
	S_un.s_addr = htonl(addr);
}

in_addr::~in_addr() {
}

ULONG
in_addr::Set_s_addr(ULONG addr) {
	return S_un.s_addr = addr;
}

ULONG
in_addr::Get_s_addr() {
	return S_un.s_addr;
}

in_addr::operator ULONG() {
	return S_un.s_addr;
}

in_addr::operator = (ULONG addr) {
	return S_un.s_addr = addr;
}

void
in_addr::Set_s_addr(UCHAR i1, UCHAR i2, UCHAR i3, UCHAR i4) {
	S_un.s_addr = htonl((i1 << 24) | (i2 << 16) | (i3 << 8) | i4);
}

int
in_addr::Set_s_addr(PCHAR addrtext) {
UCHAR i1, i2, i3, i4, i;
int ok = -1;
	if (sscanf(addrtext, "%u.%u.%u.%u%c",
		&i1, &i2, &i3, &i4, &i) == 4) {
		Set_s_addr(i1, i2, i3, i4);
		ok = 0;
	}
	return ok;
}

void
in_addr::Get_inet_addr(PCHAR adress) {
#ifdef __IBMCPP__                        // wegen Bug in VAC++ 3.0
	sprintf(adress, "%u.%u.%u.%u",
		(UINT)S_un.S_un_b.s_b1, (UINT)S_un.S_un_b.s_b2,
		(UINT)S_un.S_un_b.s_b3, (UINT)S_un.S_un_b.s_b4);
#else
	sprintf(adress, "%s", ::inet_ntoa((in_addr)ntohl(S_un.s_addr));
#endif
}

//*********************************************************
Sockaddr_in::Sockaddr_in() {
	memset(&data, 0, sizeof(data));
	data.sin_family = AF_INET;
}

Sockaddr_in::~Sockaddr_in() {
}

Sockaddr_in::operator sockaddr_in() {
	return data;
}

Sockaddr_in::operator psockaddr_in() {
	return &data;
}

USHORT
Sockaddr_in::Setin_port(USHORT port) {
	return data.sin_port = htons(port);
}

USHORT
Sockaddr_in::Getin_port() {
	return ntohs(data.sin_port);
}

ULONG
Sockaddr_in::Set_sin_addr(ULONG addr) {
	return data.sin_addr = addr;
}

ULONG
Sockaddr_in::Set_sin_addr(PCHAR target) {
	return data.sin_addr = ::inet_addr(target);
}

ULONG
Sockaddr_in::Get_sin_addr() {
	return data.sin_addr;
}

short
Sockaddr_in::Set_sin_family(short family) {
   return data.sin_family = family;
}

short
Sockaddr_in::Get_sin_family() {
   return data.sin_family;
}

void
Sockaddr_in::Get_inet_addr(PCHAR adress) {
   data.sin_addr.Get_inet_addr(adress);
}

