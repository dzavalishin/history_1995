/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Create & open temp. file
 *
 *      $Log: mktemp.c $
 *      Revision 1.4  1995/08/07 14:55:01  dz
 *      Using class App
 *
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



