head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	95.08.07.14.55.01;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.09.18.25.15;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.32.26;	author dz;	state Exp;
branches;
next	;


desc
@UU2 News Unbatcher
@


1.3
log
@Using class App
@
text
@/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Read batch header
 *
 *      $Log: gethdr.c $
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


@


1.2
log
@IBM C Set version. Phew, it was not a piece of cake to get here...
@
text
@d8 6
d28 1
a28 1
getheader( char *buf, int len, VReadBase &in )
@


1.1
log
@Initial revision
@
text
@d7 4
a10 10
 *      $Log:   Q:/news/unbatch/vcs/gethdr.c_v  $
 *      
 *         Rev 1.1   05 Mar 1993 03:10:00   dz
 *      Direct unbatching
 *      
 *         Rev 1.0.1.0   03 Mar 1993 20:29:44   dz
 *      Temp: trying to implement direct unbatching scheme.
 *      
 *         Rev 1.0   21 Feb 1992 11:21:46   dz
 *      Initial revision.
@
