/*
	CT.NET++ "ComeTogether"
	OS/2 V3.0 , WIN NT 3.51 , WIN 95

	DEMO\MINI\SOCKC.CPP

	TCP/IP Mini-Client
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <BASE\CLISOCK.HPP>		// ClientSocket
#include <BASE\COMMON.HPP>

#define LOGO	"CT.NET++ Mini-Client"

// ein "//*" am Zeilenende markiert Befehle,
// in denen eine 'Socket::SocketException'
// ausgelöst werden kann

void
usage() {
	printf("Aufruf: SOCKC hostname port kommando [text]\n"
		"kommando : '='       - Anzeige\n"
		"           '+' text  - text eintragen\n"
		"           '?' text  - text suchen\n");
	exit(1);
}

int main(int argc, char *argv[]) {
	int ok;
	printf("%s %s\n", LOGO, APINAME);
	if (argc < 4) {
		usage();
	}
	char command = argv[3][0];
	char* pcommandtext = "";
	switch (command) {
		case '=': break;
		case '+': case '?': {
			if (argc > 4) {
				pcommandtext = argv[4];
				break;
			}
		}
		default: usage();
	}
	ClientSocket	client;
	client.sysinit(1 | (1 << 8));

	try {
		client.socket();								//*
		client.Gethostbyname(argv[1]);			//*
		client.Set_sin_addr();
		client.Setin_port((USHORT)atoi(argv[2]));
		client.connect();								//*
		char data[1024];
		char serveradress[20];
		client.Get_inet_addr(serveradress);
		printf("Verbindung zu \"%s\" : %s:%d\n", argv[1],
			serveradress, client.Getin_port());
		client.recv(data, sizeof(data));			//*
		printf("-> : '%s'\n", data);
		sprintf(data, "%c %s", command, pcommandtext);
		client.send(data, strlen(data) + 1);	//*
		printf("<- : '%s'\n", data);
		client.recv(data, sizeof(data));			//*
		printf("-> : '%s'\n", data);
		ok = 0;
	} catch (Socket::SocketException e) {
		char* p;
		switch (e.etyp) {
			case Socket::SocketException::esocket:
				p = "Socket konnte nicht erzeugt werden";
			case Socket::SocketException::egethostbyname:
				p = "Hostname ist unbekannt";
			case Socket::SocketException::econnect:
				p = "Verbindung konnte nicht erzeugt werden";
			case Socket::SocketException::esend:
				p = "Fehler beim Senden";
			case Socket::SocketException::erecv:
				p = "Fehler beim Empfangen";
		}
		printf("%s (%d)\n", p, e.value);
		ok = -1;
	} catch (...) {
		printf("unbekannte Exception\n");
		ok = -1;
	}
	client.close();
	client.sysfree();
	return ok;
}

