head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	95.11.06.02.33.43;	author dz;	state Exp;
branches;
next	1.2;

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


1.3
log
@Fixes & cleanups caused by FTN_Msg introduction
@
text
@/************************ UU2 Conf. file parser ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Free domains list parser/checker
 *
 *      $Log: FreeDom.c $
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
 *         Rev 1.3   07 Jun 1993 17:47:42   dz
 *      Debugging stripped
 *      
 *         Rev 1.2   23 Oct 1992 15:30:26   dz
 *      error()
 *      
 *         Rev 1.1   09 Feb 1992 00:19:56   dz
 *      Bug fix: returned Ok. Changed to Yes.
 *      
 *         Rev 1.0   04 Feb 1992 22:11:14   dz
 *      Initial revision.
 *
 *
\*/


#include	<string.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<ctype.h>

#include	"setup.h"
#include	"log.h"

#include	"FreeDom.h"


#define	skipws( p )		{	while( *p == ' ' || *p == '\t' )	p++;	}

static uint dlen( uchar * );

void
get_free_domains( uchar *param )
	{
	struct freedom	*last = NULL;
	int				l;

	skipws( param );
	while( *param ) {
		freedom		*curr	= new freedom;
		if( curr == NULL )
			{
			error( EI_None, "Out of memory loading free domains list" );
			return;
			}

		if( last == NULL )
			conf.free_domains = curr;
		else
			last->next = curr;

		last = curr;
		curr->next = NULL;

		l = dlen( param );
		curr->domain = new char[l+1];
		if( curr->domain == NULL )
			{
			error( EI_None, "Out of memory loading free domains list" );
			return;
			}

		strncpy( curr->domain, (const char *)param, l );
		(curr->domain)[l] = '\0';
		param += l;

		skipws( param );
		}
	}



static uint
dlen( uchar *d )
	{
	uint		l = 0;

	while( *d != ' ' && *d != '\t' && *d )
		{
		l++;
		d++;
		}

	return l;
	}




/****************************************************************************
			Check given address aginst list of free domains
****************************************************************************/

//#define	MAX_ADR_LEN		100

static bool		is_free_domain( uchar *domain );

bool _Export
is_free_address( const string adr_a )
	{
	char	*adr, *p;

	adr = (char *) (const char *)adr_a;
	debug("is_free_address( %s )", adr );


	// Check for restricted mode

	if( conf.free_domains == NULL )			// All the domins are free
		{
//        debug("Everything is free");
		return Yes;
		}



	// Check for bang path

	if( (p = strchr( adr, '!' )) != NULL )	// Pity - I can't check this
		{
		// Check each component of bang path

		*p = '\0';							// Cut in two parts

//        debug("Got bang, checking domain '%s' and address '%s'", adr, p+1 );

		if( is_free_domain( (uchar *)adr ) && is_free_address( p+1 ) )
			return Yes;

		return No;
		}




	// Ok, let's check for '@@domain:' style routing

	if( (p = strchr( adr, ':' )) != NULL )
		{
		if( *adr != '@@' && *adr != '%' )
			{
			error( EI_None, "No leading '@@' in '@@domain:'-style routing: %s", adr );
			return No;
			}

		*p = '\0';                          // Cut it

//        debug("Got '@@...:', checking domain '%s' and address '%s'", adr+1, p+1 );

		if( is_free_domain( (uchar *)(adr+1) ) && is_free_address( p+1 ) )
			return Yes;

		return No;
		}



	if( (p = strrchr( adr, '%' )) != NULL )
		{
//        debug( "% to @@ in '%s'", p );
		*p = '@@';
		}



	// Ok, user@@domain and user%domain@@domain

	if( (p = strrchr( adr, '@@' )) != NULL )
		{

		*p = '\0';

//        debug("Got '@@', checking domain '%s' and address '%s'", p+1, adr );

		if( !is_free_domain( (uchar *)(p+1) ) )
			return No;

		return is_free_address( adr );
		}

//    debug( "Username: '%s'", adr );
	return Yes;
	}




/****************************************************************************
			Check given domain aginst list of free domains
****************************************************************************/


static bool		match_domain( char *d_inner, char *d_outer );

static bool
is_free_domain( uchar *d )
	{
//    debug( "is_free_domain( %s )", d );

	freedom		*f = conf.free_domains;

	while( f ) {
		if( match_domain( f->domain, (char *)d ) )
			{
//            debug("Match: %s is in %s", d, f->domain );
			return Yes;
			}

		f = f->next;
		}

	return No;
	}



static bool
match_domain( char *d_inner, char *d_outer )
	{

	// Quick and ditry method :(

	int		li, lo;

	li = strlen( d_inner );
	lo = strlen( d_outer );

	if( li > lo )
		return No;

	d_outer += ( lo - li );

	return stricmp( d_inner, d_outer ) == 0 ? Yes : No;
	}

@


1.2
log
@debug
@
text
@d8 3
d111 1
a111 1
#define	MAX_ADR_LEN		100
d116 1
a116 1
is_free_address( char *adr_a )
d118 1
a118 7
	char	adr[MAX_ADR_LEN], *p;

	if( strlen( adr_a ) > (MAX_ADR_LEN-1) )
		{
		error( EI_None, "is_free_address( %s ): too long", adr_a );
		return No;
		}
d120 1
a120 1
	strcpy( adr, adr_a );
@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/uu2/vcs/freedom.c_v  $
d39 3
d77 1
a77 1
		strncpy( curr->domain, param, l );
d112 1
a112 1
bool
d147 1
a147 1
		if( is_free_domain( adr ) && is_free_address( p+1 ) )
d170 1
a170 1
		if( is_free_domain( adr+1 ) && is_free_address( p+1 ) )
d195 1
a195 1
		if( !is_free_domain( p+1 ) )
d223 1
a223 1
		if( match_domain( f->domain, d ) )
@
