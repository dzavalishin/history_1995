/*
 * datetok - date tokenisation
 */

#include	<stdio.h>
#include	<ctype.h>
#include	<string.h>
#include	<sys/types.h>		/* for dateconv.h */
#include	<stdlib.h>

#include	"dateconv.h"
#include	"datetok.h"

#include	"datetok.tbl"

/* imports */
int dtok_numparsed;

/* forwards */
//extern datetkn datetktbl[];
//extern unsigned szdatetktbl;

datetkn *
datetoktype( char *s, int *bigvalp ) 
	{
	register char *cp = s;
	register char c = *cp;
	static datetkn t;
	register datetkn *tp = &t;

	if (isascii(c) && isdigit(c)) 
		{
		register int len = strlen(cp);

		if (len > 3 && (cp[1] == ':' || cp[2] == ':'))
			tp->type = TIME;
		else 
			{
			if (bigvalp != NULL)
				/* won't fit in tp->value */
				*bigvalp = atoi(cp);
			if (len == 4)
				tp->type = YEAR;
			else if (++dtok_numparsed == 1)
				tp->type = DAY;
			else
				tp->type = YEAR;
			}
		}
	else if (c == '-' || c == '+') 
		{
		register int val = atoi(cp + 1);
		register int hr =  val / 100;
		register int min = val % 100;

		val = hr*60 + min;
		if (c == '-')
			val = -val;
		tp->type = TZ;
		TOVAL(tp, val);
		} 
	else 
		{
		char lowtoken[TOKMAXLEN+1];
		register char *ltp = lowtoken, *endltp = lowtoken+TOKMAXLEN;

		/* copy to lowtoken to avoid modifying s */
		while ((c = *cp++) != '\0' && ltp < endltp)
			*ltp++ = (isascii(c) && isupper(c)? tolower(c): c);
		*ltp = '\0';
		tp = datebsearch(lowtoken, datetktbl, szdatetktbl);
		if (tp == NULL) 
			{
			tp = &t;
			tp->type = IGNORE;
			}
		}
	return tp;
	}

/*
 * Binary search -- from Knuth (6.2.1) Algorithm B.  Special case like this
 * is WAY faster than the generic bsearch().
 */
datetkn *
datebsearch( char *key, datetkn *base, uint nel ) 
	{
	register datetkn *last = base + nel - 1, *position;
	register int result;

	while (last >= base) 
		{
		position = base + ((last - base) >> 1);
		result = key[0] - position->token[0];
		if (result == 0) 
			{
			result = strncmp(key, position->token, TOKMAXLEN);
			if (result == 0)
				return position;
			}
		if (result < 0)
			last = position - 1;
		else
			base = position + 1;
		}
	return 0;
	}


