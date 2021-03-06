/************************ FIDO2UU ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	UUENCODE
 *
 *      $Log: UUEncode.c $
 *      Revision 1.4  1996/03/27 10:46:53  dz
 *      current
 *
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

inline char ENC( int ic )
    { 
    char c = (((char)ic) & 077) + ' ';
    if( c == ' ' ) c = '`';
    return c;
    }


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
