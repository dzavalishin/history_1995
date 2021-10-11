/************************ UUCP to FIDO gate ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Find or create echo packet
 *
 *      $Log: Open_PKT.c $
 *      Revision 1.5  1995/08/07 14:55:01  dz
 *      Using class App
 *
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
