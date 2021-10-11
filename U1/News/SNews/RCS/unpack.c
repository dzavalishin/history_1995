head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	95.08.07.13.57.10;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.07.13.28.26;	author dz;	state Exp;
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


1.4
log
@Cleanup
@
text
@/************************ UU2 Snews ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	Echo packet decomposition
 *
 *      $Log: unpack.c $
 *      Revision 1.3  1995/08/07 13:28:26  dz
 *      Using class App
 *
 * Revision 1.2  1995/04/09  23:24:27  dz
 * IBM C Set version
 *
 * Revision 1.1  1995/03/11  18:31:14  dz
 * Initial revision
 *
 *      
 *         Rev 1.3   07 Jun 1993 17:11:22   dz
 *      Copyright
 *      
 *         Rev 1.2   23 Oct 1992 14:57:32   dz
 *      error()
 *      
 *         Rev 1.1   18 Jun 1992 11:09:32   dz
 *      update
 *      
 *         Rev 1.0   06 Oct 1991 00:19:26   dz
 *      Initial revision.
 *
 *
 *
\*/

#include	"snews.h"
#include	"..\..\version.h"
#include	"fido_pkt.h"

// NB!!
// Add file locking!


bool
SnewsApp::do_unpack( FILE *fp )
	{
	fido_pkt_hdr	ph;

	if( fread( &ph, sizeof( ph ), 1, fp ) != 1 )
		{
		error( EI_Full, "Can't read packet header" );
		return Err;
		}

	if( ph.pkt_type != 2 )
		{
		error( EI_None, "Unknown packet type" );
		return Err;
		}




	while( !feof( fp ) )
		{
		char	b2[2];

		if( fread( b2, 2, 1, fp ) != 1 )
			{
			error( EI_Full, "Can't read packet delimiter" );
			return Err;
			}

		if( b2[0] == 0 && b2[1] == 0 )
			{
			debug("Normal end of packet");
			break;
			}

		if( send( fp ) == Err )
			return Err;

		}


	return Ok;
	}




	/*************************************************************
			   Allocate buffer for file
	*************************************************************/


bool
SnewsApp::unpack( String pkt_name )
	{
	FILE	*fp;
	bool	rc;

	printf("Unpacking %s\n", (const char *)pkt_name );

	if( (fp = fopen( (const char *)pkt_name, "rb" )) == NULL )
		error( EI_Full, "Can't open echo packet '%s'", (const char *)pkt_name );
	else
		{
		setvbuf( fp, NULL, _IOFBF, 16384 );

		rc = do_unpack( fp );
		if( rc == Err )
			error( EI_None, "Can't unpack '%s'", (const char *)pkt_name );

		fclose( fp );
		}

	return rc;
	}
@


1.3
log
@Using class App
@
text
@d8 3
a94 2
//#define     FBUFSIZE        (12*1024)

a95 1
//SnewsApp::unpack( char *pkt_name )
@


1.2
log
@IBM C Set version
@
text
@d8 3
d39 2
a40 2
static bool
do_unpack( 	FILE *fp )
d95 2
a96 1
unpack( char *pkt_name )
d101 1
a101 1
	printf("Unpacking %s\n", pkt_name );
d103 2
a104 2
	if( (fp = fopen( pkt_name, "rb" )) == NULL )
		error( EI_Full, "Can't open echo packet '%s'", pkt_name );
d111 1
a111 1
			error( EI_None, "Can't unpack '%s'", pkt_name );
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/snews/vcs/unpack.c_v  $
a93 2
//    void    *buf;
//    int     bs;
a98 7
//    buf = malloc( FBUFSIZE );
//    if( buf == NULL ) {
//        fatal( 3, "Unpack: Out of memory!" );
//        }
//    else
//        bs = FBUFSIZE;

d103 2
a110 3

//    if( bs == FBUFSIZE )
//        free( buf );
@
