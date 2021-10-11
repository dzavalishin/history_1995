/************************ UUCP to FIDO convertor ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	FIDO address to String conversion
 *
 *      $Log: FA2Strng.c $
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
 *         Rev 1.0   25 Jan 1993 21:52:38   dz
 *      Initial revision.
 *
 *
\*/


#include	<string.h>
#include	"style.h"
#include	"log.h"
#include	"fidoaddr.h"
#include	<stdlib.h>
#include	<stdio.h>
//#include	<ctype.h>

#include	<strng.h>


/*--------------------------------------------------------------------------*/


fido_addr::operator String () const
	{
	char buf[66];

//        fix_z();

	sprintf( buf, "%u:%u/%u.%u%s%.36s",
		zone_v, net_v, node_v, point_v,
		strlen( domain_v ) ? "@" : "",
		domain_v
		);

	return String( buf );
	}


fido_user::operator String () const
	{
	char buf[66+38];

//        fix_z();

	sprintf( buf, "%.36s, %u:%u/%u.%u%s%.36s",
		name_v, zone(), net(), node(), point(),
		strlen( domain() ) ? "@" : "",
		domain()
		);

	return String( buf );
	}
