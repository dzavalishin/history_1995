/************************ FIDO MSG file class ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:		Create new msg
 *
 *      $Log: Msg_New.c $
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 *
 *
\*/

#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#include	<os2.h>


#include	<style.h>
#include	<ctype.h>
#include	<uu2io.h>
#include	<errno.h>
#include	<share.h>
#include	<fcntl.h>
//#include	<string.h>
#include	<strng.h>
#include	<stdlib.h>
#include	<time.h>

#include	"fido_msg.h"
#include	"log.h"



static int
newmax( char *mn )
	{
	char	*pp;

	if( !isdigit( *mn ) )
		return -1;

	if( (pp = strpbrk( mn, ". \t" )) != NULL && pp - mn <= 9 )
		pp = '\0';

	return atoi( mn );
	}


#ifndef max
inline int
max( int a, int b )
	{
	if( a > b )		return a;
	else			return b;
	}
#endif

  
#define ATTRS2FIND	0x27
	




int
find_last_msg( const char *dir )
	{
	HDIR		h = HDIR_SYSTEM;
	FILEFINDBUF3	ff = { 0 };
	int		maxmsg = 0;
	char		mask[150];
	ULONG		count = 1;
	APIRET		rc;

	sprintf( mask, "%.120s\\*.msg", dir );

	rc = DosFindFirst( mask, &h, ATTRS2FIND, &ff, sizeof(ff), &count, FIL_STANDARD );
	if( rc != NO_ERROR )
		{
		if( rc == ERROR_NO_MORE_FILES )
			return 0;			// It will start from 1st

		fatal( EC_OS_Error, EI_Full, "FindFirst rc = %d", rc );
		}


	do {
		maxmsg = max( maxmsg, newmax( ff.achName ) );
		count = 1;
		rc = DosFindNext( h, &ff, sizeof(ff), &count );
		} while( rc == NO_ERROR );

	if( rc != ERROR_NO_MORE_FILES )
		fatal( EC_OS_Error, EI_Full, "FindNext rc = %d", rc );

	if( DosFindClose( h ) != NO_ERROR )
		error( EI_Full, "FindClose rc = %d", rc );

	return maxmsg;
	}














bool
fido_msg::new_msg( const char *dir )
	{
	int		fd;

	detach();
	clear_hdr();

	error = Yes;

	while( 1 )
		{
		int		last = find_last_msg( dir );

		sprintf( fname, "%s\\%d.msg", dir, last+1 );

		fd	= _sopen( fname, O_RDWR|O_CREAT|O_EXCL|O_BINARY, SH_DENYRW, 0666 );

		if( fd >= 0 )	// Opened!
			break;

		if( errno != EEXIST )
			return Err;
		}


	if( (fp = fdopen( fd, "rb+" )) == NULL )
		{
		close( fd );
		unlink( fname );
		return Err;
		}

	// Without header text will be appended in incorrect place!


	fseek( fp, 0L, SEEK_SET );
	if( fwrite( &hdr, sizeof(hdr), 1, fp ) != 1 )
		return Err;

	flush();

	error = No;
	return Ok;
	}



