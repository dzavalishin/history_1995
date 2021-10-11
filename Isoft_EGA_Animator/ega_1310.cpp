/*************************   Animate   *********************\
 *                                                               
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728		 
 *                                                               
 *	Module : EGA direct read/write support.                   
 *                                                               
 *	$Log:   D:/animate/vcs/ega.cpv  $
 *      
 *         Rev 1.3.1.0   09 Jan 1991 00:21:38   dz
 *      Doesn't work due to some error in wline.
 *      
 *         Rev 1.3   17 Dec 1990 08:38:04   dz
 *      Parts of code rewritten with assembly
 *      language to improve performance.
 *      
 *         Rev 1.1   11 Dec 1990 06:24:36   dz
 *      Manual optimization.
 *      
 *         Rev 1.0   11 Dec 1990 02:11:12   dz
 *      Initial revision.
 *
 *                                                               
\**********/

#pragma inline

#include	<style.h>
#include	<dos.h>
#include	"animate.h"
#include	"ega.h"



uint	ega::page_shift = 0;	// Displacement from the start of page


/**Doc*****************************************************************
 *	Function :	visual_page
 *
 *	Description :	Set visual page number.
 *
 *
**/


void pascal	
ega::visual_page( uint page_no ) {

	_SI = _DI = 0;				// Force push ...

	asm	push	ds
	asm	mov	ax, 0500h		// Set page no.
	if( page_no )
		_AL = 1;			// 2nd page.

	asm	int	10h			// Video bios
	asm	pop	ds
	}


/**Doc****************************************************************
 *	Function :	active_page
 *
 *	Description :	Set active page number.
 *
 *
**/

void pascal	
ega::active_page( uint page_no ) {
//	ega::page_shift = page_no ? 32*1024 : 0;

	ega::page_shift = page_no ? 16*1024 : 0;
	}





/**Doc****************************************************************
 *	Function :	get_visual_page
 *
 *	Description :	Get visual page number.
 *
 *
**/


uint pascal	
ega::get_visual_page( void ) {

	_SI = _DI = 0;				// Force push ...

	asm	push	ds
	asm	mov	ax, 0F00h		// Get page no.

	asm	int	10h			// Video bios
	asm	pop	ds

	return _BH;
	}





/**Doc****************************************************************
 *	Function :	ega::reset()
 *
 *	Description :	Resets Write logic, rotate count and
 *			write mask to normal values.
 *
**/

void pascal
ega::reset( void ) {
	asm {

//		WREGA( EGA_GDC, 3, 0 );		// Rotate / write op.

		mov	dx,974
		mov	al,3
		out	dx,al
		inc	dx
		xor	ax,ax
		out	dx,al

//		WREGA( EGA_GDC, 5, 0 );		// R/W mode

		dec	dx
		mov	al,5
		out	dx,al
		inc	dx
		mov	al,ah				// ah = 0
		out	dx,al

//		WREGA( EGA_GDC, 8, 0xFF );	// Byte mask.

		dec	dx
		mov	al,8
		out	dx,al
		inc	dx
		mov	al,255
		out	dx,al

//		WREGA( EGA_GDC, 7, 0x00 );	// Compare logic disable.

		dec	dx
		mov	al,7
		out	dx,al
		inc	dx
		mov	al,ah
		out	dx,al

//		WREGA( EGA_SEQ, 2, 0x0F );	// Planes mask

		mov	dx,964
		mov	al,2
		out	dx,al
		inc	dx
		mov	al,15
		out	dx,al

		}

	}

void pascal
ega::setm2( void ) {
//	WREGA( EGA_GDC, 5, 2 );

	asm {
		mov	dx,974
		mov	al,5
		out	dx,al
		inc	dx
		mov	al,2
		out	dx,al
		}
	}




void pascal
ega::wline( uchar huge *pic, uint len, uint x, uint y ) {
	uint		rest = x & 7;
	uint		bxstart = x >> 3;
	uint		shift = (len & 7) - rest;
	uint		blen;
	char near	*dst = (uchar *)( bxstart + y * ( 640/8 ) );

	asm	push	ds

	blen = len+shift;
	blen = blen & 7 ? (blen >> 3) + 1 : blen >> 3;
	blen = rest ? blen + 1 : blen;
	blen = shift ? blen + 1 : blen;


	EGA_GDC_SELECT( 8 );			// Select byte mask register
	for( uint bsh = 0, mask = 0x80; bsh < 8; bsh++, mask >>= 1 ) {
		EGA_GDC_WRITE( mask );		// Send byte mask.
		int		sd   = bsh - 8 + shift;

		uint		cnt = blen;
		while( sd < 0 ) {    sd += 8; dst++; cnt--;	}
/*		if( sd < 0 ) {
			int t = - sd;
			if( t & 0x07 )
				t = ( t & 0xF8 ) + 0x08;

			sd += t;
			}
*/

		while( sd + (cnt << 3) >= len )	cnt--;

		_AX	= ega::page_shift;	// Changes ES
		asm	push	ax

		asm	mov	cx, cnt;

		asm	mov	di, dst
		asm	mov	ax, 0A000h	// EGA segment
		asm	mov	es, ax

		asm	lds	si, pic		// saved at the beginning

		asm	add	si, sd		// advance

		asm	pop	ax
		asm	add	di, ax		// Page 0/1

		asm	jcxz	skip_output	// "can't happen", but ...

		asm	cld

	ega_out_loop:	

		asm	lodsb			// Get the next byte

		asm	test	al, 0C0h	// If invisible
		asm	jnz	short inv_point	// Skip it

		asm	mov	ah, [es:di]	// Update latch registers
		asm	stosb			// Put point
		asm	add	si, 7		// .. every 8th ..
		asm	jmp	short done_disp

        inv_point:
		asm	add	si, 7		// Advance pointers
		asm	inc	di

	done_disp:
		asm	loop ega_out_loop	// start over

	skip_output:	;

		} /* for */

	asm	pop	ds

	}





/****************************************************************************
			Turn graphics mode on/off
****************************************************************************/


void pascal
ega::start_graphics( void ) {
	REGPACK		rr;

	rr.r_ax = 0x0F00;		// Get video mode
	intr( 0x10, &rr );

	old_vmode = rr.r_ax & 0xFFu;	// Save old video mode number
	
	rr.r_ax = 0x000E;		// Set 640x200 color graphics mode
	intr( 0x10, &rr );
	}




void pascal
ega::stop_graphics( void ) {
	REGPACK		rr;

	rr.r_ax = old_vmode & 0xFFu;	// Get old video mode number
	intr( 0x10, &rr );
	}





#if 0

/****************************************************************************
			Draw a picture
****************************************************************************/

#define	SETUP_EGA_FOR_WRITE						\
	{								\
	static	bool	first_pass = Yes;				\
									\
	if( first_pass == Yes ) {					\
		ega::reset();		// Reset EGA hardware registers	\
		ega::setm2();		// Select write mode 2		\
		first_pass = No;					\
		}							\
	}



void
image::draw( int x, int y, int xb, int yb, int xsz, int ysz ) {
	char huge	*src;
	char huge	*dst;

	int		scrx = x + xb;			// Screen X
	int		scry = y + yb;			// Screen Y

//??	int		src_jump = xs - xsz;		// Advance source ptr




	SETUP_EGA_FOR_WRITE;


	}
#endif