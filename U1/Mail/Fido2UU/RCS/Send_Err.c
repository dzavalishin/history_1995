head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	96.03.09.11.31.02;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.11.06.01.42.16;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.08.13.22.34;	author dz;	state Exp;
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


1.5
log
@Send error letter to postmaster only if requseted.
@
text
@/************************ FIDO to UUCP convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :	Send error back
 *
 *      $Log: Send_Err.c $
 *      Revision 1.4  1995/11/06 01:42:16  dz
 *      Using FTN_Msg! Seems to be working.
 *
 *      Revision 1.3  1995/08/08 13:22:34  dz
 *      App class used
 *
 *      Revision 1.2  1995/04/12  21:20:45  dz
 *      IBM C Set version seems to be working...
 *
 *      Revision 1.1  1995/03/11  18:24:01  dz
 *      Initial revision
 *
 *      
 *         Rev 1.8   07 Jun 1993 16:41:12   dz
 *      Message flags system redone
 *      
 *         Rev 1.7   28 Nov 1992 23:11:56   dz
 *      consts, putting from/to addresses to error letter
 *      
 *         Rev 1.6   11 Nov 1992 05:56:12   dz
 *      New address conversion scheme
 *      
 *         Rev 1.5   23 Oct 1992 14:39:34   dz
 *      error()
 *      
 *         Rev 1.4   18 Sep 1992 02:48:28   dz
 *      exit codes
 *      cosmetical changes
 *      
 *         Rev 1.3   18 Jun 1992 10:37:40   dz
 *      Includes rearranged
 *      
 *         Rev 1.2   04 Feb 1992 00:16:30   dz
 *      fido_msg::set_date( time_t ) used
 *
 *         Rev 1.1   11 Oct 1991 05:26:56   dz
 *      Log track handling corrected.
 *      Now taking postmaster address from setup.
 *      
 *         Rev 1.0   29 Aug 1991 06:37:22   dz
 *      Initial revision.
 *
 *
\*/


#include	"fido2uu.h"
#include	<userlist.h>
#include	<time.h>




static bool
search_dest( fido_user *fido_to, const char *to )
    {
    char	shrt[100];
    char	*p;
    
    
    strncpy( shrt, to, 100 );
    if( (p = strpbrk( shrt, "@@%" ) ) != NULL )
        *p = '\0';
    
    if( (p = strrchr( shrt, '!' ) ) != NULL )
        strcpy( shrt, p+1 );
    
    strlwr( shrt );
    
    //	debug("Long  UUCP name = '%s'", to );
    //	debug("Short UUCP name = '%s'", shrt );
    
    fido_user_def	ud;
    
    if( fuser.get( ud, shrt ) != Err )
        {
        *fido_to = ud.fido_address();
        return Ok;
        }
    
    return Err;
    }



static bool
postmaster_addr( fido_user *fido_to )
    {
    
    if( search_dest( fido_to, conf.postmaster.c_str() ) == Err )
        {
        error( EI_None, "postmaster not found! Error message lost!" );
        fido_to->net( 0 );
        fido_to->node( 0 );
        return Err;
        }
    
    return Ok;
    }




void Fido2uuApp::
put_track( FTN_Msg &fm )
    {
    char	*track = track_get();
    
    while( *track )
        {
        char	part[100], *pp;
        
        while( *track < ' ' )
            {
            if( *track == '\0' )
                break;
            track++;
            }
        
        if( *track == '\0' )
            break;
        
        pp = part;
        
        while( (*track) && (*track >= ' ') && (pp-part < 99) )
            *pp++ = *track++;
        
        *pp = '\0';
        
        fm.puts( part );
        }
    }


void Fido2uuApp::error_letter( FTN_Msg &orig )
    {
    FTN_Msg	fm1;
    FTN_Msg     fm2;
    fido_user	orig_from, orig_to, postm_to;
    long     	t;
    string     	buf;
    
    bool        copy_to_postmaster = log_debug; // In debug mode only
    //if( conf.error_to_postmaster ) copy_to_postmaster = Yes;
    
    time( &t );
    orig.get_from( orig_from );
    orig.get_to( orig_to );
    
    if( fm1.new_msg( conf.netmail_dir ) == Err )
        fatal( EC_UU2_Error, EI_None, "Can't create message file" );
    
    if( copy_to_postmaster && fm2.new_msg( conf.netmail_dir ) == Err )
        error( EI_None, "Can't create error message file" );
    
    buf = 
        string(">>> Module: FIDO2UU\r\n>>> Error delivering message from '") +
        ((string)orig_from) +
        string("' to '")+ ((string)orig_to)+ string("'");
    
    fm1.add_attr( FF_KillSent|FF_Private|FF_Local );
    fm1.set_date( t );
    fm1.set_subj("UU2 Error!");
    fm1.rewind();
    fm1.puts( buf );
    fm1.puts( ">>> Gate log track follows:");
    fm1.puts( "\r\n");
    put_track( fm1 );
    fm1.puts( "\r\n");
    fm1.puts( ">>> Unsent message follows:");
    fm1.puts( "\r\n");
    
    if( copy_to_postmaster )
        {
        fm2.add_attr( FF_KillSent|FF_Private|FF_Local );
        fm2.set_date( t );
        fm2.set_subj("UU2 Error!");
        fm2.rewind();
        fm2.puts( buf );
        fm2.puts( ">>> Gate log track follows:");
        fm2.puts( "\r\n");
        put_track( fm2 );
        fm2.puts( "\r\n");
        fm2.puts( ">>> Unsent message follows:");
        fm2.puts( "\r\n");
        }

    
    orig.rewind();
    while( orig.gets( buf ) != Err )
        {
        fm1.puts( buf );
        if( copy_to_postmaster ) fm2.puts( buf );
        }
    
    
    fm1.set_to( orig_from );
    fm1.set_from( fido_user( ftn::match_aka( orig_from ), conf.magic.c_str() ) );
    
    fm1.detach();						// Send it.

    
    if( copy_to_postmaster )
        {
        if( postmaster_addr( &postm_to ) != Err )
            {
            fm2.set_to( postm_to );
            fm2.set_from( fido_user( ftn::match_aka( postm_to ), conf.magic.c_str() ) );
            fm2.detach();						// Send it.
            }
        }
    }



@


1.4
log
@Using FTN_Msg! Seems to be working.
@
text
@d8 3
d150 3
d160 2
a161 16
    if( fm2.new_msg( conf.netmail_dir ) == Err )
        fatal( EC_UU2_Error, EI_None, "Can't create message file" );
    
    fm1.add_attr( FF_KillSent|FF_Private|FF_Local );
    fm2.add_attr( FF_KillSent|FF_Private|FF_Local );
    
    fm1.set_date( t );
    fm2.set_date( t );
    
    
    fm1.set_subj("UU2 Error!");
    fm2.set_subj("UU2 Error!");
    
    
    fm1.rewind();
    fm2.rewind();
d168 4
a172 2
    fm2.puts( buf );
    
a173 2
    fm2.puts( ">>> Gate log track follows:");
    
a174 2
    fm2.puts( "\r\n");
    
a175 2
    put_track( fm2 );
    
a176 2
    fm2.puts( "\r\n");
    
a177 2
    fm2.puts( ">>> Unsent message follows:");
    
a178 1
    fm2.puts( "\r\n");
d180 15
d200 1
a200 1
        fm2.puts( buf );
d210 1
a210 1
    if( debug )
@


1.3
log
@App class used
@
text
@d8 3
d60 27
a86 27
	{
	char	shrt[100];
	char	*p;


	strncpy( shrt, to, 100 );
	if( (p = strpbrk( shrt, "@@%" ) ) != NULL )
		*p = '\0';

	if( (p = strrchr( shrt, '!' ) ) != NULL )
		strcpy( shrt, p+1 );

	strlwr( shrt );

//	debug("Long  UUCP name = '%s'", to );
//	debug("Short UUCP name = '%s'", shrt );

	fido_user_def	ud;

	if( fuser.get( ud, shrt ) != Err )
		{
		*fido_to = ud.fido_address();
		return Ok;
		}

	return Err;
	}
d92 115
a206 130
	{

	if( search_dest( fido_to, conf.postmaster.c_str() ) == Err )
		{
		error( EI_None, "postmaster not found! Error message lost!" );
		fido_to->net( 0 );
		fido_to->node( 0 );
		return Err;
		}

	return Ok;
	}




void
put_track( fido_msg *fm )
	{
	char	*track = track_get();

	while( *track )
		{
		char	part[100], *pp;

		while( *track < ' ' )
			{
			if( *track == '\0' )
				break;
			track++;
			}

		if( *track == '\0' )
			break;

		pp = part;

		while( (*track) && (*track >= ' ') && (pp-part < 99) )
			*pp++ = *track++;

		*pp = '\0';

		fm->puts( part );
		}
	}


void
Fido2uuApp::error_letter( fido_msg *orig )
	{
	fido_msg	fm1;
	fido_msg	fm2;
	fido_user	orig_from, orig_to, postm_to;
	long		t;
	char		buf[500];

	time( &t );
	orig->get_from( orig_from );
	orig->get_to( orig_to );

	if( fm1.new_msg( conf.netmail_dir.c_str() ) == Err )
		fatal( EC_UU2_Error, EI_None, "Can't create message file" );

	if( fm2.new_msg( conf.netmail_dir.c_str() ) == Err )
		fatal( EC_UU2_Error, EI_None, "Can't create message file" );

	fm1.add_attr( FF_KillSent|FF_Private|FF_Local );
	fm2.add_attr( FF_KillSent|FF_Private|FF_Local );

//	fm1.set_from( conf.our_addr );
//	fm2.set_from( conf.our_addr );

	fm1.set_date( t );
	fm2.set_date( t );


	fm1.set_subj("UU2 Error!");
	fm2.set_subj("UU2 Error!");


	fm1.rewind();
	fm2.rewind();

//#if PUT_ADDRS_TO_ERRLETTER
#if 1
	sprintf( buf,
		">>> Module: FIDO2UU\r\n"
		">>> Error delivering message from '%s' to '%s'",
					(const char *)((String)orig_from).c_str(),
					(const char *)((String)orig_to).c_str()
					);

	fm1.puts( buf );
	fm2.puts( buf );

#else

	fm1.puts( ">>> Error delivering message.");
	fm2.puts( ">>> Error delivering message.");

#endif

	fm1.puts( ">>> Gate log track follows:");
	fm2.puts( ">>> Gate log track follows:");

	fm1.puts( "\r\n");
	fm2.puts( "\r\n");

	put_track( &fm1 );
	put_track( &fm2 );

	fm1.puts( "\r\n");
	fm2.puts( "\r\n");

	fm1.puts( ">>> Unsent message follows:");
	fm2.puts( ">>> Unsent message follows:");

	fm1.puts( "\r\n");
	fm2.puts( "\r\n");


	orig->rewind();
	while( orig->gets( buf ) != Err )
		{
		fm1.puts( buf );
		fm2.puts( buf );
		}


	fm1.set_to( orig_from );
d208 2
d211 11
a221 14
	fm1.flush();
	fm1.detach();						// Send it.



	if( postmaster_addr( &postm_to ) != Err )
		{
		fm2.set_to( postm_to );
        fm2.set_from( fido_user( ftn::match_aka( postm_to ), conf.magic.c_str() ) );
		fm2.flush();
		fm2.detach();						// Send it.
		}

	}
@


1.2
log
@IBM C Set version seems to be working...
@
text
@d8 5
a12 2
 * Revision 1.1  1995/03/11  18:24:01  dz
 * Initial revision
d56 1
a56 1
search_dest( fido_addr *fido_to, const char *to )
d88 1
a88 1
postmaster_addr( fido_addr *fido_to )
d137 1
a137 1
error_letter( fido_msg *orig )
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/fido2uu/vcs/send_err.c_v  $
d47 1
@
