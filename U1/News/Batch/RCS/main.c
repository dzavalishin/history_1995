head	1.7;
access;
symbols;
locks;
comment	@ * @;


1.7
date	96.05.21.00.21.04;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.05.20.23.58.45;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	95.08.07.05.15.02;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.08.07.05.13.33;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.02.13.30.34;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.10.02.08.14;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.26.07;	author dz;	state Exp;
branches;
next	;


desc
@@


1.7
log
@cosmetical changes
@
text
@/************************ UU2 News Batcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Main
 *
 *      $Log: main.c $
 *      Revision 1.6  1996/05/20 23:58:45  dz
 *      current
 *
 *      Revision 1.5  1995/08/07 05:15:02  dz
 *      Using App class
 *
 *      Revision 1.3  1995/08/02 13:30:34  dz
 *      cosmetical
 *
 *      Revision 1.2  1995/04/10  02:08:14  dz
 *      IBM C Set version
 *
 *      Revision 1.1  1995/03/11  18:26:07  dz
 *      Initial revision
 *
 *      Rev 1.0   26 Apr 1992 14:17:56   dz
 *      Initial revision.
 *
 *
\*/

#include	"batch.h"
#include	"..\..\version.h"

#include	"feeds.h"





#define	OPTS	"x"

int BatchApp::Run()
    {

    while( 1 )
        {
        switch( GetOpt() )
            {
            case o_Arg:
            case o_Opt:
            case o_Err:
                PrintUsage("Unknown option");
                return 0;

            case o_Eof:
                break;

            }
        break;
        }

    scan();

    if( sent_something && strlen( (const char*) conf.kick_uucp_flag.c_str()) != 0 )
        {
        FILE    *flag;

        if( ( flag = fopen( conf.kick_uucp_flag.c_str(), "w" ) ) == NULL )
            error( EI_Full, "Can't create flag file '%s'",
                  (const char *)conf.kick_uucp_flag.c_str() );
        else
            fclose( flag );
        }

    return 0;
    }





int main( int ac, char **av )
    {
    U1App::program_name( "U1 Batch " U1_VER_STR " (" __DATE__ ")" );
    // U1App::_usage = "Personal args: None\n";
    // U1App::_announceable = Yes;

    BatchApp	app;

    app.Setup( ac, av, OPTS );

    return app.Run();
    }


@


1.6
log
@current
@
text
@d8 3
d17 2
a18 2
 * Revision 1.2  1995/04/10  02:08:14  dz
 * IBM C Set version
d20 2
a21 2
 * Revision 1.1  1995/03/11  18:26:07  dz
 * Initial revision
d23 1
a23 1
 *         Rev 1.0   26 Apr 1992 14:17:56   dz
d40 2
a41 3
int
BatchApp::Run()
	{
d43 9
a51 9
	while( 1 )
		{
		switch( GetOpt() )
			{
		case o_Arg:
		case o_Opt:
		case o_Err:
			PrintUsage("Unknown option");
			return 0;
d53 2
a54 2
		case o_Eof:
			break;
d56 3
a58 3
			}
		break;
		}
d60 1
a60 1
	scan();
d62 3
a64 3
        if( sent_something && strlen( (const char*) conf.kick_uucp_flag.c_str()) != 0 )
                {
                FILE    *flag;
d66 6
a71 6
                if( ( flag = fopen( conf.kick_uucp_flag.c_str(), "w" ) ) == NULL )
                        error( EI_Full, "Can't create flag file '%s'",
                                (const char *)conf.kick_uucp_flag.c_str() );
                else
                        fclose( flag );
                }
d73 2
a74 2
	return 0;
	}
d80 5
a84 6
int
main( int ac, char **av )
	{
	U1App::program_name( "U1 Batch " U1_VER_STR " (" __DATE__ ")" );
//	U1App::_usage = "Personal args: None\n";
//	U1App::_announceable = Yes;
d86 1
a86 1
	BatchApp	app;
d88 1
a88 1
	app.Setup( ac, av, OPTS );
d90 2
a91 2
	return app.Run();
	}
@


1.5
log
@Using App class
@
text
@d8 2
a9 2
 *      Revision 1.4  1995/08/07 05:13:33  dz
 *      using U1App class
d81 1
a81 3
	BatchApp	app;

	U1App::_program_name = "U1 Batch " U1_VER_STR " (" __DATE__ ")";
d84 2
@


1.4
log
@using U1App class
@
text
@d8 3
a34 9


//static void		usage( char *msg );

//extern char   *optarg;			/* pointer to argument of current option */
//extern int		opterr;			/* no error messages	*/

//int		getopt(int argc, char *argv[], char *optionS);

a35 2

bool		sent_something = No;
@


1.3
log
@cosmetical
@
text
@d8 3
a27 1
char _program_name[] = "batch " U1_VER_STR " (" __DATE__ ")";
a29 2
static void		usage( char *msg );
// unsigned _stklen	= 32 * 1024;
a30 1
// from getopt.c
a31 2
extern char   *optarg;			/* pointer to argument of current option */
extern int		opterr;			/* no error messages	*/
d33 7
a39 1
int		getopt(int argc, char *argv[], char *optionS);
d45 2
a46 2
void
main( int ac, char **argv )
a47 9
	set_program_name ( _program_name );

	get_setup();

	if( conf.announce_module )
		fprintf( stderr, "%s, " U1_COPYRIGHT "\n", _program_name );


	opterr = 0;
d51 1
a51 1
		switch( getopt( ac, argv, OPTS ) )
d53 5
a57 7
		case '?':
			usage("");
			exit(0);

		case 'x':
//			batchdir = optarg;
			continue;
d59 1
a59 2

		case EOF:
d79 1
a79 1
	exit(0);
a83 3
		/*************************************************************
							   Usage info
		*************************************************************/
d86 4
a89 12
void
usage( char *msg ) {
	printf(
		"U1 Batcher vers. " U1_VER_STR " (" __DATE__ ")\n"
		U1_COPYRIGHT "\n"
		"\n"
        "%s\n\n"
        "Usage: batch\n",
		msg
		);
	}

d91 3
d95 1
d97 2
@


1.2
log
@IBM C Set version
@
text
@d8 3
a13 7
 *      
 *         Rev 1.2   28 Nov 1992 22:00:46   dz
 *      Announce
 *      
 *         Rev 1.1   23 Oct 1992 14:50:44   dz
 *      Usage
 *      
d29 1
a29 1
unsigned _stklen	= 32 * 1024;
d47 1
a47 1
	get_setup();									// Read .CTL file
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/batch/vcs/main.c_v  $
d29 1
a29 1
char program_name[] = "batch " U1_VER_STR " (" __DATE__ ")";
d49 2
d54 1
a54 1
		fprintf( stderr, "%s, copyright (C) 1991,92 by Dmitry Zavalishin\n", program_name );
@
