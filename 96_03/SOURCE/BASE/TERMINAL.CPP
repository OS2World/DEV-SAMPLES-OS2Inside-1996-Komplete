/*
	CT.NET++ "ComeTogether"
	OS/2 V3.0 , WIN NT 3.51 , WIN 95

	SOURCE\BASE\TERMINAL.CPP
*/

#include <stdio.h>

#include <BASE\TERMINAL.HPP>

void
Terminal::show(char* text) {
	printf(text);
	fflush(stdout);
}

void
Terminal::show(char* format, char* text) {
	printf(format, text);
	fflush(stdout);
}

void
Terminal::show(char* format, unsigned long value) {
	printf(format, value);
	fflush(stdout);
}

void
Terminal::show(char* format, unsigned long value1,
		unsigned long value2) {
	printf(format, value1, value2);
	fflush(stdout);
}
