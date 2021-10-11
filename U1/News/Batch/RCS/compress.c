head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	96.05.21.00.21.04;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.03.31.23.04.30;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.07.05.15.02;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.08.02.13.29.41;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.26.07;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@cosmetical changes
@
text
@/************************ UU2 News Batcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Run 'compress' on a batch file
 *
 *      $Log: compress.c $
 *      Revision 1.4  1996/03/31 23:04:30  dz
 *      stdin/stdout now saved/restored
 *
 *      Revision 1.3  1995/08/07 05:15:02  dz
 *      Using App class
 *
 *      Revision 1.2  1995/08/02 13:29:41  dz
 *      freopens... kludges! must be fixed...
 *
 *      Revision 1.1  1995/03/11  18:26:07  dz
 *      Initial revision
 *
 *      Revision 1.1  1995/03/11  18:26:07  dz
 *      Initial revision
 *
 *      Rev 1.1   23 Oct 1992 14:53:12   dz
 *      error()
 *
 *      Rev 1.0   27 Apr 1992 16:21:06   dz
 *      Initial revision.
 *
 *
\*/

#include    "batch.h"
#include    "..\..\version.h"
#include    <process.h>

#define INCL_DOSQUEUES   /* Queue values */
#include <os2.h>


// NB!!!
// Some decompressors need -i parameter to compress
// in 'image' mode. Think, I need one more conf. file parameter.



bool BatchApp::compress( bool m12, int in_fd, int out_fd )
    {
    int     ret;
    
    debug( "Compressing in %d bit mode", m12 ? 12 : 16 );
    
    int stdin_save  = dup( 0 );
    int stdout_save = dup( 1 );
    
    if( stdin_save < 0 || stdout_save < 0 )
        {
        error( EI_Full, "Can't save stdin/out" );
        return Err;
        }
    
    DosSetFHState( stdin_save,  OPEN_FLAGS_NOINHERIT );
    DosSetFHState( stdout_save, OPEN_FLAGS_NOINHERIT );
    
    if( dup2( in_fd, 0 ) < 0 )
        {
        error( EI_Full, "Can't dup fd to stdin" );
        return Err;
        }

    if( dup2( out_fd, 1 ) < 0 )
        {
        error( EI_Full, "Can't dup fd to stdout" );
        return Err;
        }

    if( m12 )
        ret = spawnlp( P_WAIT, "compress", "compress.exe", "-b", "12", NULL );
    else
        ret = spawnlp( P_WAIT, "compress", "compress.exe", NULL );

    if( ret != 0 )
        error( EI_Full, "Can't run compressor" );

    close( 0 );
    if( dup2( stdin_save, 0 ) < 0 )
        {
        error( EI_Full, "Can't dup stdin_save to stdin" );
        return Err;
        }
    close( stdin_save );


    close( 1 );
    if( dup2( stdout_save, 1 ) < 0 )
        {
        error( EI_Full, "Can't dup stdout_save to stdout" );
        return Err;
        }
    close( stdout_save );

    return ret == 0 ? Ok : Err;
    }


@


1.4
log
@stdin/stdout now saved/restored
@
text
@d8 3
d22 2
a23 2
 *      
 *         Rev 1.1   23 Oct 1992 14:53:12   dz
d25 2
a26 2
 *      
 *         Rev 1.0   27 Apr 1992 16:21:06   dz
@


1.3
log
@Using App class
@
text
@d8 3
d14 2
a15 5
 * Revision 1.1  1995/03/11  18:26:07  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:26:07  dz
 * Initial revision
d17 2
d31 5
a35 1
#include	<process.h>
d43 54
a96 30
bool
BatchApp::compress( bool m12, int in_fd, int out_fd )
	{
	int     ret;

	debug( "Compressing in %d bit mode", m12 ? 12 : 16 );

	if( dup2( in_fd, 0 ) < 0 )
		error( EI_Full, "Can't dup fd to stdin" );
	else
	if( dup2( out_fd, 1 ) < 0 )
		error( EI_Full, "Can't dup fd to stdout" );
	else
		{
		if( m12 )
			ret = spawnlp( P_WAIT, "compress", "compress.exe", "-b", "12", NULL );
		else
			ret = spawnlp( P_WAIT, "compress", "compress.exe", NULL );
		}

	if( ret != 0 )
		error( EI_Full, "Can't run compressor" );

	close( 0 );
//	freopen( "CON", "rt", stdin );
	freopen( "CON", "r", stdin );

	close( 1 );
//	freopen( "CON", "wt", stdout );
	freopen( "CON", "w", stdout );
d98 2
a99 2
	return ret == 0 ? Ok : Err;
	}
@


1.2
log
@freopens... kludges! must be fixed...
@
text
@d8 3
d38 1
a38 1
compress( bool m12, int in_fd, int out_fd )
@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/news/batch/vcs/compress.c_v  $
d58 2
a59 1
	freopen( "CON", "rt", stdin );
d62 2
a63 1
	freopen( "CON", "wt", stdout );
@
