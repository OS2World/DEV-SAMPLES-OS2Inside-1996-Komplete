/**********************************************************

	CTNET++ "Come Together"
	raoul@ranet.de

	DEMO\SMTP\SMTPD.CPP

	SMTP Daemon Version 1.13 multithread, portabel

	OS/2 Warp				VAC++ 3.0 , BC++ 2.0
	WIN NT 3.51 , WIN95	BC++ 4.52

***********************************************************

+	Daten-Verzeichnis lt. Argument '-f d:\ctnet\mail' verwenden
	(absolute Pfadangabe, mu· bereits existieren)
+	Backspace 0x08 wird in einer nicht leeren Zeile interpretiert
+	LOG - Funktion mit Option -l schreibt SMTPD.LOG
+	Debug-Modus mit Option -d gibt Ablaufmeldungen aus
+	max 25 OK-Names werden beim Start aus OKNAMESFILE gelesen

-	mehrere EmpfÑnger werden nicht unterstÅtzt
-	kein Routen fÅr unbekannte EmpfÑnger

Compilermakros:

				OS/2					WIN NT

				__OS2__				__WIN32__

VAC++			__IBMCPP__
BCOS2			__BORLANDC__
BC++ 4.52							__BORLANDC__

**********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <io.h>
#include <process.h>
#include <fstream.h>

#ifdef __OS2__
	#include <direct.h>
	#define MAXPATH   260
#else
	#include <dir.h>
#endif

#include <BASE\COMMON.HPP>
#include <BASE\BASEAPP.HPP>
#include <BASE\TERMINAL.HPP>
#include <BASE\SRVSOCK.HPP>

#define IDNUMBER		"CBS00163"
#define DEFAULTPORT	25
#define MAXCLIENTS	5
#define OKNAMESFILE	"NAMES.LST"
#define LOGFILENAME	"SMTPD.LOG"

#define MODE_DEBUG	0x0001
#define MODE_LOG		0x0002
#define DEBUG_MODUS	(modus & MODE_DEBUG)
#define LOG_MODUS		(modus & MODE_LOG)

#define OKNAMES_MAX	25
#define OKNAMES_LEN	40
char	oknames[OKNAMES_MAX][OKNAMES_LEN + 1];	// Namen x Zeichen
int	oknames_count = 0;

BOOL	mainrun = TRUE;
UINT	clients = 0;

// *** class Directory ************************************

class Directory {
public:
						Directory();

		int			Get_drive();
		int			Set_directory(char* dir);
		char*			Get_directory();
		char*			Get_currentdir(int drive = 0);
		FILE*			Make_tmpname(char* userpath);

protected:

		int			drive;
		char			directory[MAXPATH];
};

// *** class SMTPD ****************************************

class SMTPDApp;
class SMTPDConnect;

class SMTPD : public ServerSocket, public Directory {
public:
	enum Status {Is_Close = 0, Is_Listen = 1, Is_Running = 2 };

				SMTPD(SMTPDApp* papplication,
					ULONG optionflags = 0);
				~SMTPD();

	int		mainloop();
	SMTPD::Status	Get_status() const;
	void		Set_status(SMTPD::Status st);
	ULONG		Get_modus() const;
	void		Set_modus(ULONG mode);
	SMTPDApp*	Get_papp() const;

protected:

	SMTPDApp*		papp;
	SMTPDConnect*	pconnect;
	SMTPD::Status	status;
	ULONG				modus;
};

// *** class SMTPDConnect *********************************

class SMTPDConnect : public ClientSocket {
public:
	enum Step	{ ZERO = 0, HELO = 1, MAIL = 2,
			  RCPT = 4, DATA = 8 };

				SMTPDConnect(SMTPD* pdaem,
					ULONG optionflags = 0);
	int		mainloop();

protected:

	SMTPD*	pdaemon;
	ULONG		modus;
	SMTPDConnect::Step	step;
	static char* 	commands[];
	char		data[1024];
	char		from_name[128];
	char		to_name[128];
	char		to_domain[128];

	void		Set_modus(ULONG mode);
	ULONG		Get_modus() const;
	int		recvline(BOOL lowercase = TRUE);
	int		send(char* pdata);
	SMTPDConnect::Step	Set_step(SMTPDConnect::Step st);
	SMTPDConnect::Step	Add_step(SMTPDConnect::Step st);
	SMTPDConnect::Step	Get_step() const;
	void			LogData(char* pheader, char* pdata);

	int		do_quit();
	int		do_helo();
	void		do_rset();
	void		do_mail();
	void		do_rcpt();
	void		do_data();

};

// *** class SMTPDApp *************************************

class SMTPDApp : public BaseApp , public Terminal {
public:
				SMTPDApp();
				~SMTPDApp();

	void		Break();

	char		hostname[256];

protected:

	SMTPD*	pserver;
	ULONG		options;

	int		parsearguments();
	int		startup();
	void		cleanup();
	int		mainloop();
};

// *** class SMTPDApp *************************************

SMTPDApp::SMTPDApp() :
	BaseApp(),
	Terminal() {

	pserver = NULL;
	options = 0;
	hostname[0] = '\0';
}

SMTPDApp::~SMTPDApp() {
}

void
SMTPDApp::Break() {
	cleanup();
	exit(1);
}

void
SMTPDApp::cleanup() {
	show("CT.NET++ SMTPD ist beendet\nbye ...\n");
	pserver->close();
	pserver->sysfree();
	delete pserver;
}

int
SMTPDApp::parsearguments() {
	show("CT.NET++ SMTPD %s", APINAME);
	show(" [%s]\n", COMPILERNAME);

	if ((pserver = new SMTPD(this)) == NULL) {
		show("'server' konnte nicht erstellt werden\n");
		return 1;
	}

	if (pserver->sysinit(1 | (1 << 8))) return 1;

	// das aktuelle Arbeitsverzeichnis ermitteln
	pserver->Get_currentdir();

	for (int i = 1; i < argc ; i++) {
		char* arg = argv[i];

		if ((*arg == '-') || (*arg == '/')) {
			arg++;
			if (!strcmp(arg, "?")) {
				show("SMTPD.EXE [Arguments]\n Arguments :\n"
					" -? - Hilfe\n"
					" -d - Debug-Modus\n"
					" -l - Log in SMTPD.LOG schreiben\n"
					" -f - Datenverzeichnis angeben\n");
				return 1;
			} else if (!strcmp(arg, "f")) {
				BOOL ok = 0;
				if ((argc - 1) > i) {
					arg = argv[++i];
					// beginnt das folgende Argument
					// nicht mit '-' , '/' ?
					if ((*arg != '-') && (*arg != '/')) {
						ok = !(pserver->Set_directory(arg));
					} else {
						--i;
					}
				}
				if (!ok) {
					if (options && MODE_DEBUG)
						show("das Verzeichnis-Argument fehlt oder ist fehlerhaft,"
							"verwende aktuelles Verzeichnis\n");
					// wird das fehlende Folge-Argument als Fehler
					// betrachtet, hier abbrechen
					// return 1;
				}

			} else if (!strcmp(arg, "d")) {
				options |= MODE_DEBUG;
				show("Debug-Modus aktiv\n");

			} else if (!strcmp(arg, "l")) {
				options |= MODE_LOG;
				show("LOG-Modus in SMTPD.LOG aktiv\n");

			} else {
				show("Option '-%s' ist unbekannt\n", arg);
			}
		}
	}
	pserver->Set_modus(options);
	if (options && MODE_DEBUG)
		printf("Datenverzeichnis : '%s'\n",
			pserver->Get_directory() );

	// OK-Namen in oknames[][] einlesen
	ifstream file(OKNAMESFILE, ios::in | ios::nocreate);
	if (!file) {
		if (options && MODE_DEBUG)
			printf("Namensdatei '%s' kann nicht gelesen werden\n",
				OKNAMESFILE);
		return 1;
	} else {
		char line[256];
		while (1) {
			file.get(line, sizeof(line), '\n');
			if (file.eof()) break;
			file.get();
			if ((oknames_count) >= OKNAMES_MAX) break;
			if ((strlen(line)) && (line[0] != ';')) {
				strncpy(oknames[oknames_count++], line,
					OKNAMES_LEN);
			}
		}
		file.close();
	}

	return 0;
}

int
SMTPDApp::startup() {
	int ok = 0;
	try {
		pserver->socket();					// Socket erzeugen

		pserver->Gethostname(hostname, sizeof(hostname));
		pserver->Set_address(DEFAULTPORT, hostname);
		char hostadress[20];
		pserver->Get_inet_addr(hostadress);
		printf("Host : %s - %s:%d [%d]\n",
			hostname, hostadress,
			pserver->Getin_port(),
			pserver->Get_socket() );

		pserver->Set_sin_addr(INADDR_ANY);
		pserver->bind();						// mit Port und Adresse binden
		pserver->listen(MAXCLIENTS);		// Bereitschaft
	} catch (Socket::SocketException e) {
		char* p;
		switch (e.etyp) {
			case Socket::SocketException::esocket:
				p = "Socket konnte nicht erzeugt werden.";
			case Socket::SocketException::egethostbyname:
				p = "Hostname ist unbekannt.";
			case Socket::SocketException::ebind:
				p = "Socket konnte nicht gebunden werden.";
			case Socket::SocketException::elisten:
				p = "Listen des Socket schlug fehl.";
		}
		if (!(ok = e.value)) ok = -1;
		printf("%s (%d)\n", p, ok);
	}
	return ok;
}

int
SMTPDApp::mainloop() {
	return pserver->mainloop();
}

// *** class SMTPD ****************************************

char* SMTPDConnect::commands[] = {
	"helo", "mail from:", "rcpt to:",
	"data", "quit", "rset", "noop" };
#define COMMANDCOUNT		6

SMTPD::SMTPD(SMTPDApp* papplication, ULONG optionflags) :
	ServerSocket() , Directory() {

	papp = papplication;
	Set_status(Is_Close);
	modus = optionflags;
	pconnect = NULL;
}

SMTPD::~SMTPD() {
	close();
}

void
SMTPD::Set_modus(ULONG mode) {
	modus = mode;
}

ULONG
SMTPD::Get_modus() const {
	return modus;
}

SMTPD::Status
SMTPD::Get_status() const {
	return status;
}

void
SMTPD::Set_status(Status st) {
	status = st;
}

SMTPDApp*
SMTPD::Get_papp() const {
	return papp;
}

/*** void ClientRunProc(void* param) **********************

	die AusfÅhrungsfunktion einer SMTPD-Connection
	in einem eigenen Thread
*/

void _THREADLINK_
ClientRunProc(void* param) {

	SMTPDConnect*	pconnect = (SMTPDConnect*)param;
	printf("* Connect-Thread startet *\n");
	if (pconnect) {
		clients++;
		int ok = pconnect->mainloop();
		clients--;
		if ((ok == -1) && (clients == 0)) {
			mainrun = FALSE;
		}

		pconnect->close();
		delete pconnect;
	}
	printf("* Connect-Thread endet *\n");
	_endthread();
}

int
SMTPD::mainloop() {
	while (mainrun) {
		if (DEBUG_MODUS)
			papp->show("warte auf Connection ...\n");

		// *** auf Anfrage eines Client warten ***

		Set_status(Is_Listen);
		pconnect = new SMTPDConnect(this, Get_modus());
		accept(pconnect);
		Set_status(Is_Running);
		if (pconnect->Get_socket() != -1) {
			STARTTHREAD(ClientRunProc, 8192, pconnect);
		} else {
			delete pconnect;
		}
	}
	return 0;
}

// *** class SMTPDConnect *********************************

SMTPDConnect::SMTPDConnect(SMTPD* pdaem, ULONG optionflags) :
	ClientSocket() {

	pdaemon = pdaem;

	modus = optionflags;
	Set_step(ZERO);
	data[0] = from_name[0] = to_name[0] = '\0';
}

void
SMTPDConnect::Set_modus(ULONG mode) {
	modus = mode;
}

ULONG
SMTPDConnect::Get_modus() const {
	return modus;
}

SMTPDConnect::Step
SMTPDConnect::Set_step(SMTPDConnect::Step st) {
	Step s = Get_step();
	step = st;
	return s;
}

SMTPDConnect::Step
SMTPDConnect::Add_step(SMTPDConnect::Step st) {
	SMTPDConnect::Step s = Get_step();
	Set_step(SMTPDConnect::Step(s | st));
	return s;
}

SMTPDConnect::Step
SMTPDConnect::Get_step() const {
	return step;
}

void
SMTPDConnect::LogData(char* pheader, char* pdata) {
	FILE* pfile = fopen(LOGFILENAME, "a");
	if (pfile) {
		fprintf(pfile, "%s '%s'\n", pheader, pdata);
		fflush(pfile);
		fclose(pfile);
	}
}

int
SMTPDConnect::recvline(BOOL lowercase) {

//	int	ok = 0;
	int	index = 0;
	BOOL	CRLF = FALSE;
	BOOL	loop = TRUE;
	do {
		if (index >= sizeof(data) - 1) break;
		try {
			char	inp[1];
			int ok = recv((char*)inp, sizeof(inp));
			char ch = inp[0];
			if (CRLF) {
				if (ch == 0x0A) {
					loop = FALSE;
					break;
				}
			} else {
				CRLF = FALSE;
			}
			if (ch == 0x0D) {
				CRLF = TRUE;
				ch = '\0';
			}
			if (lowercase) {
				if ((ch > 64) && (ch < 91)) ch += 32;
			}
			// Backspace Åberspringen
			if (ch == 0x08) {
				if (index) index--;
			} else {
				data[index++] = ch;
			}
		} catch (Socket::SocketException e) {
			SLEEP(50);
		}
	} while (loop);
	data[index] = '\0';
	if (DEBUG_MODUS)
		printf(">> '%s'\n", data);
	if (LOG_MODUS)
		LogData(">>", data);
	return strlen(data);
}

int
SMTPDConnect::send(char* pdata) {
	int ok = 0;
	try {
		ok = ClientSocket::send(pdata, strlen(pdata));
		if (DEBUG_MODUS)
			printf("<< '%s'\n", pdata);
		if (LOG_MODUS)
			LogData("<<", pdata);
	} catch (Socket::SocketException e) {
		if (DEBUG_MODUS)
			printf("Server-Fehler 'send' (%d)\n", e.value);
		if (LOG_MODUS)
			LogData("*Server-Fehler*", "'send'");
	}
	return ok;
}

int
SMTPDConnect::mainloop() {

	sprintf(data, "220 '%s' ready CT.NET++ SMTPD V1.13/MT"
		"%d %s\r\n",
		pdaemon->Get_papp()->hostname, MAXCLIENTS, APINAME);
	send(data);

	if (DEBUG_MODUS) {
		char clientadress[20];
		Get_inet_addr(clientadress);
		printf("Verbindung mit Client : %s:%d [%d]\n",
			clientadress, Getin_port(),
			Get_socket() );
	}

	int ok = 0;
	BOOL run = TRUE;
	while (run) {
		if ((ok = recvline()) > 0) {

			// empfangene Daten auswerten / bearbeiten

			int command = -1;
			for (int i = 0; i <= COMMANDCOUNT; i++) {
				if (!strncmp(commands[i], data, strlen(commands[i]))) {
					command = i;
					break;
				}
			}
			if (command == 4) 				// quit
				return do_quit();

			if (!(Get_step() && HELO)) {
				int x = 0;
				if (command == 0) {			// erstes helo
					x = do_helo();				// 1 -> User OK
				}
				continue;
			};
			switch (command) {
				case 0: {
					do_helo();		// weiteres helo
					break;
				}
				case 1:	{			// mail from
					do_mail();
					break;
				}
				case 2:	{			// rcpt to
					do_rcpt();
					break;
				}
				case 3:	{			// data
					do_data();
					break;
				}
				case 5:	{			// rset
					do_rset();
					break;
				}
				case 6:	{			//	noop
					send("250 OK\r\n");
					break;
				}
				default:	{
					send("502 Kommando ist unbekannt\r\n");
				}
			}
		}
//		else { /* eine leere Kommandozeile */ }
	}
	return ok;
}

int
SMTPDConnect::do_quit() {
	if (DEBUG_MODUS)
		printf("* Session ist beendet *\n");
	sprintf(data, "221 '%s' bye\r\n", pdaemon->Get_papp()->hostname);
	send(data);
	return 221;
}

int
SMTPDConnect::do_helo() {

	int ok = 0;
	char* temp = data + 5;
	if (DEBUG_MODUS)
		printf("*'%s'*\n", temp);

	for (int i = 0; i < oknames_count; i++) {
		char* name = oknames[i];
		printf("vergleiche : '%s' '%s'", name, temp);
		if (!strnicmp(temp, name, strlen(name)) ) {
			strcpy(from_name, temp);
			Set_step(HELO);
			sprintf(data, "250 '%s' OK\r\n", temp);
			send(data);
			ok++;
			break;
		}
	}
	if (!ok) {
		Set_step(ZERO);
		send("501 User-ID ist ungÅltig\r\n");
	}
	return ok;
}

void
SMTPDConnect::do_mail() {
	if ((Get_step() && HELO)) {
		Add_step(MAIL);
		sprintf(data, "250 'mail from:'\r\n");
	} else {
		sprintf(data, "501 erwarte 'helo'\r\n");
	}
	send(data);
}

void
SMTPDConnect::do_rcpt() {
	if ((Get_step() && MAIL)) {
		int	ok = 0;
		char*	pref = strchr(data, '<');
		char* suf  = strchr(data, '>');
		if (pref && suf && (suf > pref)) {
			ok++;
			char* temp = strchr(data, '@');
			if (temp && (temp < suf)) {
				*temp = '\0';
				int x = 0;
				while (*(++pref) == ' ');
				for (int i = 0; i < oknames_count; i++) {
					char* name = oknames[i];
					if (!strnicmp(pref, name, strlen(name)) ) {
						strcpy(to_name, pref);
						x++;
						break;
					}
				}
				// Domain auslesen
				if (x) {
					*suf = '\0';
					strcpy(to_domain, ++temp);
					if (strlen(to_domain)) {
						Add_step(RCPT);
						sprintf(data, "250 rcpt to: '%s@%s' OK\r\n",
							to_name, to_domain);
						send(data);
					} else {
						ok = 0;
					}
				}
				if (!x) {
					send( "550 Adresse ist unbekannt\r\n");
				}
			} else {
				// kein '@' in 'data'; ggf. eine Vorgabe benutzen
				send( "553 Domain-Angabe fehlt\r\n");
			}
		}
		if (!ok) {
			send( "501 Parameter-Angabe ist fehlerhaft\r\n");
		}

	} else {
		send("501 erwarte 'mail from:'\r\n");
	}
}

void
SMTPDConnect::do_data() {
	if ((Get_step() && RCPT)) {
		Add_step(DATA);
		send("354 OK Beginn Dateneingabe, Ende mit <CR><LF>.<CR><LF>\r\n");

		// eine neue Datei mit eindeutigem Name im
		// aktuell eingestellten Datenverzeichnis anlegen
		// Verzeichnis des Users verwenden
		int ok = 0;
		FILE* pfile = pdaemon->Make_tmpname(to_name);
		if (pfile) {

			time_t t = ::time(NULL);

			// Header in Datei schreiben
			fprintf(pfile, "Received: from %s with CT.NET++"
				" SMTPD V1.13/MT%d %s id %s; %s",
				pdaemon->Get_papp()->hostname,
				MAXCLIENTS, APINAME, IDNUMBER, ctime(&t) );
			fprintf(pfile, "From: <%s>\n", from_name);
			fprintf(pfile, "Message-Id: <%lX08.%s@%s>\n",
				(long)t, IDNUMBER, pdaemon->Get_papp()->hostname);
			fprintf(pfile, "Date: %s", ctime(&t) );
			fprintf(pfile, "To: %s@%s\n", to_name, to_domain);

			// Daten empfangen und in Datei schreiben
			for (;;) {
				data[0] = '\0';
				ok = recvline(FALSE);
				if (data[0] == '.') {
					if (data[1] == '\0') {
						break;
					}
				}
				fprintf(pfile, "%s%c%c", data, 0x0D,0x0A);
			}

			// OK, Datei schlie·en
			fflush(pfile);
			fclose(pfile);
			send("250 OK Mail wurde Åbertragen\r\n");
			Set_step(SMTPDConnect::Step(HELO | MAIL));
		} else {
			ok = -1;
		}
		if (ok == -1) {
			send("451 SMTPD interner Fehler\r\n");
		}
	} else {
		send("501 erwarte 'rcpt to:'\r\n");
	}
}

void
SMTPDConnect::do_rset() {
	Set_step(ZERO);
	send("250 'rset' OK\r\n");
}

// *** class Directory ************************************

Directory::Directory() {
	drive = 0;
	directory[0] = '\0';
}

int
Directory::Get_drive() {
	return drive = GET_DRIVE();
}

int
Directory::Set_directory(char* dir) {
	char temp[MAXPATH];
	int ok = 0;
	strncpy(temp, dir, sizeof(temp));
	if ((ok = ::chdir(temp)) == 0) {
		strcpy(directory, dir);
	}
	return ok;
}

char*
Directory::Get_directory() {
	return directory;
}

char*
Directory::Get_currentdir(int drive) {
	char  temp[MAXPATH];
#ifdef __IBMCPP__
	::_getcwd(temp, sizeof(temp));
#else
	::getcurdir(drive, directory);
	sprintf(temp, "%c:\\%s\\", Get_drive() + 97, directory);
#endif
	Set_directory(temp);
	return Get_directory();
}

FILE*
Directory::Make_tmpname(char* userpath) {

	time_t	t = time(NULL);

	FILE*		pfile = NULL;
	char*		filename[MAXPATH];
	for (int i = 0; i < 1000; i++) {
		sprintf((char*)filename,  "%s\\%s\\%.8lx.%03d",
			Get_directory(), userpath, (ULONG)t, i);
		printf("Datendatei : '%s'", filename);

		int fhandle = SOPEN((const char*)filename,
			O_WRONLY | O_CREAT | O_EXCL,
			SH_DENYRW, S_IREAD | S_IWRITE);
		if (fhandle != -1) {
			CLOSE(fhandle);
			pfile = fopen((const char*)filename, "w");
			printf(" - OK\n");
			break;
		} else {
			printf(" - NO\n");
		}
	}
	return pfile;
}

// *** main(...) ******************************************

SMTPDApp		app;

void
AppBreak(int) {
	app.Break();
}

int
main(int argc, char* argv[]) {

#ifdef __BORLANDC__
	signal(SIGINT, (void (*)(int))AppBreak);
#else
	signal(SIGINT, (_SigFunc)AppBreak);
#endif
	return app.run(argc, argv);
}

// *** E O F **********************************************
// SMTPD.CPP (C) 1996 CT.NET++ raoul@ranet.de

