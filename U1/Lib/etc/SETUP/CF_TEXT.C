/************************ UUCP to FIDO gate ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Implementation of config_text class
 *
 *      $Log: CF_TEXT.C $
 * Revision 1.1  1995/03/11  18:14:11  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   14 May 1993 13:41:56   dz
 *      Initial revision.
 *
 *
\*/

#include	"config.h"
#include	<ctype.h>
#include	<strstream.h>
#include	<fstream.h>
#include	<assoc.h>
#include	<sys/stat.h>

#ifdef	TEST
#include	"./test.h"
#endif


// NB!
// There must be way of providing comment-checking func.

config_text::
config_text( istream &f )
	{
	char	*val;

	bad = Yes;

	while( !f.eof() )
		{
		char	buf[200];

		f.getline( buf, 200 );
		if( f.gcount() == 0 )
			break;

		if( f.fail() )
			{
			::error( "Input stream failed" );
			return;
			}

		char *str = buf;

		while( isspace( *str ) )	// Skip leading spaces
			str++;

		if( *(val = strpbrk( str, "\r\n" )) != '\0' )
			*val = '\0';

		val = str;
		while( *val && !isspace( *val ) )
			val++;

		if( val == str )
			continue;  // Line is empty - skip it

		if( *val )
			{
			*val = '\0';
			val++;
			}

		strlwr( str );

		String *Key = new String( (const signed char *)str );
		if( Key == Object::ZERO )
			{
			e_nomem("for key");
			return;
			}

		while( *val && isspace( *val ) )
			val++;

		String *Value = new String( (const signed char *)val );
		if( Value == Object::ZERO )
			{
			e_nomem("for value");
			return;
			}

		Association	*Pair = new Association( *Key, *Value );
		if( Pair == Object::ZERO )
			{
			e_nomem("for association");
			return;
			}

		lines.add( *Pair );

		}

	bad = No;
	}



bool config_text::
find( const String &keyw, String &value ) const
	{
	Association& out = lines.lookup( (Object &) keyw );

	if( out == NOOBJECT )
		return No;

	value = (String &)out.value();
	return Yes;
	}













