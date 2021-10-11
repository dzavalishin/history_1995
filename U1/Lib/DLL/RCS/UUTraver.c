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
date	95.03.11.18.07.42;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@debug
@
text
@/************************ UUCP interface library ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	UUCP address traverse function
 *
 *      $Log: UUTraver.c $
 * Revision 1.1  1995/03/11  18:07:42  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:07:42  dz
 * Initial revision
 *
 *      
 *         Rev 1.1   07 Jun 1993 18:03:24   dz
 *      const
 *      
 *         Rev 1.0   09 Nov 1992 22:58:48   dz
 *      Initial revision.
 *
 *
\*/

#include	"uucpaddr.h"
#include	<log.h>

#include	<stdlib.h>
#include	<ctype.h>



bool uucp_addr::
traverse( tr_func func, void *arg ) const
	{
	const char	*vp = (const char *)addr_v;

	while( isspace( *vp ) )		vp++;


// check for @@...: source routing
//
//	@@ dd1 , dd2 : hh6 ! dd7 ! uu8 @@ dd5 % dd4 @@ dd3
//  ~~~~~~~~~~~~~

	if( *vp == '@@' )
		{
		const char	*part;
		// v_domain

		while( 1 ) {
			part = ++vp;

			while( isalnum( *vp ) || *vp == '-' || *vp == '.' )
				vp++;

			if( *vp != ',' && *vp != ':' )
				{
				error( EI_None,
				"Traverse - incorrect uucp source routing (%s)", addr_v );
				return Err;
				}

			if( func( UU_via, part, (int)(vp-part), arg ) != Yes )
				return No;

			if( *vp == ',' )
				continue;

			++vp;
			break;
			}
		}

// Ok, done with @@...: source routing
//
// check for @@% at the end
//
//	hh6 ! dd7 ! uu8 @@ dd5 % dd4 @@ dd3
//                  ~~~~~~~~~~~~~~~~~

	const char	*dom = vp + strlen(vp);
	const char	*end;

	while( dom > vp )
		{
		end = --dom;

		while( dom > vp && *dom != '@@' && *dom != '%' )
			dom--;

		if( *dom != '@@' && *dom != '%' )
			break;

		if( func( UU_domain, dom+1, (int)(end-dom), arg ) != Yes )
			return No;
		}

// Ok, done with @@% at the end
//
// check for bangs
//
//	hh6 ! dd7 ! uu8
//  ~~~~~~~~~~~

	const char	*bang;

	while( 1 )
		{
		bang = vp;

		if( bang > end )
			break;

		const char *bend = strchr( bang+1, '!' );
		if( bend == NULL || bend > end )
			break;

		if( func( UU_bang, bang, (int)(bend-bang), arg )  != Yes )
			return No;

		vp = bend+1;
		}

// Ok, done with bangs
//
// check for username
//
//	uu8
//  ~~~

	if( vp > end )
		{
		error( EI_None, "Traverse syntax error - no user name (%s)", addr_v );
		return Err;
		}

	if( func( UU_name, vp, (int)(end-vp+1), arg ) != Yes )
		return No;

	return Yes;
	}














@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/uucp/vcs/uutraver.c_v  $
d35 1
a35 1
	const char	*vp = addr_v;
@
