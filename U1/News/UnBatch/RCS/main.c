head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	95.08.07.14.55.01;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.04.12.17.32.19;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.09.18.25.15;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.32.26;	author dz;	state Exp;
branches;
next	;


desc
@UU2 News Unbatcher
@


1.4
log
@Using class App
@
text
@/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Main
 *
 *      $Log: main.c $
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

	while( 1 )
		{
		switch( GetOpt() )
			{
		case o_Opt:
			if( OptChar == 'd' )
				batchdir = OptArg;

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


#define OPTS "d:"

int
main( int ac, char **av )
	{
	UnbatchApp	app;

	U1App::_program_name = "U1 Unbatch " U1_VER_STR " (" __DATE__ ")";
	U1App::_announceable = Yes;
	U1App::_usage = 
		"Personal args:\n"
		"    -d <dir>    - Unbatch from this directory, not from spool\n"

		;

	app.Setup( ac, av, OPTS );

	return app.Run();
	}







#if 0


char _program_name[] = "unbatch " U1_VER_STR " (" __DATE__ ")";


static void		usage( char *msg );
unsigned _stklen	= 12 * 1024;

// from getopt.c

extern char   *optarg;			/* pointer to argument of current option */
extern int		opterr;			/* no error messages	*/

int		getopt(int argc, char *argv[], char *optionS);

#define	OPTS	"d:r"


bool	eat_crlf = No;


void
main( int ac, char **argv )
	{
	set_program_name ( _program_name );


	get_setup();									// Read .CTL file

	const char	*batchdir = conf.newsin_dir.c_str();

	if( conf.announce_module )
		fprintf( stderr, "%s, " U1_COPYRIGHT "\n", _program_name );

	opterr = 0;

	while( 1 )
		{
		switch( getopt( ac, argv, OPTS ) )
			{
		case '?':
			usage("");
			exit(0);

		case 'd':
			batchdir = optarg;
			continue;

		case 'r':
			eat_crlf = Yes;
			continue;


		case EOF:
			break;

			}
		break;
		}

	scan( batchdir );

	exit(0);
	}



		/*************************************************************
							   Usage info
		*************************************************************/


void
usage( char *msg ) {
	printf(
        "U1 Unbatcher vers. " U1_VER_STR " (" __DATE__ ")\n"
		U1_COPYRIGHT "\n"
		"\n"
        "%s\n\n"
		"Usage: unbatch [options]\n"
		"Options:\n"
		"	-dbatch_dir     - Batch files directory\n"
		"	-r              - Treat CRLF as 1 byte\n"
        ,
		msg
		);
	}


#endif



@


1.3
log
@Bug fixes
@
text
@d8 3
d24 66
d177 1
a177 1

@


1.2
log
@IBM C Set version. Phew, it was not a piece of cake to get here...
@
text
@d8 3
d21 1
a21 1
char program_name[] = "unbatch " U1_VER_STR " (" __DATE__ ")";
d43 3
d51 1
a51 1
		fprintf( stderr, "%s, copyright (C) 1991,92 by Dmitry Zavalishin\n", program_name );
@


1.1
log
@Initial revision
@
text
@d7 4
a10 19
 *      $Log:   Q:/news/unbatch/vcs/main.c_v  $
 *      
 *         Rev 1.5   10 Mar 1993 14:04:58   dz
 *      Stack size increased.
 *      
 *         Rev 1.4   28 Nov 1992 22:30:00   dz
 *      Announce
 *      
 *         Rev 1.3   11 Oct 1992 05:47:52   dz
 *      Usage text corrections
 *      
 *         Rev 1.2   18 Jun 1992 11:13:58   dz
 *      Stack size decreased :(
 *      
 *         Rev 1.1   17 May 1992 11:06:00   dz
 *      Internal packetizer, kiae-specific unbatcher
 *      
 *         Rev 1.0   21 Feb 1992 11:21:02   dz
 *      Initial revision.
@
