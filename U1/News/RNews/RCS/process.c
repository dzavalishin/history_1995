head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	95.08.07.13.27.53;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.07.05.57.47;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.09.21.05.55;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.06.14.45;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
log
@Cleqanup
@
text
@/************************ UU2 Rnews ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Process incoming batch, copy it to inbound with
 *				proper extension
 *
 *      $Log: process.c $
 *      Revision 1.3  1995/08/07 05:57:47  dz
 *      Using class App
 *
 *      Revision 1.2  1995/04/09  21:05:55  dz
 *      IBM C Set version
 *
 *      Revision 1.1  1995/03/11  06:14:45  dz
 *      Initial revision
 *
 *      
 *         Rev 1.5   28 Dec 1992 22:48:38   dz
 *      Bug in recognition of 16-bit compress headers fixed.
 *      
 *         Rev 1.4   28 Nov 1992 22:13:12   dz
 *      String to const char *
 *      
 *         Rev 1.3   23 Oct 1992 14:55:20   dz
 *      error()
 *      
 *         Rev 1.2   18 Sep 1992 03:08:32   dz
 *      exit codes
 *      
 *         Rev 1.1   18 Jun 1992 11:06:42   dz
 *      update
 *      
 *         Rev 1.0   08 Feb 1992 15:26:48   dz
 *      Initial revision.
 *
 *
\*/

#include	"rnews.h"
#include	<share.h>


static void        p_copy( char *ext );


bool
getheader( char *buf, int len, int fd )
	{
	long	pp = 0;
	int 	i;
	uchar	c;

	while( --len > 0 )
		{

		i = read( fd, &c, 1 );
		if( i < 0 )
			return Err;

		if( c == '\r' || c == '\n' || i == 0 )
			break;

		*buf++ = c;
		}

	*buf = '\0';

	pp = tell( fd );
	i = read( fd, &c, 1 );
	if( (i == 1) && (c == '\r' || c == '\n') )
		return Ok;

	lseek( fd, pp, SEEK_SET );
	return Ok;
	}

static bool
zheader( const char *h )
	{
	if( strncmp( h, "\x1F\x8B\x08", 3 ) == 0 )
		return Yes;

	if( h[0] != '' )               return No;
	if( h[1] != 'ù' )               return No;
	if( h[2] < 'å' || h[2] > 'ê' )  return No;  // 12 to 16 bit

	return Yes;
	}


bool
RnewsApp::process( void )
	{
	char	line[50], *cp;

	setmode( 0, O_BINARY );

debug("processing");

	if( getheader( line, 50, 0 ) != Ok )
		fatal( EC_IO_Error, EI_Full, "Can't read batch header" );


	if( zheader( line ) )    				// compressed, no 'cunbatch' hdr
		{
		lseek( 0, 0L, SEEK_SET );
		p_copy( ".Z" );
		log( "bn", "Compressed batch" );
		return Ok;
		}


	if( line[0] != '#' || line[1] != '!' )  // No '#!' - one art.
		{
		lseek( 0, 0L, SEEK_SET );
		p_copy( ".A" );
		log( "n", "Article" );
		return Ok;
		}

	cp = line+2;
	while( *cp == ' ' || *cp == '\t' )
		cp++;

	if( strncmp( cp, "cunbatch", 8 ) == 0 )	// #! cunbatch - compressed
		{
		p_copy( ".Z" );
		log( "bn", "cunbatch header" );
		return Ok;
		}

	lseek( 0, 0L, SEEK_SET );
	p_copy( ".B" );                         // Batch
	log( "bn", "Non-compressed batch" );

	return Ok;
	}



/****************************************************************************
						Create temp file
****************************************************************************/


#define	MAX_TRY		20

void
maketemp( char *ext, int *o_fd ) 
	{
	int			fd;
	char		fn[80];
	long		t = time(NULL) << 8;
	long		tt = t;

	do
		{
		sprintf( fn, "%s\\%08lX%s", (const char *)conf.newsin_dir.c_str(), t++, ext );
		fd = _sopen( fn, O_CREAT|O_EXCL|O_BINARY|O_WRONLY, SH_DENYRW, 0666 );

		if( t - tt > MAX_TRY )
			fatal( EC_OS_Error, EI_Full, "Too many tries - can't create temp file");
		}
	while( fd < 0 );

	debug("Saved in %s", fn);

	*o_fd = fd;
	}



/****************************************************************************
                        Copy file
****************************************************************************/

#define CBUFSIZE    (16*1024)

static char     cbuf[CBUFSIZE];

static void
copyf( int out, int in )
	{
	int l;

	do {

		l = read( in, cbuf, CBUFSIZE );        // Read block
		if( l < 0 )
			{
			error( EI_Full, "Can't read input" );
			return;
			}
		if( l && write( out, cbuf, l ) != l )
			{
			error( EI_Full, "Can't write to temp. file" );
			return;
			}
		} while( l );
	}


static void
p_copy( char *ext )
	{
	int		fd;

	maketemp( ext, &fd );           		// Create temp file

	copyf( fd, 0 );							// Copy stdin to it

	if( close( fd ) )
		fatal( EC_IO_Error, EI_Full, "Can't close temp file" );
	}




@


1.3
log
@Using class App
@
text
@d9 2
a10 2
 * Revision 1.2  1995/04/09  21:05:55  dz
 * IBM C Set version
d12 5
a16 2
 * Revision 1.1  1995/03/11  06:14:45  dz
 * Initial revision
d137 1
a137 1
    return Ok;
@


1.2
log
@IBM C Set version
@
text
@d9 3
d90 1
a90 1
process( void )
@


1.1
log
@Initial revision
@
text
@d8 4
a11 1
 *      $Log:   Q:/news/rnews/vcs/process.c_v  $
d35 1
d38 1
a38 1
void        p_copy( char *ext );
d75 8
a82 4
    if( h[0] != '' )               return No;
    if( h[1] != 'ù' )               return No;
    if( h[2] < 'å' || h[2] > 'ê' )  return No;  // 12 to 16 bit
    return Yes;
d144 2
a145 1
maketemp( char *ext, int *o_fd ) {
d154 1
a154 1
		fd = open( fn, O_CREAT|O_EXCL|O_BINARY|O_WRONLY|O_DENYALL, 0666 );
@
