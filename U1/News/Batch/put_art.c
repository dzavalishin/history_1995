/************************ UU2 News Batcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Put an article to batch
 *
 *      $Log: put_art.c $
 *      Revision 1.4  1996/05/21 00:21:04  dz
 *      cosmetical changes
 *
 *      Revision 1.3  1995/08/07 05:15:02  dz
 *      Using App class
 *
 *      Revision 1.2  1995/04/10  02:08:14  dz
 *      IBM C Set version
 *
 *      Revision 1.1  1995/03/11  18:26:07  dz
 *      Initial revision
 *
 *      Rev 1.3   07 Jun 1993 17:06:44   dz
 *      Batcher verification code added
 *      
 *      Rev 1.2   23 Oct 1992 14:51:34   dz
 *      const's added
 *      
 *      Rev 1.1   08 Jun 1992 09:09:32   dz
 *      Code table conversion written
 *      
 *      Rev 1.0   26 Apr 1992 14:18:42   dz
 *      Initial revision.
 *
 *
\*/

#include    "batch.h"
#include    "..\..\version.h"
#include    "recode.h"
#include    <share.h>
#include    <fcntl.h>


static bool    do_recode( const char *tn, int ofd, int ifd );


#define	       CBS    (8*1024)

static char    cb[CBS];



bool BatchApp::do_put_art( const char *code, int ofd, int ifd )
    {
    char	hdr[24];
    long	size = filelength( ifd );
    int		l;
    bool	r;

    l = sprintf( hdr, "#! rnews %ld\n", size );
    if( l != write( ofd, hdr, l ) )
        {
        error( EI_Full, "Error writing #!rnews line");
        return Err;
        }

    long pos1 = tell( ofd );
    r = do_recode( code, ofd, ifd );
    long pos2 = tell( ofd );

    if( (pos2 - pos1) != size )
        {
        error( EI_None, "* * * Internal error in batcher * * *" );
        error( EI_None, "%ld bytes article was batched as %ld-%ld (%ld)", size, pos1, pos2, pos2 - pos1 );
        return Err;
        }

    return r;
    }





bool BatchApp::put_art( char *art, int ofd, const char *code )
    {
    int		ifd;
    bool	ret;

    ifd = _sopen( art, O_RDONLY|O_BINARY, SH_DENYWR );
    if( ifd < 0 )
        {
        error( EI_Full, "Can't open article %s", art );
        return Err;
        }

    ret = do_put_art( code, ofd, ifd );

    close( ifd );

    return ret;
    }





/****************************************************************************
                               Recode file
****************************************************************************/

static bool do_recode( const char *tn, int ofd, int ifd )
    {
    recoder	r( (char*) tn );

    if( !r.valid() )
        {
        error( EI_None, "Can't load code table");
        return Err;
        }

    long	size = filelength( ifd );
    if( size <= 0 )
        {
        error( EI_Full, "Can't get article size");
        return Err;
        }

    while( size > 0 )
        {
        int		toread	= size > CBS ? CBS : ((int)size);
        int		l, w;

        l = read( ifd, cb, toread );
        if( l == EOF )
            {
            error( EI_Full, "Can't read article???" );
            return Err;
            }

        if( l != toread )
            error( EI_Full, "Asked %d, got %d - very suspicious", toread, l );

        cb[l] = '\0';

        r.rs( (uchar *)cb );

        w = write( ofd, cb, l );
        if( w != l )
            {
            error( EI_Full, "Can't write batch" );
            return Err;
            }

        size -= toread;
        }

    return Ok;
    }

bool BatchApp::recode( char *art, int ofd, const char *codetab )
    {
    int		ifd;
    bool	ret;

    ifd = _sopen( art, O_RDONLY|O_BINARY, SH_DENYWR );
    if( ifd < 0 )
        {
        error( EI_Full, "Can't open article %s", art );
        return Err;
        }

    ret = do_recode( codetab, ofd, ifd );

    close( ifd );

    return ret;
    }



