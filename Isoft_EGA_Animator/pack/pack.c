/************************** Pack byte utilities ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *				Dmitry	Zavalishin	[DZ],
 *			      	Serge	Mednov		[MS],
 *				Gregory	Nickonov	[GN].
 *
 *	Module 	:	Pack input stream.
 *
 *      $Log:$
 *
 *
\*/

#include	<Style.h>
#include	<Stdlib.h>
#include	<String.h>
#include	<Stdio.h>

#define	PACK_C

#include	"Pack.h"


static	char	Pack_Sign = 0xFF;	// Pack label


Pack::Pack( void (*make_s) (byte b) ) {

	make_sym	= make_s;
	exit_flag 	= Ok;
	status    	= STARTED;

	b		= '\0';
	count		= 0;
	}



void
Pack::Do( byte sym ) {

	if( status == STARTED ) {
		b 		= sym;
		exit_flag	= Ok;
		count		= 1;
		status		= REPEAT;
		return;
		}

	if( status == COMPRESS && b == sym ) {
		count++;

		if( count >= 250 ) {
			release_bytes();
			count	= 0;
			status	= STARTED;
			}

		return;
		}

	if( status == REPEAT && b == sym ) {
		status		= COMPRESS;
		count     	= 2;
		exit_flag	= Ok;
		return;
		}

	if( status == COMPRESS && b != sym ) {
		release_bytes();

		if( exit_flag != Ok )
			return;

		b		= sym;
		status		= REPEAT;
		count		= 1;
		exit_flag	= Ok;
		return;
		}

	release_bytes();

	if( exit_flag != Ok )
		return;

	b		= sym;
	status		= REPEAT;
	count		= 1;
	exit_flag 	= Ok;
	}



void
Pack::release_bytes( void ) {

	switch( count ) {

		case 1:
			if( b == Pack_Sign ) {
				(*make_sym) ( Pack_Sign );
				(*make_sym) ( '\0' );
				}
			else
				(*make_sym) ( b );

			break;

		default:
			(*make_sym) ( Pack_Sign );
			(*make_sym) ( count );
			(*make_sym) ( b );
			break;
		}

	exit_flag = Ok;
	}


/****************************************************************************
			UNPACK	support
****************************************************************************/


Unpack::Unpack( int (*make_s) ( void ) ) {


	make_sym	= make_s;
	exit_flag 	= Ok;
	status    	= STARTED;

	b		= '\0';
	count		= 0;
	}



int
Unpack::Do( void ) {

	if( status == STARTED )
		status = REPEAT;

	switch( status ) {

		case REPEAT:
			b	= (*make_sym) ();

			if( b == EOF )
				return	EOF;


			if( b != Pack_Sign ) {
				exit_flag	= Ok;
				return	b;
				}

			count = (*make_sym) ();
			if( count == 0 ) {
				exit_flag	= Ok;
				return	b;
				}

			b 	= (*make_sym) ();

			status	= COMPRESS;
			count--;
			return	b;

		case COMPRESS:
			if( --count < 1 )
				status = REPEAT;

			exit_flag = Ok;
			return	b;

		}

	exit_flag = Err;
	return	Err;
	}


/****************************************************************************
				Destructors
****************************************************************************/


Pack::~Pack( void ) {

	release_bytes();
	}

Unpack::~Unpack( void ) {

	}
