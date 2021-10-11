/************************ UUCP to FIDO convertor ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	FIDO to UUCP address conversion
 *
 *      $Log: FA2UA.c $
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

	sprintf( buf, "%.36s@p%u.f%u.n%u.z%u.%s",
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

