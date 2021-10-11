head	1.7;
access;
symbols;
locks;
comment	@ * @;


1.7
date	95.11.29.12.44.26;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	95.08.07.15.20.41;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	95.08.07.13.27.38;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.08.07.05.57.47;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.02.13.26.42;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.09.21.05.55;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.06.14.45;	author dz;	state Exp;
branches;
next	;


desc
@@


1.7
log
@*** empty log message ***
@
text
@/************************ UU2 Rnews ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Main
 *
 *      $Log: main.c $
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


static char _RCS_Info[] = "$Header: D:/U1/News/RNews/RCS/main.c 1.6 1995/08/07 15:20:41 dz Exp $";



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






@


1.6
log
@_RCS_Info
@
text
@d8 3
d37 1
a37 3
static _RCS_Info[] = "$Header:$";

//char _program_name[] = "rnews " U1_VER_STR " (" __DATE__ ")";
a39 2
//static void		usage( char *msg );

d105 1
a105 3
	RnewsApp	app;

	U1App::_program_name = "U1 Rnews " U1_VER_STR " (" __DATE__ ")";
d117 2
@


1.5
log
@Cleanup
@
text
@d8 3
d34 3
a36 1
char _program_name[] = "rnews " U1_VER_STR " (" __DATE__ ")";
@


1.4
log
@Using class App
@
text
@d8 3
d15 2
a16 2
 * Revision 1.2  1995/04/09  21:05:55  dz
 * IBM C Set version
d18 2
a19 2
 * Revision 1.1  1995/03/11  06:14:45  dz
 * Initial revision
d34 1
a34 1
static void		usage( char *msg );
d56 6
a76 3
		case o_Err:
			fprintf( stderr, "Unknown option, ignored\n" );
			continue;
d107 5
a111 5
		"	-f <name>   - Process file <name> instead fo stdin\n"
		"	              (Wildcards allowed in <name>)\n"
		"	-i          - Ignored, please don't use\n"
		"	-l          - Ignored, please don't use\n"
		"	-e          - Ignored, please don't use\n"
a125 92
#if 0



void
main( int ac, char **av )
	{

	set_program_name ( _program_name );

	get_setup();									// Read .CTL file

	if( ac > 1 )
		{
		if( strcmp( av[1], "-file" ) )
			{
			warning( EI_None, "wrong arg: %s", av[1] );
			usage("");
			}
		else
			{
			av++;
			ac--;
			file = Yes;
			}
		}


    if( file )
        {
        while( ac > 1 )
            {
            if( freopen( av[1], "rb", stdin ) == NULL )
            	fatal( EC_IO_Error, EI_Full, "Can't open file: %s", av[1] );
            if( process() == Err )
            	fatal( EC_Dunno, EI_None, "Can't process input: %s", av[1] );

            if( fclose( stdin ) )
            	fatal( EC_IO_Error, EI_Full, "Can't close file: %s", av[1] );

			chmod( av[1], 0666 );	// Writeable
            if( unlink( av[1] ) )
            	fatal( EC_IO_Error, EI_Full, "Can't delete file: %s", av[1] );

            ac--;
            av++;
            }

        }
	else
		if( process() == Err )
           	fatal( EC_Dunno, EI_None, "Can't process stdin" );

    add_stat_news();

	exit(0);
	}



		/*************************************************************
							   Usage info
		*************************************************************/


void
usage( char *msg ) {

	printf(
		"U1 Rnews vers. " U1_VER_STR " (" __DATE__ ")\n"
		U1_COPYRIGHT "\n"
		"\n"
		"%s\n\n"
		"Usage: rnews < news_batch\n"
		"   Or: rnews -file filename {...}\n"
		"\n"
		"       (Wildcards allowed)\n"
		"\n"
		"If file name is given from command line, file will\n"
		"be deleted after processing\n"
		,
		msg
		);

	}






#endif
@


1.3
log
@Don't exit on wrong args - workaround for
uupc/ext uuxqt bug
@
text
@d8 4
d25 2
d34 90
a126 1
	bool	file = No;
d211 1
@


1.2
log
@IBM C Set version
@
text
@a0 2
//#define empty

d8 3
a21 1
//#ifndef empty
a22 1
//#endif
a34 1
#ifndef empty
a35 2
debug("got setup");
#endif
d41 1
a41 3
#ifndef empty
			error( EI_None, "wrong arg: %s", av[1] );
#endif
a42 1
//			exit(0);
a52 4
#ifndef empty

debug("passed arg parser");

a78 1
#endif
a92 1
#ifndef empty
a108 1
#endif
@


1.1
log
@Initial revision
@
text
@d9 4
a12 10
 *      $Log:   Q:/news/rnews/vcs/main.c_v  $
 *      
 *         Rev 1.4   07 Jun 1993 17:08:30   dz
 *      Able to read files which names are given on command-line.
 *      
 *         Rev 1.3   23 Oct 1992 14:55:02   dz
 *      Usage
 *      
 *         Rev 1.2   18 Sep 1992 03:08:04   dz
 *      performanse calculations support
a13 5
 *         Rev 1.1   18 Jun 1992 11:05:36   dz
 *      Copyright text cahnged
 *      
 *         Rev 1.0   08 Feb 1992 15:27:08   dz
 *      Initial revision.
d22 1
a22 1
char program_name[] = "rnews " U1_VER_STR " (" __DATE__ ")";
d34 2
d38 1
a39 1
debug("got setup");
d45 1
d47 1
a58 2
debug("passed arg parser");

d61 2
@
