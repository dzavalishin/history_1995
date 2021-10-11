head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	95.11.05.13.52.48;	author dz;	state Exp;
branches;
next	1.2;

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


1.3
log
@current.
@
text
@/********************** Date/time conversion library *************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	time_t to Internet-style lime line conversion.
 *
 *      $Log: RFC_Date.c $
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:07:42  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:07:42  dz
 * Initial revision
 *
 *      
 *         Rev 1.1   26 Nov 1992 19:32:24   dz
 *      const
 *      
 *         Rev 1.0   11 Oct 1992 04:30:50   dz
 *      Initial revision.
 *
 *
\*/

#include	"datec.h"
#include	<stdio.h>

	/*************************************************************
				   Make time/date string withno '\n'
	*************************************************************/

static char *s_week[] =
		{
		"Sun", "Mon", "Tue", "Wed",
		"Thu", "Fri", "Sat"
		};

static char *s_month[] =
		{
		"Jan", "Feb", "Mar", "Apr",
		"May", "Jun", "Jul", "Aug",
		"Sep", "Oct", "Nov", "Dec"
		};

const char *
timeline( time_t t, const uchar *tz )
	{
	static char		tl[50];
	struct tm 		*tb;

	tb = localtime(&t);

//*   [Weekday,] dd Mmm [19]yy hh:mm[:ss] Timezone

	sprintf( tl, "%s, %2d %.3s %d %02d:%02d:%02d %s",
		s_week[ (tb->tm_wday) % 7 ],
		tb->tm_mday,
		s_month[ (tb->tm_mon) % 12 ],
		tb->tm_year+1900,
		tb->tm_hour,
		tb->tm_min,
		tb->tm_sec,
		tz
		);

	return tl;
	}
@


1.2
log
@debug
@
text
@d8 3
d58 1
a58 1
		s_week[ tb->tm_wday ],
d60 1
a60 1
		s_month[ tb->tm_mon ],
@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/date/vcs/rfc_date.c_v  $
@
