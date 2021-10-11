/**--------------------   Graphics Windows   ------------------**\
 *                                                              
 *     Module : Graphic windows tools, text tools.              
 *                                                              
 *	$Log:   C:/c/gw/vcs/tools.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:19:48   dz
 *      Initial revision.
 *
 *	Revision 1.2    9 Mar 90				
 *		Corrections related to proportional		
 *		fonts.						
 *								
 *	Revision 1.1     10.07.89				
 *		Initial revision.				
 *                                                              
 *                                                              
\**------------------------------------------------------------**/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "graphics.h"
#include "local.h"

#include "font.h"
#include "mouse.h"


static bool
check_xy( int x, int y ) {
	if( x > Xend || y > Yend )	return Err;
	else				return Ok;
	}

/**Doc************************************************************************
 *	Function: bool gwborder()      		Header: gw.h
 *
 *	Parameters:
 *		int			width	- Border width, points.
 *		char			color	- Border color.
 *
 *	Return value on error:		Err
 *
 *	Normal return value:		Ok
 *
 *	Description:
 *		Draws a border IN the current window.
 *
**/


bool
gwborder( int width, char color ) {
	register	i;

	assert_init;
	assert_xy( width, width );

	mouse_off();

	setcolor( color );

	for( i = 0; i < width; i++ ) {
		moveto( 0+i , 0+i );
		lineto( Xend-i-1, 0+i );
		lineto( Xend-i-1, Yend-i-1 );
		lineto( 0+i, Yend-i-1 );
		lineto( 0+i, 0+i );
		}

	mouse_on();
	return Ok;
	}



/**Doc************************************************************************
 *	Function: bool gwborder3d()    		Header: gw.h
 *
 *	Parameters:
 *		int			bwidth	- Border width, points.
 *		char			bcolor	- Border color.
 *		int			swidth	- Side width.
 *		char			scolor	- Side color.
 *		char			ccolor	- Background ( clear ) color.
 *
 *	Return value on error:		Err
 *
 *	Normal return value:		Ok
 *
 *	Description:
 *		Draws a 3-dimensional border IN the current window.
 *
**/


bool
gwborder3d( int bwidth, char bcolor, int swidth, char scolor, char ccolor ) {
	register	i;

	assert_init;
	assert_xy( bwidth, bwidth );
	assert_xy( swidth, swidth );

	mouse_off();

	setcolor( scolor );			/* Draw a shadow	*/
	for( i = 0; i < swidth; i++ ) {
		moveto( Xend-swidth+i, i+1 );
		lineto( Xend-swidth+i, Yend-swidth+i );
		lineto( i, Yend-swidth+i );
		}

	setcolor( bcolor );			/* Stripes on a shadow	*/
	moveto( Xend-swidth-1, 0 );		/* Upper right		*/
	lineto( Xend-1, swidth );		/* *			*/
	lineto( Xend-1, Yend-1 );		/* * Back line		*/

	moveto( Xend-1-swidth, Yend-1-swidth );	/* Lower right		*/
	lineto( Xend-1, Yend-1 );
	lineto( swidth, Yend-1 );		/* Back line		*/

	moveto( 0, Yend-1-swidth );		/* Lower left		*/
	lineto( swidth, Yend );


	setfillstyle( SOLID_FILL, ccolor );	/* Fill workspace	*/
	bar( 0, 0, Xend-1-swidth, Yend-1-swidth );



	setcolor( bcolor );			/* Draw a border	*/

	for( i = 0; i < bwidth; i++ ) {
		moveto( 0+i , 0+i );
		lineto( Xend-i-1-swidth, 0+i );
		lineto( Xend-i-1-swidth, Yend-i-1-swidth );
		lineto( 0+i, Yend-i-1-swidth );
		lineto( 0+i, 0+i );
		}


	mouse_on();
	return Ok;
	}











bool
gwshadow( int bwidth, char bcolor, int swidth, char scolor, char ccolor ) {
	register	i;

	assert_init;
	assert_xy( bwidth, bwidth );
	assert_xy( swidth, swidth );

	mouse_off();

	setcolor( scolor );
	for( i = 0; i < swidth; i++ ) {
		moveto( 0+i+swidth, 0+i+swidth );
		lineto( Xend-i-1, 0+i+swidth );
		lineto( Xend-i-1, Yend-i-1 );
		lineto( 0+i+swidth, Yend-i-1 );
		lineto( 0+i+swidth, 0+i+swidth );
		}

	setfillstyle( SOLID_FILL, ccolor );
	bar( 0, 0, Xend-1-swidth, Yend-1-swidth );



	setcolor( bcolor );

	for( i = 0; i < bwidth; i++ ) {
		moveto( 0+i , 0+i );
		lineto( Xend-i-1-swidth, 0+i );
		lineto( Xend-i-1-swidth, Yend-i-1-swidth );
		lineto( 0+i, Yend-i-1-swidth );
		lineto( 0+i, 0+i );
		}


	mouse_on();
	return Ok;
	}






/**Doc************************************************************************
 *	Function: bool gwclear()    		Header: gw.h
 *
 *	Parameters:
 *		char			ccolor	- Background ( clear ) color.
 *
 *	Return value on error:		Err
 *
 *	Normal return value:		Ok
 *
 *	Description:
 *		Clears current window.
 *
**/



bool
gwclear( char color ) {
	assert_init;

	mouse_off();

	setfillstyle( SOLID_FILL, color );
	bar( 0, 0, Xend-1, Yend-1 );

	mouse_on();

	return Ok;
	}



/************************************************
 *	Text management functions.
**/

static char	text_back_color = BLACK;
static char	text_for_color  = WHITE;

extern font	_8x16t_font;

#define	st_width( st )	((st).s_font->hsize)
#define st_height( st )	((st).s_font->vsize)

#define st_lwidth( l )	(st_width(def_style) * strlen( (l)) )
#define st_lheight( l )	(st_height(def_style))


cstyle	def_style = {
	&_8x16t_font, LIGHTGRAY,
	RED|LIGHTGREEN,1,1,	/* SHcol, SHh, SHv	*/
	1,		/* Bold factor		*/
	1,		/* Italic factor	*/
	0,0,		/* Udist, Uheight	*/
	0,0,		/* Ocol, Othick		*/
	0,		/* Back color */
	{0},
	1,0,0,0,0,	/* S, B, I, U, O	*/
	0,		/* Copy = 1		*/
	0		/* Background fill	*/
	};

/**Doc************************************************************************
 *	Function: gwtextattr()      		Header: font.h
 *
 *	Parameters:
 *		unsigned		attr	- Text color/back.
 *
 *	Return value:			None.
 *
 *	Description:
 *		Sets default text color/background.
 *
**/


void
gwtextattr( unsigned color ) {
	def_style.b_color = text_back_color = ((unsigned)color >> 4) & 0x0F;
	def_style.c_color = text_for_color = ((unsigned)color) & 0x0F;
	}

void
gwtextback( unsigned color ) {
	def_style.b_color = text_back_color = ((unsigned)color) & 0x0F;
	}

void
gwtextcolor( unsigned color ) {
	def_style.c_color = text_for_color = ((unsigned)color) & 0x0F;
	}


/**Doc************************************************************************
 *	Function: gwtextfill()      		Header: font.h
 *
 *	Parameters:
 *		bool			fill	- Background fill flag.
 *
 *	Return value:			None
 *
 *	Description:
 *		Turns background fill for character output
 *		on/off. Affects functions gwputs/gwsputs/
 *		gwprintf/gwsprintf.
 *
**/


void
gwtextfill( bool OnOff ) {
	def_style.b_fill = OnOff;
	}


void
gwsgotoxy( cstyle *style, int x, int y ) {
	moveto( (x-1) * style->s_font->hsize, (y-1) * style->s_font->vsize );
	}

/**Doc************************************************************************
 *	Function: gwsheight()      		Header: font.h
 *
 *	Parameters:
 *		cstyle *		style	- Character style.
 *		char *			text	- Text string.
 *
 *	Return value:			text height in points.
 *
 *	Description:
 *		Calculates height of the given string using
 *		given character style.
 *
 *	Also:
 *		gwheight() - has no 'style' parameter, calculates
 *		height of the string using default style/font.
 *
**/


int
gwsheight( cstyle *style, char *text ) {
	text = text;
	return style->s_font->vsize;
	}

/**Doc************************************************************************
 *	Function: gwswidth()   		Header: font.h
 *
 *	Parameters:
 *		cstyle *		style	- Character style.
 *		char *			text	- Text string.
 *
 *	Return value:			text width in points.
 *
 *	Description:
 *		Calculates width of the given string using
 *		given character style.
 *
 *	Also:
 *		gwwidth() - has no 'style' parameter, calculates
 *		width of the string using default style/font.
 *
**/


int
gwswidth( cstyle *style, char *text ) {
	register unsigned i;

/*	Note:
 *		Character generator doesn't add additional
 *		spaces between shadowed or bold characters,
 *		so we are adding s_xs and b_width to the
 *		size of string, not character.
 *
 *		BUG! Curr. point will be moved to the right
 *		by char_size*strlen, but our size for a
 *		shadowed or bold characters is greater.
 */

	if( style->s_font->prop ) {
		register char	*pp = style->s_font->propptr;

		i = 0;
		while( *text )
			i += pp[*text];

		}
	else {
		i = style->s_font->hsize;
		i *= strlen( text );
		}

	if( style->s_flag )
		i += style->s_xs;

	if( style->b_flag )
		i += style->b_width;

	return i /* 	* strlen( text ) 	*/;
	}


void
gwscenter( cstyle *style, char *text ) {
	moveto( (Xend-gwswidth( style, text ))/2, gety());
	gwsputs( style, text );
	}


void
gwsnl( cstyle *style, int xpos ) {
	moverel( xpos - getx(), style->s_font->vsize );
	}


/**Doc************************************************************************
 *	Function: gwsputs()      		Header: font.h
 *
 *	Parameters:
 *		cstyle *		style	- Character style.
 *		char *			text	- Text string to print.
 *
 *	Return value:			None
 *
 *	Description:
 *		Prints string on the screen using given ( gwsputs )
 *		or default ( gwputs ) character style/font.
 *
**/


#ifndef gwputs
void
gwputs( char *output ) {
	int	x,  y;
	int	xs, ys;

	int	corr = style->s_font->baseline;

	x  = getx();
	y  = gety();

	mouse_off();

	if( def_style.b_fill ) {
		ys = st_lheight( output );
		xs = st_lwidth( output );

		setfillstyle( SOLID_FILL, def_style.b_color );
		bar( x, y - corr, x+xs-1, y+ys-1-corr );
		}

	linegen( x+Xbase, y+Ybase, output, strlen( output ), &def_style );

	mouse_on();
	}
#endif

void
gwsputs( cstyle * style, char *output ) {
	int	x,  y;
	int	xs, ys;

	int	corr = style->s_font->baseline;

	x  = getx();
	y  = gety();

	mouse_off();

	if( style->b_fill ) {
		ys = gwsheight( style, output );
		xs = gwswidth( style, output );

		setfillstyle( SOLID_FILL, style->b_color );
		bar( x, y - corr, x+xs-1, y+ys-1-corr );
		}

	linegen( x+Xbase, y+Ybase, output, strlen( output ), style );

	mouse_on();
	}

/**Doc************************************************************************
 *	Function: gwsprintf()      		Header: font.h
 *
 *	Parameters:
 *		cstyle *		style	- Character style.
 *		char *			format	- Printf format.
 *		...				- According to format.
 *
 *	Return value:			None
 *
 *	Description:
 *		Prints string on the screen using given ( gwsprintf )
 *		or default ( gwprintf ) character style/font.
 *
**/


void
gwprintf( va_list arg_list, ...) {

	va_list arg_ptr;
	char *format;

	char	output[ 80 ];

	va_start(arg_ptr, arg_list);
	format = arg_list;
	vsprintf(output, format, arg_ptr);

	gwputs( output );

	}



void
gwsprintf( cstyle *style, va_list arg_list, ...) {

	va_list arg_ptr;
	char *format;

	char	output[ 80 ];

	va_start(arg_ptr, arg_list);
	format = arg_list;
	vsprintf(output, format, arg_ptr);

	gwsputs( style, output );

	}


