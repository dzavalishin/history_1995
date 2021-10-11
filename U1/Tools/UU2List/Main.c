/************************ UUCP List processor ***************************\
 *
 *  Copyright (C) 1991-1995 by Infinity Soft
 *
 *  Module  :   Main
 *
 *      $Log:   Q:/uu2fido/vcs/main.c_v  $
 *      
 *
 *
\*/

#include    "uu2list.h"

#define INCL_DOSQUEUES
#define INCL_DOSFILEMGR
#include    <os2.h>

#include    <process.h>


char program_name[] = "uu2list " U1_VER_STR " (" __DATE__ ")";

static void     usage( char *msg );
unsigned _stklen    = 16 * 1024;

void		send_it( FILE *tempfp, char *addr );


int
main( int ac, char **av )
	{
	char *listname;

	get_setup();

	if( ac == 4 || ac == 6 )
		listname = av[3];
	else
	if( ac == 2 )
		listname = av[1];
	else
		{
		usage( "No list name given" );
		return 1;
		}


	FILE *tempfp;

	tempfp = tmpfile();
	if( tempfp == NULL )
		fatal( EC_OS_Error, EI_Full, "Can't create temp file" );

	fseek( tempfp, 0L, SEEK_SET );

	const int	bs = 8196;
	static char	buf[bs];
	int	 	i;

	while( 1 )
		{
		i = read( 0, buf, bs );
		if( i < 0 )
			fatal( EC_OS_Error, EI_Full, "stdin read error" );

		if( i == 0 )
			break;

		if( fwrite( buf, 1, i, tempfp ) != i )
			fatal( EC_OS_Error, EI_Full, "tmp file write error" );
		}


	FILE *listfp;
	static char fn[200];

	sprintf( fn, "%s.List", listname );

	listfp = fopen( fn, "rt" );
	if( listfp == NULL )
		{
		listfp = fopen( listname, "rt" );
		if( listfp == NULL )
			{
			send_it( tempfp, "postmaster" );
			exit( 2 );
			}
		}


	static char addr[200];

	while( fgets( addr, 200, listfp ) != NULL )
		{
		fseek( tempfp, 0L, SEEK_SET );

		char *p;

		if( (p = strpbrk( addr, "\r\n" )) != NULL )
			*p = '\0';

		send_it( tempfp, addr );
		}

	if( ferror( listfp ))
		{
		error( EI_Full, "read error: %s", listname );
		exit( 3 );
		}

	return 0;
	}






        /*************************************************************
                               Usage info
        *************************************************************/


void
usage( char *msg )
    {
    printf(
        "U1 UU2Fido vers. " U1_VER_STR " (" __DATE__ ")\n"
	U1_COPYRIGHT "\n"
        "\n"
        "%s\n"
	"\n"
        "Usage:\n"
        "\t1-st way: uu2list list_name\n"
        "\t2-nd way: uu2list ignored_arg-1 ignored_arg-2 list_name\n"
        "\n"
        "2-nd way is suitable for running from UUPC hostpath\n"
        ,
        msg
        );
    }





//
// Spawn rmail, return descriptor of our end of it's pipe
//

int
mail( char * const args[] )
	{
	unsigned long rd, wr;

	int stdin_save = dup( 0 );

	if ( DosCreatePipe( &rd, &wr, 4096 ) != 0 )
		return -1;

	if( 0 != dup2( rd, 0 ) )
		return -1;

	close( rd );
        DosSetFHState( wr, OPEN_FLAGS_NOINHERIT );

	if( spawnvp( P_NOWAITO, "rmail", args ) < 0 )
		return -1;

	if( 0 != dup2( stdin_save, 0 ) )
		return -1;

	close( stdin_save );

	return wr;
	}

void		
send_it( FILE *tempfp, char *addr )
	{
	char	*a[5];

	a[0] = "rmail";
	a[1] = "-w";
	a[2] = addr;
	a[3] = 0;

	fprintf( stderr, "--- %s\n", addr );

	int fd = mail( a );

	if( fd < 0 ) 
		{
		error( EI_Full, "can't run rmail %s", addr );
		return;
		}

	const		bs = 16*1024;
	static char	buf[bs];
	int		i;

	while( ( i = fread( buf, 1, bs, tempfp )) > 0 )
		{
		if( write( fd, buf, i ) != i )
			{
			error( EI_Full, "write error: %s", addr );
			break;
			}
		}

	if( ferror( tempfp ) )
		error( EI_Full, "read error: %s", addr );

	close( fd );	
	}












