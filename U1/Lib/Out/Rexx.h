/************************ U1 Rexx interface ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	Rexx support
 *
 *      $Log: Rexx.h $
 *      Revision 1.1  1995/11/05 13:52:48  dz
 *      Initial revision
 *
 *
 *
\*/

#include	"style.h"
#include	"strng.h"

#if 0
#include	<stdlib.h>
#include	<stdio.h>
#include	<ctype.h>
#include	<errno.h>
#include	<time.h>
#include    <fcntl.h>

#include	"uuparse.h"

#include	"..\..\u1.h"
#include	"..\..\version.h"

#include	<setup.h>
#include	<log.h>
#include	<uucpaddr.h>
#include	<userlist.h>
#include	<datec.h>
#include	<recode.h>
#include    "stat.h"
#include	<hl_match.h>

#endif

/****************************************************************************
						Global variables
****************************************************************************/


/****************************************************************************
							Functions
****************************************************************************/




bool 		CallRexx( const char *prog, string &out, const char *a1, const char *a2 );
