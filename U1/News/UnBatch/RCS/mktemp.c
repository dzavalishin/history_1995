head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	95.08.07.14.55.01;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.04.09.18.25.15;	author dz;	state Exp;
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


1.4
log
@Using class App
@
text
@/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Create & open temp. file
 *
 *      $Log: mktemp.c $
 * Revision 1.3  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.3  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.2  1995/04/09  10:45:11  dz
 * rewriting for C Set
 *
 * Revision 1.1  1995/03/11  18:32:26  dz
 * Initial revision
 *
 *      
 *
\*/

#include    "unbatch.h"
#include    "..\..\version.h"
#include	<fcntl.h>
#include	<share.h>
#include	<stdlib.h>
#include	<errno.h>


/****************************************************************************
                               Create temp file
****************************************************************************/


#define	MAX_TRY		20

void
UnbatchApp::maketemp( const char *outdir, const char *ext, int *o_fd )
	{
	int			fd;
	char		fn[80];
	ulong		t = time(NULL);
	static		modt = 0;
	int			n_try = 0;

	do
		{
		sprintf( fn, "%s\\%08lX%s", outdir, (t<<5) + (modt++), ext );
		fd = _sopen( fn, O_CREAT|O_EXCL|O_BINARY|O_WRONLY, SH_DENYRW, 0666 );

		if( n_try++ > MAX_TRY )
			fatal( EC_UU2_Error, EI_Full, "Too many tries - can't create temp file");
		}
	while( fd < 0 );

	*o_fd = fd;
	}



@


1.3
log
@IBM C Set version. Phew, it was not a piece of cake to get here...
@
text
@d8 6
d40 1
a40 1
maketemp( const char *outdir, const char *ext, int *o_fd )
@


1.2
log
@rewriting for C Set
@
text
@d8 3
a14 19
 *         Rev 1.5   28 Nov 1992 22:31:44   dz
 *      consts
 *      
 *         Rev 1.4   23 Oct 1992 15:02:42   dz
 *      error()
 *      
 *         Rev 1.3   11 Sep 1992 23:36:58   dz
 *      'try' is a keyword now
 *      
 *         Rev 1.2   07 Jul 1992 00:01:02   dz
 *      fatal() - exit codes
 *      
 *         Rev 1.1   21 Feb 1992 12:10:30   dz
 *      Name generation method changed to let us
 *      create up to 32 files/sec.
 *      
 *         Rev 1.0   21 Feb 1992 11:22:14   dz
 *      Initial revision.
 *
a19 1
//#include	<dir.h>
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/unbatch/vcs/mktemp.c_v  $
d36 1
a36 1
#include	<dir.h>
d38 1
d44 1
a44 1
						Create temp file
d62 1
a62 1
		fd = open( fn, O_CREAT|O_EXCL|O_BINARY|O_WRONLY|O_DENYALL, 0666 );
@
