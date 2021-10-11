head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	95.08.07.14.55.01;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.08.02.13.43.25;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.04.09.18.25.15;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.09.10.45.11;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.32.26;	author dz;	state Exp;
branches;
next	;


desc
@UU2 News Unbatcher
@


1.5
log
@Using class App
@
text
@/************************ UUCP to FIDO gate ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Find or create echo packet
 *
 *      $Log: Open_PKT.c $
 *      Revision 1.4  1995/08/02 13:43:25  dz
 *      rewritten with buffered file interface
 *
 * Revision 1.3  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.3  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.2  1995/04/09  10:45:11  dz
 * rewriting for C Set
 *
 * Revision 1.1  1995/03/11  18:32:26  dz
 * Initial revision
 *
 *         Rev 1.0   08 Nov 1991 23:15:12   dz
 *      Initial revision.
 *
 *
 *
\*/

#include	"unbatch.h"
#include	"fido_pkt.h"
#include	"..\..\version.h"
#include	"..\..\u1.h"

#include	<stdio.h>
#include	<uu2io.h>
#include	<share.h>
#include	<fcntl.h>


void	fill_ph_date( pkt_date &date );



static FILE *
new_packet_file( void )
	{
	char			name[200];
	int				fd;
	fido_pkt_hdr	ph;

	// Create new packet

	debug("Creating new packet");

	while( 1 )
		{
		int				i;
		long			t;
		static int		seq;

		time( &t );
		i = (int)(t % 10000) + (seq++);

		sprintf( name, "%.120s\\news%04d.pkt", (const char *)conf.netfile_dir.c_str(), i );

		fd	= _sopen( name, O_RDWR|O_CREAT|O_EXCL|O_BINARY, SH_DENYRW, 0666 );

		if( fd >= 0 )	// Opened!
			break;

		if( errno != EEXIST )
			return NULL;
		}


	// Fill packet header

	ph.origNode		= conf.echo_from_addr.node();
	ph.destNode		= conf.echo_to_addr.node();
	ph.origNet		= conf.echo_from_addr.net();
	ph.destNet		= conf.echo_to_addr.net();
	ph.origZone		= conf.echo_from_addr.zone();
	ph.destZone		= conf.echo_to_addr.zone();
	ph._origZone	= conf.echo_from_addr.zone();
	ph._destZone	= conf.echo_to_addr.zone();
	ph.origPoint	= conf.echo_from_addr.point();
	ph.destPoint	= conf.echo_to_addr.point();

	ph.baud				= 0;
	ph.pkt_type			= 2;
	ph.lo_product_code	= UU2_PROD_CODE;							// (D'Bridge = 0x1a)
	ph.hi_product_code	= 0;							//
	ph.lo_version_no	= U1_VER_NUM & 0xFFu;			// Version of
	ph.hi_version_no	= (U1_VER_NUM >> 8) & 0xFFu;	// product

	ph.ProdData			= 0;							// Nothing to say :)
	ph.CapWord			= 0x0001;						// Type 2 CW
	ph.CapValid			= 0x0100;						// SWAB


	memset( ph.password, '\0', 8 );					// Zero-fill unused
	memset( ph.fill, '\0', sizeof(ph.fill) );		// fields

	fill_ph_date( ph.date );

	FILE *fp = fdopen( fd, "wb" );
	if( fp == NULL )
		{
		error( EI_Full, "new_packet_file: can't fdopen" );
		close( fd );
		return NULL;
		}

	fseek( fp, 0L, SEEK_SET );
	if( fwrite( &ph, 1, sizeof( ph ), fp ) != sizeof( ph ) )
		{
		error( EI_Full, "new_packet_file: write error" );
		close( fd );
		return NULL;
		}


	return fp;
	}


static  FILE *	pf_fp	= NULL;

extern "SYSTEM" void
close_packet_file( void )
	{
	int		i = 0;

	if( pf_fp == 0 )
		return;

	if( fwrite(  &i, 1, 2, pf_fp ) != 2 )	// Write out end-of-packet marker
		error( EI_Full, "Can't write End-Of-Packet marker" );

	if( fclose( pf_fp ) != 0 )			// Ok, done.
		error( EI_Full, "Can't close packet file" );

	pf_fp = 0;							// Free.

	}

//#pragma exit close_packet_file


FILE *
UnbatchApp::get_packet_file( void )
	{

	static int do_close = 0;

	if( do_close == 0 )
		{
		do_close = 1;
		atexit( close_packet_file );
		}


	if( pf_fp != 0 && ftell( pf_fp ) > conf.pkt_size )	// Too big?
		close_packet_file();								// Ok, close it.

	if( pf_fp == 0 )											// Need new one?
		{
		pf_fp = new_packet_file();							// Create.
		return pf_fp;
		}

	return pf_fp;											// Ok, we have.
	}




        /*************************************************************
                        Get system date/time & initialize
                          packet date structure with it.
        *************************************************************/


void
fill_ph_date( pkt_date &date )
	{
	long		t;
	struct tm	*tb;

	time( &t );
	tb = localtime( &t );

	date.year	= (short)tb->tm_year;
	date.month	= (short)tb->tm_mon;
	date.day	= (short)tb->tm_mday;
	date.hour	= (short)tb->tm_hour;
	date.minute	= (short)tb->tm_min;
	date.second	= (short)tb->tm_sec;
	}
@


1.4
log
@rewritten with buffered file interface
@
text
@d8 3
a34 1
//#include	<dir.h>
d152 1
a152 1
get_packet_file( void )
@


1.3
log
@IBM C Set version. Phew, it was not a piece of cake to get here...
@
text
@d8 6
d33 1
d43 1
a43 1
static int
d71 1
a71 1
			return -1;
d105 2
a106 2
	lseek( fd, 0L, SEEK_SET );
	if( write( fd, &ph, sizeof( ph ) ) != sizeof( ph ) )
d108 1
d110 1
a110 1
		return -1;
d113 10
a122 1
	return fd;
d126 1
a126 1
static int	pf_fd	= -1;
d133 1
a133 1
	if( pf_fd < 0 )
d136 1
a136 1
	if( write( pf_fd, &i, 2 ) != 2 )	// Write out end-of-packet marker
d139 1
a139 1
	if( close( pf_fd ) != 0 )			// Ok, done.
d142 1
a142 1
	pf_fd = -1;							// Free.
d149 1
a149 1
int
d162 1
a162 1
	if( pf_fd >= 0 && filelength( pf_fd ) > conf.pkt_size )	// Too big?
d165 1
a165 1
	if( pf_fd < 0 )											// Need new one?
d167 2
a168 2
		pf_fd = new_packet_file();							// Create.
		return pf_fd;
d171 1
a171 1
	return pf_fd;											// Ok, we have.
d177 4
a180 4
	/*************************************************************
				 Get system date/time & initialize
				   packet date structure with it.
	*************************************************************/
@


1.2
log
@rewriting for C Set
@
text
@d8 3
a13 22
 *      
 *         Rev 1.4   03 Mar 1993 20:34:02   dz
 *      Portability issues... 
 *      
 *         Rev 1.3   28 Nov 1992 22:37:34   dz
 *      String to const char *, style fixes
 *      
 *         Rev 1.2   23 Oct 1992 15:07:36   dz
 *      error()
 *      
 *         Rev 1.1   29 Jun 1992 21:50:32   dz
 *      Multi-message packets
 *      
 *         Rev 1.0   17 May 1992 11:06:28   dz
 *      Internal packetizer, kiae-specific unbatcher
 *      
 *         Rev 1.2   07 Apr 1992 12:32:02   dz
 *      Own product code
 *      
 *         Rev 1.1   03 Feb 1992 23:57:26   dz
 *      Type 2+ packets
 *      
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/unbatch/vcs/open_pkt.c_v  $
d45 1
a45 1
#include	<dir.h>
d47 1
d77 1
a77 1
		fd	= open( name, O_RDWR|O_CREAT|O_EXCL|O_BINARY|O_DENYALL, 0666 );
d130 1
a130 1
void
d148 1
a148 1
#pragma exit close_packet_file
d154 9
@
