/************************ UU2 GetFatt ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :       Main
 *
 *      $Log:   Q:/fido2uu/vcs/main.c_v  $
 *
 *
\*/

#include		"getfatt.h"
#include		"version.h"

extern uint _stklen = (10*1024u);


char program_name[] = "GetFatt " UU2_VER_STR " (" __DATE__ ")";


static void				usage( char *msg );

void
main( int ac, char **av )
	{
	bool	scan = No;
	bool	flo = No;

	get_setup();

	if( conf.announce_module )
		fprintf( stderr, "%s, copyright (C) 1991,92 by NetDialogue\n", program_name );


	ac--;
	av++;

	while( ac-- )
		{
		char *ap = *av++;

		if( *ap != '-' )
			{
			get_flo( ap );
			flo = Yes;
			continue;
			}

		for( ap++; *ap; ap++ )
			{
			switch( *ap  )
				{
			case 'm':
				scan = Yes;
				break;

			default:
				usage("Error: Bad flag\n\n");
				exit(1);
				break;
				} /* switch */
			}
		}


	if( scan )
		scan_netmail();
	else if( !flo )
		error( EI_None, "Nothing to do" );

	exit(0);
	}



	/*************************************************************
							 Usage info
	*************************************************************/


void
usage( char *msg ) {
	fprintf( stderr,
		"UU2 GetFatt vers. " UU2_VER_STR " (" __DATE__ ")\n"
		"Copyright (C) 1991,92 by NetDialogue.\n"
		"\n"
		"Copy/move file-attaches to UU2 spool directory\n"
		"\n"
		"%s\n\n"
		"Usage: getfatt [-flags] [flo_name [...]]\n"
		"Flags:\n"
		"       -m		\t- Scan netmail (not implemented)\n",
		msg
		);
	}

