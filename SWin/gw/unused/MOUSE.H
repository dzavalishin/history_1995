/**-------------------   Graphics Windows   -------------------**\
 *
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728
 *
 *	Module : Definitions for mouse driver interface.
 *
 *	$Log:   C:/c/gw/vcs/mouse.h_v  $
 * 
 *    Rev 1.0   29 Apr 1990 16:22:18   dz
 * Initial revision.
 *
 *
 *
\******/



#ifdef MOUSE_C
#define	MOUSE_INT	0x33

enum mouse_commands {
	Mouse_reset,		/* 0 - Reset, set to center.		*/

	Mouse_disp_on,		/* 1 - Enable mouse marker.		*/
	Mouse_disp_off,		/* 2 - Disable mouse marker.		*/

	Mouse_get_pos,		/* 3 - Get current position.		*/
	Mouse_set_pos,		/* 4 - Set current position.		*/

	Mouse_press_latch,	/* 5 - Mouse position when batton 	*/
				/*     was pressed			*/

	Mouse_release_latch,	/* 6 - Mouse position when batton	*/
				/*     was released			*/

	Mouse_X_region,		/* 7 - Set X bounds.			*/
	Mouse_Y_region,		/* 8 - Set Y bounds.			*/

	Mouse_shape,		/* 9 - Graphics shape.			*/
	Mouse_marker,		/* A - Text mode.			*/

	Mouse_delta,		/* B - X/Y delta.			*/
	Mouse_func,		/* C - set function			*/
	};

#endif

#ifndef MOUSE_H
#define MOUSE_H

/*			Funclion prototypes				*/



bool	mreset( void );
void	mdisplay( bool OnOff );
bool	mcheckin( int x1, int x2, int y1, int y2 );
void	mgetpos( int *x, int *y, int *battons );
void	msetpos( int x, int y );
void	mxregion( int xmin, int xmax );
void	myregion( int ymin, int ymax );
void	mdelta( int *dx, int *dy );
void    mtextcursor( unsigned char ch, unsigned char color );

bool	mpress( int button_number, int *x, int *y, int *but );
bool	mrelease( int button_number, int *x, int *y, int *but );

void	msetfunc( void far (*func)(), uchar mask );

void	mstdcurs( void );

void	reset_latches( void );

/*		mcurs.c							*/

void	mouse_off( void );
void	mouse_on( void );
void	init_mouse_cursor( void );
void	reset_mouse_cursor( void );

void	set_mouse_cursor( unsigned char cursor_no );

#ifndef	M_BUTTONS_DEF
#define M_BUTTONS_DEF
enum {
	MB_left		= 0,
	MB_right	= 1,
	MB_middle	= 2,
	};
#endif



/*			Global variables				*/

extern int      Nbuttons;	/* Number of mouse buttons		*/
extern char     Mouse_attached;

#endif

