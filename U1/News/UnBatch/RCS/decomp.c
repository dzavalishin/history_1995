head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	95.08.07.14.55.01;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.09.18.25.15;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.32.26;	author dz;	state Exp;
branches;
next	;


desc
@UU2 News Unbatcher
@


1.3
log
@Using class App
@
text
@/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Run 'compress -d' on a .z file
 *
 *      $Log: decomp.c $
 * Revision 1.2  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.2  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.1  1995/03/11  18:32:26  dz
 * Initial revision
 *
 *
 *
\*/

#include    "unbatch.h"
#include    "..\..\version.h"
#include	<process.h>

// NB!!!
// Some decompressors need -i parameter to decompress
// in 'image' mode. Think, I need one more conf. file parameter.



bool
UnbatchApp::decompress( const char *dir, const char *fn )
	{
	char	bn[80];
	int     ret;

	sprintf( bn, "%s\\%s", dir, fn );
	debug( "Decompressing %s", bn );

	ret = spawnlp( P_WAIT, "compress", "compress.exe", "-d", bn, NULL );

	if( ret != 0 )
		{
		error( EI_Full, "Can't decompress '%s'", bn );
		change_ext( bn, ".bz" );
		}

	return ret == 0 ? Ok : Err;
	}


@


1.2
log
@IBM C Set version. Phew, it was not a piece of cake to get here...
@
text
@d8 6
d32 1
a32 1
decompress( const char *dir, const char *fn )
@


1.1
log
@Initial revision
@
text
@d7 4
a10 10
 *      $Log:   Q:/news/unbatch/vcs/decomp.c_v  $
 *      
 *         Rev 1.2   28 Nov 1992 22:32:32   dz
 *      consts
 *      
 *         Rev 1.1   23 Oct 1992 15:03:56   dz
 *      error()
 *      
 *         Rev 1.0   21 Feb 1992 11:07:12   dz
 *      Initial revision.
@
