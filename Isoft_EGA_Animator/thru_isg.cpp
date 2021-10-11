/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:	Look-thru : invisible points filler
 *
 *      $Log:   D:/animate/vcs/thru_isg.cpv  $
 *      
 *         Rev 1.6   09 Jan 1991 06:45:00   dz
 *      Checkpoint.
 *      
 *         Rev 1.5   18 Dec 1990 02:26:10   dz
 *      Assembly language optimization
 *      
 *         Rev 1.4   17 Dec 1990 09:03:12   dz
 *      checkpoint
 *      
 *         Rev 1.3   11 Dec 1990 06:18:42   dz
 *      Bug fix: frg_ystart calculation.
 *      
 *         Rev 1.2   11 Dec 1990 05:34:50   dz
 *      Rewritten.
 *      
 *         Rev 1.1   11 Dec 1990 02:16:46   dz
 *      SpeedUp, some bug exist
 *      
 *         Rev 1.0   08 Dec 1990 01:12:08   dz
 *      Initial revision.
 *
 *
\*/

#pragma inline

#include	<style.h>
#include	<stdlib.h>

#include	"animate.h"

#define	HUGE_PIC	1




inline int
min( int a, int b ) {
	return (a) > (b) ? b : a;
	}

inline int
max( int a, int b ) {
	return (a) > (b) ? a : b;
	}

/****************************************************************************



	 fx1                              fx2
      fy1ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   bx1   ³                           bx2     ³
by1ÚÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿    ³
   ³     ³                              ³    ³
   ³  fy2ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÙ
   ³                                    ³
   ³                                    ³      bx,by  - BackGround
   ³                                    ³      Origin = (bx1,by1)
   ³                                    ³
   ³                                    ³
   ³                                    ³
   ³                                    ³
by2ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

			
****************************************************************************/


/////////////////////////////////////////////////////////////////////////////
//
//	Fill invisible points of foreground
//	with corresponding ones from given background.
//
//	This implementation works well even when
//	two given pictures is not overlapped or
//	overlapped just partially.
//
//	(sx,sy)	- displacement of foreground
//		  picture from the background.
//
//	bkg	- background
//


void
image::thru( image &bkg, int sx, int sy ) {
	int		bkg_xs		= bkg.xs;
	int		bkg_ys		= bkg.ys;
	Summpl huge	*bkg_data	= bkg.data;

	int		frg_xs		= xs;
	int		frg_ys		= ys;
	Summpl huge	*frg_data	= data;
	Summpl huge	*bkgp, *frgp;

//	int		bkg_xe		= bkg_xs;
//	int		bkg_ye		= bkg_ys;

#define			bkg_xe		bkg_xs
#define			bkg_ye		bkg_ys


	int		frg_xe		= sx+frg_xs;
	int		frg_ye		= sy+frg_ys;

	// Relative to the bkg left upper corner
	uint		bkg_xstart	= sx < 0 ? 0 : sx; 
	uint		bkg_ystart	= sy < 0 ? 0 : sy;

	// Relative to the frg left upper corner
	uint		frg_xstart	= sx > 0 ? 0 :-sx;
	uint		frg_ystart	= sy > 0 ? 0 :-sy;

	int		ovr_xs		= min( bkg_xe, frg_xe ) - bkg_xstart;
	int		ovr_ys		= min( bkg_ye, frg_ye ) - bkg_ystart;

	uint		bkg_jump	= bkg_xs - ovr_xs;
	uint		frg_jump	= frg_xs - ovr_xs;


	bkgp	= bkg_data + bkg_ystart * bkg_xs + bkg_xstart;
	frgp	= frg_data + frg_ystart * frg_xs + frg_xstart;

	asm	cld

	asm	push	ds

	asm	les	di, frgp
	asm	lds	si, bkgp

	asm	mov	cx, ovr_ys
	asm	cmp	cx, 0			// Pictures isn't overlap ?
	asm	jle	skip_Y_loop		// Signed compare
Y_loop:
	asm	push	cx

	asm	mov	cx, ovr_xs
	asm	cmp	cx, 0
	asm	jle	skip_X_loop		// Signed compare
X_loop:
	asm	push	cx

//	if( frgp->invisible )
//		(frgp++)->color = (bkgp++)->color;
//	else {
//		bkgp++;
//		frgp++;
//		}

	asm	mov	ah, [es:di]	// ah = *frgp
	asm	test	ah, 01000000b	// if( frgp->invisible )
	asm	jz	visible_point

	asm	and	ah, 11000000b	// Clear color value

	asm	lodsb			// al = *(bkgp++)
	asm	and	al, 00111111b   // Strip invisibility bit
	asm	or	al, ah
	asm	stosb			// *(frgp++) = al

	asm	jmp	repeat
visible_point:
	
	asm	inc	si
	asm	inc	di

repeat:


#if HUGE_PIC		// Huge pictures

	//	Check DS:SI

	asm	test	si, 0FFF0h	// Offset got too far ?
	asm	jz	si_ok		// No, skip it

	asm	mov	ax, si		// Get copy of ofsset value
	asm	and	si, 0000Fh	// Strip out hi bits of offset

	asm	mov	cl, 4		// Shift copy of offset
	asm	shr	ax, cl		// 4 bits right ( divide by 16 )

	asm	mov	bx, ds		// Get segment
	asm	add	ax, bx		// Advance it
	asm	mov	ds, ax		// Store it back

si_ok:
	//	Check ES:DI

	asm	test	di, 0FFF0h
	asm	jz	di_ok

	asm	mov	ax, di
	asm	and	di, 0000Fh

	asm	mov	cl, 4
	asm	shr	ax, cl

	asm	mov	bx, es
	asm	add	ax, bx
	asm	mov	es, ax


di_ok:

#endif			// Huge pictures

	asm	pop	cx
	asm	loop	X_loop
skip_X_loop:

	_DI += frg_jump;
	_SI += bkg_jump;

	asm	pop	cx
	asm	loop	Y_loop
skip_Y_loop:

	asm	pop	ds

	}







