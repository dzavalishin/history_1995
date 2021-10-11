head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	95.08.07.14.55.01;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.08.02.13.44.44;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.05.08.02.37.10;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@Using class App
@
text
@/************************ UUCP to FIDO gate ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Add letter to echo packet.
 *
 *      $Log: SendFile.c $
 *      Revision 1.2  1995/08/02 13:44:44  dz
 *      Buffers extended, diagnostics added.
 *
 * Revision 1.1  1995/05/08  02:37:10  dz
 * Initial revision
 *
 *
 *
 *
\*/

#include    "unbatch.h"
#include    "fido_pkt.h"
#include    "stat.h"
#include    "..\..\version.h"
#include    "..\..\u1.h"
#include    <recode.h>
#include    <hl_match.h>

#include    <uu2io.h>
#include    <fcntl.h>
#include    <share.h>

#include    "vread.h"




bool
UnbatchApp::save_to_file( VReadBase &in, uuinfo &, echo_def &edef, bool )
	{
	char	fn[100];
	FILE	*fp;
	int	fd;

	recoder		r( conf.code_uf.c_str() );
	if( !r.valid() )
		error( EI_None, "Codetable '%s' not found", (const char *)conf.code_uf.c_str() );


	while( 1 )
		{
		sprintf( fn, "%s\\%lx", (const char *)edef.in_file_dir().c_str(), time(NULL) );


		errno = 0;

//		fd = _sopen( fn, O_WRONLY|O_TEXT, SH_DENYWR, 0666 );
		fd = _sopen( fn, O_WRONLY|O_TEXT|O_CREAT|O_EXCL, SH_DENYWR, 0666 );
		if( fd < 0 )
			{
			if( errno == EEXIST ) 
				continue;
			error( EI_Full, "Can't create (_sopen) %s", fn );
			return Err;
			}

#if 0
		fp = fdopen( fd, "w" );
		if( fd != NULL )
			{
			setvbuf( fp, NULL, _IOFBF, 64*1024 );
			break;
			}

		error( EI_Full, "Can't fdopen %s", fn );
		return Err;
#else
		break;
#endif
		}

	log( "n", "Saving article to %s", fn );
	fprintf( stderr, "Saving article to %s\n", fn );

	in.seekg( 0 );

	const	bl = (16*1024);
	char	buf[bl];
	int		l;

	while( (l = in.read( buf, bl )) > 0 )
		{
		for( int i = l; i; i-- )
			buf[i] = r.rc( buf[i] );

#if 0
		if( fwrite( buf, 1, l, fp ) != 1 )
			{
			error( EI_Full, "Error writing output file" );
			break;
			}
#else
		if( write( fd, buf, l ) != l )
			{
			error( EI_Full, "Error writing output file" );
			break;
			}
#endif
		}

	if( in.ferr() )
		{
		error( EI_Full, "Error reading article file" );
		fclose( fp );
		return Err;
		}

#if 0
	if( ferror( fp ) )
		{
		error( EI_Full, "Error writing output file" );
		fclose( fp );
		return Err;
		}

	if( fclose( fp ) )
		{
		error( EI_Full, "Error closing output file" );
		return Err;
		}
#else
	if( close( fd ) < 0 )
		{
		error( EI_Full, "Error closing output file" );
		return Err;
		}
#endif
	add_stat_news();                            // Increase statistics counter
	return Ok;
	}



@


1.2
log
@Buffers extended, diagnostics added.
@
text
@d8 3
d37 1
a37 1
save_to_file( VReadBase &in, uuinfo &, echo_def &edef, bool )
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: SendNews.C $
d38 1
a48 1
		int fd;
d62 1
d65 2
d68 1
d72 3
d82 1
a82 1
	const	bl = 8192;
d91 10
a100 1
		if( fwrite( buf, l, 1, fp ) != l )
d102 2
d113 1
d126 7
a132 1

@
