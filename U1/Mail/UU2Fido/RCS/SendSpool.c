head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	95.11.05.13.50.34;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.11.04.01.21.46;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.11.02.14.25.20;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.08.02.13.11.41;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.07.00.20.29;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@Before fido_msg rewrite
@
text
@/************************ UUCP to FIDO gate ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	Put letter to InNewsSpool
 *
 *      $Log: SendSpool.c $
 *      Revision 1.4  1995/11/04 01:21:46  dz
 *      First RFC_Msg-based version, that compiles and does something
 *
 *      Revision 1.3  1995/11/02 14:25:20  dz
 *      Before switching to rfc_msg
 *
 *      Revision 1.2  1995/08/02 13:11:41  dz
 *      C Set fixes
 *
 *      Revision 1.1  1995/03/07  00:20:29  dz
 *      Initial revision
 *
 *      
 *         Rev 1.3   12 Apr 1993 12:08:24   dz
 *      stdin replaced with in - support for uupc 5.06
 *      
 *         Rev 1.2   28 Nov 1992 23:27:22   dz
 *      String to const char *
 *      
 *         Rev 1.1   23 Oct 1992 14:49:28   dz
 *      'try' ident. eliminated
 *      
 *         Rev 1.0   18 Jun 1992 11:30:42   dz
 *      Initial revision.
 *
 *
 *
 *
\*/

#include	"uu2fido.h"
#include	<fido_pkt.h>

#include	<uu2io.h>
#include	<share.h>

static bool		maketemp( char *ext, int *o_fd );

bool Uu2FidoApp::
spool_news()
    {
    int		ofd;

    log( "b", "Moving to spool directory" );

    if( maketemp( ".a", &ofd ) == Err )
        return Err;

    FILE *out = fdopen( ofd, "w" );
    
    if( NULL == out )
        {
        close( ofd );
        return Err;
        }
    
    bool rc = copy_data( msg, out );
    
    fclose( out );
    
    return rc;
    }


/****************************************************************************
						Create temp file
****************************************************************************/


#define	MAX_TRY		20

static bool
maketemp( char *ext, int *o_fd )
    {
    int			fd;
    char		fn[80];
    ulong		t = time(NULL);
    static		modt = 0;
    int			ntry = 0;

    do
        {
        sprintf( fn, "%s\\%08lX%s", (const char *)conf.newsin_dir.c_str(),
                                                                (t<<5) + (modt++), ext );
        fd = _sopen( fn, O_CREAT|O_EXCL|O_BINARY|O_WRONLY, SH_DENYRW, 0666 );

        if( ntry++ > MAX_TRY )
            {
            error( EI_Full, "Too many tries - can't create temp file" );
            return Err;
            }
        }
    while( fd < 0 );

    *o_fd = fd;
    return Ok;
    }





@


1.4
log
@First RFC_Msg-based version, that compiles and does something
@
text
@d8 3
a44 2
//static bool		copy_data( int ffd, int tfd  );

a55 4

//	rewind_to_text();
//	lseek( 0, 0L, SEEK_SET );
    
a68 11
    
#if 0
    if( copy_data( 0, ofd ) == Err )
        {
        close( ofd );
        return Err;
        }

    close( ofd );
    return Ok;
#endif
a106 58
#if 0
static char *getbuf( int * );

static bool
copy_data( int ffd, int tfd  )
    {
    int		bl, r;
    char	*buf;

    if( (buf = getbuf( &bl )) == NULL )
        return Err;

// NB! buf must be < 32K!

    while( 1 )
        {
        if( (r = ::read( ffd, buf, bl )) <= 0 )
            break;

        if( ::write( tfd, buf, r ) != r )
            {
            r = -1;
            break;
            }
        }

    free( buf );

    if( r == -1 )
        return Err;

    return Ok;
    }


#define MAXB	(31*1024)

static char *
getbuf( int *bs )
    {
    int		s = MAXB;
    char	*b;

    do {
        b = (char*) malloc( s );
        *bs = s;
        s /= 2;

        if( s == 2 )
            return NULL;

        } while( b == NULL );

    return b;
    }


#endif
@


1.3
log
@Before switching to rfc_msg
@
text
@d8 3
d42 1
a42 1
static bool		copy_data( int ffd, int tfd  );
d45 1
a45 1
bool
d47 2
a48 2
	{
	int		ofd;
d50 1
a50 1
	log( "b", "Moving to spool directory" );
d52 2
a53 2
	if( maketemp( ".a", &ofd ) == Err )
		return Err;
d58 26
a83 10

	if( copy_data( 0, ofd ) == Err )
		{
		close( ofd );
		return Err;
		}

	close( ofd );
	return Ok;
	}
d95 24
a118 25
	{
	int			fd;
	char		fn[80];
	ulong		t = time(NULL);
	static		modt = 0;
	int			ntry = 0;

	do
		{
		sprintf( fn, "%s\\%08lX%s", (const char *)conf.newsin_dir.c_str(),
									(t<<5) + (modt++), ext );
		fd = _sopen( fn, O_CREAT|O_EXCL|O_BINARY|O_WRONLY, SH_DENYRW, 0666 );

		if( ntry++ > MAX_TRY )
			{
			error( EI_Full, "Too many tries - can't create temp file" );
			return Err;
			}
		}
	while( fd < 0 );

	*o_fd = fd;
	return Ok;
	}

d121 1
d126 3
a128 3
	{
	int		bl, r;
	char	*buf;
d130 2
a131 2
	if( (buf = getbuf( &bl )) == NULL )
		return Err;
d135 11
a145 11
	while( 1 )
		{
		if( (r = ::read( ffd, buf, bl )) <= 0 )
			break;

		if( ::write( tfd, buf, r ) != r )
			{
			r = -1;
			break;
			}
		}
d147 1
a147 1
	free( buf );
d149 2
a150 2
	if( r == -1 )
		return Err;
d152 2
a153 2
	return Ok;
	}
d160 3
a162 8
	{
	int		s = MAXB;
	char	*b;

	do {
		b = (char*) malloc( s );
		*bs = s;
		s /= 2;
d164 4
a167 2
		if( s == 2 )
			return NULL;
d169 2
a170 1
		} while( b == NULL );
d172 1
a172 2
	return b;
	}
d174 2
d178 1
@


1.2
log
@C Set fixes
@
text
@d8 5
a12 2
 * Revision 1.1  1995/03/07  00:20:29  dz
 * Initial revision
d33 1
a33 1
#include	"fido_pkt.h"
d53 1
a53 1
	rewind_to_text();
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/uu2fido/vcs/sendspol.c_v  $
a31 1
#include	<dir.h>
d33 1
d84 1
a84 1
		fd = open( fn, O_CREAT|O_EXCL|O_BINARY|O_WRONLY|O_DENYALL, 0666 );
@
