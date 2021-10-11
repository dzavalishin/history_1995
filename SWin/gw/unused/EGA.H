/**--------------------   Graphics Windows   -------------------**\
 *                                                               
 *     Copyright (C) 1989 by DZ - Moscow, 110.6728		 
 *                                                               
 *     Module : EGA support header file                          
 *                                                               
 *	$Log:   C:/c/gw/vcs/ega.h_v  $
 * 
 *    Rev 1.0   29 Apr 1990 16:20:58   dz
 * Initial revision.
 *
 *     Revision 1.1     20.08.89                                 
 *       Initial revision.                                       
 *                                                               
 *                                                               
\**-------------------------------------------------------------**/

/*	*NoDoc*		*/


#ifndef EGA_H
#define EGA_H

#ifndef STYLE_H
#	include <style.h>
#endif	STYLE_H


bool	_EGA_alloc( char far *bufs[4], unsigned Xsize, unsigned Ysize, unsigned *plane_size_p );
bool	_EGA_free(  char far *bufs[4] );

bool	_EGA_get(   char far *bufs[4],
			unsigned Xbase, unsigned Ybase,
			unsigned Xsize, unsigned Ysize );

bool	_EGA_put(   char far *bufs[4],
			unsigned Xbase, unsigned Ybase,
			unsigned Xsize, unsigned Ysize );

bool	_EGA_xchg(   char far *bufs[4],
			unsigned Xbase, unsigned Ybase,
			unsigned Xsize, unsigned Ysize );

void pascal		_EGA_reset_write_mode( void );
void pascal		_EGA_write_color( unsigned char color );
void pascal		_EGA_write_op( unsigned op );



#include <dos.h>

/****************************************************************************
			EGA Registers access macros
****************************************************************************/


#define	WREGA( adr, reg, val )	(outportb( adr, reg ), outportb( adr+1, val ))
#define RDEGA( adr, reg )	( outportb( adr, reg ), inportb( adr+1 ))

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
			EGA.SYS interface
****************************************************************************/


#define ES_GETVER       0xFA
#define	ES_GETONE	0xF0
#define	ES_PUTONE	0xF1
#define	ES_GETPORT	0xF2
#define	ES_PUTPORT	0xF3
#define	ES_GETMANY	0xF4
#define	ES_PUTMANY	0xF5
#define	ES_RESET	0xF6
#define	ES_INITIAL	0xF7
#define	ES_F8		0xF8
#define	ES_F9		0xF9


#define	ES_CRT		0x00
#define	ES_SEQ		0x08
#define	ES_GDC		0x10
#define	ES_ATT		0x18
#define	ES_MSC		0x20
#define	ES_FEA		0x28
#define	ES_G1P		0x30
#define	ES_G2P		0x38




extern bool	Ega_Sys_attached;

bool	es_attach( void );

void	es_get_port( char far *buf, uint port, uint from, uint num );
void	es_put_port( char far *buf, uint port, uint from, uint num );


/****************************************************************************
		Palette save/restore functions ( SAVEPAL.C )
****************************************************************************/

void	save_palette( void );
void	rest_palette( void );


#endif EGA_H