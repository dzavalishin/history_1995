head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	96.03.27.10.46.53;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.03.09.11.27.12;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.12.21.20.45;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.24.01;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
log
@current
@
text
@/************************ FIDO2UU ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *    Module     :    Attachments encoding engine
 *
 *      $Log: Encode.c $
 *      Revision 1.3  1996/03/09 11:27:12  dz
 *      reformatted
 *
 *      Revision 1.2  1995/04/12  21:20:45  dz
 *      IBM C Set version seems to be working...
 *
 *      Revision 1.1  1995/03/11  18:24:01  dz
 *      Initial revision
 *
 *         Rev 1.5   07 Jun 1993 16:35:34   dz
 *      New message flags system, bug fixes.
 *      
 *         Rev 1.4   28 Nov 1992 22:52:30   dz
 *      Error fixed: list of directories was overwritten
 *      after first call.
 *      
 *         Rev 1.3   23 Oct 1992 14:32:06   dz
 *      error
 *      
 *         Rev 1.2   18 Sep 1992 02:44:56   dz
 *      Searching in given directories
 *      
 *         Rev 1.1   18 Jun 1992 10:30:04   dz
 *      Cosmetic
 *      
 *         Rev 1.0   04 Feb 1992 00:07:18   dz
 *      Initial revision.
 *
 *
\*/

//    TODO
//
//    - This stuff doesn't check for errors!
//    - ATOB
//


#include    "fido2uu.h"

#include    "encode.h"
#include    <ctype.h>
//#include    <dir.h>



static bool        extract_name( string &name, string &names );
static bool        find_file_to_encode( char *found_fn, int ffn_len, string fn );

bool
encode_attaches( FTN_Msg &fm, RFC_Msg &out, long *files_sent )
    {
    fmsg_attr   at;
    string      names;                            // File names list
    string      fn;                                // Current file name, relative
    char        found_fn[80];                        // Current file name, absolute
    bool        ll;                                    // Local letter
    bool        was_an_error = No;

    fm.get_attr( at );                                // Get msg attributes
    if( !(at & FF_FileAttached) )                    // No file attached?
        return Ok;                                    // All done.


    ll = at & FF_Local ? Yes : No;                    // Local letters have full
                                                    // file names in subject

    fm.get_subj( names );                            // Get file names
    log( "fu", "Attaches: %s", (const char *)names );                // Log activity

    while( extract_name( fn, names ) == Yes )        // Extract one name
        {

        if( !ll )                                    // Remote letter
            {
            if( find_file_to_encode( found_fn, 80, fn ) == Err )
                {
                error( EI_None, "File '%s' not found", fn );
                was_an_error = Yes;
                continue;
                }
            }
        else                                        // Local letter
            {
            strncpy( found_fn, fn, 80 );
            }

        debug("Encoding '%s'", fn );
        uuencode( found_fn, out );                        // Encode it

        (*files_sent)++;

        if( (at & FF_KillFileSent) || !(at & FF_Local) )    // Local message?
            {
            debug("Deleting '%s'", found_fn );
            chmod( found_fn, 0666 );                        // --> R/W mode
            unlink( found_fn );                            // Kill к ядpене фене
            }
        }

    return was_an_error ? Err : Ok;                    // Опаньки...
    }



static bool extract_name( string &name, string &all_names )
    {
    //const char    *beg = names;                  // Start of string
    const char *begin = all_names;
    const char *namep = begin;
    const char *name_end;

    while( isspace( *namep ) )                     // Skip spaces
        namep++;

    if( *namep == '\0' )                           // No more names?
        return No;                                 // Done

    name_end = namep;
    while( *name_end && !isspace( *name_end ) )    // Get one
        name_end++;

    name = all_names.substr( namep-begin, name_end-namep );
    all_names = name_end;                          // Move rest of string
        
    return Yes;
    }



static bool
find_file_to_encode( char *found_fn, int ffn_len, string fn )
    {
    char    drv[_MAX_DRIVE], dir[_MAX_DIR];
    char    abs[_MAX_PATH], name[_MAX_FNAME], ext[_MAX_EXT];
    char    *p;

    _splitpath( (char*)(fn.c_str()), drv, dir, name, ext );
    _makepath( abs, "", (char *)(conf.netfile_dir.c_str()), name, ext );

    if( access( abs, 04 ) == 0 )
        {
        strncpy( found_fn, abs, ffn_len );
        return Ok;
        }

    const        blen = 300;
    char        buf[blen+1];

    strncpy( buf, conf.infile_dirs.c_str(), blen );
    buf[blen-1] = '\0';

    for(
        p = strtok( buf, "," );
        p;
        p = strtok( NULL, "," )
       )
        {
        _makepath( abs, "", p, name, ext );

        if( access( abs, 04 ) == 0 )
            {
            strncpy( found_fn, abs, ffn_len );
            return Ok;
            }
        }

    return Err;
    }










@


1.3
log
@reformatted
@
text
@d8 3
d54 2
a55 2
static bool        extract_name( char *name, const char *names );
static bool        find_file_to_encode( char *found_fn, int ffn_len, char *fn );
d58 1
a58 1
encode_attaches( FTN_Msg &fm, FILE *out, long *files_sent )
d62 1
a62 1
    char        fn[80];                                // Current file name, relative
d113 20
a132 20
static bool
extract_name( char *name, const char *names ) {
    const char    *beg = names;                      // Start of string
    char    nbuf[50], *np;

    while( isspace( *names ) )                        // Skip spaces
        names++;

    if( *names == '\0' )                            // No more names?
        return No;                                    // Done

    np = nbuf;
    while( *names && !isspace( *names ) )            // Get one
        *np++ = *names++;

    *np = '\0';

    beg = names;                            // Move rest of string
    strcpy( name, nbuf );                        // Full name

d139 1
a139 1
find_file_to_encode( char *found_fn, int ffn_len, char *fn )
d145 1
a145 1
    _splitpath( fn, drv, dir, name, ext );
@


1.2
log
@IBM C Set version seems to be working...
@
text
@d5 1
a5 1
 *	Module 	:	Attachments encoding engine
d8 5
a12 2
 * Revision 1.1  1995/03/11  18:24:01  dz
 * Initial revision
a13 1
 *      
d36 1
a36 1
//	TODO
d38 2
a39 2
//	- This stuff doesn't check for errors!
//	- ATOB
d43 1
a43 1
#include	"fido2uu.h"
d45 3
a47 3
#include	"encode.h"
#include	<ctype.h>
//#include	<dir.h>
d51 2
a52 2
static bool		extract_name( char *name, char *names );
static bool		find_file_to_encode( char *found_fn, int ffn_len, char *fn );
d55 49
a103 49
encode_attaches( fido_msg *fm, FILE *out, long *files_sent )
	{
	fmsg_attr	at;
	char		names[80];							// File names list
	char		fn[80];								// Current file name, relative
	char		found_fn[80];						// Current file name, absolute
	bool		ll;									// Local letter
	bool		was_an_error = No;

	fm->get_attr( at );								// Get msg attributes
	if( !(at & FF_FileAttached) )					// No file attached?
		return Ok;									// All done.


	ll = at & FF_Local ? Yes : No;					// Local letters have full
													// file names in subject

	fm->get_subj( names );							// Get file names
	log( "fu", "Attaches: %s", names );				// Log activity

	while( extract_name( fn, names ) == Yes )		// Extract one name
		{

		if( !ll )									// Remote letter
			{
			if( find_file_to_encode( found_fn, 80, fn ) == Err )
				{
				error( EI_None, "File '%s' not found", fn );
				was_an_error = Yes;
				continue;
				}
			}
		else										// Local letter
			{
			strncpy( found_fn, fn, 80 );
			}

		debug("Encoding '%s'", fn );
		uuencode( found_fn, out );						// Encode it

		(*files_sent)++;

		if( (at & FF_KillFileSent) || !(at & FF_Local) )	// Local message?
			{
			debug("Deleting '%s'", found_fn );
			chmod( found_fn, 0666 );						// --> R/W mode
			unlink( found_fn );							// Kill к ядpене фене
			}
		}
d105 2
a106 2
	return was_an_error ? Err : Ok;					// Опаньки...
	}
d111 3
a113 3
extract_name( char *name, char *names ) {
	char	*beg = names;							// Start of string
	char	nbuf[50], *np;
d115 2
a116 2
	while( isspace( *names ) )						// Skip spaces
		names++;
d118 2
a119 2
	if( *names == '\0' )							// No more names?
		return No;									// Done
d121 3
a123 3
	np = nbuf;
	while( *names && !isspace( *names ) )			// Get one
		*np++ = *names++;
d125 1
a125 1
	*np = '\0';
d127 2
a128 2
	strcpy( beg, names );							// Move rest of string
	strcpy( name, nbuf );						// Full name
d130 2
a131 2
	return Yes;
	}
d137 34
a170 41
	{
	char	drv[_MAX_DRIVE], dir[_MAX_DIR];
	char	abs[_MAX_PATH], name[_MAX_FNAME], ext[_MAX_EXT];
	char	*p;

	_splitpath( fn, drv, dir, name, ext );

//	fnsplit( conf.netfile_dir, drv, dir, NULL, NULL );
//	fnmerge( abs, drv, dir, name, ext );
//	fnmerge( abs, NULL, conf.netfile_dir.c_str(), name, ext );
	_makepath( abs, "", (char *)(conf.netfile_dir.c_str()), name, ext );

	if( access( abs, 04 ) == 0 )
		{
		strncpy( found_fn, abs, ffn_len );
		return Ok;
		}

	const		blen = 300;
	char		buf[blen+1];

	strncpy( buf, conf.infile_dirs.c_str(), blen );
	buf[blen-1] = '\0';

	for(
			p = strtok( buf, "," );
			p;
			p = strtok( NULL, "," )
			)
		{

//		fnsplit( p, drv, dir, NULL, NULL );
//		fnmerge( abs, drv, dir, name, ext );
		_makepath( abs, "", p, name, ext );

		if( access( abs, 04 ) == 0 )
			{
			strncpy( found_fn, abs, ffn_len );
			return Ok;
			}
		}
d172 2
a173 2
	return Err;
	}
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/fido2uu/vcs/encode.c_v  $
d45 1
a45 1
#include	<dir.h>
d136 2
a137 2
//	char	drv[5], dir[70];
	char	abs[85], name[15], ext[5];
d140 1
a140 1
	fnsplit( fn, NULL, NULL, name, ext );
d144 2
a145 1
	fnmerge( abs, NULL, conf.netfile_dir.c_str(), name, ext );
d168 1
a168 1
		fnmerge( abs, NULL, p, name, ext );
@
