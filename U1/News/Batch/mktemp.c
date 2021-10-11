/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Create & open temp. file
 *
 *      $Log: mktemp.c $
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
 *      Rev 1.1   28 Nov 1992 22:07:04   dz
 *      String to const char *, try -> ntry
 *
 *      Rev 1.0   26 Apr 1992 14:19:06   dz
 *      Initial revision.
 *
 *
 *
\*/

#include        "batch.h"
#include        "..\..\version.h"
#include	<stdlib.h>
#include	<fcntl.h>
#include	<stdlib.h>
#include	<errno.h>
#include	<share.h>
#include	<time.h>


/****************************************************************************
                                Create temp file
****************************************************************************/


#define	MAX_TRY		80

bool BatchApp::mk_temp_batch( char *o_fn, int *o_fd )
    {
    int		fd;
    char		fn[80];
    int		ntry = 0;
    static int	id = 0;

    if( id == 0 )
        {
        srand( time(NULL) );
        id = rand();
        }

    do
        {
        sprintf( fn, "%s\\b-%.6X.tmp",
                (const char *)conf.newsout_dir.c_str(),
                (id++) % 0xFFFFFFu
               );

        fd = _sopen( fn, O_CREAT|O_EXCL|O_BINARY|O_RDWR, SH_DENYRW, 0666 );

        if( ntry++ > MAX_TRY )
            fatal( EC_OS_Error, EI_Full, "Too many tries - can't create temp file (%s)", fn );
        }
    while( fd < 0 );

    debug( "mk_temp_batch - '%s'", fn );

    *o_fd = fd;
    strcpy( o_fn, fn );
    return Ok;
    }



