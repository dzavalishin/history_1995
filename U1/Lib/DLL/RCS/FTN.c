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
date	95.03.11.18.04.17;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@debug
@
text
@/************************ UUCP to FIDO convertor ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Setup: FTN array implement.
 *
 *      $Log: FTN.c $
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *
 *
 *
\*/

#include "ftn.h"
#include "log.h"
#include <uucpaddr.h>

#include <stdio.h>
#include <ctype.h>


ftn_def			ftn::ftn_v[_max_ftn];
int				ftn::ftn_count;




//
// Def: ftn-short-domain our-ftn-aka inet-domain [z=low-high]
//

bool
ftn::add_def( const char *def )	// Parse definition
	{

#ifdef __BORLANDC__
#pragma warn -ccc
#pragma warn -rch
#endif
	if( _max_ftn < 2 )
		{
		fatal( EC_Dumb_Programmer, EI_None, "_max_ftn < 2!!" );
		}
#ifdef __BORLANDC__
#pragma warn +ccc
#pragma warn +rch
#endif

	if( ftn_count >= _max_ftn )
		{
		error( EI_None, "Too many FTN's defined, skipping '%s'", def );
		return Err;
		}

	char		s_name[9],
				f_addr[51],
				u_dom[101];

	int			e;	// End of scan step

	if( 3 != sscanf( def, "%8s %50s %100s %n", s_name, f_addr, u_dom, &e ) )
		{
		error( EI_None, "Incorrect format: '%s'", def );
		return Err;
		}

	def += e;				// Skip scanned piece

	fido_addr	a;
	const char *p = f_addr; // To make Borland happy

	if( a.aparse( p ) == Err )
		{
		error( EI_None, "Can't parse fido address: '%s'", f_addr );
		return Err;
		}

	if( !a.has_domain() )
		{
		error( EI_None, "No domain in fido address: '%s'", f_addr );
		return Err;
		}

	//
	// It is allright with the first 3 fields, let's try to proceed
	// with scanning.
	//

	int		minz = 0, maxz = 0;		// 0 = unused

	while( isspace( *def ) )
		def++;

	if( tolower( *def ) == 'z' )
		{

		if( 2 != sscanf( def, "z=%d-%d%n", &minz, &maxz, &e ) )
			{
			error( EI_None, "Incorrect FTN z subfield format: '%s'", def );
			return Err;
			}

		def += e;				// Skip scanned piece

		}

	while( isspace( *def ) )
		def++;

	if( *def )
		error( EI_None, "Unidentified junk at the end of FTN definition ignored: %s", def );

	//
	// Everything is fine, fill up one record
	//

	ftn_v[ftn_count].shortname = s_name;
	ftn_v[ftn_count].fullname = a.domain();
	ftn_v[ftn_count].fido_a = a;
	ftn_v[ftn_count].uucp_d = u_dom;

	ftn_v[ftn_count].min_zone = minz;		// unused
	ftn_v[ftn_count].max_zone = maxz;


	if( ftn_count == 0 )
		{
		ftn_v[1] = ftn_v[0];	// Default one
		ftn_count++;
		}

	ftn_count++;
	return Ok;
	}

bool
ftn::valid()					// Can we continue?
	{
	return ftn_count == 0 ? No : Yes;
	}


const ftn_def &
ftn::def()
	{
	if( !valid() )
		fatal( EC_Incorrect_Setup, EI_None, "No FTNs defined" );

	return ftn_v[0];
	}





	// Find AKA matching to given address, or default
const fido_addr
ftn::match_aka( const fido_addr &his_a )
	{
	ftn_def	d;

	if( match( d, his_a ) == Ok )
		return d.fido_a;

	error( EI_None, "Can't find matching AKA for %s", (const char *)(((const String)his_a).c_str()) );
	return ftn_v[0].fido_a;
	}

bool
ftn::match( ftn_def &out, const fido_addr &his_a )
	{
	if( !valid() )
		fatal( EC_Incorrect_Setup, EI_None, "No FTNs defined" );

	if( his_a.has_domain() )
		{
		for( int i = 0; i < ftn_count; i++ )
			{

			// Domain math - either short or long form ?
			if(
			(!stricmp( his_a.domain(), ftn_v[i].shortname.c_str() ))
			||
			(!stricmp( his_a.domain(), ftn_v[i].fullname.c_str() ))
			)
				{
				// Zone value is not limited?
				if( ftn_v[i].min_zone == 0 || ftn_v[i].max_zone == 0 )
					{
					out = ftn_v[i];
					return Ok;
					}

				// Zone value is limited and our zone fits into the limit?
				if( (his_a.zone() >= ftn_v[i].min_zone)  && (his_a.zone() <= ftn_v[i].max_zone) )
					{
					out = ftn_v[i];
					return Ok;
					}
				}
			}

		}
	else
		{
		debug( "No domain, trying to match zone num: %s", (const char *)(((String)his_a).c_str()) );

		for( int i = 0; i < ftn_count; i++ )
			{
			if( ftn_v[i].min_zone == 0 || ftn_v[i].max_zone == 0 )
				continue;

			if( (his_a.zone() >= ftn_v[i].min_zone)  && (his_a.zone() <= ftn_v[i].max_zone) )
				{
				out = ftn_v[i];
				return Ok;
				}
			}
		}

	return Err;
	}




struct trav_data
	{
	const char	*mask;
	int			length;
	bool		matched;
	};

static bool
uu_traverse( uucp_addr::el_type t, const char *el, int len, void *arg )
	{
	trav_data	*e = (trav_data *)arg;

	if( t == uucp_addr::UU_name )		// No, no... don't check it
		return Yes;

	if( e->length == len && strnicmp( el, e->mask, len ) == 0 )
		e->matched = Yes;

	return Yes;
	}

bool
ftn::match( ftn_def &out, const uucp_addr &in )
	{
	if( !valid() )
		fatal( EC_Incorrect_Setup, EI_None, "No FTNs defined" );


	if( !strpbrk( in, "@@%!:" ) )
		{
		//
		// Simple case - pure domain
		//

		for( int i = 0; i < ftn_count; i++ )
			{
			const char *cp1 = (const char *)ftn_v[i].uucp_d.c_str();
			const char *cp2 = (const char *)in;

			if( stricmp( cp1, cp2 ) == 0 )
				{
				out = ftn_v[i];
				return Ok;
				}
			}

		return Err;
		}

	trav_data	e;

	e.matched = No;

	for( int i = 0; i < ftn_count; i++ )
		{
		e.mask = ftn_v[i].uucp_d.c_str();
		e.length = strlen( e.mask );

		in.traverse( uu_traverse, (void *)&e );

		if( e.matched )
			{
			out = ftn_v[i];
			return Ok;
			}
		}

	return Err;
	}


/*
	// Find FTN record by FIDO or UUCP address.
bool
ftn::find( ftn_def &out, const uucp_addr &in )
	{
	return Err;
	}
*/

bool
ftn::find( ftn_def &out, const fido_addr &in )
	{
	for( int i = 0; i < ftn_count; i++ )
		{
		if( ftn_v[i].fido_a == in )
			{
			out = ftn_v[i];
			return Ok;
			}
		}

	return Err;
	}

@


1.1
log
@Initial revision
@
text
@d7 3
a9 1
 *      $Log:   Q:/lib/uu2/vcs/setup.h_v  $
d11 2
d15 2
d41 1
d44 1
d49 1
d52 1
d247 1
a247 1
	if( e->length == len && strncmpi( el, e->mask, len ) == 0 )
@
