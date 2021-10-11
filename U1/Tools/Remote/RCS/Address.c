head	1.2;
access;
symbols;
locks;
comment	@ *  @;


1.2
date	95.11.06.14.28.09;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.07.01.21.19.55;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@Current
@
text
@/************************ Gate remote control ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :	Address conversions, etc.
 *
 *      $Log: Address.c $
 *      Revision 1.1  1995/07/01  21:19:55  dz
 *      Initial revision
 *      
 *         Rev 1.2   07 Jun 1993 17:24:44   dz
 *      Using new basic methods to convert addresses
 *      
 *         Rev 1.1   11 Nov 1992 02:03:32   dz
 *      Using new address conversion scheme
 *
 *         Rev 1.0   04 Feb 1992 00:19:10   dz
 *      Initial revision.
 *
 *
 *
\*/



#include	"gremote.h"
#include	<userlist.h>


bool GRemoteApp::
get_from( uucp_addr &uu_fm, const fido_user &ffm )
    {
    fido_user_def	ud;

    if( fuser.get( ud, ffm ) != Err )
        {
        // char	buf[2 0 0];
        // sprintf( buf, "%s@@%s", (const char *)ud.uu_name().c_str(), (const char *)ud.uu_from_domain().c_str() );
        // uu_fm = buf;
        uu_fm = ud.uu_name() + "@@" + ud.uu_from_domain();
        debug("get_from: %s", (const char *)uu_fm );
        return Ok;
        }

    if( conf.free_mode )
        {
        log( "a", "Free mode user '%s'", (const char *)((String)ffm).c_str() );

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




bool // GRemoteApp::
postm_address( fido_user &u )
    {
    fido_user_def	ud;

    if( fuser.get( ud, conf.postmaster.c_str() ) != Ok )
        {
        error( EI_None, "postmaster not found in user table!" );
        return Err;
        }

    u = ud.fido_address();
    return Ok;
    }

@


1.1
log
@Initial revision
@
text
@d7 3
a9 1
 *      $Log:   Q:/tools/remote/vcs/address.c_v  $
d30 1
a30 1
bool
d32 2
a33 2
	{
	fido_user_def	ud;
d35 13
a47 12
	if( fuser.get( ud, ffm ) != Err )
		{
		char	buf[200];
		sprintf( buf, "%s@@%s", (const char *)ud.uu_name().c_str(), (const char *)ud.uu_from_domain().c_str() );
		uu_fm = buf;
		debug("get_from: %s", (const char *)uu_fm );
		return Ok;
		}

	if( conf.free_mode )
		{
		log( "a", "Free mode user '%s'", (const char *)((String)ffm).c_str() );
d57 6
a62 6
		if( ffm.convert_to_uucp( uu_fm, def.uucp_d.c_str() ) == Err )
			{
			error( EI_None, "Can't convert FIDO address (%s) to uucp form",
				(const char *)((String)ffm).c_str() );
			return Err;
			}
d64 3
a66 3
		debug( "get_from: %s", (const char *)uu_fm );
		return Ok;
		}
d68 3
a70 3
	error( EI_None, "invalid user '%s'", (const char *)((String)ffm).c_str() );
	return Err;
	}
d75 1
a75 1
bool
d77 2
a78 2
	{
	fido_user_def	ud;
d80 5
a84 5
	if( fuser.get( ud, conf.postmaster.c_str() ) != Ok )
		{
		error( EI_None, "postmaster not found in user table!" );
		return Err;
		}
d87 2
a88 2
	return Ok;
	}
@
