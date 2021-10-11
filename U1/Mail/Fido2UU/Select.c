/************************ FIDO to UUCP convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :    Separator
 *
 *      $Log: Select.c $
 *      Revision 1.3  1995/11/06 01:42:16  dz
 *      Using FTN_Msg! Seems to be working.
 *
 *      Revision 1.2  1995/08/08 13:22:34  dz
 *      App class used
 *
 *      Revision 1.1  1995/03/11  18:24:01  dz
 *      Initial revision
 *
 *      Revision 1.1  1995/03/11  18:24:01  dz
 *      Initial revision
 *
 *      
 *         Rev 1.7   07 Jun 1993 16:38:00   dz
 *      New message flags system
 *      
 *         Rev 1.6   28 Nov 1992 23:09:06   dz
 *      Style fixes
 *      
 *         Rev 1.5   29 Jun 1992 15:34:06   dz
 *      Ignoring InTransit flag
 *      
 *         Rev 1.4   18 Jun 1992 10:32:34   dz
 *      Selecting messages by dest. address
 *      
 *         Rev 1.3   04 Feb 1992 00:10:16   dz
 *      Ignore in-Transit messages!
 *      
 *         Rev 1.2   29 Aug 1991 07:06:36   dz
 *      Reckognition of messages to user@domain added.
 *      
 *         Rev 1.1   29 Aug 1991 06:38:02   dz
 *      Log record shortened
 *      
 *         Rev 1.0   27 Aug 1991 02:47:18   dz
 *      Initial revision.
 *
 *
\*/


#include    "fido2uu.h"
#include    <ctype.h>



bool
Fido2uuApp::select( FTN_Msg &fm )
    {
    fido_user    to;
    
    fm.get_to( to );
    
    if( (fm.attr()) & (FF_Sent|FF_Lock) )
        return No;
    
    if( !conf.to_any_addr )                    // Take dest. address in account
        {
        ftn_def def;
        
        if( Ok != ftn::find( def, to ) )
            return No;
        }
    
    if( strchr( to.name(), '@' ) )                // Seems to be user@domain
        {
        log( "a", "-----> fido to '%s', converting",    to.name() );
        return Yes;
        }
    
    
    const char    *a, *b;
    bool        eq = No;
    
    a = conf.magic.c_str();
    b = to.name();
    
    while( 1 )
        {
        if( tolower( *a ) == tolower( *b ) )
            {
            if( *a == '\0' )
                {
                eq = Yes;
                break;
                }
            a++; b++;
            continue;
            }
        
        if(
           (*a == ' ' && *b == '_') ||
           (*b == ' ' && *a == '_')
          ) {
        a++; b++;
        continue;
        }
        
        eq = No;
        break;
        }
    
    if( eq == Yes )
        log( "a", "-----> to '%s', converting",    to.name() );
    
    return eq;
    }














