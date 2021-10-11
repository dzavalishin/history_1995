/************************ UU2 Config ***************************\
 *
 *	Copyright (C) 1991,1992 by Infinity Soft
 *
 *	Module 	:	User table
 *
 *      $Log: UserList.c $
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   15 Nov 1992 00:09:12   dz
 *      Initial revision.
 *
 *
 *
\*/


#include	<string.h>
#include	<stdlib.h>

#include	"userlist.h"
#include	"setup.h"
#include	"log.h"

fido_userlist	fuser;			// Real user list

	/*************************************************************
						   C'tor/D'tor
	*************************************************************/

fido_userlist::
fido_userlist( void )
	{
	last_free_slot = 0;
	}

fido_userlist::
~fido_userlist( void )
	{
	for( int i = 0; i < last_free_slot; i++ )
		delete users[i];

	last_free_slot = 0;
	}



bool fido_userlist::
add( const char *def )
	{
	if( last_free_slot >= fido_userlist_max_users )
		{
		error( EI_None, "FIDO User table overflow" );
		return Err;
		}

	fido_user_def	*n = new fido_user_def( def );

	if( n->invalid() )
		{
		error( EI_None, "Skipping user def: '%s'", def );
		delete n;
		}

	users[last_free_slot++] = n;
	return Ok;
	}

bool fido_userlist::
get( fido_user_def &result, const char *uucp_name ) const
	{
	for( int i = 0; i < last_free_slot; i++ )
		{
		if( strcmpi( uucp_name, (users[i]->uu_name()).c_str() ) )
			continue;

		result = *(users[i]);
		return Ok;
		}

	return Err;
	}

bool fido_userlist::
get( fido_user_def &result, const fido_user &fa ) const
	{
	for( int i = 0; i < last_free_slot; i++ )
		{

		if( users[i]->fido_address() != fa )
			continue;

		result = *(users[i]);
		return Ok;
		}

	return Err;
	}



bool  fido_userlist::
foreach( bool (*func)( const fido_user_def &, void *arg ), void *arg )
	{
	for( int i = 0; i < last_free_slot; i++ )
		{

		if( func( *(users[i]), arg ) == Err )
			return Err;
		}

	return Ok;
	}














