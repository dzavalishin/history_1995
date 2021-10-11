/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Change file extension (rename)
 *
 *      $Log: chg_ext.c $
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
#include    <stdlib.h>


bool
UnbatchApp::change_ext( const char *fname_a, const char *new_ext_a )
	{
	char    odr[_MAX_DRIVE], opt[_MAX_DIR], ofn[_MAX_FNAME], oext[_MAX_EXT];
	char    fname[_MAX_PATH], new_fname[_MAX_PATH], fullpath[_MAX_PATH];
	char	new_ext[_MAX_EXT];

	strncpy( fname, fname_a, _MAX_PATH );
	strncpy( new_ext, new_ext_a, _MAX_EXT );

	if( _fullpath( fullpath, fname, _MAX_PATH ) == NULL )
		{
		error( EI_Full, "_fullpath failed" );
		return Err;
		}

	_splitpath( fullpath,  odr, opt, ofn, oext );
	_makepath( new_fname, odr, opt, ofn, new_ext );

	if( rename( fullpath, new_fname ) != 0 )
		{
		error( EI_Full, "Can't rename '%s' to '%s'", fullpath, new_fname );
		return Err;
		}

	return Ok;
	}





