/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Main
 *
 *      $Log: main.c $
 *      Revision 1.4  1995/08/07 14:55:01  dz
 *      Using class App
 *
 * Revision 1.3  1995/04/12  17:32:19  dz
 * Bug fixes
 *
 * Revision 1.2  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.1  1995/03/11  18:32:26  dz
 * Initial revision
 *
 *      
 *
\*/

#include	"unbatch.h"
#include	"..\..\version.h"




int
UnbatchApp::Run()
	{
	bool	file = No;
	String	batchdir = conf.newsin_dir;
        verbose = No;

	while( 1 )
		{
		switch( GetOpt() )
			{
		case o_Opt:
			if( OptChar == 'd' )        batchdir = OptArg;
                        else if( OptChar == 'v' )   verbose = Yes;

			// fall through...

		case o_Arg:
		case o_Err:
			fprintf( stderr, "Unknown option, ignored\n" );
			continue;

		case o_Eof:
			break;

			}
		break;
		}

	scan( batchdir );

	return 0;
	}


#define OPTS "vd:"

int
main( int ac, char **av )
	{
	U1App::program_name( "U1 Unbatch " U1_VER_STR " (" __DATE__ ")" );
	U1App::_announceable = Yes;
	U1App::_usage = 
		"Personal args:\n"
		"    -d <dir>    - Unbatch from this directory, not from spool\n"
		"    -v          - Verbose mode\n"

		;

	UnbatchApp	app;

	app.Setup( ac, av, OPTS );

	return app.Run();
	}







