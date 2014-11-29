/*
	CT.NET++ "ComeTogether"
	OS/2 V3.0 , WIN NT 3.51 , WIN 95

	DEMO\MINI\SOCKS.CPP

	TCP/IP Mini-Server
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <except.h>
#include <fstream.h>

#include <BASE\SRVSOCK.HPP>			// Server-Socket
#include <BASE\COMMON.HPP>

#define LOGO		"CT.NET++ Mini-Server"
#define FILENAME	"DEMO.TXT"

// ein "//*" am Zeilenende markiert Befehle,
// in denen eine 'Socket::SocketException'
// ausgelöst werden kann

int main(int argc, char* argv[]) {
	printf("%s %s\n", LOGO, APINAME);
	if (argc != 2) {
		printf("Aufruf : SOCKS Portnummer\n");
		exit(1);
	}
	ServerSocket	server;
	ClientSocket	client;
	int ok;
	try {
		server.sysinit(1 | (1 << 8));				// TCP/IP Init
		server.socket();								//* Socket erzeugen
		server.Setin_port((USHORT)atoi(argv[1]));		// Port setzen
		server.Set_sin_addr(INADDR_ANY);    	// IP-Adresse
		server.bind();									//* binden
		server.listen();								//* Bereitschaft
		for (;;) {
			server.accept(&client);					// auf Client warten
			if ((int)client == -1) exit(1);
			char clientadress[20];
			client.Get_inet_addr(clientadress);
			printf("Verbindung mit Client : %s:%d [%d]\n",
				clientadress, client.Getin_port(), (int)client);
			try {
				char data[1024];
				sprintf(data, "%s %s", LOGO, APINAME);
				client.send(data, strlen(data) + 1);	//* Logo senden
				client.recv(data, sizeof(data));			//* empfangen
				printf("-> : '%s'\n", data);
				switch (data[0]) {
					case '+': {			// anfügen
						ofstream file(FILENAME,
							ios::out | ios::ate);
						if (file) {
							file << (char*)(data + 2) << endl;
							strcpy(data, "OK");
						} else {
							strcpy(data, "interner Fehler beim speichern");
						}
						file.close();
						break;
					}
					case '=': {			// ausgeben
						ifstream file(FILENAME,
							ios::in | ios::nocreate);
						if (!file) {
							sprintf(data, "es sind keine Informationen vorhanden");
						} else {
							file.get(data, sizeof(data), '\0');
						}
						file.close();
						break;
					}
					case '?': {			// suchen
						ifstream file(FILENAME,
							ios::in | ios::nocreate);
						if (!file) {
							sprintf(data, "es sind keine Informationen vorhanden");
						} else {
							char line[256];
							int found = 0;
							while (!found) {
								file.get(line, sizeof(line), '\n');
								if (file.eof()) break;
								file.get();
								if (!strcmp(data + 2, line))
									found++;
							}
							strcpy(data, (found) ?
								"OK" : "Suchbegriff wurde nicht gefunden");
						}
						file.close();
						break;
					}
					default: {
						sprintf(data, "unbekanntes Kommando");
					}
				}
				client.send(data, strlen(data) + 1);	//* Antwort senden
				printf("<- : '%s'\n", data);
				client.close();
			} catch (Socket::SocketException e) {
				printf("Server-Fehler : %d\n", e.value);
			}
			ok = 0;
		}
	} catch (Socket::SocketException e) {
		char* p;
		switch (e.etyp) {
			case Socket::SocketException::esocket:
				p = "Socket konnte nicht erzeugt werden";
			case Socket::SocketException::elisten:
				p = "listen des Socket schlug fehl";
			case Socket::SocketException::ebind:
				p = "Socket konnte nicht gebunden werden";
		}
		printf("%s (%d)\n", p, e.value);
		ok = -1;
	} catch (...) {
		printf("unbekannte Exception\n");
		ok = -1;
	}
	server.close();
	server.sysfree();
	return ok;
}

