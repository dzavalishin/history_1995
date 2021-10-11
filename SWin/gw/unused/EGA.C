/*************************   Graphics Windows   *********************\
 *                                                               
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728		 
 *                                                               
 *	Module : EGA direct read/write support.                   
 *                                                               
 *	$Log:   C:/c/gw/vcs/ega.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:20:18   dz
 *      Initial revision.
 *
 *	   Rev 1.0     20.08.89                                 
 *	Initial revision.                                       
 *                                                               
 *                                                               
\**********/

#define	EGA_C

#include "local.h"
#include "ega.h"

#include <alloc.h>
#include <mem.h>

static void pascal	_EGA_set_read_plane( int npage );
static void pascal	_EGA_set_write_plane( int npage );
static void pascal	_EGA_X_boundary( unsigned *xbase, unsigned *xsize );
static unsigned pascal	_EGA_plane_size( unsigned Xsize, unsigned Ysize );
static void pascal	_EGA_norm_fp( char far **pp );



/*********************************************************************
 *	Function :	_EGA_get()
 *
 *	Description :	Reads rectangular part of screen
 *			into the buffer.
 *
**/




bool
_EGA_get(   char far *bufs[4],
			unsigned xb, unsigned yb,
			unsigned xs, unsigned ys ) {

	int			plane;
	register int		y, ymax = yb + ys - 1;
	char far		*bp;
/*	register char far	*vp;	*/


	_EGA_X_boundary( &xb, &xs );

	for( plane = 0; plane < 4; plane++ ) {
		_EGA_set_read_plane( plane );

		bp = bufs[plane];
		_EGA_norm_fp( &bp );

		for( y = yb; y <= ymax; y++ ) {

/*vp = MK_FP( 0xA000, ( y * (640/8) ) + xb );*/
/*movedata( FP_SEG(vp), FP_OFF(vp), FP_SEG(bp), FP_OFF(bp), xs );*/
			movedata( 0xA000, y * (640/8) + xb, FP_SEG( bp ), FP_OFF( bp ), xs );
			bp += xs;

			} /* for y */
		} /* for plane */

	_EGA_reset_write_mode();
	return Ok;
	}


/*********************************************************************
 *	Function : _EGA_xchg()
 *
 *	Description : Makes exchange between given buffer
 *		      and rectangular part of screen.
 *
**/


bool
_EGA_xchg(   char far *bufs[4],
			unsigned xb, unsigned yb,
			unsigned xs, unsigned ys ) {

	int			plane;
	int			y, ymax = yb + ys - 1;
	char far		*bp;
	char far		*pp;
	register char far	*vp;

	/* Buffer for one plane of pixels	*/
/*	static char		planebuf[28000];	defined in LOCAL.H */


	_EGA_X_boundary( &xb, &xs );
	_EGA_reset_write_mode();

	for( plane = 0; plane < 4; plane++ ) {
		_EGA_set_read_plane( plane );
		_EGA_set_write_plane( plane );

		bp = bufs[plane];
		pp = planebuf;

		_EGA_norm_fp( &bp );
		_EGA_norm_fp( &pp );

		movedata( FP_SEG(bp), FP_OFF(bp), FP_SEG(planebuf), FP_OFF(planebuf), 28000 );


		for( y = yb; y <= ymax; y++ ) {

			vp = MK_FP( 0xA000, ( y * (640/8) ) + xb );

			movedata( FP_SEG(vp), FP_OFF(vp), FP_SEG(bp), FP_OFF(bp), xs );
			movedata( FP_SEG(pp), FP_OFF(pp), FP_SEG(vp), FP_OFF(vp), xs );
			bp += xs;
			pp += xs;

			} /* for y */
		} /* for plane */

	_EGA_reset_write_mode();
	return Ok;
	}




/*********************************************************************
 *	Function :	_EGA_put()
 *
 *	Description :	Writes rectangular image onto
 *			the screen.
 *
**/




bool
_EGA_put( char far *bufs[4], unsigned xb, unsigned yb,
			     unsigned xs, unsigned ys ) {

	int			plane;
	register int		y, ymax = yb + ys - 1;
	char far		*bp;
/*	register char far	*vp;	*/


	_EGA_reset_write_mode();
	_EGA_X_boundary( &xb, &xs );

	for( plane = 0; plane < 4; plane++ ) {
		_EGA_set_write_plane( plane );

		bp = bufs[plane];
		_EGA_norm_fp( &bp );

		for( y = yb; y <= ymax; y++ ) {

/*vp = MK_FP( 0xA000, ( y * (640/8) ) + xb );*/
/*movedata( FP_SEG(bp), FP_OFF(bp), FP_SEG(vp), FP_OFF(vp), xs );*/

			movedata( FP_SEG( bp ), FP_OFF( bp ), 0xA000, y * (640/8) + xb, xs );
			bp += xs;

			} /* for y */
		} /* for plane */
	_EGA_reset_write_mode();
	return Ok;
	}











/*********************************************************************
 *	Function :	_EGA_reset_wrie_mode()
 *
 *	Description :	Resets Write logic, rotate count and
 *			write mask to normal values.
 *
**/

void pascal
_EGA_reset_write_mode( void ) {
	WREGA( EGA_GDC, 3, 0 );		/* Rotate / write op.		*/
	WREGA( EGA_GDC, 5, 0 );		/* R/W mode			*/
	WREGA( EGA_GDC, 8, 0xFF );	/* Mask.			*/
	WREGA( EGA_GDC, 7, 0x00 );	/* Compare logic disable.	*/
	WREGA( EGA_SEQ, 2, 0x0F );
	}

void pascal
_EGA_write_op( unsigned op ) {
	WREGA( EGA_GDC, 3, op << 3 );
	}


/*********************************************************************
 *	Function :	_EGA_set_read_plane()
 *
 *	Description :	Selects plane to read from.
 *
 *
**/

static void pascal
_EGA_set_read_plane( int plane ) {
	WREGA( EGA_GDC, 4, plane );
	}


/*********************************************************************
 *	Function :	_EGA_set_write_plane()
 *
 *	Description :	Selects plane to write to.
 *
 *
**/


static void pascal
_EGA_set_write_plane( int plane ) {
	WREGA( EGA_SEQ, 2, (1 << plane));
	}



/*********************************************************************
 *	Function :      _EGA_X_boundary( int *xbase, int *xsize )
 *
 *	Description :	Converts xbase & xsize from pixels to
 *			bytes, also shifts this values to byte
 *			boundaries.
 *
**/


static void pascal
_EGA_X_boundary( unsigned *xbase, unsigned *xsize ) {
	register int	xb, xs;

	xb = *xbase;
	xs = *xsize;

	xs += xb % 8;
	*xsize = xs % 8 ? xs / 8 + 1 : xs / 8;
	*xbase = xb / 8;
	}



/*********************************************************************
 *	Function :	_EGA_plane_size()
 *
 *	Description :	Calculates size of image for one plane.
 *
**/

static unsigned pascal
_EGA_plane_size( unsigned xs, unsigned ys ) {
	return (xs / 8 + 2) * ys;
	}


/*********************************************************************
 *	Function :	_EGA_alloc()
 *
 *	Description :	Allocates a buffer to store image with
 *			given size.
 *
**/

bool
_EGA_alloc( char far *bufs[4], unsigned xs, unsigned ys, uint *planesize_p ) {
	unsigned	is;	/*	Image size, bytes		*/
	register int	plane;	/*	Allocating for ...		*/

	*planesize_p = is = _EGA_plane_size( xs, ys );

	for( plane = 0; plane < 4; plane++ ) {
		bufs[plane] = farmalloc( is );	/* Get buf. for this plane */
		if( bufs[plane] != NULL )	/* Got it ??		*/
			continue;		/* Ok, next ...		*/

		/* No, so free all previously allocated buffers.	*/

		for( plane--; plane >= 0; plane-- )
			farfree( bufs[plane] );

		return Err;				/* Report an error			*/
		}

	return Ok;
	}






/*********************************************************************
 *	Function :	_EGA_free()
 *
 *	Description :	Deallocates image buffer.
 *
**/



bool
_EGA_free( char far *bufs[4] ) {
	register int	plane;

	for( plane = 0; plane < 4; plane++ ) {
		if( bufs[plane] == NULL )
			return Err;

		farfree( bufs[plane] );
		}
	return Ok;
	}





/*********************************************************************
 *	Function :	_EGA_norm_fp()
 *
 *	Description :	Normalize far pointer.
 *
 *
**/





static void pascal
_EGA_norm_fp( char far **pp ) {
	unsigned off, seg;

	off = FP_OFF( *pp );
	seg = FP_SEG( *pp );

	seg += off >> 4;
	off &= 0x0F;

	*pp = MK_FP( seg, off );

	}



void pascal
_EGA_write_color( unsigned char color ) {
	WREGA( EGA_SEQ, 2, color );
	}


