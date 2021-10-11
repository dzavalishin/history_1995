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
@/********************** Date/time conversion library *************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	dateconv - convert a split-out date back into a time_t
 *
 *      $Log: DateConv.c $
 * Revision 1.1  1995/03/11  18:07:42  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:07:42  dz
 * Initial revision
 *
 *      
 *         Rev 1.1   11 Oct 1992 04:30:26   dz
 *      Validation corrected (day/mon)
 *      
 *         Rev 1.0   20 Oct 1991 00:36:36   dz
 *      Initial revision.
 *
 *
\*/


#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>



/* turn a (struct tm) and a few variables into a time_t, with range checking */

time_t
dateconv( struct tm * tm, int zone) {

	tm->tm_wday = tm->tm_yday = 0;

	/* validate, before going out of range on some members */
	if (tm->tm_year < 0 || tm->tm_mon < 0 || tm->tm_mon > 12 ||
		tm->tm_mday < 0 || tm->tm_hour < 0 || tm->tm_hour >= 24 ||
		tm->tm_min < 0 || tm->tm_min > 59 ||
		tm->tm_sec < 0 || tm->tm_sec > 59)
	  return -1;

	/*
	 * zone should really be -zone, and tz should be set to tp->value, not
	 * -tp->value.  Or the table could be fixed.
	 */
	tm->tm_min += zone;       /* mktime lets it be out of range */

	/* convert to seconds */
	return mktime(tm);
}

@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/date/vcs/dateconv.c_v  $
@
