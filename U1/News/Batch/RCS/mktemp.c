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
@/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Create & open temp. file
 *
 *      $Log: mktemp.c $
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
 *         Rev 1.1   28 Nov 1992 22:07:04   dz
d19 2
a20 2
 *      
 *         Rev 1.0   26 Apr 1992 14:19:06   dz
d27 2
a28 2
#include    "batch.h"
#include    "..\..\version.h"
d44 33
a76 37
bool
BatchApp::mk_temp_batch( char *o_fn, int *o_fd )
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
			{
			fatal( EC_OS_Error, EI_Full, "Too many tries - can't create temp file (%s)", fn );
			}
		}
		while( fd < 0 );


	debug( "mk_temp_batch - '%s'", fn );

	*o_fd = fd;
	strcpy( o_fn, fn );
	return Ok;
	}
@


1.2
log
@IBM C Set version
@
text
@d8 3
d43 1
a43 1
mk_temp_batch( char *o_fn, int *o_fd )
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/batch/vcs/mktemp.c_v  $
d24 1
a24 1
#include	<dir.h>
d28 2
d33 1
a33 1
						Create temp file
d37 1
a37 1
#define	MAX_TRY		20
d42 1
a42 1
	int			fd;
d44 8
a51 1
	int			ntry = 0;
d55 6
a60 3
		sprintf( fn, "%s\\tmpbatXXXXXX", (const char *)conf.newsout_dir.c_str() );
		mktemp( fn );
		fd = open( fn, O_CREAT|O_EXCL|O_BINARY|O_RDWR|O_DENYALL, 0666 );
d67 4
a70 1
	while( fd < 0 );
d73 2
a74 2
    strcpy( o_fn, fn );
    return Ok;
@
