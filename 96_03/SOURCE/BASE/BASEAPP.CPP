/*
	CT.NET++ "ComeTogether"
	OS/2 V3.0 , WIN NT 3.51 , WIN 95

	SOURCE\BASE\BASEAPP.CPP
*/

#include <stdio.h>

#include <BASE\BASEAPP.HPP>

BaseApp::BaseApp() {
	runstatus = init();
}

BaseApp::~BaseApp() {
	done();
}

int
BaseApp::init() {
	return 0;
}

int
BaseApp::done() {
	return 0;
}

int
BaseApp::run(int margc, char** margv) {
	SetArguments(margc, margv);
	if ((runstatus = parsearguments()) == 0)
		runstatus = run();
	return runstatus;
}

int
BaseApp::run() {
	if (!runstatus) runstatus = startup();
	if (!runstatus) runstatus = mainloop();
	cleanup();
	return runstatus;
}

int
BaseApp::startup() {
	return 0;
}

void
BaseApp::cleanup() {
}

int
BaseApp::mainloop() {
	return 0;
}

void
BaseApp::SetArguments(int margc, char** margv) {
	argc = margc;
	argv = margv;
}

int
BaseApp::parsearguments() {
	return 0;
}
