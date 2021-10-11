head	1.2;
access;
symbols;
locks;
comment	@ * @;


1.2
date	95.04.13.11.06.28;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.00.54;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@debug
@
text
@/************************ UU2 FILE2UU ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Load one item from .TIC file
 *
 *      $Log: Tic_Fill.c $
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 *      
 *         Rev 1.1   29 Dec 1992 01:32:14   dz
 *      
 *         Rev 1.0   05 Oct 1992 19:17:32   dz
 *      Initial revision.
 *      
 *
\*/

#include    "tic.h"
#include    "tic_kw.h"
#include	<log.h>


bool tic_file::
fill_in( tic_kw::tic_token t, const char *val )
	{
	while( *val == ' ' || *val == '\t' )
		val++;

	switch( t )
		{
		case tic_kw::area:
			strncpy( area_v, val, TIC_MAXAREA );
			area_v[TIC_MAXAREA] = '\0';
			break;

		case tic_kw::file:
			strncpy( name_v, val, TIC_MAXFN );
			name_v[TIC_MAXFN] = '\0';
			break;

		case tic_kw::pw:
			strncpy( passwd_v, val, TIC_MAXPW );
			passwd_v[TIC_MAXPW] = '\0';
			break;

		case tic_kw::desc:
			{
//			String	*s = new String( (signed char *) val );
			String	*s = new String( val );
			if( s == Object::ZERO )
				{
				error( EI_None, "Out of memory" );
				return Err;
				}

// BUG! Check result of operation
			desc_v.addAtTail( (Object &)*s );
			break;
			}

		case tic_kw::size:
			size_v = atol( val );
			break;

		case tic_kw::crc:
			if( sscanf( val, "%lx", &crc_v ) != 1 )
				{
				error( EI_None, "Can't parse CRC value: %s", val );
				return Err;
				}
			break;

		case tic_kw::path:
			{
			String	*s = new String( val );
			if( s == Object::ZERO )
				{
				error( EI_None, "Out of memory" );
				return Err;
				}

// BUG! Check result of operation
			path_v.addAtTail( (Object &)*s );
			break;
			}

		case tic_kw::seenby:
			{
			String	*s = new String( val );
			if( s == Object::ZERO )
				{
				error( EI_None, "Out of memory" );
				return Err;
				}

// BUG! Check result of operation
			seenby_v.addAtTail( (Object &)*s );
			break;
			}

		case tic_kw::replaces:
			{
			String	*s = new String( val );
			if( s == Object::ZERO )
				{
				error( EI_None, "Out of memory" );
				return Err;
				}

// BUG! Check result of operation
			replaces_v.addAtTail( (Object &)*s );
			break;
			}

// BUG! aparse doesn't understand 'domain>z:n/n.p' syntax
		case tic_kw::origin:
			if( origin_v.aparse( val ) == Err )
				{
				error( EI_None, "Can't parse origin address" );
				return Err;
				}
			break;

		case tic_kw::from:
			if( from_v.aparse( val ) == Err )
				{
				error( EI_None, "Can't parse 'from' address" );
				return Err;
				}
			break;

		case tic_kw::app:
		case tic_kw::created:		// ...by which program (who cares?:)
		case tic_kw::date:
		case tic_kw::release:
		default:
			break;
		}
	return Ok;
	}

















@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/file/file2uu/vcs/tic_fill.c_v  $
@
