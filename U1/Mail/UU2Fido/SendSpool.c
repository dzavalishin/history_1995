/************************ UUCP to FIDO gate ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	Put letter to InNewsSpool
 *
 *      $Log: SendSpool.c $
 *      Revision 1.5  1995/11/05 13:50:34  dz
 *      Before fido_msg rewrite
 *
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





