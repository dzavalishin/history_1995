/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:	Load ISG picture.
 *
 *      $Log:   D:/animate/vcs/load_isg.cpv  $
 *      
 *         Rev 1.6   09 Jan 1991 06:42:22   gn
 *      getc() instead of fread()
 *      
 *         Rev 1.5   24 Dec 1990 08:15:52   gn
 *      Buffered unpack.
 *      
 *         Rev 1.4   24 Dec 1990 02:50:40   gn
 *      Squeezing (Byte repeat compress) added.
 *      
 *         Rev 1.3   23 Dec 1990 12:06:20   gn
 *      Added disk buffer in function LOAD_IMAGE_DATA (for speed only).
 *      
 *         Rev 1.2   18 Dec 1990 02:25:08   dz
 *      Correction of error messages.
 *      
 *         Rev 1.1   11 Dec 1990 06:21:32   dz
 *      Bug fix by [gn] - faralloc instead of new,
 *      Optimization of file read loop made.
 *      
 *         Rev 1.0   05 Dec 1990 00:44:02   gn
 *      Initial revision.
 *      
 *
\*/

#include	<style.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<alloc.h>


#include	"isg_fmt.h"
#include	"animate.h"
#include	"pack.h"

#define		DISK_BUFFER_SIZE	(12*1024)	// 12K for buffer.

static		byte	*Disk_buffer;
static		bool	packed = No;


static void
error( char *s1, char *s2 ) {
	printf( "Loader:\t%s%s\n", s1, s2 );
	}



static	bool
check_file( FILE *fp, long &offset ) {
	FHeader	h;


	fseek( fp, 0L, SEEK_SET );
	if( fread( &h, sizeof( FHeader ), 1, fp ) != 1 )
		return	Err;

	if( strncmp( h.magic, "ISGF", 4 ) )
		return	Err;

	offset  = h.first;

	if( h.method == 0x01 )		// Byte repeater
		packed = Yes;
	else
		packed = No;

        return	Ok;
	}


static	bool
load_image_header( FILE *fp, long &offset, image &i ) {
	_Block	b;
	PHeader	p;

	fseek( fp, offset, SEEK_SET );

	if( fread( &b, sizeof( _Block ), 1, fp ) != 1 )
		return	Err;

	if( b.btype != Header )
		return	Err;

	offset = b.bnext;
	if( fread( &p, sizeof( PHeader ), 1, fp ) != 1 )
		return	Err;

	i.xs 	= p.xsize;
	i.ys 	= p.ysize;
	i.data  = NULL;
	return	Ok;
	}


static	FILE	*fp_in;


static	int
Make_sym( void ) {
#if 0
	int	t;

	if( fread( &t, 1, 1, fp_in ) != 1 )
		return	EOF;

	return	( (int) t );
#else
	
	return	getc( fp_in );
#endif
	}

static	bool
load_image_data( FILE *fp, long offset, image &i ) {
	_Block	b;
	char huge	*bpp;
	long		bps;
	long		size;
	Unpack		u( Make_sym );

	fseek( fp, offset, SEEK_SET );

	if( fread( &b, sizeof( _Block ), 1, fp ) != 1 )
		return	Err;

	if( b.btype != All_planes )
		return	Err;

	Disk_buffer	= new byte[DISK_BUFFER_SIZE];
	if( Disk_buffer != NULL ) {
		setvbuf( fp, Disk_buffer, _IOFBF, DISK_BUFFER_SIZE );
		}


	fp_in = fp;

	size	= b.bsize;
	bpp	= (char huge*)i.data;

	if( packed == No ) {
		while( size > 0 ) {

			bps = size % (32*1024);
			if( bps == 0 )
				bps = size < 1000 ? size : 1000;

			size -= bps;

			if( fread( bpp, (uint)bps, 1, fp ) != 1 ) {
				if( Disk_buffer != NULL )
					delete	Disk_buffer;

				return	Err;
				}

			bpp += bps;
			}
		}
	else {
		if( u.Do( ((byte huge *) bpp), size ) == Err ) {
			if( Disk_buffer != NULL )
				delete	Disk_buffer;

			return	Err;
			}

		}		// if( packed )

	if( Disk_buffer != NULL )
		delete	Disk_buffer;

	return	Ok;
	}




bool
image::load( char *name ) {
	FILE	*fp;
	long	offset;

	if( (fp = fopen( name, "rb" )) == NULL ) {
		error( "Can't open file ", name );
		return	Err;
		}

	if( check_file( fp, offset ) == Err ) {
		error( "Illegal file format!", "" );
		return	Err;
		}

	if( load_image_header( fp, offset, *this ) == Err ) {
		error( "Can't read file ", name );
		return	Err;
		}

	remove();

	if( ( data = (Summpl huge *) farmalloc( ((long)xs)*((long)ys) )) == NULL ) {
		error( "Out of memory!", "" );
		return	Err;
		}

	if( load_image_data( fp, offset, *this ) ) {
		error( "Can't read file ", name );
		return	Err;
		}

	return	Ok;
	}



void
image::remove( void ) {
	if( data )	farfree( data );
	data	= NULL;
	}

