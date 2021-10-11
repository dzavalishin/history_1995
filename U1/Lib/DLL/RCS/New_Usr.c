head	1.2;
access;
symbols;
locks;
comment	@ * @;


1.2
date	95.04.13.11.06.28;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.04.17;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@debug
@
text
@/********************** UU2 Rules sender interface *************************\
 *
 *	Copyright (C) 1991,1992 by Infinity Soft
 *
 *	Module 	:	Rules sender interface
 *
 *      $Log: New_Usr.c $
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *         Rev 1.1   07 Jun 1993 17:50:46   dz
 *      Diagnostics fixed
 *      
 *         Rev 1.0   15 Nov 1992 00:08:36   dz
 *      Initial revision.
 *
 *
\*/

#define TURNED_OFF


//#include	<string.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<uu2io.h>
#include	<share.h>
#include	<errno.h>
//#include	<ctype.h>

#include	"setup.h"
#include	"log.h"

#include	"new_usr.h"
//#include	<sys\locking.h>

static bool	add_addr( FILE *f, const char *group, const char *addr );
static bool	scan_addr( FILE *f, bool (*func)( const char *group, const char *address) );


bool rules_sender::
active( void )
	{
#ifdef TURNED_OFF
	return No;
#else

	if( access( conf.rulelist_file.c_str(), 0 ) != 0 )
		return No;

	return Yes;
#endif
	}

bool rules_sender::
add_address( const char *group, const char *address )
	{
#ifdef TURNED_OFF
	return Ok;
#else

	if( !active() ) return Ok;
	debug("Sendrules interface - adding address '%s'", address );

	FILE	*nuf = _fsopen( conf.newusr_file.c_str(), "at", SH_DENYRW );

	if( nuf == NULL )
		{
		error( EI_Full, "SendRules: Can't create %s", (const char *)conf.newusr_file.c_str() );
		return Err;
		}

	fflush( nuf );

	int	fd = fileno( nuf );
	lseek( fd, 0L, SEEK_SET );
/*
	for( int i = 1; i < 13; i++ )
		{
		if( 0 == locking( fd, LK_RLCK, 1 ) )
			break;

		switch( errno )
			{
		case EDEADLOCK:
		case EACCES:
			error( EI_Full, "Can't lock %s, retry %d", conf.newusr_file, i );
			continue;

		default:
			error( EI_Full, "Can't lock %s", conf.newusr_file );
			fclose( nuf );
			return Err;
			}
		}
*/
	bool r = add_addr( nuf, group, address );

	fflush( nuf );
	lseek( fd, 0L, SEEK_SET );
/*
	if( 0 != locking( fd, LK_UNLCK, 1 ) )
		{
		error( EI_Full, "Can't unlock %s", conf.newusr_file );
		r = Err;
		}
*/
	if( 0 != fclose( nuf ))
		{
		error( EI_Full, "Can't close %s", (const char *)conf.newusr_file.c_str() );
		r = Err;
		}

	return r;
#endif
	}




bool rules_sender::
for_each_address( bool (*func)(const char *group, const char *address) )
	{
#ifdef TURNED_OFF
	return Ok;
#else

	if( !active() ) return Ok;

	FILE	*nuf = _fsopen( conf.newusr_file.c_str(), "rt+", SH_DENYRW );

	if( nuf == NULL )
		{
		error( EI_Full, "SendRules: Can't open %s", (const char *)conf.newusr_file.c_str() );
		return Err;
		}

	fflush( nuf );

	int	fd = fileno( nuf );
	lseek( fd, 0L, SEEK_SET );
/*
	for( int i = 1; i < 13; i++ )
		{
		if( 0 == locking( fd, LK_RLCK, 1 ) )
			break;

		switch( errno )
			{
		case EDEADLOCK:
		case EACCES:
			error( EI_Full, "Can't lock %s, retry %d", conf.newusr_file, i );
			continue;

		default:
			error( EI_Full, "Can't lock %s", conf.newusr_file );
			fclose( nuf );
			return Err;
			}
		}
*/
	bool r = scan_addr( nuf, func );

	fflush( nuf );
	lseek( fd, 0L, SEEK_SET );

	if( chsize( fd, 0L ) != 0 )
		{
		error( EI_Full, "Can't truncate file %s", (const char *)conf.newusr_file.c_str() );
		r = Err;
		}
/*
	if( 0 != locking( fd, LK_UNLCK, 1 ) )
		{
		error( EI_Full, "Can't unlock %s", conf.newusr_file );
		r = Err;
		}
*/
	if( 0 != fclose( nuf ))
		{
		error( EI_Full, "Can't close %s", (const char *)conf.newusr_file.c_str() );
		r = Err;
		}

	return r;
#endif
	}


/****************************************************************************
						Implementation
****************************************************************************/


static bool
add_addr( FILE *f, const char *group, const char *addr )
	{
#ifndef TURNED_OFF
	fseek( f, 0L, SEEK_END );
	fprintf( f, "%s %s\n", group, addr );

	if( ferror( f ) )
		{
		error( EI_Full, "Error writing %s", (const char *)conf.newusr_file.c_str() );
		return Err;
		}

#endif
	return Ok;
	}


static bool
scan_addr( FILE *f, bool (*func)( const char *group, const char *address) )
	{
	char	buf1[100], buf2[100];
	int		r;

	while( (r = fscanf( f, "%99s %99s\n", buf1, buf2 )) != EOF )
		{
		if( r != 2 )
			{
			error( EI_None, "File (%s) format error", (const char *)conf.newusr_file.c_str() );
			continue;
			}

		func( buf1, buf2 );
		}

	if( ferror( f ) )
		{
		error( EI_Full, "Error reading %s", (const char *)conf.newusr_file.c_str() );
		return Err;
		}

	return Ok;
	}









@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/uu2/vcs/new_usr.c_v  $
d24 2
d39 1
a39 1
#include	<sys\locking.h>
d48 4
d56 1
d62 4
d119 1
d128 4
d190 1
d202 1
d212 1
@
