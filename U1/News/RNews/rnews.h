/************************ UU2 Rnews ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Header
 *
 *      $Log: rnews.h $
 *      Revision 1.3  1995/11/29 12:44:26  dz
 *      *** empty log message ***
 *
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




