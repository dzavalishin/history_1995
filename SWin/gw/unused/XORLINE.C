/**Doc**********************************************************************
 *
 *	Function: xorline() -- Draw a XOR'ed line.	Header:	gw.h
 *
 *	$Log:   C:/c/gw/vcs/xorline.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:19:02   dz
 *      Initial revision.
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
#include "gw.h"

#include "ega.h"
#include "local.h"


#define GRDELTAX   (x2 - x1)
#define GRDELTAY   (y2 - y1)

#define sign( v )	( v==0 ? 0 : ( v > 0 ? 1 : -1 ))


#define grptwrit( x, y ) {					\
	char _es	*vp;					\
								\
	_AX = _GW_apb;						\
	_ES = _AX;						\
								\
	vp = (char _es *) (( (y) * 80 ) + ( (x) >> 3 ));	\
	_AX = *vp;						\
	*vp = 0x80U >> ( (x) & 7 );				\
								\
	}							\







void
xorline( int x1, int y1, int x2, int y2, uchar color ) {
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


    WREGA( EGA_SEQ, 2, color );	/* Select planes		*/
    WREGA( EGA_GDC, 3, (3 << 3) );	/* Write mode - XOR		*/


    x = x1;
    y = y1;

    grptwrit( x, y );

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
	    grptwrit( x, y );
	    npts  ++;
	    fplot --;
	}
    }

	_EGA_reset_write_mode();	/* Restore EGA state		*/


}


