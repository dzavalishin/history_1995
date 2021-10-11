/************************ Gate remote control ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :       Main
 *
 *      $Log: Main.c $
 *      Revision 1.3  1995/11/06 14:28:09  dz
 *      Current
 *
 *      Revision 1.2  1995/08/02 13:54:02  dz
 *      cosmetical
 *
 *  Revision 1.1  1995/07/01  21:19:55  dz
 *  Initial revision
 *
 *      
 *         Rev 1.3   28 Nov 1992 23:37:04   dz
 *      Announce
 *      
 *         Rev 1.2   23 Oct 1992 15:59:00   dz
 *      Usage
 *      
 *         Rev 1.1   04 Feb 1992 00:18:02   dz
 *      program_name variable added
 *      
 *         Rev 1.0   29 Oct 1991 10:35:34   dz
 *      Initial revision.
 *
 *
\*/

#include		"gremote.h"
#include		"..\..\version.h"

#if 1



static char _RCS_Info[] = "$Header: D:/U1/Tools/Remote/RCS/Main.c 1.3 1995/11/06 14:28:09 dz Exp $";


int
GRemoteApp::Run()
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
	U1App::program_name( "U1 GRemote " U1_VER_STR " (" __DATE__ ")" );
	U1App::_announceable = Yes;
	U1App::_usage = 
		"Personal args:\n"
		"    None\n"
		;

	GRemoteApp	app;

	app.Setup( ac, av, OPTS );

	return app.Run();
	}







#else



char program_name[] = "U1 GRemote " U1_VER_STR " (" __DATE__ ")";

int		_stklen = 8*1024;



static void				usage( char *msg );

void
main( int ac, char **av )
    {

	Register_Gr_Rexx_Subcommand_Handler();

	get_setup();

	if( conf.announce_module )
		fprintf( stderr, "%s " U1_COPYRIGHT "\n", program_name );


	ac--;
	av++;

	while( ac-- )
        {
		char *ap = *av++;
		if( *ap != '-' )
            {
			usage("Error: not a flag on command line\n\n");
			exit(1);
			}

		for( ap++; *ap; ap++ )
            {
			switch( *ap  )
                {
			case 'h':
				usage("");
				exit(0);
				break;

			default:
				usage("Error: Bad flag\n\n");
				exit(1);
				break;
				} /* switch */
			}
		}


	mainloop();

	exit(0);
	}



	/*************************************************************
							 Usage info
	*************************************************************/


void
usage( char *msg )
    {
	printf(
/*
		"Gate remote control unit vers. " U1_VER_STR " (" __DATE__ ")\n"
	        U1_COPYRIGHT "\n"

		"\n"
*/
        	"%s\n\n"
		"Usage: gremote [-flags]\n"
		"Flags:\n"
		"       -h     \t- Help (this screen)\n"
	        "\n",
		msg
		);
	}


#endif

