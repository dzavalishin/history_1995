/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Read batch header
 *
 *      $Log: gethdr.c $
 *      Revision 1.3  1995/08/07 14:55:01  dz
 *      Using class App
 *
 * Revision 1.2  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.2  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.1  1995/03/11  18:32:26  dz
 * Initial revision
 *
 *
 *
\*/

#include    "unbatch.h"
#include    "..\..\version.h"
#include	"vread.h"



bool
UnbatchApp::getheader( char *buf, int len, VReadBase &in )
	{
	long	pp = 0;
	int 	i;
	uchar	c;

	while( --len > 0 )
		{

		i = in.read( &c, 1 );
		if( i < 0 )
			return Err;

		if( c == '\r' || c == '\n' || i == 0 )
			break;

		*buf++ = c;
		}

	*buf = '\0';

	pp = in.tellg();
	i = in.read( &c, 1 );
	if( (i == 1) && (c == '\r' || c == '\n') )
		return Ok;

	in.seekg( pp );
	return Ok;
	}


