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
@/*
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


@


1.1
log
@Initial revision
@
text
@d14 2
d20 2
a21 2
extern datetkn datetktbl[];
extern unsigned szdatetktbl;
d24 2
a25 1
datetoktype( char *s, int *bigvalp ) {
d31 2
a32 1
	if (isascii(c) && isdigit(c)) {
d37 2
a38 1
		else {
d48 1
d50 2
a51 1
	} else if (c == '-' || c == '+') {
d61 3
a63 1
	} else {
d72 2
a73 1
		if (tp == NULL) {
d76 1
d78 1
a79 2
	return tp;
}
d86 2
a87 1
datebsearch( char *key, datetkn *base, uint nel ) {
d91 2
a92 1
	while (last >= base) {
d95 2
a96 1
		if (result == 0) {
d100 1
a100 1
		}
d105 2
a107 2
	return 0;
}
a109 179
/*
 * to keep this table reasonably small, we divide the lexval for TZ and DTZ
 * entries by 10 and truncate the text field at MAXTOKLEN characters.
 * the text field is not guaranteed to be NUL-terminated.
 */
static datetkn datetktbl[] = {
/*	text		token	lexval */
	"acsst",	DTZ,	63,			/* Cent. Australia */
	"acst",		TZ,		57,			/* Cent. Australia */
	"adt",		DTZ,	NEG(18),	/* Atlantic Daylight Time */
	"aesst",	DTZ,	66,			/* E. Australia */
	"aest",		TZ,		60,			/* Australia Eastern Std Time */
	"ahst",		TZ,		60,			/* Alaska-Hawaii Std Time */
	"am",		AMPM,	AM,
	"apr",		MONTH,	4,
	"april",	MONTH,	4,
	"ast",		TZ,		NEG(24),	/* Atlantic Std Time (Canada) */
	"at",		IGNORE,	0,			/* "at" (throwaway) */
	"aug",		MONTH,	8,
	"august",	MONTH,	8,
	"awsst",	DTZ,	54,			/* W. Australia */
	"awst",		TZ,		48,			/* W. Australia */
	"bst",		TZ,		6,			/* British Summer Time */
	"bt",		TZ,		18,			/* Baghdad Time */
	"cadt",		DTZ,	63,			/* Central Australian DST */
	"cast",		TZ,		57,			/* Central Australian ST */
	"cat",		TZ,		NEG(60),	/* Central Alaska Time */
	"cct",		TZ,		48,			/* China Coast */
	"cdt",		DTZ,	NEG(30),	/* Central Daylight Time */
	"cet",		TZ,		6,			/* Central European Time */
	"cetdst",	DTZ,	12,			/* Central European Dayl.Time */
	"cst",		TZ,		NEG(36),	/* Central Standard Time */
	"dec",		MONTH,	12,
	"decemb",	MONTH,	12,
	"dnt",		TZ,		6,			/* Dansk Normal Tid */
	"dst",		IGNORE,	0,
	"east",		TZ,		NEG(60),	/* East Australian Std Time */
	"edt",		DTZ,	NEG(24),	/* Eastern Daylight Time */
	"eet",		TZ,		12,			/* East. Europe, USSR Zone 1 */
	"eetdst",	DTZ,	18,			/* Eastern Europe */
	"est",		TZ,		NEG(30),	/* Eastern Standard Time */
	"feb",		MONTH,	2,
	"februa",	MONTH,	2,
	"fri",		IGNORE,	5,
	"friday",	IGNORE,	5,
	"fst",		TZ,		6,			/* French Summer Time */
	"fwt",		DTZ,	12,			/* French Winter Time  */
	"gmt",		TZ,		0,			/* Greenwish Mean Time */
	"gst",		TZ,		60,			/* Guam Std Time, USSR Zone 9 */
	"hdt",		DTZ,	NEG(54),	/* Hawaii/Alaska */
	"hmt",		DTZ,	18,			/* Hellas ? ? */
	"hst",		TZ,		NEG(60),	/* Hawaii Std Time */
	"idle",		TZ,		72,			/* Intl. Date Line, East */
	"idlw",		TZ,		NEG(72),	/* Intl. Date Line, West */
	"ist",		TZ,		12,			/* Israel */
	"it",		TZ,		22,			/* Iran Time */
	"jan",		MONTH,	1,
	"januar",	MONTH,	1,
	"jst",		TZ,		54,			/* Japan Std Time,USSR Zone 8 */
	"jt",		TZ,		45,			/* Java Time */
	"jul",		MONTH,	7,
	"july",		MONTH,	7,
	"jun",		MONTH,	6,
	"june",		MONTH,	6,
	"kst",		TZ,		54,			/* Korea Standard Time */
	"ligt",		TZ,		60,			/* From Melbourne, Australia */
	"mar",		MONTH,	3,
	"march",	MONTH,	3,
	"may",		MONTH,	5,
	"mdt",		DTZ,	NEG(36),	/* Mountain Daylight Time */
	"mest",		DTZ,	12,			/* Middle Europe Summer Time */
	"met",		TZ,		6,			/* Middle Europe Time */
	"metdst",	DTZ,	12,			/* Middle Europe Daylight Time*/
	"mewt",		TZ,		6,			/* Middle Europe Winter Time */
	"mez",		TZ,		6,			/* Middle Europe Zone */
	"mon",		IGNORE,	1,
	"monday",	IGNORE,	1,
/*    "msd",      DTZ,    18,         /* Moscow Daylight - correct? */
    "msk",      TZ,     18,         /* Moscow, USSR Zone 1 */
	"mst",		TZ,		NEG(42),	/* Mountain Standard Time */
	"mt",		TZ,		51,			/* Moluccas Time */
	"ndt",		DTZ,	NEG(15),	/* Nfld. Daylight Time */
	"nft",		TZ,		NEG(21),	/* Newfoundland Standard Time */
	"nor",		TZ,		6,			/* Norway Standard Time */
	"nov",		MONTH,	11,
	"novemb",	MONTH,	11,
	"nst",		TZ,		NEG(21),	/* Nfld. Standard Time */
	"nt",		TZ,		NEG(66),	/* Nome Time */
	"nzdt",		DTZ,	78,			/* New Zealand Daylight Time */
	"nzst",		TZ,		72,			/* New Zealand Standard Time */
	"nzt",		TZ,		72,			/* New Zealand Time */
	"oct",		MONTH,	10,
	"octobe",	MONTH,	10,
	"on",		IGNORE,	0,			/* "on" (throwaway) */
	"pdt",		DTZ,	NEG(42),	/* Pacific Daylight Time */
	"pm",		AMPM,	PM,
	"pst",		TZ,		NEG(48),	/* Pacific Standard Time */
	"sadt",		DTZ,	63,			/* S. Australian Dayl. Time */
	"sast",		TZ,		57,			/* South Australian Std Time */
	"sat",		IGNORE,	6,
	"saturd",	IGNORE,	6,
	"sep",		MONTH,	9,
	"sept",		MONTH,	9,
	"septem",	MONTH,	9,
	"set",		TZ,		NEG(6),		/* Seychelles Time ?? */
	"sst",		DTZ,	12,			/* Swedish Summer Time */
	"sun",		IGNORE,	0,
	"sunday",	IGNORE,	0,
	"swt",		TZ,		6,			/* Swedish Winter Time  */
	"thu",		IGNORE,	4,
	"thur",		IGNORE,	4,
	"thurs",	IGNORE,	4,
	"thursd",	IGNORE,	4,
	"tue",		IGNORE,	2,
	"tues",		IGNORE,	2,
	"tuesda",	IGNORE,	2,
	"ut",		TZ,		0,
	"utc",		TZ,		0,
	"wadt",		DTZ,	48,			/* West Australian DST */
	"wast",		TZ,		42,			/* West Australian Std Time */
	"wat",		TZ,		NEG(6),		/* West Africa Time */
	"wdt",		DTZ,	54,			/* West Australian DST */
	"wed",		IGNORE,	3,
	"wednes",	IGNORE,	3,
	"weds",		IGNORE,	3,
	"wet",		TZ,		0,			/* Western Europe */
	"wetdst",	DTZ,	6,			/* Western Europe */
	"wst",		TZ,		48,			/* West Australian Std Time */
	"ydt",		DTZ,	NEG(48),	/* Yukon Daylight Time */
	"yst",		TZ,		NEG(54),	/* Yukon Standard Time */
	"zp4",		TZ,		NEG(24),	/* GMT +4  hours. */
	"zp5",		TZ,		NEG(30),	/* GMT +5  hours. */
	"zp6",		TZ,		NEG(36),	/* GMT +6  hours. */
};

#if	0
/*
 * these time zones are orphans, i.e. the name is also used by a more
 * likely-to-appear time zone
 */
	"at",		TZ,		NEG(12),	/* Azores Time */
	"bst",		TZ,		NEG(18),	/* Brazil Std Time */
	"bt",		TZ,		NEG(66),	/* Bering Time */
	"edt",		TZ,		66,			/* Australian Eastern DaylTime*/
	"est",		TZ,		60,			/* Australian Eastern Std Time*/
	"ist",		TZ,		33,			/* Indian Standard Time */
	"nst",		TZ,		51,			/* North Sumatra Time */
	"sst",		TZ,		42,			/* South Sumatra, USSR Zone 6 */
	"sst",		TZ,		48,			/* Singapore Std Time */
	"wet",		TZ,		6,			/* Western European Time */
/* military timezones are deprecated by RFC 1123 section 5.2.14 */
	"a",		TZ,		6,			/* UTC+1h */
	"b",		TZ,		12,			/* UTC+2h */
	"c",		TZ,		18,			/* UTC+3h */
	"d",		TZ,		24,			/* UTC+4h */
	"e",		TZ,		30,			/* UTC+5h */
	"f",		TZ,		36,			/* UTC+6h */
	"g",		TZ,		42,			/* UTC+7h */
	"h",		TZ,		48,			/* UTC+8h */
	"i",		TZ,		54,			/* UTC+9h */
	"k",		TZ,		60,			/* UTC+10h */
	"l",		TZ,		66,			/* UTC+11h */
	"m",		TZ,		72,			/* UTC+12h */
	"n",		TZ,		NEG(6),		/* UTC-1h */
	"o",		TZ,		NEG(12),	/* UTC-2h */
	"p",		TZ,		NEG(18),	/* UTC-3h */
	"q",		TZ,		NEG(24),	/* UTC-4h */
	"r",		TZ,		NEG(30),	/* UTC-5h */
	"s",		TZ,		NEG(36),	/* UTC-6h */
	"t",		TZ,		NEG(42),	/* UTC-7h */
	"u",		TZ,		NEG(48),	/* UTC-8h */
	"v",		TZ,		NEG(54),	/* UTC-9h */
	"w",		TZ,		NEG(60),	/* UTC-10h */
	"x",		TZ,		NEG(66),	/* UTC-11h */
	"y",		TZ,		NEG(72),	/* UTC-12h */
	"z",		TZ,		0,			/* UTC */
#endif

static unsigned int szdatetktbl = sizeof datetktbl / sizeof datetktbl[0];
@
