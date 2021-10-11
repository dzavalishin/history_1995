/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Run 'compress -d' on a .z file
 *
 *      $Log: decomp.c $
 *      Revision 1.3  1995/08/07 14:55:01  dz
 *      Using class App
 *
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


