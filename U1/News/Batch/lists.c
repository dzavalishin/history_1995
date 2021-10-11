/************************ UU2 News Batcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	list files iterator
 *
 *      $Log: lists.c $
 *      Revision 1.3  1996/05/21 00:21:04  dz
 *      cosmetical changes
 *
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
