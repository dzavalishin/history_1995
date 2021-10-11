head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	96.03.27.10.46.53;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.08.13.22.34;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.12.21.20.45;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.24.01;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
log
@current
@
text
@/************************ FIDO2UU ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	UUENCODE
 *
 *      $Log: UUEncode.c $
 *      Revision 1.3  1995/08/08 13:22:34  dz
 *      App class used
 *
 *      Revision 1.2  1995/04/12  21:20:45  dz
 *      IBM C Set version seems to be working...
 *
 *      Revision 1.1  1995/03/11  18:24:01  dz
 *      Initial revision
 *
 *      
 *         Rev 1.4   07 Jun 1993 16:36:30   dz
 *      Copyright
 *      
 *         Rev 1.3   23 Oct 1992 14:32:18   dz
 *      error()
 *      
 *         Rev 1.2   29 Jun 1992 15:16:22   dz
 *      Open in deny-write mode
 *      
 *         Rev 1.1   18 Jun 1992 10:30:38   dz
 *      Bugfix: file now opened in binary mode
 *      
 *         Rev 1.0   04 Feb 1992 00:07:34   dz
 *      Initial revision.
 *
 *
\*/

#include	"fido2uu.h"

#include	<sys/types.h>
#include	<sys/stat.h>
#include	<share.h>
#include	<fcntl.h>


/* ENC is the basic 1 character encoding function to make a char printing */

//#define ENC(c) (((c) & 077) + ' ')

inline char ENC( int c )  { return (char) (((char)c) & 077) + ' '; }


static void      encode( FILE *in, RFC_Msg &out );
static void      outdec( char *p, char *op );
//static void		outdec( char *p, FILE *f );
static int	 fr( FILE *fd, char *buf, int cnt );



bool
uuencode( char *infn, RFC_Msg &out )
	{
	int	fd;
	FILE	*in;

	if ((fd = _sopen( infn, O_RDONLY|O_BINARY, SH_DENYWR )) < 0)
		{
		error( EI_Full, "Can't open %s for uuencode", infn );
		return Err;
		}

	if ((in = fdopen( fd, "rb" )) == NULL)
		{
		close( fd );
		error( EI_Full, "Can't fdopen %s for uuencode", infn );
		return Err;
		}

		{
		char	bname[_MAX_FNAME], bext[_MAX_EXT], d[_MAX_PATH];
                char    buf[100];
		_splitpath( infn, d, d, bname, bext );
                sprintf( buf, "begin %o %s%s", 0666, bname, bext );
                out.text.add( "" );
                out.text.add( buf );
		}

	encode(in, out);

        out.text.add( "end" );
        out.text.add( "" );

	fclose( in );
	return Ok;
	}

/*
 * copy from in to out, encoding as you go along.
 */
static void
encode( FILE *in, RFC_Msg &out )
    {
    char buf[80], outbuf[100], *outp;
    int i, n;
    
    for (;;)
        {
        outp = outbuf;
        /* 1 (up to) 45 character line */
        n = fr(in, buf, 45);
        *outp++ = ENC(n);
        
        for (i=0; i<n; i += 3)
            {
            outdec(&buf[i], outp);
            outp += 4;
            }
        
        *outp = 0;
        out.text.add( outbuf );
        if (n <= 0)
            break;
        }
    }


/*
 * output one group of 3 bytes, pointed at by p, on file f.
 */
static void
outdec( char *p, char *op )
	{
	int c1, c2, c3, c4;

	c1 = *p >> 2;
	c2 = ((*p << 4) & 060) | ((p[1] >> 4) & 017);
	c3 = ((p[1] << 2) & 074) | ((p[2] >> 6) & 03);
	c4 = p[2] & 077;
	*op++ = ENC(c1);
	*op++ = ENC(c2);
	*op++ = ENC(c3);
	*op++ = ENC(c4);
	}



/* fr: like read but stdio */

static int
fr( FILE *fd, char *buf, int cnt )
	{
	int c, i;

	for( i=0; i<cnt; i++ )
		{
		c = getc( fd );
		if( c == EOF )
			return i;

		buf[i] = (char) c;
		}

	return cnt;
	}
@


1.3
log
@App class used
@
text
@d8 3
d51 4
a54 3
static void		encode( FILE *in, FILE *out );
static void		outdec( char *p, FILE *f );
static int		fr( FILE *fd, char *buf, int cnt );
d59 1
a59 1
uuencode( char *infn, FILE *out )
d79 1
a79 1

d81 3
a83 1
		fprintf( out, "\nbegin %o %s%s\n", 0666, bname, bext );
d88 2
a89 1
	fprintf( out, "end\n\n" );
d99 24
a122 19
encode( FILE *in, FILE *out )
	{
	char buf[80];
	int i, n;

	for (;;)
		{
		/* 1 (up to) 45 character line */
		n = fr(in, buf, 45);
		putc( ENC(n), out);

		for (i=0; i<n; i += 3)
			outdec(&buf[i], out);

		putc('\n', out);
		if (n <= 0)
			break;
		}
	}
d129 1
a129 1
outdec( char *p, FILE *f )
d137 4
a140 4
	putc(ENC(c1), f);
	putc(ENC(c2), f);
	putc(ENC(c3), f);
	putc(ENC(c4), f);
@


1.2
log
@IBM C Set version seems to be working...
@
text
@d8 5
a12 2
 * Revision 1.1  1995/03/11  18:24:01  dz
 * Initial revision
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/fido2uu/vcs/uuencode.c_v  $
a33 1
#include	<dir.h>
d35 1
d53 2
a54 1
{
d57 2
a58 2
	if ((in = _fsopen( infn, "rb", SH_DENYWR )) == NULL)
	{
d61 8
a68 1
	}
d70 2
a71 2
	{
		char	bname[20], bext[5], d[80];
d73 1
a73 1
		fnsplit( infn, d, d, bname, bext );
d75 1
a75 1
	}
d83 1
a83 1
}
d90 1
a90 1
{
d95 1
a95 1
	{
d106 1
a107 1
}
d115 1
a115 1
{
d126 1
a126 1
}
d134 1
a134 1
{
d137 6
a142 5
	for (i=0; i<cnt; i++)
	{
		c = getc(fd);
		if (c == EOF)
			return(i);
d144 3
a147 2
	return (cnt);
}
@
