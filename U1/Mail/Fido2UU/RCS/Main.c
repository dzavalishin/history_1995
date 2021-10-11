head	1.6;
access;
symbols;
locks;
comment	@ * @;


1.6
date	95.11.06.02.20.45;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	95.11.05.12.56.20;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.08.08.13.22.34;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.02.12.46.06;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.12.21.20.45;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.24.01;	author dz;	state Exp;
branches;
next	;


desc
@@


1.6
log
@Cleanup
@
text
@/************************ FIDO to UUCP convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :       Main
 *
 *      $Log: Main.c $
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

static char _RCS_Info[] = "$Header: D:/U1/Mail/Fido2UU/RCS/Main.c 1.5 1995/11/05 12:56:20 dz Exp $";


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




@


1.5
log
@Save of current (working?) code
@
text
@d8 3
d51 1
a51 1
static char _RCS_Info[] = "$Header: N:/U1/Mail/Fido2UU/RCS/Main.c 1.4 1995/08/08 13:22:34 dz Exp $";
a104 107

#if 0


//extern uint _stklen = (10*1024u);


char _program_name[] = "fido2uu " U1_VER_STR " (" __DATE__ ")";


bool		f_silent		= No;		// Don't chat
bool		f_keeporig		= No;		// Keep original message


static void				usage( char *msg );

void 
main( int ac, char **av )
    {
	set_program_name ( _program_name );

	get_setup();

	if( conf.announce_module )
		fprintf( stderr, "%s, "U1_COPYRIGHT "\n", _program_name );


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
			case 'q':
				f_silent = Yes;
				break;

			case 'k':
				f_keeporig = Yes;
				break;

			default:
				usage("Error: Bad flag\n\n");
				exit(1);
				break;
				} /* switch */
			}
		}

	// Perform conversion

//  debug("Don't delete msg        : %s", f_keeporig    ? "Yes" : "No" );

    //
    //	Check for incompartible flags
    //

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
		"U1 Fido2UU vers. " U1_VER_STR " (" __DATE__ ")\n"
		U1_COPYRIGHT "\n"
		"\n"
        	"%s\n\n"
		"Usage: fido2uu [-flags]\n"
		"Flags:\n"
	        "       -q     \t- Keep silence\n",
		msg
		);
	}


// kludge!! redefines op<, defined in U1..DLL

#if 0
int operator < ( const string &s1, const string &s2 )
	{
	return strcmp( s1.v, s2.v ) < 0;
	}

#endif


#endif
@


1.4
log
@App class used
@
text
@d8 3
d48 1
a48 1
static char _RCS_Info[] = "$Header: D:/U1/News/RNews/RCS/main.c 1.6 1995/08/07 15:20:41 dz Exp $";
d86 1
a86 3
	Fido2uuApp	app;

	U1App::_program_name = "U1 Fido2UU " U1_VER_STR " (" __DATE__ ")";
d92 2
@


1.3
log
@cosmetical...
@
text
@d8 2
a9 2
 * Revision 1.2  1995/04/12  21:20:45  dz
 * IBM C Set version seems to be working...
d11 5
a15 2
 * Revision 1.1  1995/03/11  18:24:01  dz
 * Initial revision
d45 58
d203 4
@


1.2
log
@IBM C Set version seems to be working...
@
text
@d8 3
d42 1
a42 1
extern uint _stklen = (10*1024u);
d45 1
a45 1
char program_name[] = "fido2uu " U1_VER_STR " (" __DATE__ ")";
d57 1
d62 1
a62 1
		fprintf( stderr, "%s, copyright (C) 1991-1993 by Dmitry Zavalishin\n", program_name );
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/fido2uu/vcs/main.c_v  $
d128 10
@
