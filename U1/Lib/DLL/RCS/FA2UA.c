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
@/************************ UUCP to FIDO convertor ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	FIDO to UUCP address conversion
 *
 *      $Log: FA2UA.c $
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   25 Jan 1993 21:52:24   dz
 *      Initial revision.
 *
 *
\*/


#include	"style.h"
#include	"log.h"
#include	"fidoaddr.h"
#include	<stdlib.h>
#include	<stdio.h>
#include	<ctype.h>
#include	<uucpaddr.h>



/*--------------------------------------------------------------------------*/


bool fido_user::
convert_to_uucp( uucp_addr &ua, const char *u_domain ) const
	{
	char buf[66+38+4+80];

	sprintf( buf, "%.36s@@p%u.f%u.n%u.z%u.%s",
		name_v, point(), node(), net(), zone(),
		u_domain
		);

	for( unsigned char *cp = (uchar *) buf; *cp; cp++ )
		if( isspace( *cp ) )
			*cp = '_';

	ua = (const char *)buf;
	return Ok;
	}


bool fido_addr::
convert_to_uucp( uucp_addr &ua, const char *u_domain ) const
	{
	char buf[66+38+4+80];

	sprintf( buf, "p%u.f%u.n%u.z%u.%s",
		point(), node(), net(), zone(),
		u_domain
		);

	ua = (const char *)buf;
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
 *      $Log:   Q:/lib/fido/vcs/fa2ua.c_v  $
d45 1
a45 1
	for( unsigned char *cp = buf; *cp; cp++ )
@
