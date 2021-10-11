/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:	EGA support header.
 *
 *      $Log:   D:/animate/vcs/ega.h_v  $
 *      
 *         Rev 1.0   18 Dec 1990 04:27:26   dz
 *      Initial revision.
 *
 *
\*/

#ifndef EGA_H
#define EGA_H

#ifndef STYLE_H
#	include <style.h>
#endif	STYLE_H

#include <dos.h>

/****************************************************************************
			EGA Registers access macros
****************************************************************************/


#define	WREGA( adr, reg, val )	( outportb( adr, reg ), outportb( adr+1, val ))
#define RDEGA( adr, reg )	( outportb( adr, reg ), inportb( adr+1 ))

#define	EGA_GDC_SELECT( reg )	outportb( EGA_GDC, reg )
#define	EGA_GDC_WRITE( val )	outportb( EGA_GDC+1, val )


/****************************************************************************
			EGA Registers addresses
****************************************************************************/


#define	EGA_CRT		0x3D4
#define	EGA_SEQ		0x3C4
#define	EGA_GDC		0x3CE
#define	EGA_ATT		0x3C0
#define	EGA_MSC		0x3C2
#define	EGA_FEA		0x3DA
#define	EGA_G1P		0x3CC
#define	EGA_G2P		0x3Ca



/****************************************************************************
			Class EGA - used for card access
****************************************************************************/

struct ega {
	static uint	page_shift;

	static void pascal	visual_page( uint page_no );
	static void pascal	active_page( uint page_no );

	static uint pascal	get_visual_page( void );


	static void pascal	start_graphics( void );
	static void pascal	stop_graphics( void );
	static int		old_vmode;		// Saved video mode number

	static void pascal	reset( void );		// Reset registers to normal values
	static void pascal	setm2( void );		// Set write mode 2
	static void pascal	wline( uchar huge *src, uint len, uint x, uint y );
	};






#endif EGA_H