head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	95.11.05.18.50.12;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.11.05.13.52.48;	author dz;	state Exp;
branches;
next	1.2;

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


1.4
log
@RFC_Msg used
@
text
@/************************ UUCP to FIDO convertor ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	FIDO address operations module
 *
 *      $Log: FidoAddr.c $
 *      Revision 1.3  1995/11/05 13:52:48  dz
 *      current.
 *
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 *      
 *         Rev 1.7   07 Jun 1993 17:35:50   dz
 *      A lot of changes - see diff
 *      
 *         Rev 1.6   06 Nov 1992 19:19:46   dz
 *      Ver 1.12a save
 *      
 *         Rev 1.5   23 Oct 1992 15:48:18   dz
 *      error()
 *      
 *         Rev 1.4   16 Oct 1992 03:41:34   dz
 *      Domain support
 *      
 *         Rev 1.3   18 Sep 1992 00:53:04   dz
 *      Lots of additions/changes, see diff
 *      
 *         Rev 1.2   22 Mar 1992 19:21:12   dz
 *      Parser method added
 *      
 *         Rev 1.1   11 Oct 1991 05:02:50   dz
 *      include <string.h>
 *      
 *         Rev 1.0   12 Sep 1991 01:30:40   dz
 *      Initial revision.
 *
 *
\*/


#include	<string.h>
#include	"style.h"
#include	"log.h"
#include	"ftn_msg.h"
#include	<stdlib.h>
#include	<ctype.h>

/*--------------------------------------------------------------------------*/

ushort   fido_addr::def_zone_v = 0;
String   fido_addr::def_domain_v = "fidonet.org";

/*--------------------------------------------------------------------------*/


fido_addr::fido_addr( const fido_addr &r )
	{
	strncpy( domain_v, r.domain_v, sizeof( domain_v ) );
	domain_v[ sizeof(domain_v)-1 ] = '\0';

	zone_v	= r.zone_v;
	net_v	= r.net_v;
	node_v	= r.node_v;
	point_v	= r.point_v;

        fix_z();
        }

fido_user::fido_user( const fido_user &r )
	: fido_addr( (const fido_addr&) r )
	{
	strncpy( name_v, r.name_v, sizeof( name_v ) );
	name_v[ sizeof(name_v)-1 ] = '\0';
	}


fido_addr::fido_addr( void )
	{
	zone_v	=	net_v	=
	node_v	=	point_v	= 0;
	domain_v[0]			= '\0';
        fix_z();
        }

fido_user::fido_user( void )
	: fido_addr()
	{
	name_v[0]			= '\0';
	}


fido_user::fido_user( const fido_addr &a, const char *name )
        : fido_addr( a )
	{
        strncpy( name_v, name, sizeof( name_v ) );
	name_v[ sizeof(name_v)-1 ] = '\0';
	}



/*--------------------------------------------------------------------------*/


bool
fido_addr::operator == ( const fido_addr &r ) const
	{

// we can't fix zone since we are const. it should be fixed before
//        fix_z();
//        r.fix_z();

        if(
			r.zone_v	!= zone_v ||
			r.net_v		!= net_v  ||
			r.node_v	!= node_v ||
			r.point_v	!= point_v
	) return No;

	if( (!has_domain()) || (!r.has_domain()) )
		return Yes;

	if( strcmpi( domain_v, r.domain_v ) == 0 )
		return Yes;

	if( !strchr( domain_v, '.' ) )		// dotless domain
		{
		// try to strip dot and following from other domain
		int l = strlen( domain_v );
		if( (r.domain_v)[l] == '.' && 0 == strnicmp( domain_v, r.domain_v, l ) )
			return Yes;
		}

	if( !strchr( r.domain_v, '.' ) )		// dotless domain
		{
		// try to strip dot and following from other domain
		int l = strlen( r.domain_v );
		if( (domain_v)[l] == '.' && 0 == strnicmp( domain_v, r.domain_v, l ) )
			return Yes;
		}

	return Yes;
	}

bool
fido_user::operator == ( const fido_user &r ) const
	{
	if( ((fido_addr&)*(this)) != ((fido_addr&)r) )
		return No;

	const char	*a, *b;

	a = name_v; b = r.name_v;

	while( isspace( *a ) )
		a++;

	while( isspace( *b ) )
		b++;


	while( *a && *b )
		{

		if(
			(isspace( *a )	&& isspace( *b )) ||
			( *a == '\0'	&& isspace( *b )) ||
			(isspace( *a )  && *b == '\0'  )
			) {

			while( isspace( *a ) )
				a++;

			while( isspace( *b ) )
				b++;
			}


		if( tolower( *a ) != tolower( *b ) )
			return No;

		if( ( *a == '\0' ) && ( *b == '\0' ) )
			return No;

		a++, b++;
		}

	return Yes;
	}


/*--------------------------------------------------------------------------*/


fido_addr &
fido_addr::operator = ( const fido_addr &r )
	{
	strncpy( domain_v, r.domain_v, sizeof( domain_v ) );
	domain_v[ sizeof(domain_v)-1 ] = '\0';

	zone_v	= r.zone_v;
	net_v	= r.net_v;
	node_v	= r.node_v;
	point_v	= r.point_v;

        fix_z();
        return *this;
	}

fido_user &
fido_user::operator = ( const fido_user &r )
	{

	((fido_addr&)*this).operator = ( (fido_addr&)r );

	strncpy( name_v, r.name_v, sizeof( name_v ) );
	name_v[ sizeof(name_v)-1 ] = '\0';

	return *this;
	}

/*--------------------------------------------------------------------------*
 *
 *	General Zone/Net/Node/Point/Domain parser
 *
**/

bool
fido_addr::parse_znnpd( const char *&ap )
	{
	int		nread;

	while( *ap <= ' ' && *ap )
		ap++;

	if(	sscanf( ap, "%d:%d/%d%n", &zone_v, &net_v, &node_v, &nread ) != 3 )
		{
		error( EI_None, "Bad address, can't read z:n/n: %s\n", ap );
		return Err;
		}

	ap += nread;		// Skip what we got

	if( *ap == '.' )	// Point number follows
		{
		if(	sscanf( ap, ".%d%n", &point_v, &nread ) != 1 )
			{
			error( EI_None, "Bad address point part: %s\n", ap );
			return Err;
			}

		ap += nread;		// Skip what we got
		}
	else
		point_v = 0;

	if( *ap == '@@' )	// Domain name follows
		{
		if(	sscanf( ap, "@@%36s%n", domain_v, &nread ) != 1 )
			{
			error( EI_None, "Bad address domain part: %s\n", ap );
			return Err;
			}

		ap += nread;		// Skip what we got
		}
	else
		domain_v[0] = '\0';

        fix_z();
        return Ok;
	}

/*--------------------------------------------------------------------------*/



bool
fido_user::parse( const char *ap )
	{

	if( parse_znnpd( ap ) == Err )
		return Err;

	if( *ap != ' ' && *ap != '\t' )
			{
			error( EI_None, "Bad address - no whitespace before username: %s\n", ap );
			return Err;
			}

	if(	sscanf( ap, " %36s", name_v ) != 1 )
		{
		error( EI_None, "Bad address username part: %s\n", ap );
		return Err;
		}

	for( char *p = name_v; *p; p++ )
		if( *p == '_' || *p == '.' )
			*p = ' ';

	return Ok;
	}


bool
fido_user::fparse( const char *ap )
	{
	const char	*p;

	if( parse_znnpd( ap ) == Err )
		return Err;

	p = ap;

//	while( *p && *p != ' ' && *p != '\t' )
//		p++;

	while( *p == ' ' || *p == '\t' )
		p++;

	strncpy( name_v, p, 35 );
	name_v[35] = '\0';

	for( char *pp = name_v; *pp; pp++ )
		if( *pp == '_' || *pp == '.' )
			*pp = ' ';


	return Ok;
	}


bool
fido_user::cparse( const char *ap )
	{
	const char	*p, *ea;
	bool		addr_before = No;

	p = strchr( ap, ':' );
	if( p == ap || p == NULL )
		{
		error( EI_None, "Bad address: %s", ap );
		return Err;
		}

	while( p > ap && isdigit( *(p-1) ) )
		p--;

	while( p > ap && isspace( *(p-1) ) )
		p--;

	if( p == ap )
		addr_before = Yes;			// z:n/f.p is before name
	else
		ea = p;

	if( parse_znnpd( p ) == Err )
		return Err;

	if( addr_before )
		{
		p = ap;
		while( *p && *p != ' ' && *p != '\t' )			p++;
		while( *p == ' ' || *p == '\t' )				p++;
		if( *p == ',' )							*p++;
		while( *p == ' ' || *p == '\t' )				p++;

		strncpy( name_v, p, 35 );
		name_v[35] = '\0';
		}
	else
		{
		p = ap;
		while( *p == ' ' || *p == '\t' )
			p++;

		int		l = (int)(ea - p);

		l = l < 34 ? l : 34;

		strncpy( name_v, p, l );
		name_v[l] = '\0';
		}

	for( char *pp = name_v; *pp; pp++ )
		if( *pp == '_' || *pp == '.' )
			*pp = ' ';


	return Ok;
	}



/*--------------------------------------------------------------------------*/
/*
fido_addr::operator const char * () const
	{
	char buf[66+38];

//        fix_z();

        sprintf( buf, "%s, %d:%d/%d.%d%s%s",
		name_v, zone, net, node, point,
		strlen( domain_v ) ? "@@" : "",
		domain_v
		);

	return strdup( buf );
	}
*/

/*--------------------------------------------------------------------------*/

void fido_user::
normalize_name( void )						// Replace "_." with spaces
	{
	char *n = name_v;

	while( *n && (n - name_v < sizeof(name_v)) )
		{
		if( *n == '_' || *n == '.' )
			*n = ' ';
		n++;
		}
	}




@


1.3
log
@current.
@
text
@d8 3
d52 1
a52 1
#include	"fido_msg.h"
@


1.2
log
@debug
@
text
@d8 3
d127 12
a138 1
	if( strcmpi( domain_v, r.domain_v ) != 0 )
d140 4
a143 2
//		error( EI_None, "Domains doesn't match: %s vs. %s", domain_v, r.domain_v );
		return No;
@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/fido/vcs/fidoaddr.c_v  $
d110 3
a112 2
        fix_z();
        r.fix_z();
d389 1
a389 1
        fix_z();
@
