head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	95.11.29.12.44.26;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.08.07.05.58.23;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.04.09.21.05.55;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@*** empty log message ***
@
text
@/************************ UU2 Rnews ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Header
 *
 *      $Log: rnews.h $
 *      Revision 1.2  1995/08/07 05:58:23  dz
 *      Using class App
 *
 *      Revision 1.1  1995/04/09  21:05:55  dz
 *      Initial revision
 *
 *      
 *         Rev 1.0   11 Nov 1992 05:22:56   dz
 *      Initial revision.
 *      
 *
\*/

#include	<app.h>
#include	<log.h>

#include	<stdlib.h>
#include	<uu2io.h>
#include	<fcntl.h>
#include	<stdio.h>
#include	<ctype.h>
#include	<errno.h>
#include	<time.h>
#include	<stat.h>

#include	<setup.h>
#include	"..\..\version.h"


class RnewsApp : public U1App
	{
	bool	process( void );


public:
	int	Run();
	};




@


1.2
log
@Using class App
@
text
@d8 5
a12 2
 * Revision 1.1  1995/04/09  21:05:55  dz
 * Initial revision
a23 1
//#include	<fido_msg.h>
d31 1
a31 1
#include    <stat.h>
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/rnews/vcs/rnews.h_v  $
d18 4
a21 1
#include	<fido_msg.h>
a31 1
#include	<log.h>
d35 3
a38 3
/****************************************************************************
						Global variables
****************************************************************************/
d40 3
a44 3
/****************************************************************************
							Functions
****************************************************************************/
a46 1
bool	process( void );
@
