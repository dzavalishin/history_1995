head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	95.04.12.17.32.19;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.09.10.45.11;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.32.26;	author dz;	state Exp;
branches;
next	;


desc
@UU2 News Unbatcher
@


1.3
log
@Bug fixes
@
text
@/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1993 by Infinity Soft
 *
 *	Module 	:	Virtual file reader - file (unbuf)
 *
 *      $Log: vread_fi.c $
 * Revision 1.2  1995/04/09  10:45:11  dz
 * rewriting for C Set
 *
 * Revision 1.2  1995/04/09  10:45:11  dz
 * rewriting for C Set
 *
 * Revision 1.1  1995/03/11  18:32:26  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   05 Mar 1993 00:05:46   dz
 *      Initial revision.
 *
 *
\*/

#include	<stddef.h>
#include	<fcntl.h>
#include	<share.h>

#include	"vread.h"


VReadFile::
VReadFile( void )
	{
	xfd = -1;
	do_close = No;
	}

VReadFile::
~VReadFile( void )
	{
	close();
	}

	/*************************************************************
						   Open/Close
	*************************************************************/


bool VReadFile::
open( int fd )
	{
	close();
	xfd = fd;
	do_close = No;
	return Ok;
	}

bool VReadFile::
open( const char *name )
	{
	close();

	xfd = ::_sopen( name, O_RDONLY|O_BINARY, SH_DENYWR );
	if( xfd < 0 )
		return Err;

	return Ok;
	}

bool VReadFile::
close( void )
	{
	if( !do_close )
		return Ok;

	do_close = No;
	if( ::close( xfd ) != 0 )
		return Err;

	return Ok;
	}

	/*************************************************************
						   Access
	*************************************************************/


int VReadFile::
read( void *buf, int blen )
	{
	return ::read( xfd, buf, blen);
	}



long VReadFile::
seekg( long pos )
	{
	return ::lseek( xfd, pos, SEEK_SET );
	}



long VReadFile::
tellg( void )
	{
	return ::tell( xfd );
	}


long VReadFile::
size( void )
	{
	return filelength( xfd );
	}
@


1.2
log
@rewriting for C Set
@
text
@d8 6
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/unbatch/vcs/vread_fi.c_v  $
d20 1
d57 1
a57 1
	xfd = ::open( name, O_RDONLY|O_BINARY|O_DENYWRITE );
d70 1
@
