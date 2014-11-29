/*
	CT.NET++ "ComeTogether"
	OS/2 V3.0 , WIN NT 3.51 , WIN 95

	INCLUDE\BASE\BASEAPP.HPP
*/

#ifndef _BASEAPP_HPP_
#define _BASEAPP_HPP_

class BaseApp {

public:
					BaseApp();
virtual			~BaseApp();

virtual	int	run();
virtual	int	run(int margc, char** margv);
virtual	int	parsearguments();
virtual	int	startup();
virtual	void	cleanup();
virtual	int	mainloop();

			void	SetArguments(int margc, char** margv);

protected:

virtual	int	init();
virtual	int	done();

			int	runstatus;
			int	argc;
			char** argv;
};

#endif	// _BASEAPP_HPP_
