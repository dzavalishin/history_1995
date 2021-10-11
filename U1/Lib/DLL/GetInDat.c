/********************** Date/time conversion library *************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	: getindate - parse the common Internet date case
 *                (rfc 822 & 1123) *fast*
 *
 *      $Log: GetInDat.c $
 *      Revision 1.5  1996/03/29 22:13:24  dz
 *      remove comment
 *
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

	GOBBLE_NUM( line, ch, c, &tm->tm_mday );

	/*
	 * we have to map to canonical case because RFC 822 requires
	 * case independence, so we pay a performance penalty for the sake
	 * of 0.1% of dates actually seen in Date: headers in news.
	 * Way to go, IETF.
	 */

	ch = *line++;
	if (ch == '\0')
		return -1;      /* no month */
	if (isascii(ch) && islower(ch))
		ch = toupper(ch);
	c2 = *line++;
	if (c2 == '\0')
		return -1;      /* month too short */
	if (isascii(c2) && isupper(c2))
		c2 = tolower(c2);

	switch (PACK_TWO_CHARS(ch, c2)) {
	case PACK_TWO_CHARS('J', 'a'):
		tm->tm_mon = 1;
		break;
	case PACK_TWO_CHARS('F', 'e'):
		tm->tm_mon = 2;
		break;
	case PACK_TWO_CHARS('M', 'a'):   /* March, May */
		tm->tm_mon = ((ch = *line) == 'r' || ch == 'R'? 3: 5);
		break;
	case PACK_TWO_CHARS('A', 'p'):
		tm->tm_mon = 4;
		break;
	case PACK_TWO_CHARS('J', 'u'):
		tm->tm_mon = 6;
		if ((ch = *line) == 'l' || ch == 'L')
			tm->tm_mon++;   /* July */
		break;
	case PACK_TWO_CHARS('A', 'u'):
		tm->tm_mon = 8;
		break;
	case PACK_TWO_CHARS('S', 'e'):
		tm->tm_mon = 9;
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




