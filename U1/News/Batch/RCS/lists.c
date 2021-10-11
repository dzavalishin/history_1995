head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	96.05.21.00.21.04;	author dz;	state Exp;
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


1.3
log
@cosmetical changes
@
text
@/************************ UU2 News Batcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	list files iterator
 *
 *      $Log: lists.c $
 *      Revision 1.2  1995/04/10  02:08:14  dz
 *      IBM C Set version
 *
 *      Revision 1.1  1995/03/11  18:26:07  dz
 *      Initial revision
 *
 *      Rev 1.3   07 Jun 1993 17:07:36   dz
 *      Portability fix
 *
 *      Rev 1.2   28 Nov 1992 22:08:38   dz
 *      consts, error info in fatal
 *
 *      Rev 1.1   18 Sep 1992 03:07:02   dz
 *      exit codes
 *
 *      Rev 1.0   26 Apr 1992 14:19:34   dz
 *      Initial revision.
 *
 *
\*/

#include	"lists.h"
#include	<log.h>
#include	<setup.h>

#include	<share.h>
#include	<uu2io.h>
//#include	<io.h>
#include	<fcntl.h>


/**********************************************************************/

lists::lists( const char *ld )
    {
    listdir = ld;
    ff = NULL;
    listno = -1;
    }

lists::~lists()
    {
    if( ff )	fclose(ff);
    }

/**********************************************************************/

void lists::rewind()
    {
    listno = -1;
    };

void lists::kill()
    {
    fflush( ff );
    fseek( ff, 0L, SEEK_SET );
    if( chsize( fileno( ff ), 0L ) != 0 )
        fatal( EC_IO_Error, EI_Full, "Can't change list file length!" );
    };


/**********************************************************************/


FILE * lists::next()
    {
    static char	fn[_MAX_PATH];

    if( ff )	fclose( ff );

// NB! Will find only LIST0-LIST19

    do 
        {
        sprintf( fn, "%s\\list%d", listdir, ++listno );
        int fd = _sopen( fn, O_RDWR|O_BINARY, SH_DENYRW );
        if( fd < 0 )
            ff = NULL;
        else
            {
            ff = fdopen( fd, "rb+" );
            if( ff == NULL )
                _close( fd );
            }
        } while( ff == NULL && listno < 20 );

    return ff;
    }

/**********************************************************************
                     alist class implementation
                       article list  iterator
**********************************************************************/

alist::~alist()
	{
	}


/**********************************************************************/

// Assume PATH is no longer than 5000 bytes
#define	BUFLEN	(AL_MAX_GROUP+(_MAX_PATH*2)+5000)

bool alist::next( char * afn, char * grp, char* path )
    {
    static char	buf[BUFLEN];

    while( 1 )
        {

        if( fgets( buf, BUFLEN, lf ) == NULL )
            return Err;

	
        if( sscanf( buf, "%80s %3000s %5000s", afn, grp, path ) == 3 )
            return Ok;

        *path ='\0';

        if( sscanf( buf, "%80s %3000s", afn, grp ) == 2 )
            return Ok;

        error( EI_None, "Wrong line in list: '%s'", buf );
        }

    }
@


1.2
log
@IBM C Set version
@
text
@d8 2
a9 2
 * Revision 1.1  1995/03/11  18:26:07  dz
 * Initial revision
d11 4
a14 2
 *      
 *         Rev 1.3   07 Jun 1993 17:07:36   dz
d16 2
a17 2
 *      
 *         Rev 1.2   28 Nov 1992 22:08:38   dz
d19 2
a20 2
 *      
 *         Rev 1.1   18 Sep 1992 03:07:02   dz
d22 2
a23 2
 *      
 *         Rev 1.0   26 Apr 1992 14:19:34   dz
d42 1
a42 1
	{
d45 1
a45 1
    listno	= -1;
d49 1
a49 1
	{
d55 2
a56 3
void lists::
rewind()
	{
d60 2
a61 3
void lists::
kill()
	{
d64 2
a65 2
	if( chsize( fileno( ff ), 0L ) != 0 )
		fatal( EC_IO_Error, EI_Full, "Can't change list file length!" );
d72 2
a73 4

FILE * lists::
next()
	{
d80 12
a91 11
    do {
    	sprintf( fn, "%s\\list%d", listdir, ++listno );
	int fd = _sopen( fn, O_RDWR|O_BINARY, SH_DENYRW );
	if( fd < 0 )
		ff = NULL;
	else
		{
        	ff = fdopen( fd, "rb+" );
		if( ff == NULL )
			_close( fd );
		}
d98 2
a99 2
					alist class implementation
                      article list  iterator
d102 1
a102 2
alist::
~alist()
d112 3
a114 4
bool alist::
next( char * afn, char * grp, char* path )
	{
	static char	buf[BUFLEN];
d116 2
a117 2
    while( 1 ) 
	{
d119 2
a120 2
	    if( fgets( buf, BUFLEN, lf ) == NULL )
    		return Err;
d124 1
a124 1
		return Ok;
d126 1
a126 1
	*path ='\0';
d129 1
a129 1
		return Ok;
d131 2
a132 2
	error( EI_None, "Wrong line in list: '%s'", buf );
	}
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/batch/vcs/lists.c_v  $
d33 2
a34 1
#include	<dir.h>
d76 1
a76 1
    static char	fn[MAXPATH];
d84 9
a92 1
        ff = _fsopen( fn, "rb+", SH_DENYRW );
d112 1
a112 1
#define	BUFLEN	(AL_MAX_GROUP+(MAXPATH*2)+5000)
@
