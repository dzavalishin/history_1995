head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	96.05.21.00.21.04;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.07.05.15.02;	author dz;	state Exp;
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


1.4
log
@cosmetical changes
@
text
@/************************ UU2 News Batcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Put an article to batch
 *
 *      $Log: put_art.c $
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



@


1.3
log
@Using App class
@
text
@d8 2
a9 2
 * Revision 1.2  1995/04/10  02:08:14  dz
 * IBM C Set version
d11 2
a12 2
 * Revision 1.1  1995/03/11  18:26:07  dz
 * Initial revision
d14 4
a17 2
 *      
 *         Rev 1.3   07 Jun 1993 17:06:44   dz
d20 1
a20 1
 *         Rev 1.2   23 Oct 1992 14:51:34   dz
d23 1
a23 1
 *         Rev 1.1   08 Jun 1992 09:09:32   dz
d26 1
a26 1
 *         Rev 1.0   26 Apr 1992 14:18:42   dz
d39 1
a39 1
static bool			do_recode( const char *tn, int ofd, int ifd );
d42 1
a42 1
#define		CBS		(8*1024)
d44 1
a44 1
static char		cb[CBS];
d48 6
a53 7
bool
BatchApp::do_put_art( const char *code, int ofd, int ifd )
	{
	char	hdr[24];
	long	size = filelength( ifd );
	int		l;
	bool	r;
d55 6
a60 6
	l = sprintf( hdr, "#! rnews %ld\n", size );
	if( l != write( ofd, hdr, l ) )
		{
		error( EI_Full, "Error writing #!rnews line");
		return Err;
		}
d62 3
a64 3
	long pos1 = tell( ofd );
	r = do_recode( code, ofd, ifd );
	long pos2 = tell( ofd );
d66 6
a71 6
	if( (pos2 - pos1) != size )
		{
		error( EI_None, "* * * Internal error in batcher * * *" );
		error( EI_None, "%ld bytes article was batched as %ld-%ld (%ld)", size, pos1, pos2, pos2 - pos1 );
		return Err;
		}
d73 2
a74 2
	return r;
	}
d80 4
a83 5
bool
BatchApp::put_art( char *art, int ofd, const char *code )
	{
	int		ifd;
	bool	ret;
d85 6
a90 6
	ifd = _sopen( art, O_RDONLY|O_BINARY, SH_DENYWR );
	if( ifd < 0 )
		{
		error( EI_Full, "Can't open article %s", art );
		return Err;
		}
d92 1
a92 1
	ret = do_put_art( code, ofd, ifd );
d94 1
a94 1
	close( ifd );
d96 2
a97 2
	return ret;
	}
d104 1
a104 1
						Recode file
d107 67
a173 62
static bool
do_recode( const char *tn, int ofd, int ifd )
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

bool
BatchApp::recode( char *art, int ofd, const char *codetab )
	{
	int		ifd;
	bool	ret;

	ifd = _sopen( art, O_RDONLY|O_BINARY, SH_DENYWR );
	if( ifd < 0 )
		{
		error( EI_Full, "Can't open article %s", art );
		return Err;
		}
a174 1
	ret = do_recode( codetab, ofd, ifd );
a175 1
	close( ifd );
a176 2
	return ret;
	}
@


1.2
log
@IBM C Set version
@
text
@d8 3
d46 2
a47 2
static bool
do_put_art( const char *code, int ofd, int ifd )
d80 1
a80 1
put_art( char *art, int ofd, const char *code )
d158 1
a158 1
recode( char *art, int ofd, const char *codetab )
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/batch/vcs/put_art.c_v  $
d30 1
d82 1
a82 1
	ifd = open( art, O_RDONLY|O_BINARY|O_DENYWRITE );
d139 1
a139 1
		r.rs( cb );
d160 1
a160 1
	ifd = open( art, O_RDONLY|O_BINARY|O_DENYWRITE );
@
