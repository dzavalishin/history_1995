/************************ UU2 UU2FILE ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	UUDECODE
 *
 *      $Log:   Q:/news/snews/vcs/main.c_v  $
 *
 *
\*/

#include    "uu2file.h"
/*
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
*/

/* single character decode */
#define DEC(c)	(((c) - ' ') & 077)

/*
 * 	decode one line of text to given file
 */

bool
uudecode( const char *buf, FILE *out)
	{
	const char *bp;
	int n;
	int c1, c2, c3;

	n = DEC( buf[0] );
	if( n <= 0 )
		return Ok;			// n < 0 - BUG??

	bp = &buf[1];
	while( n > 0 )
		{
		c1 = (DEC(*bp) << 2) | (DEC(bp[1]) >> 4);
		c2 = (DEC(bp[1]) << 4) | (DEC(bp[2]) >> 2);
		c3 = (DEC(bp[2]) << 6) | (DEC(bp[3]));
		if( n >= 1 ) putc( c1, out );
		if( n >= 2 ) putc( c2, out );
		if( n >= 3 ) putc( c3, out );
		bp += 4;
		n -= 3;
		}

	if( ferror( out ) )
		{
		error( EI_Full, "Can't write to temp. file" );
		return Err;
		}

	return Ok;
	}

