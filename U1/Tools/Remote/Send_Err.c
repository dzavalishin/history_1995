/************************ FIDO to UUCP convertor ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :	Send error back
 *
 *      $Log: Send_Err.c $
 *      Revision 1.3  1995/11/06 14:28:09  dz
 *      Current
 *
 *      Revision 1.2  1995/08/02 14:01:29  dz
 *      minor
 *
 *  Revision 1.1  1995/07/01  21:19:55  dz
 *  Initial revision
 *
 *      
 *         Rev 1.2   07 Jun 1993 17:25:26   dz
 *      New message flags system
 *      
 *         Rev 1.1   28 Nov 1992 23:39:36   dz
 *      up to date
 *      
 *         Rev 1.0   04 Feb 1992 00:19:52   dz
 *      Initial revision.
 *
\*/

#include	"gremote.h"
#include	<userlist.h>

#include	<time.h>
#include	<errno.h>



static bool
search_dest( fido_addr *fido_to, const char *to )
	{
	char	shrt[100];
	char	*p;


	if( (p = strrchr( shrt, '!' ) ) != NULL )
		strncpy( shrt, p+1, 100 );
	else
		strncpy( shrt, to, 100 );
	shrt[99] = '\0';

	if( (p = strpbrk( shrt, "@%" ) ) != NULL )
		*p = '\0';

	strlwr( shrt );

	debug("Long  UUCP name = '%s'", to );
	debug("Short UUCP name = '%s'", shrt );

	fido_user_def	ud;

	if( fuser.get( ud, shrt ) != Err )
		{
		*fido_to = ud.fido_address();
		return Ok;
		}

	return Err;
	}



static bool
postmaster_addr( fido_addr *fido_to )
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


void GRemoteApp::
error_letter( FTN_Msg &orig )
	{
	FTN_Msg     fm1;
	FTN_Msg     fm2;
	fido_user   to;
	long        t;
	string      buf;

	time( &t );

	if( fm1.new_msg( conf.netmail_dir.c_str() ) == Err )
		fatal( EC_UU2_Error, EI_Full, "Can't create message file" );

	if( fm2.new_msg( conf.netmail_dir.c_str() ) == Err )
		fatal( EC_UU2_Error, EI_Full, "Can't create message file" );

	fm1.add_attr( FF_Private|FF_KillSent|FF_Local );
	fm2.add_attr( FF_Private|FF_KillSent|FF_Local );

	fm1.set_date( t );
	fm2.set_date( t );


	fm1.set_subj("Fido2UU Error!");
	fm2.set_subj("Fido2UU Error!");


	fm1.rewind();
	fm2.rewind();

	fm1.puts( ">>> Error delivering message.");
	fm1.puts( ">>> Gate log track follows:");
	fm2.puts( ">>> Error delivering message.");
	fm2.puts( ">>> Gate log track follows:");

	fm1.puts( "\r\n");
	fm2.puts( "\r\n");

//	fm1.puts( track_get() );			// Fill it with track buffer
//	fm2.puts( track_get() );

	put_track( fm1 );
	put_track( fm2 );

	fm1.puts( "\r\n");
	fm2.puts( "\r\n");

	fm1.puts( ">>> Unsent message follows:");
	fm2.puts( ">>> Unsent message follows:");

	fm1.puts( "\r\n");
	fm2.puts( "\r\n");


	orig.rewind();
	while( orig.gets( buf ) != Err )
		{
		fm1.puts( buf );
		fm2.puts( buf );
		}


	orig.get_from( to );
	fm1.set_to( to );

	fm1.set_from(  fido_user( ftn::match_aka( to ), conf.magic.c_str() )  );
	fm2.set_from(  fido_user( ftn::match_aka( to ), conf.magic.c_str() )  );



	fm1.detach();						// Send it.

	if( postmaster_addr( &to ) != Err )
		{
		fm2.set_to( to );
		fm2.detach();						// Send it.
		}

	}



