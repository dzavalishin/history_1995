#ifndef DATETOK_H__
#define DATETOK_H__

#ifndef	STYLE_H
#include	<style.h>
#endif

#define AM 0
#define PM 1

/* can't have more of these than there are bits in an unsigned long */
#define MONTH  1
#define YEAR  2
#define DAY   3
#define TIME  4
#define TZ   5
#define DTZ   6
#define IGNORE 7
#define AMPM  8
/* below here are unused so far */
#define SECONDS     9
#define MONTHS 10
#define YEARS  11
#define NUMBER 12
/* these are only for relative dates */
#define BEFORE 13
#define AFTER  14
#define AGO   15


#define SECS(n)       ((time_t)(n))
#define MINS(n)       ((time_t)(n) * SECS(60))
#define HOURS(n)    ((time_t)(n) * MINS(60))  /* 3600 secs */
#define DAYS(n)       ((time_t)(n) * HOURS(24))     /* 86400 secs */
/* months and years are not constant length, must be specially dealt with */

#define TOKMAXLEN 7   /* only this many chars are stored in datetktbl */

/* definitions for squeezing values into "value" */
#define SIGNBIT 0200
#define VALMASK 0177
#define NEG(n)      ((n)|SIGNBIT)
#define SIGNEDCHAR(c)  ((c)&SIGNBIT? -((c)&VALMASK): (c))
#define FROMVAL(tp)   (-SIGNEDCHAR((tp)->value) * 10)  /* uncompress */
#define TOVAL(tp, v)  ((tp)->value = ((v) < 0? NEG((-(v))/10): (v)/10))

/* keep this struct small; it gets used a lot */
typedef struct {
	char			token[TOKMAXLEN];
	char			type;
	signed char		value;       /* this may be unsigned, alas */
	} datetkn;

extern datetkn *datetoktype( char *s, int *bigvalp );
extern datetkn *datebsearch( char *key, datetkn *base, uint nel );

#endif /* DATETOK_H__ */ /* Do not add anything after this line */

