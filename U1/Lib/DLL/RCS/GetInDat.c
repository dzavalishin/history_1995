head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	96.03.29.22.13.24;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.03.15.08.42.37;	author dz;	state Exp;
branches;
next	1.3;

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


1.5
log
@remove comment
@
text
@/********************** Date/time conversion library *************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	: getindate - parse the common Internet date case
 *                (rfc 822 & 1123) *fast*
 *
 *      $Log: GetInDat.c $
 *      Revision 1.4  1996/03/15 08:42:37  dz
 *      Error message
 *
 *      Revision 1.3  1995/11/05 13:52:48  dz
 *      current.
 *
 *      Revision 1.2  1995/04/13  11:06:28  dz
 *      debug
 *
 *      Revision 1.1  1995/03/11  18:07:42  dz
 *      Initial revision
 *
 *      Revision 1.1  1995/03/11  18:07:42  dz
 *      Initial revision
 *
 *      
 *         Rev 1.2   03 Mar 1992 19:34:10   dz
 *      Changes 19XX to XX - a lot of nodes in Russia create such dates :(
 *      
 *         Rev 1.1   03 Mar 1992 19:32:52   dz
 *      Ignore junk at the end of line.
 *      Cosmetical changes.
 *      
 *         Rev 1.0   20 Oct 1991 00:38:54   dz
 *      Initial revision.
 *
 *
\*/

#include	<style.h>
#include	<log.h>
#include	<stdlib.h>
#include	"datec.h"
#include	<stdio.h>
#include	<ctype.h>
#include	<string.h>
#include	<time.h>
#include	<sys/types.h>
#include	<sys/timeb.h>

#include	"dateconv.h"
#include	"datetok.h"



static int    parsetime( const char *time, struct tm *tm );
static int    prsindate( char * line, struct tm *tm, int *tzp );
static void   remove_comment( char * line );
static int    try_numeric_tz( int *tzp, char *tztext );




/* STREQ is an optimised strcmp(a,b)==0 */
#define STREQ(a, b) ((a)[0] == (b)[0] && strcmp(a, b) == 0)

#define PACK_TWO_CHARS(c1, c2)   (((c1)<<8)|(c2))
#define ISSPACE(c) ((c) == ' ' || (c) == '\n' || (c) == '\t')
#define SKIPTOSPC(s) \
    while ((ch = *(s)++), (!ISSPACE(ch) && ch != '\0')) \
      ; \
    (s)--           /* N.B.: no semi-colon */
#define SKIPSPC(s) \
    while ((ch = *(s)++), ISSPACE(ch)) \
      ; \
    (s)--           /* N.B.: no semi-colon */
#define SKIPOVER(s) \
    SKIPTOSPC(s); \
    SKIPSPC(s)      /* N.B.: no semi-colon */

/* this is fast but dirty.  note the return's in the middle. */
#define GOBBLE_NUM(cp, c, x, ip) \
    (c) = *(cp)++; \
    if ((c) < '0' || (c) > '9') \
      return -1;      /* missing digit */ \
    (x) = (c) - '0'; \
    (c) = *(cp)++; \
    if ((c) >= '0' && (c) <= '9') { \
      (x) = 10*(x) + (c) - '0'; \
      (c) = *(cp)++; \
    } \
    if ((c) != ':' && (c) != '\0' && !ISSPACE(c)) \
      return -1;      /* missing colon */ \
    *(ip) = (x)        /* N.B.: no semi-colon here */

/*
 * If the date is in the form
 *   [Weekday,] dd Mmm [19]yy hh:mm[:ss] Timezone
 * as most dates in news articles are, then we can parse it much quicker than
 * getdate and quite a bit faster than getabsdate.
 *
 * parse and convert Internet date in timestr (the normal interface)
 *
 * parameter 'line' can be modified.
 *
 *
 */

time_t
getindate( const char * line ) 
    {
    int tz = 0;
    struct tm date;
    
    char *l = strdup( line );
    time_t t;
    
    t = prsindate(l, &date, &tz) < 0? -1: dateconv(&date, tz);
    if( t == -1 ) 
        remove_comment(l);
    t = prsindate(l, &date, &tz) < 0? -1: dateconv(&date, tz);
    
    free( l );

    if( t == -1 ) 
        error( EI_None, "getindate: can't parse '%s'", line );
    
    return t;
    }


static void remove_comment( char *line )
    {
    char *orig = line;
    while( isspace(*line) ) line++;
    if( *line != '(' ) return;
    line++;
    while( *line && *line != ')' ) line++;
    if( *line == ')' ) line++;
    while( isspace(*line) ) line++;
    debug( "orig (%s), line (%s)", orig, line );
    strcpy( orig, line );
    }


/*
 * just parse the Internet date in timestr and get back a broken-out date.
 */

int
prsindate( char * line, struct tm *tm, int *tzp ) {
	int c;
	char ch;       /* used by SKIPTOSPC */
	char *cp;
	char c2;

	tm->tm_isdst = 0;
	SKIPSPC(line);
	if( (ch = *line) < '0' || ch > '9' ) {
		cp = line;
		while ((ch = *cp++), (!ISSPACE(ch) && ch != ',' && ch != '\0'))
			;
		cp--;
		if( ch == ',' ) {
			line = cp;
			SKIPOVER(line);      /* skip weekday */
			}
		else
			return -1;      /* missing comma after weekday */
		}

	GOBBLE_NUM( line, ch, c, &tm->tm_mday1?11?1?1? 1p ???"????"`?????`????`#?`???#????#a?????`????`#?`???1? ???11?p?1?121 ?1? ?3!?????!?4?r??#a???????`4?r??#a??????2pp2???2?????????!56???r???s?????s???66???r???s????p1?pp12?11111?1?1?p112 ????!q??`??4r?5q????rr???????ar?`4?5q????rr??11?1p11?@?11?1?11111p 1?1? 1?????2??!????4?2??"2?`??3???????4?2??"2?????? p???????p112?1?1????!!?!??????r?`????????"??????r?`????1???p?? 2p2?111?21p?p2p?????!?"6??6?5??r?r??????#r6??6?5??r?r?p??1 p ??1p1?211?pp ??!??4"4?"????6?`??rrr????5#???????6?`??rrr?? 1p??1?12??1p??1p?1?p ???"3???!??5?4?5?r???#4???`r5?4?5?r?2? 1 p?pp1 1p1p2?1 1?1??2!4!"q!3#qr?4???4??r6?#?qqr?4???4??r2?? pp2p1??21111211 1112 !???????``55r#4`r??r??$r??->tm_mon = 9;
		break;
	case PACK_TWO_CHARS('O', 'c'):
		tm->tm_mon = 10;
		break;
	case PACK_TWO_CHARS('N', 'o'):
		tm->tm_mon = 11;
		break;
	case PACK_TWO_CHARS('D', 'e'):
		tm->tm_mon = 12;
		break;
	default:
		return -1;      /* bad month name */
		}
	tm->tm_mon--;		/* [dz] BC++ uses 0-11 for month :(	*/
	SKIPOVER(line);         /* skip month */

	tm->tm_year = atoi(line);
	if (tm->tm_year <= 0)
		return -1;      /* year is non-positive or missing */

	if( tm->tm_year > 1900 )		// [dz]
		tm->tm_year -= 1900;		// RFC1036 requires year to be in 2-digit form, but too many people in Russia have incorrect mailers :(

	SKIPOVER(line);         /* skip year */

	if (parsetime(line, tm) < 0)
		return -1;

	SKIPOVER(line);         /* skip time */

	cp = line;
	if (*cp++ == 'G' && *cp++ == 'M' && *cp++ == 'T' &&
			(*cp == '\n' || *cp == '\0'))
		*tzp = 0;
	else {            /* weirdo time zone */
		/* this code doesn't recognise the numeric zone case */
		register datetkn *tp;

		cp = line;      /* time zone start */
		SKIPTOSPC(line);
		c = *line;      /* save old delimiter */
		*line = '\0';    /* terminate time zone */

		tp = datetoktype(cp, (int *)NULL);
		switch (tp->type) {
		case DTZ:
#if 0
			tm->tm_isdst++;
#endif
	   /* FALLTHROUGH */
		case TZ:
			*tzp = FROMVAL(tp);
	   /* FALLTHROUGH */
		case IGNORE:
			break;
		default:
			if( try_numeric_tz( tzp, cp ) == -1 )
				return -1;    /* bad token type */
			break;
			}

//        *line = c;      /* restore old delimiter */
//        SKIPSPC(line);
//        if (*line != '\0')    /* garbage after the date? */
//            return -1;
		}

	return 0;
	}



/* return -1 on failure */
int
parsetime( const char *time, struct tm *tm ) {
	register char c;
	register int x;

	tm->tm_sec = 0;
	GOBBLE_NUM(time, c, x, &tm->tm_hour);
	if (c != ':')
	  return -1;      /* only hour; too short */
	GOBBLE_NUM(time, c, x, &tm->tm_min);
	if (c != ':')
	  return 0;       /* no seconds; okay */
	GOBBLE_NUM(time, c, x, &tm->tm_sec);
	/* this may be considered too strict.  garbage at end of time? */
	return (c == '\0' || ISSPACE(c)? 0: -1);
	}


//  tt = pointer to timezone in numeric format, eg. "+0200"
//  tzp = pointer to int to put parsed value to
// Returns 0 on success, -1 on failure
//
//                          dz

static int
try_numeric_tz( int *tzp, char *tt )
{
    int     sign = 0;       // 0 = positive
    int     hv, mv;

    switch( *tt )
    {
        case '+':
            sign = 0;
            tt++;
            break;

        case '-':
            sign = 1;
            tt++;
            break;
    }

    for( int i = 0; i < 4; i++ )
    {
        if( tt[i] < '0' || tt[i] > '9' )
            return -1;

        tt[i] -= '0';
    }


    hv = tt[1] + tt[0] * 10;
    mv = tt[3] + tt[2] * 10;

    *tzp = hv * 60 + mv;
    if( sign )	*tzp = -*tzp;

    return 0;
}




@


1.4
log
@Error message
@
text
@d5 2
a6 2
 *	Module 	:	getindate - parse the common Internet date case
 *				(rfc 822 & 1123) *fast*
d9 3
d15 2
a16 2
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
d18 2
a19 2
 * Revision 1.1  1995/03/11  18:07:42  dz
 * Initial revision
d21 2
a22 2
 * Revision 1.1  1995/03/11  18:07:42  dz
 * Initial revision
d54 4
a57 3
static int			parsetime( const char *time, struct tm *tm );
static int			prsindate( char * line, struct tm *tm, int *tzp );
static int			try_numeric_tz( int *tzp, char *tztext );
d65 1
a65 1
#define     PACK_TWO_CHARS(c1, c2)   (((c1)<<8)|(c2))
d114 1
d116 4
a119 1
    time_t t = prsindate(l, &date, &tz) < 0? -1: dateconv(&date, tz);
d129 13
@


1.3
log
@current.
@
text
@d9 3
d36 1
d114 3
@


1.2
log
@debug
@
text
@d9 3
d47 1
a47 1
static int			parsetime( char *time, struct tm *tm );
d100 13
a112 6
getindate( char * line ) {
	int tz = 0;
	struct tm date;

	return prsindate(line, &date, &tz) < 0? -1: dateconv(&date, tz);
	}
d122 4
a125 4
	register int c;
	register char ch;       /* used by SKIPTOSPC */
	register char *cp;
	register char c2;
d259 1
a259 1
parsetime( char *time, struct tm *tm ) {
@


1.1
log
@Initial revision
@
text
@d8 7
a14 1
 *      $Log:   Q:/lib/date/vcs/getindat.c_v  $
@
