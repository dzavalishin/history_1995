/************************ UU2 library ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:   RFC-822/1036 support
 *
 *      $Log: RFC.c $
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
 *         Rev 1.1   07 Jun 1993 17:51:24   dz
 *      String -> const char
 *      
 *         Rev 1.0   06 Nov 1992 01:47:42   dz
 *      Initial revision.
 *
 *
\*/

#include	<style.h>
#include	<setup.h>
#include	<datec.h>

#include	"rfc.h"

#include	<time.h>
#include	<stdio.h>


/****************************************************************************
						Make leading From_
****************************************************************************/

void
mk_from_( char *buf, const char *name )
	{
	time_t	t;

	time( &t );

	switch( conf.uucp )
		{
		case UUCP_Ache30:
            *buf = '\0';
			break;

		case UUCP_Waffle:
			sprintf( buf, "From %s%%%.80s %s\n",
                name,
				(const char *)conf.def_domain.c_str(),
				timeline( t, (const uchar *)conf.tz.c_str() )
				);
			break;

		default:
			sprintf( buf, "From %s%%%.50s %s remote from %.50s\n",
                name,
				(const char *)conf.def_domain.c_str(),
				timeline( t, (const uchar *)conf.tz.c_str() ),
				(const char *)conf.def_domain.c_str()
				);
			break;
		}
	}
