/************************ FIDO to UUCP convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :	Dest. address detection module
 *
 *      $Log: Address.c $
 *      Revision 1.5  1996/03/09 11:25:44  dz
 *      reformatted
 *
 *      Revision 1.4  1995/08/08 13:22:20  dz
 *      App class used
 *
 *      Revision 1.2  1995/04/12  21:20:45  dz
 *      IBM C Set version seems to be working...
 *
 *      Revision 1.1  1995/03/11  18:24:01  dz
 *      Initial revision
 *      
 *         Rev 1.12   07 Jun 1993 16:38:34   dz
 *      String to const char *, part 2 :(
 *      
 *         Rev 1.11   28 Nov 1992 23:09:54   dz
 *      String to const char *
 *      
 *         Rev 1.10   11 Nov 1992 05:54:06   dz
 *      New address conversion scheme
 *
 *         Rev 1.8   09 Oct 1992 04:24:52   dz
 *      echo2uu support removed
 *
 *         Rev 1.6   18 Jun 1992 10:33:50   dz
 *      fidoaddr2uuaddr() written
 *      conf.free_domain support
 *
 *         Rev 1.5   22 Mar 1992 19:28:50   dz
 *      "registered" flag - identifies registered user mode.
 *
 *         Rev 1.4   04 Feb 1992 00:10:58   dz
 *      p.f.n.z address format fixed
 *
 *         Rev 1.3   11 Oct 1991 05:22:02   dz
 *      Free mode and echo2uu support added.
 *
 *         Rev 1.2   29 Aug 1991 07:05:20   dz
 *      Now reckognizing messages to user@domain withno to: line in text
 *
 *         Rev 1.1   29 Aug 1991 06:36:26   dz
 *      Bug fixed in 'to:' line search.
 *
 *         Rev 1.0   27 Aug 1991 02:46:46   dz
 *      Initial revision.
 *
 *
 *
\*/


#include	"fido2uu.h"
#include	<userlist.h>
#include	<ctype.h>




bool
Fido2uuApp::get_address( char *uu_to, FTN_Msg &fm )
    {
    string         	buf;
    fido_user	fido_to;
    
    strcpy( uu_to, "Unknown" );					// Fill in dummy name
    
    fm.get_to( fido_to );						// Get FIDO destination
    if( strchr( fido_to.name(), '@' ) )			// Seems to be user@domain
        {
        const uchar	*cp	= (const uchar *)fido_to.name();		// Use it as destination
        
        debug("Using FIDO msg 'to' field: '%s'", cp );
        
        while( *cp == ' ' || *cp == '\t' )		// Skip leading spaces
            cp++;
        
        while( *cp > ' ' )						// Get address
            *uu_to++ = *cp++;
        
        *uu_to = '\0';							// Close string
        
        return Ok;								// Done
        }
    
    
    fm.rewind();
    while( !fm.gets( buf ) )
        {
        buf.lcase();   	                        // To lower case
        
        if( strnicmp( buf, "to:", 3 ) )		// to: line?
            continue;
        
        debug("get_address: '%s'", buf.c_str() );
        
        const uchar *cp = buf;
        
        while( *cp != ':' && *cp )			// Find ':'
            cp++;
        
        if( *cp != ':' )
            {
            error( EI_None, "Missing ':' in 'To:' line: '%s'", (const char *)buf );
            return Err;
            }
        
        cp++;								// Skip ':'
        
        while( *cp && (*cp == ' ' || *cp == '\t') )
            cp++;
        
        if( *cp < ' ' )
            {
            error( EI_None, "Bad `To:' line contents: '%s'", cp );
            return Err;
            }
        
        while( *cp > ' ' )
            *uu_to++ = *cp++;
        
        *uu_to = '\0';
        
        return Ok;
        }
    
    error( EI_None, "`To:' line missing, msg skipped");
    return Err;
    }






bool
Fido2uuApp::get_from( bool *registered, uucp_addr &uu_fm, const fido_user &ffm )
    {
    fido_user_def	ud;
    
    if( fuser.get( ud, ffm ) != Err )
        {
        char	buf[200];
        sprintf( buf, "%s@%s", (const char *)ud.uu_name().c_str(), (const char *)ud.uu_from_domain().c_str() );
        uu_fm = buf;
        debug("get_from: %s", (const char *)uu_fm );
        *registered = Yes;
        return Ok;
        }
    
    *registered = No;
    
    if( conf.free_mode )
        {
        log( "a", "Free mode user '%s'", (const char *)((String) ffm).c_str() );
        
        ftn_def   def;
        
        if( Ok != ftn::match( def, ffm ) )
            {
            error( EI_None, "Can't find corresponding FTN for '%s'", (const char *)((String)ffm).c_str() );
            return Err;
            }
        
        if( ffm.convert_to_uucp( uu_fm, def.uucp_d.c_str() ) == Err )
            {
            error( EI_None, "Can't convert FIDO address (%s) to uucp form",
                  (const char *)((String)ffm).c_str() );
            return Err;
            }
        
        debug( "get_from: %s", (const char *)uu_fm );
        return Ok;
        }
    
    error( EI_None, "invalid user '%s'", (const char *)((String)ffm).c_str() );
    return Err;
    }



bool
Fido2uuApp::fidoaddr2uuaddr( uucp_addr &uu_fm, const char *f_fm )
    {
    fido_user 		ffm;
    fido_user_def	ud;
    
    log( "A", "Converting to uucp: '%s'", f_fm );
    
    if( ffm.cparse( f_fm ) == Err )
        return Err;
    
    if( strchr( ffm.name(), '@' ) )
        {
        log( "A", "Domain-type address, refused: %s", ffm.name() );
        return Err;
        }
    
    //	if( ffm.zone() == 0 )
    //		ffm.zone( conf.our_addr.zone );
    
    
    if( fuser.get( ud, ffm ) != Err )
        {
        char	buf[200];
        sprintf( buf, "%s@%s (%s)", (const char *)ud.uu_name().c_str(), (const char *)ud.uu_from_domain().c_str(),
                ud.fido_address().name() );
        uu_fm = buf;
        log( "A", "fidoaddr2uuaddr: %s", (const char *)uu_fm );
        return Ok;
        }
    
    log( "A", "Free mode address '%s'",	(const char *)((String)ffm).c_str() );
    
    ftn_def   def;
    
    if( Ok != ftn::match( def, ffm ) )
        {
        error( EI_None, "Can't find corresponding FTN for '%s'", (const char *)((String)ffm).c_str() );
        return Err;
        }
    
    if( ffm.convert_to_uucp( uu_fm, def.uucp_d.c_str() ) == Err )
        {
        error( EI_None, "Can't convert FIDO address (%s) to uucp form",
              (const char *)((String)ffm).c_str() );
        return Err;
        }
    
    log( "A", "fidoaddr2uuaddr: %s", (const char *)uu_fm );
    return Ok;
    }



