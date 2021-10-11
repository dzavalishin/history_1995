/**Doc**********************************************************************
 *
 *	Function: xorline() -- Draw a XOR'ed line.	Header:	gw.h
 *
 *	$Log:   C:/c/gw/vcs/paintlin.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:19:04   dz
 *      Initial revision.
 *
 *
 *
 *	Parameters:
 *
 *		int		x1	- Start position
 *		int		y1
 *		int		x2	- End position
 *		int		y2
 *		int		color	- Color of the line
 *
 *	Return value:		None.
 *
 *	Description:
 *		This function draws a line on the current display page
 *		from x1/y1 to x2/y2 using given color.
 *
**/

/**
 *
 *	Method:
 *		The variables x and y keep count of when the tracing
 *		point coordinates whould be incremented.  The variable
 *		fplot is used as a flag when a new point is plotted.
 *		The same point is not plotted twice, because a color
 *		value greater than 128 should XOR the current point.
 *
**/

#include <style.h>
#include <dos.h>
#include "gw.h"

#define GRDELTAX   (x2 - x1)
#define GRDELTAY   (y2 - y1)

#define sign( v )	( v==0 ? 0 : ( v > 0 ? 1 : -1 ))


#define grptwrit( x, y, color ) 					\
	putpixel( (x), (y), (color) )		                        \

static 	void	pencil( int x, int y, int xn, int yn, uint col, int dmode );



void
paintline( int x1, int y1, int x2, int y2,int xn, int yn, uchar color , int dmode ) {
	int sdeltax        = sign(GRDELTAX);
	int sdeltay        = sign(GRDELTAY);
	int incx	       = abs(GRDELTAX);
	int incy	       = abs(GRDELTAY);
	int steps	       = max(incx, incy);
	register int xerr  = 0;
	register int yerr  = 0;
	register int fplot = 0;
	int npts	       = 1;
	int i;

	int		x, y;

	/* Initialize the position variable, plot the first point and     */
	/* trace the rest of the line.				      */

	x = x1;
	y = y1;

	pencil( x, y, xn, yn, color, dmode );
	grptwrit( x, y, color );

	for (i = 0;  i <= steps;  i ++) {
		xerr += incx;
		yerr += incy;

		if (yerr > steps) {
			yerr  -= steps;
			y += sdeltay;
			fplot ++;
			}

		if (xerr > steps) {
			xerr  -= steps;
			x += sdeltax;
			fplot ++;
			}

		if (fplot) {	/* Plot only if a new point.		    */
			pencil( x, y, xn, yn, color, dmode );			grptwrit( x, y, color );
			npts  ++;
			fplot --;
			}
		}
	}

static void
pencil( int x, int y, int xn, int yn, uint col, int dmode ) {

	switch( dmode ){
		case 1:
			xorline( x,	y,	x + 15,		y - 15,		col );
			xorline( x + 1,	y,	x + 15 + 1,	y - 15 + 1,	col );
			xorline( x,	y + 1,	x + 15 - 1,	y - 15 - 1,	col );

			xorline( x,	y,	x + 15,		y - 15,		col );
			xorline( x + 1,	y,	x + 15 + 1,	y - 15 + 1,	col );
			xorline( x,	y + 1,	x + 15 - 1,	y - 15 - 1,	col );

			break;
		case 2:
			xorline( xn,yn, x, y, col );

			xorline( xn,yn, x, y, col );

			break;
		case 3:
			delay( 1 );
			break;
		default:
			delay( 1 );
		}
	}
