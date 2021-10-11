/**-------------------   Graphics Windows   -------------------**\
 *                                                              
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728		
 *                                                              
 *	Module : Character generation.                           
 *                                                              
 *	$Log:   C:/c/gw/vcs/chgen.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:19:18   dz
 *      Initial revision.
 *
 *								
 *	   Rev 1.0	22 Oct 89				
 *	Initial revision.				
 *                                                              
 *                                                              
\*********/

#pragma inline
#define EGA_C

#include "local.h"
#include "font.h"
#include "ega.h"
#include <dos.h>


		/* 640 / 8 */
#define	VSTEP	80

static void pascal	near gench( int x, int y, char *ll, int hs, int vs, int istep, uchar op );

void
linegen( int x, int y, char *string, int count, cstyle *style ) {
	char		*sp;
	int		c,i;
	register font	*font = style->s_font;

	int	istep;
	int	copy_op = (style->w_copy) ? 0 : 2;
	int	horsize, versize;
	int	prop;
	char	*fptr;
	char	*pptr;
	int	fbpl;
	int	horstep;

	y -= font->baseline;

	horsize	= font->hsize;
	versize	= font->vsize;
	pptr	= font->propptr;
	fptr	= font->fontptr;
	fbpl	= font->bpl;
	prop	= font->prop;

	istep = style->i_flag ? style->i_step : 0;

	if( horsize > 128 || versize > 128 || horsize < 0 || versize < 0 )
		return;		/* Bad character size, give it up	*/


	_EGA_reset_write_mode();
	_EGA_write_color( style -> c_color );
	_EGA_write_op( 2 );


	sp = string;
	c = count;
	horstep = horsize;
	while( c-- ) {

		if( prop )		/*	Proportional spacing ?	*/
			horstep = pptr[*sp];	/* Yes, get real width	*/

		if( style->b_flag ) {
			for( i = style->b_width; i > 0 ; i-- ) {

/*				gench( x-i, y,	fptr + (fbpl*(*sp)), horsize, versize, istep, copy_op );	/*	Italics		*/

				gench( x+i, y,
					fptr + (fbpl*(*sp)),
					horsize, versize,
					istep, copy_op );	/*	Italics		*/
				}
			}

		if( style->s_flag ) {
			_EGA_write_color( style -> s_color );
			gench( x+style->s_xs, y+style->s_xs, fptr + (fbpl*(*sp)),
				horsize,
				versize,
				istep, 3 );
			_EGA_write_color( style -> c_color );
			}




		gench( x, y,
			fptr + (fbpl*(*sp++)),
			horsize,
			versize,
			istep, copy_op );	/*	Italics		*/

		x += horstep;

		moverel( horstep, 0 );	/*	Move current point	*/

		}

	_EGA_reset_write_mode();
	_EGA_write_op( 0 );
	_EGA_write_color( 0x0F );

	}

/** 
 * NB!
 * 
 * Multy-page mode is inoperative 
 * because of inaccessibility of
 * _GW_apb when DS redirected.
 * This must be fixed in assembly
 * version.
**/




#pragma warn -aus

/*	static void pascal near		*/

#define putbyte( x, vbyte, bb, op ) {					\
/*	register char far	*vp;			*/		\
	char _es		*vp;					\
/*	char			bb = byte;		*/		\
	register unsigned char	sh;					\
/*	register unsigned char	dummy;			*/		\
									\
	sh = (x) & 7;							\
									\
/*	vp = MK_FP( _GW_apb, vbyte + ((x)>>3) );	*/		\
	_AX = /* _GW_apb */ 0xA000;					\
	_ES = _AX;							\
	vp = vbyte + ( (x) >> 3 );					\
									\
	WREGA( EGA_GDC, 3, sh | (op) );					\
	WREGA( EGA_GDC, 8, 0xFFU >> sh );				\
									\
	_AX = (*vp);							\
	*vp = (bb);							\
									\
	if( sh ) {							\
/*		WREGA( EGA_GDC, 3, ((sh-8) & 0x07) | (op) );	*/	\
		WREGA( EGA_GDC, 3, sh | (op) );				\
		WREGA( EGA_GDC, 8, 0xFFU << (8-sh) );			\
		_AX = *++vp;						\
		*vp = (bb);						\
		}							\
	}								\

#pragma warn .aus







static void pascal near
gench( int x, int y, char *let_lay, int xs, int ys, int istep, uchar op ) {

	register	h, v;
	uint		vbyte;

	char _ds	*ll;

#if !defined( __HUGE__ )
	asm push ds
#endif

	h = FP_SEG( let_lay );
	ll = FP_OFF( let_lay );
	_DS = h;

	op = op << 3;

	if( istep ) {
		int	hmax = ((xs-1) / 8) + 1;

		for( v = 0, vbyte = y*VSTEP; v < ys; v++, vbyte +=VSTEP )
			for( h = 0; h < hmax; h++ ) {
				putbyte( (x+(h*8) + (istep * (ys-v-1))), vbyte, (*ll), op );
				ll++;
				}
		}
	else {
		int hmax = ((xs-1)/8+1) << 3;

		for( v = 0, vbyte = y*VSTEP; v < ys; v++, vbyte += VSTEP )
			for( h = 0; h < hmax; h += 8 ) {
				putbyte( (x+h), vbyte, *ll, op );
				ll++;
				}
		}
#if !defined( __HUGE__ )
	asm pop ds
#endif
	}




void
putsymbol( int x, int y, font *f, uchar sym, int op, uchar color ) {

	_EGA_reset_write_mode();
	_EGA_write_color( color );

	gench( x, y,
		(f->fontptr) + (f->bpl * sym ),
			f->hsize,
			f->vsize,
			0, op );


	_EGA_reset_write_mode();
	_EGA_write_op( 0 );
	_EGA_write_color( 0x0F );
	}




void
putbitline( int x, int y, int len, char	*l, int op, int color ) {

	_EGA_reset_write_mode();
	_EGA_write_color( color );


	gench( x, y, l, len, 1, 0, op );

	_EGA_reset_write_mode();
	_EGA_write_op( 0 );
	_EGA_write_color( 0x0F );
	}






