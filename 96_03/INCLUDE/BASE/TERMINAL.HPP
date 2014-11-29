/*
	CT.NET++ "ComeTogether"
	OS/2 V3.0 , WIN NT 3.51 , WIN 95

	INCLUDE\BASE\TERMINAL.HPP
*/

#ifndef _TERMINAL_HPP_
#define _TERMINAL_HPP_

class Terminal {
public:
// Service-Funktionen
virtual	void	show(char* text);
virtual	void	show(char* format, char* text);
virtual	void	show(char* format, unsigned long value);
virtual	void	show(char* format, unsigned long value1,
						unsigned long value2);

protected:

};

#endif	// _TERMINAL_HPP_
