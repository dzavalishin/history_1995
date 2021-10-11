/************************ UU2 FILE2UU ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Scan outfile spool for TICs
 *
 *      $Log:   Q:/file/file2uu/vcs/uuencode.c_v  $
 *      
 *         Rev 1.0   29 Dec 1992 01:30:58   dz
 *      Initial revision.
 *
 *
\*/

#include    "file2uu.h"
#include	<log.h>

//#include	<sys/types.h>
//#include	<sys/stat.h>
//#include	<dir.h>
//#include	<share.h>


/* ENC is the basic 1 character encoding function to make a char printing */
#define ENC(c) (((c) & 077) + ' ')


static void		outdec( char *p, FILE *f );
static int		fr( FILE *fd, char *buf, int cnt );


/*
 * copy from in to out, encoding as you go along.
 */

bool
uuencode( FILE *out, FILE *in, long nbytes )
	{
	char buf[80];
	int i, n;

	while( nbytes > 0 )
		{
		/* 1 (up to) 45 character line */
		n = fr( in, buf, 45 );
		nbytes -= n;

		putc( ENC(n), out );

		for( i=0; i<n; i += 3 )
			outdec( &buf[i], out );

		putc( '\n', out );

		if( n <= 0 )
			break;
		}

	return Ok;
	}


/*
 * output one group of 3 bytes, pointed at by p, on file f.
 */
static void
outdec( char *p, FILE *f )
	{
	int c1, c2, c3, c4;

	c1 = *p >> 2;
	c2 = ((*p << 4) & 060) | ((p[1] >> 4) & 017);
	c3 = ((p[1] << 2) & 074) | ((p[2] >> 6) & 03);
	c4 = p[2] & 077;

	putc(ENC(c1), f);
	putc(ENC(c2), f);
	putc(ENC(c3), f);
	putc(ENC(c4), f);
	}



/* fr: like read but stdio */

static int
fr( FILE *fd, char *buf, int cnt )
	{
	int c, i;

	for ( i=0; i<cnt; i++ )
		{
		c = getc( fd );

		if( c == EOF )
			return i;

		buf[i] = c;
		}

	return (cnt);
	}
