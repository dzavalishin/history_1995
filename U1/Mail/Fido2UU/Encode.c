/************************ FIDO2UU ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *    Module     :    Attachments encoding engine
 *
 *      $Log: Encode.c $
 *      Revision 1.4  1996/03/27 10:46:53  dz
 *      current
 *
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










