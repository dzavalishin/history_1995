head	1.1;
access;
symbols;
locks;
comment	@ * @;


1.1
date	94.08.05.01.12.42;	author root;	state Exp;
branches;
next	;


desc
@main
@


1.1
log
@Initial revision
@
text
@/************************ Mail Mover ***************************\
 *
 *      Copyright (C) 1991-1994 by Infinity Soft
 *
 *      Module  :       Main
 *
 *      $Log:   Q:/fido2uu/vcs/main.c_v  $
 *      
 *
 *
\*/

#include		"Mover.h"
#include		"..\..\version.h"


char program_name[] = "Mover " U1_VER_STR " (" __DATE__ ")";


bool		f_silent		= No;		// Don't chat
bool		f_keeporig		= No;		// Keep original message


static void				usage( char *msg = NULL );

void 
main( int ac, char **av )
    {

	get_setup();

	if( conf.announce_module )
		usage( NULL );
//		fprintf( stderr, "%s, copyright (C) 1991-1994 by Dmitry Zavalishin\n", program_name );


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
		"U1 Mover vers. " U1_VER_STR " (" __DATE__ ")\n"
		U1_COPYRIGHT "\n"
	);

	if( msg == NULL )
		return;

	printf(
		"\n"
        	"%s\n\n"
		"Usage: fido2uu [-flags]\n"
		"Flags:\n"
	        "       -q     \t- Keep silence\n",
		msg
		);
	}

@
