/************************ UU2 Rnews ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Main
 *
 *      $Log: main.c $
 *      Revision 1.7  1995/11/29 12:44:26  dz
 *      *** empty log message ***
 *
 *      Revision 1.6  1995/08/07 15:20:41  dz
 *      _RCS_Info
 *
 *      Revision 1.5  1995/08/07 13:27:38  dz
 *      Cleanup
 *
 *      Revision 1.4  1995/08/07 05:57:47  dz
 *      Using class App
 *
 *      Revision 1.3  1995/08/02 13:26:42  dz
 *      Don't exit on wrong args - workaround for
 *      uupc/ext uuxqt bug
 *
 *      Revision 1.2  1995/04/09  21:05:55  dz
 *      IBM C Set version
 *
 *      Revision 1.1  1995/03/11  06:14:45  dz
 *      Initial revision
 *
 *      
 *
 *
\*/

#include	"rnews.h"

#define	OPTS	"file"


static char _RCS_Info[] = "$Header: N:/U1/News/RNews/RCS/main.c 1.7 1995/11/29 12:44:26 dz Exp $";



int
RnewsApp::Run()
	{
	bool	file = No;

	while( 1 )
		{
		switch( GetOpt() )
			{
		case o_Opt:
			if( OptChar == 'f' )
				{
				file = Yes;
				continue;
				}

			if( OptChar == 'i' || OptChar == 'l' || OptChar == 'e' )
				continue;

			// fall through

		case o_Err:
			fprintf( stderr, "Unknown option, ignored\n" );
			continue;

		case o_Arg:
			if( file )
				{
				if( freopen( OptArg, "rb", stdin ) == NULL )	
					fatal( EC_IO_Error, EI_Full, "Can't open file: %s", (const char *)OptArg );

				if( process() == Err )	fatal( EC_Dunno, EI_None, "Can't process input: %s", (const char *)OptArg );

				if( fclose( stdin ) )	fatal( EC_IO_Error, EI_Full, "Can't close file: %s", (const char *)OptArg );

				chmod( OptArg, 0666 );	// Writeable
				if( unlink( OptArg ) )	fatal( EC_IO_Error, EI_Full, "Can't delete file: %s", (const char *)OptArg );
				continue;
				}


		case o_Eof:
			break;

			}
		break;
		}


	if( !file )
		if( process() == Err )
           		fatal( EC_Dunno, EI_None, "Can't process stdin" );

	add_stat_news();

	return 0;
	}




int
main( int ac, char **av )
	{
	U1App::program_name( "U1 Rnews " U1_VER_STR " (" __DATE__ ")" );
	U1App::_announceable = No;
	U1App::_usage = 
		"Personal args:\n"
		"    -f <name>   - Process file <name> instead fo stdin\n"
		"                (Wildcards allowed in <name>)\n"
		"    -i          - Ignored, please don't use\n"
		"    -l          - Ignored, please don't use\n"
		"    -e          - Ignored, please don't use\n"
		"\n"
		"If no name given, processes stdin\n"
		;

	RnewsApp	app;

	app.Setup( ac, av, OPTS );

	return app.Run();
	}






