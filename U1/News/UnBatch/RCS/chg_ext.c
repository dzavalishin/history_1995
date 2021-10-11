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
 *  Module  :   Change file extension (rename)
 *
 *      $Log: chg_ext.c $
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





@


1.3
log
@IBM C Set version. Phew, it was not a piece of cake to get here...
@
text
@d8 6
d30 1
a30 1
change_ext( const char *fname_a, const char *new_ext_a )
@


1.2
log
@rewriting for C Set
@
text
@d8 3
a13 9
 *      
 *         Rev 1.2   28 Nov 1992 22:32:04   dz
 *      consts
 *      
 *         Rev 1.1   23 Oct 1992 15:03:42   dz
 *      error()
 *      
 *         Rev 1.0   21 Feb 1992 11:21:16   dz
 *      Initial revision.
d20 1
a20 2
//#include	<dir.h>
//#include    <fcntl.h>
d24 1
a24 1
change_ext( const char *fname, const char *new_ext )
d26 21
a46 11
    char    odr[5], opt[80], ofn[15], oext[5];
    char    new_fname[120];

    fnsplit( fname,     odr, opt, ofn, oext );
    fnmerge( new_fname, odr, opt, ofn, new_ext );

    if( rename( fname, new_fname ) != 0 )
        {
		error( EI_Full, "Can't rename '%s' to '%s'", fname, new_fname );
        return Err;
        }
d48 1
a48 1
    return Ok;
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/unbatch/vcs/chg_ext.c_v  $
d26 1
a26 1
#include	<dir.h>
@
