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
@#include	<time.h>



/* interface to dateconv() */

time_t _Export	dateconv( struct tm *tm, int zone );



/*
 *  Meridian:  am, pm, or 24-hour style.
 */
#define AM 0
#define PM 1
#define HR24 2

@


1.1
log
@Initial revision
@
text
@d7 1
a7 1
time_t	dateconv( struct tm *tm, int zone );
@
