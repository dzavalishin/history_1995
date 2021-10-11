head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	95.08.07.13.57.10;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.08.07.13.28.26;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.02.13.35.27;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.09.23.24.27;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.31.14;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@Cleanup
@
text
@/********************* UU2 Snews ************************\
 *
 *	Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	Origin line decoding
 *
 *      $Log: Origin.c $
 *      Revision 1.4  1995/08/07 13:28:26  dz
 *      Using class App
 *
 *      Revision 1.3  1995/08/02 13:35:27  dz
 *      rewritten with try_to_load()
 *
 *      Revision 1.2  1995/04/09  23:24:27  dz
 *      IBM C Set version
 *
 *      Revision 1.1  1995/03/11  18:31:14  dz
 *      Initial revision
 *
 *      
 *         Rev 1.6   09 Apr 1993 15:26:26   dz
 *      FSC-0070 implemented (MSGID --> Message-ID)
 *      
 *         Rev 1.5   23 Oct 1992 14:58:46   dz
 *      error()
 *      
 *         Rev 1.4   18 Sep 1992 03:10:40   dz
 *      exit codes
 *      
 *         Rev 1.3   18 Jun 1992 11:11:22   dz
 *      AREA: field extraction
 *      
 *         Rev 1.2   23 Mar 1992 02:59:40   dz
 *      Bug fix: Initial zeroing of info.msgid & info.reply 
 *      fields added.
 *      
 *         Rev 1.1   04 Feb 1992 00:05:08   dz
 *      read_origin reads MSGID/REPLY in addition to Origin itself :)
 *      
 *         Rev 1.0   06 Oct 1991 00:20:52   dz
 *      Initial revision.
 *
 *
\*/

#include	"snews.h"
#include	<ctype.h>

#define		BUFS	500


static bool		reads( char *buf, int len, FILE *fp );




bool
get_origin( char *or_txt, fido_addr &or, char *buf )
    {
	char	*p = buf;
	char	*a;

	while( *p && (*p == ' ' || *p == '\t'))
		p++;

	if( *p != '*' )
		return No;

	p++;

	while( *p && (*p == ' ' || *p == '\t'))
		p++;

	if( strnicmp( p, "origin:", 7 ))
		return No;

	p += 7;


	a = strrchr( p, '(' );
	if( a == NULL )
		{
		error( EI_None, "No '(' in origin line: %s", buf );
		return No;
		}

	{
	char    *pp = p;

	while( isspace( *pp )) pp++;

	int	l = a - pp;
	if( l > 79 ) l = 79;
	strncpy( or_txt, pp, l );
	or_txt[l] = '\0';

	pp = or_txt + strlen(or_txt) - 1;
	while( (pp > or_txt) && isspace(*pp) )
		{
		*pp = '\0';
		pp--;
		}

	}

        while( *a && !isdigit( *a ))
                a++;

	const maxb = 200;
	char adr_string[maxb];
	strncpy( adr_string, a, maxb );
	adr_string[maxb-1] = '\0';

	a = strchr( adr_string, ')' );
	if( a == NULL )
		{
		error( EI_None, "No ')' in origin line: %s", buf );
		return No;
		}
	*a = '\0';

	const char *var_adr_string = adr_string; // aparse will modify this pointer
	if( or.aparse( var_adr_string ) != Err )
		return Yes;

	return No;
	}


bool
get_area( char *area, char *line )
	{
	char	copy[80];

	strncpy( copy, line, 80 );
	copy[79] = '\0';

	strupr( copy );

	if( sscanf( copy, "AREA: %s", area ) == 1 )
		return Yes;

	if( sscanf( copy, "AREA:%s",  area ) == 1 )
		return Yes;

	if( sscanf( copy, "GROUP: %s", area ) == 1 )
		return Yes;

	if( sscanf( copy, "GROUP:%s",  area ) == 1 )
		return Yes;

	return No;
	}


void
try_to_load( string &s, const char *tag, const char *src )
	{
	int l = strlen( tag );

	if( 0 != strnicmp( src, tag, l ) )
		return;

	src += l;
	while( isspace( *src ) ) src++;

	s = src;
	}

bool
SnewsApp::read_origin( fido_info &info, FILE *fp )
	{
	char			buf[ BUFS ];
	long			pos;
	bool			got_area = No;

	if( (pos = ftell( fp )) < 0L )
		fatal( EC_IO_Error, EI_Full, "Can't ftell() reading origin!" );

	info.msgid[0] = '\0';
	info.reply[0] = '\0';
	info.area[0]  = '\0';

	while( 1 )
		{
		bool	r;

		if( ( r = reads( buf, BUFS, fp  )) != Yes )	// Next line
			{
			if( (r == Err) || ferror( fp ) )
				fatal( EC_IO_Error, EI_Full, "Problems reading packet" );

			break;
			}

		if( buf[0] == '\x1' ) 						// Kludge
			{
			char	*p = buf+1;

			if( !strnicmp( p, "MSGID:", 6 ) )
				{
				p += 6;
				while( isspace(*p) ) p++;
				strncpy( info.msgid, p, sizeof(info.msgid) );
				}
			else
			if( !strnicmp( p, "REPLY:", 6 ) )
				{
				p += 6;
				while( isspace(*p) ) p++;
				strncpy( info.reply, p, sizeof(info.reply) );
				}
			else

			try_to_load( info.RFC_Path, "RFC-Path:", p );
			try_to_load( info.RFC_Message_Id, "RFC-Message-Id:", p );
			try_to_load( info.RFC_From, "RFC-From:", p );
			try_to_load( info.RFC_Distribution, "RFC-Distribution:", p );
			try_to_load( info.REPLYADDR, "REPLYADDR", p );

			continue;
			}

		if( !got_area )
			if( get_area( info.area, buf ) == Yes )
				got_area = Yes;

		if( get_origin( info.origin_text, info.origin, buf ) == Yes )
			{
			if( fseek( fp, pos, SEEK_SET ) )
				fatal( EC_IO_Error, EI_Full, "Seek error" );
			return Ok;
			}

		}

	info.origin.clear();

	if( fseek( fp, pos, SEEK_SET ) )
		fatal( EC_IO_Error, EI_Full, "Seek error" );

	return Err;
	}


/****************************************************************************
						Read packed message
****************************************************************************/


static bool
reads( char *buf, int len, FILE *fp )
    {
	int		c;

    while( len > 1 )
        {
		if( (c = getc( fp )) == EOF )
			return Err;

		if( c == '\n' )								// Eat LF - allways!
			continue;

        if( ((uchar)c) == 0x0Du || ((uchar)c) == 0x8Du )
            {
			*buf = '\0';
			return Yes;
			}

        if( c == '\0' )
            {
			*buf = '\0';
			return No;
			}

		*buf++ = c;
		len--;
		}

	*buf = '\0';
	return Yes;
	}






@


1.4
log
@Using class App
@
text
@d8 3
d14 2
a15 2
 * Revision 1.2  1995/04/09  23:24:27  dz
 * IBM C Set version
d17 2
a18 2
 * Revision 1.1  1995/03/11  18:31:14  dz
 * Initial revision
@


1.3
log
@rewritten with try_to_load()
@
text
@d8 3
d168 1
a168 1
read_origin( fido_info &info, FILE *fp )
@


1.2
log
@IBM C Set version
@
text
@d8 3
a72 13
/*
	while( (a = strchr( p, '(' )) != NULL )
		{
		a++;
		p = a;

		while( *a && !isdigit( *a ))
			a++;

		if( or.aparse( a ) != Err )
			return Yes;
		}
*/
d150 2
a151 6
// See FTS-0070 for details.


/*
static void
pack_id( char *out, const char *in )
d153 1
a153 5
	int	p, f, n, z;
	int ll;
	char id_line[50];

	const char *orig_in = in;
d155 1
a155 4
	if( 3 != sscanf( in, "%d:%d/%d%n", &z, &n, &f, &ll ))
		{
		error( EI_None, "Unparsable ID: %s", orig_in );
		*out = '\0';
a156 5
		}

	in += ll;

	p = 0;
d158 2
a159 2
	if( 1 == sscanf( in, ".%d%n", &p, &ll ))
		in += ll;
d161 1
a161 20
	if( *in == '@@' )		// Fido address has domain
		{
		in++;
		while( !isspace( *in ) )
			in++;
		}

	if( 1 != sscanf( in, "%50s", id_line ) )
		{
		error( EI_None, "Unparsable ID: %s", orig_in );
		*out = '\0';
		return;
		}

	strlwr( id_line );

	if( p )
		sprintf( out, "%d_%d_%d_%d_%s", z, n, f, p, id_line );
	else
		sprintf( out, "%d_%d_%d_%s", z, n, f, id_line );
a162 2
*/

a195 1
//				pack_id( info.msgid, p + 6 );
a202 1
//				pack_id( info.reply, p + 6 );
d207 7
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/snews/vcs/origin.c_v  $
d65 1
a65 1
	if( strncmpi( p, "origin:", 7 ))
d126 2
a127 1
	if( or.aparse( adr_string ) != Err )
d223 1
@
