head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	95.11.06.13.00.40;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.08.07.13.57.10;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.07.13.28.26;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.09.23.24.27;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.31.14;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@Removed old code
@
text
@/************************ UU2 Snews ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	Main
 *
 *      $Log: main.c $
 *      Revision 1.4  1995/08/07 13:57:10  dz
 *      Cleanup
 *
 *      Revision 1.3  1995/08/07 13:28:26  dz
 *      Using class App
 *
 *      Revision 1.2  1995/04/09  23:24:27  dz
 *      IBM C Set version
 *
 *      Revision 1.1  1995/03/11  18:31:14  dz
 *      Initial revision
 *
 *      
 *         Rev 1.7   07 Jun 1993 17:09:46   dz
 *      Announces, copyrights, etc.
 *      
 *         Rev 1.6   28 Nov 1992 22:18:52   dz
 *      -q flag - don't print announce string
 *      
 *         Rev 1.5   28 Nov 1992 22:15:16   dz
 *      Announce
 *      
 *         Rev 1.4   23 Oct 1992 14:56:00   dz
 *      Usage
 *      
 *         Rev 1.3   18 Jun 1992 11:07:58   dz
 *      update
 *      
 *         Rev 1.2   31 Mar 1992 15:15:12   dz
 *      snews
 *      
 *         Rev 1.1   04 Feb 1992 00:00:14   dz
 *      -i flag, program name var.
 *      
 *         Rev 1.0   06 Oct 1991 00:17:28   dz
 *      Initial revision.
 *      
 *
\*/

#include	"snews.h"
#include	"..\..\version.h"



int
SnewsApp::Run()
	{
	bool	got_pkt = No;
	bool	killhim = Yes;
	bool	err = No;
	bool	scan_msgs = No;

	while( 1 )
		{
		switch( GetOpt() )
			{
		case o_Opt:
			if( OptChar == 's' )
				{
				scan_msgs = Yes;
				continue;
				}

			if( OptChar == 'd' )
				{
				killhim = No;
				continue;
				}

			if( OptChar == 'i' )		// just ignore one arg
				continue;

			// fall through...

		case o_Err:
			fprintf( stderr, "Unknown option '%c' ignored\n", OptChar );
			continue;


		case o_Arg:
			got_pkt = Yes;
			if( unpack( OptArg ) != Err )
				{
				if( killhim )
					{
					chmod( OptArg, 0666 );
					unlink( OptArg );
					}
				}
			else
				err = Yes;
			continue;

		case o_Eof:
			break;

			}
		break;
		}


	if( (!got_pkt) && (!scan_msgs) )
		PrintUsage( "No packet name or -s flag given" );

	if( scan_msgs )
		scan();

	return err ? 1 : 0;
	}


#define OPTS	"i:sd"

int
main( int ac, char **av )
	{
	U1App::program_name( "U1 Snews " U1_VER_STR " (" __DATE__ ")" );
	U1App::_announceable = Yes;
	U1App::_usage = 
		"Personal args:\n"
		"    -i <arg>    - Ignore <arg> completely\n"
		"    -s          - Scan netmail for fileattaches with packets\n"
		"    -d          - Don't delete packets, given on command line\n"
		"    <name>      - unpack and delete packet <name>\n"
		"                  (Wildcards allowed in <name>)\n"
		;

	SnewsApp	app;

	app.Setup( ac, av, OPTS );

	return app.Run();
	}




@


1.4
log
@Cleanup
@
text
@d8 3
d125 1
a125 3
	SnewsApp	app;

	U1App::_program_name = "U1 Snews " U1_VER_STR " (" __DATE__ ")";
d133 1
a133 1
		"                (Wildcards allowed in <name>)\n"
d136 2
a144 122


#if 0




void
main( int ac, char **av )
	{
	bool	got_addr = No;
	bool	scan_msgs = No;
	bool	silent = No;

	bool    err = No;

	set_program_name ( _program_name );

	get_setup();									// Read .CTL file

    if( conf.announce_module && !silent )
        fprintf( stderr, "%s, " U1_COPYRIGHT "\n", _program_name );

    ac--;                                           // Skip exe name
	av++;

	while( ac-- )
		{
		char *ap = *av++;
		if( *ap != '-' ) 							// Address!
			{
			got_addr = Yes;
			if( unpack( ap ) != Err )
				{
				chmod( ap, 0666 );
//#ifndef __OS2__
				unlink( ap );
//#endif
				}
			else
				err = Yes;
			continue;
			}


		for( ap++; *ap; ap++ ) 						// Option
			{

			switch( *ap  )							// 1st level opt.
				{
			case 's':
				scan_msgs = Yes;
				continue;

			case 'q':
				silent = Yes;
				continue;

			case 'h':		// Number follows
			case 'x':
				break;

			case 'i':		// Ignore next arg
				if( ac )
					{
					av++;
					ac--;
					}
				break;



			default:
				error( EI_None, "Bad flag: %c", *ap );
				continue;
				} /* switch */

			break;

			} /* for */

		}

	if( (!got_addr) && (!scan_msgs) )
		usage( "No packet name or -s flag given" );

	if( scan_msgs )
		scan();

	exit( err ? 1 : 0 );
	}



		/*************************************************************
							   Usage info
		*************************************************************/


void
usage( char *msg )
	{
	printf(
		"U1 Snews v. " U1_VER_STR " (" __DATE__ ")\n"
		U1_COPYRIGHT "\n"
		"\n"
        "%s\n\n"
		"Usage: snews [-s] [EchoPacket...]\n"
		"Flags:\n"
		"	-s     \t- Scan NetMail for packets\n"
//        "   -q     \t- Don't print greating text\n"
		"	-i     \t- Ignore next arg.\n",
		msg
		);
	}






#endif
@


1.3
log
@Using class App
@
text
@d8 2
a9 2
 * Revision 1.2  1995/04/09  23:24:27  dz
 * IBM C Set version
d11 5
a15 2
 * Revision 1.1  1995/03/11  18:31:14  dz
 * Initial revision
a46 7

//char _program_name[] = "snews " U1_VER_STR " (" __DATE__ ")";


//static void		usage( char *msg );
//unsigned _stklen	= 16 * 1024;

@


1.2
log
@IBM C Set version
@
text
@d8 3
d45 104
a148 1
char _program_name[] = "snews " U1_VER_STR " (" __DATE__ ")";
a150 2
static void		usage( char *msg );
unsigned _stklen	= 16 * 1024;
d266 2
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/snews/vcs/main.c_v  $
d42 1
a42 1
char program_name[] = "snews " U1_VER_STR " (" __DATE__ ")";
d58 2
d63 1
a63 1
        fprintf( stderr, "%s, copyright (C) 1991-1994 by Dmitry Zavalishin\n", program_name );
d153 1
a153 1
		"   -i     \t- Ignore next arg.\n",
@
