/************************ FIDO to UUCP convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :       Main
 *
 *      $Log: Main.c $
 *      Revision 1.6  1995/11/06 02:20:45  dz
 *      Cleanup
 *
 *      Revision 1.5  1995/11/05 12:56:20  dz
 *      Save of current (working?) code
 *
 *      Revision 1.4  1995/08/08 13:22:34  dz
 *      App class used
 *
 *      Revision 1.3  1995/08/02 12:46:06  dz
 *      cosmetical...
 *
 *      Revision 1.2  1995/04/12  21:20:45  dz
 *      IBM C Set version seems to be working...
 *
 *      Revision 1.1  1995/03/11  18:24:01  dz
 *      Initial revision
 *
 *      
 *         Rev 1.6   07 Jun 1993 16:37:00   dz
 *      Copyright
 *      
 *         Rev 1.5   28 Nov 1992 22:54:32   dz
 *      Announce
 *      
 *         Rev 1.4   23 Oct 1992 14:31:54   dz
 *      usage
 *      
 *         Rev 1.3   12 Jul 1992 13:33:00   dz
 *      Stack size increased to 10K
 *      
 *         Rev 1.2   18 Jun 1992 10:31:32   dz
 *      Copyright change, cosmetics
 *      
 *         Rev 1.1   29 Aug 1991 06:41:22   dz
 *      Checkpoint.
 *      
 *         Rev 1.0   27 Aug 1991 02:47:16   dz
 *      Initial revision.
 *
 *
\*/

#include		"fido2uu.h"
#include		"..\..\version.h"

static char _RCS_Info[] = "$Header: D:/U1/Mail/Fido2UU/RCS/Main.c 1.6 1995/11/06 02:20:45 dz Exp $";


int
Fido2uuApp::Run()
	{
	bool	file = No;

	while( 1 )
		{
		switch( GetOpt() )
			{
		case o_Opt:
		case o_Arg:
		case o_Err:
			fprintf( stderr, "Unknown option, ignored\n" );
			continue;


		case o_Eof:
			break;

			}
		break;
		}

	mainloop();

	return 0;
	}


#define	OPTS	""


int
main( int ac, char **av )
	{
	U1App::program_name( "U1 Fido2UU " U1_VER_STR " (" __DATE__ ")" );
	U1App::_announceable = Yes;
	U1App::_usage = 
		"Personal args:\n"
		"    None\n"
		;

	Fido2uuApp	app;

	app.Setup( ac, av, OPTS );

	return app.Run();
	}




