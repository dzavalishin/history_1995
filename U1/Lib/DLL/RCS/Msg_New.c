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
date	95.03.11.18.00.54;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@debug
@
text
@/************************ FIDO MSG file class ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:		Create new msg
 *
 *      $Log: Msg_New.c $
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



@


1.1
log
@Initial revision
@
text
@d7 3
a9 19
 *      $Log:   Q:/lib/fido/vcs/msg_new.c_v  $
 *      
 *         Rev 1.5   09 Apr 1993 10:11:32   dz
 *      findfirst attributes arg fixed for OS/2
 *      
 *         Rev 1.4   06 Nov 1992 19:20:04   dz
 *      Ver 1.12a save
 *      
 *         Rev 1.3   23 Oct 1992 15:21:34   dz
 *      fatal(), style
 *      
 *         Rev 1.2   16 Oct 1992 03:38:58   dz
 *      fatal() changed
 *      
 *         Rev 1.1   12 Sep 1991 01:38:14   dz
 *      Writing out header for append to work correctly
 *      
 *         Rev 1.0   28 Aug 1991 01:00:50   dz
 *      Initial revision.
d12 1
d15 5
d24 1
a24 2
#include	<dir.h>
#include	<dos.h>
d26 2
a27 1
#include	<string.h>
d51 1
a51 1

d58 1
d61 1
a61 1
#define ATTRS2FIND	FA_ARCH
d70 3
a72 2
	ffblk		ff;
	int			maxmsg = 0;
d74 2
d79 2
a80 1
	if( findfirst( mask, &ff, ATTRS2FIND ) )
d82 1
a82 1
		if( errno == ENOENT )
d85 1
a85 2
		perror("Findfirst");
		fatal( EC_OS_Error, EI_Full, "findfirst error");
d90 4
a93 2
		maxmsg = max( maxmsg, newmax( ff.ff_name ) );
		} while( findnext(&ff) == 0 );
d95 5
a99 5
	if( errno != ENOENT )
		{
		perror("Findnext");
		fatal( EC_OS_Error, EI_Full, "findnext error");
		}
d133 1
a133 1
		fd	= open( fname, O_RDWR|O_CREAT|O_EXCL|O_BINARY|O_DENYALL, 0666 );
@
