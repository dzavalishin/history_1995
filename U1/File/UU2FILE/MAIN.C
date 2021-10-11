/************************ UU2 UU2FILE ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Main
 *
 *      $Log:   Q:/file/file2uu/vcs/main.c_v  $
 *
 *
\*/

#include    "uu2file.h"
#include	"..\..\version.h"

char program_name[] = "UU2File " UU2_VER_STR " (" __DATE__ ")";


static void		usage( char *msg );
unsigned _stklen	= 16 * 1024;


void
main( int ac, char **av )
	{
//    bool    got_addr = No;
//    bool    scan_msgs = No;

	get_setup();									// Read .CTL file

	ac--;											// Skip exe name
	av++;

	while( ac-- )
		{
		char *ap = *av++;

/*
		if( *ap != '-' ) 							// Address!
			{
			got_addr = Yes;
			if( unpack( ap ) != Err )
				{
				chmod( ap, 0666 );
				unlink( ap );
				}
			continue;
			}
*/

		for( ap++; *ap; ap++ ) 						// Option
			{

			switch( *ap  )							// 1st level opt.
				{
//            case 's':
//                scan_msgs = Yes;
//                continue;

			case 'h':		// Number follows
			case 'x':
				break;

/*
			case 'i':		// Ignore next arg
				if( ac )
					{
					av++;
					ac--;
					}
				break;
*/


			default:
				error( EI_None, "Bad flag: %c", *ap );
				usage("");
				continue;
				} /* switch */

			break;

			} /* for */

		}

//    if( (!got_addr) && (!scan_msgs) )
//        usage( "No packet name or -s flag given" );

//    if( scan_msgs )
//        scan();

	scan_spool( "files" );

	exit(0);
	}



		/*************************************************************
							   Usage info
		*************************************************************/


void
usage( char *msg )
	{
	printf(
		"UU2 UU2File v. " UU2_VER_STR " (" __DATE__ ")\n"
		"Copyright (C) 1992 by NetDialogue.\n"
		"\n"
		"%s\n\n"
		"Usage: UU2File\n"// [-s] [EchoPacket...]\n"
//		"Flags:\n"
//		"   -s     \t- Scan NetMail for packets\n"
		,
		msg
		);
	}






