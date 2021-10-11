/************************ UU2 FILE2UU ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	.TIC file handler
 *
 *      $Log: Tic.c $
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 *      
 *         Rev 1.1   29 Dec 1992 01:31:20   dz
 *      Seems to be working
 *      
 *         Rev 1.0   05 Oct 1992 19:17:22   dz
 *      Initial revision.
 *      
 *
\*/

#include    "tic.h"
#include    "tic_kw.h"
#include	"..\..\version.h"
#include	<log.h>


#ifdef __IBMCPP__
#	define	TEXT_MODE
#else
#	define	TEXT_MODE "t"
#endif


tic_file::tic_file()
	{
	path_v.ownsElements( 1 );			// To let him kill 'em
	seenby_v.ownsElements( 1 );
	desc_v.ownsElements( 1 );
	replaces_v.ownsElements( 1 );

	clear();

	err = No;
	}

tic_file::~tic_file()
	{
	clear();
	}


bool tic_file::
load_from( const char *fn )
	{
	bool	has_err = Ok;
	tic_kw	kwp;				// Init keywords parser

	if( kwp.invalid() )
		{
		error( EI_None, "Can't init keywords parser" );
		return Err;
		}

	FILE	*f = fopen( fn, "r" TEXT_MODE );
	if( f == NULL )
		{
		error( EI_None, "Can't open TIC file: %s", fn );
		return Err;
		}

	clear();

// parse

	const	maxl = 100;
	char	l[maxl];

	while( fgets( l, maxl, f ) != NULL )
		{
		char	*lp;

		if( (lp = strpbrk( l, "\r\n" )) != NULL )		// Cut trailing \r\n
			*lp = '\0';
		else
			error( EI_None, "TIC line too long: '%s'", l );

		lp = l;

		tic_kw::tic_token	t = kwp.parse( lp );

		if( t == tic_kw::no_token )
			log( "#", "Unknown TIC keyword: %s", l );

		if( fill_in( t, lp ) == Err )
			has_err = Err;
		}

	if( ferror( f ) )
		{
		error( EI_None, "Can't read TIC file %s", fn );
		has_err = Err;
		}

// cleanup

	fclose( f );
	if( has_err == Err && kwp.finish() == Err )
		{
		error( EI_None, "Errors parsing %s", fn );
		return Err;
		}

	return Ok;
	}

/*
void
test( void )
	{
	tic_file	t;

	t.load_from( "../examples/tk121530.tic" );
	t.save_to( "dump.tic" );

	}
*/



