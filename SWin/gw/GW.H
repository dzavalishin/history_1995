/**--------------------   Graphics Windows   -------------------**\
 *                                                               
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728		 
 *                                                               
 *	Module : User header file.                                
 *                             
 *	$Log:   C:/c/gw/vcs/gw.h_v  $
 * 
 *    Rev 1.0   29 Apr 1990 16:20:50   dz
 * Initial revision.
 *                                  
 *	   Rev 1.0     20.08.89                                 
 *	Initial revision.                                       
 *                                                               
 *                                                               
\********/

#ifndef GW_H
#define GW_H

#ifndef STYLE_H
#include <style.h>
#endif

#include <stdlib.h>
#include <graphics.h>

#include <stdarg.h>


#ifndef __LARGE__
#error *** Use LARGE model, dear ! ***
#endif




/**Doc************************************************************************
 *	Type: gwin 				Header: gw.h
 *
 *	Layout:
 *		int		xb	- X position of the window
 *		int		yb	- Y position og the window
 *		int		xs	- X size of the window
 *		int		ys	- Y size of the window
 *
 *	Description:
 *		This type represents a window. To create a window
 *		you must initialize such a structure with the positions
 *		and sizes you want to have a window of, and call
 *		'gwregister' with the address of the structure.
 *		Structure itself contains more fields than shown above,
 *		but you need to initialize only this four. Other fields
 *		MUST be zero !! The easiest way to do this is to declare
 *		structure as 'STATIC'. If you need an dynamic window,
 *		use 'gwcreate', which is allocates gwin from heap and
 *		initializes it as it needed. Such a dynamic window
 *		structure will be deallocated by 'gwdestroy'.
 *
 *	See also:
 *		gwregister(),	gwdestroy(),
 *		gwattach(),	gwdetach().
 *
**/
/*	*NoDoc*		*/

typedef struct {
	int	xb;			/* Base point.			*/
	int	yb;
	int	xs;			/* Sizes.			*/
	int	ys;

	uint	nactive;		/* Activated N times.		*/

	char far	*buf[4];	/* Image buffer.		*/

	uint	planesize;

	uint	haveimage : 1;
	uint	balloc : 1;		/* Buffer allocated.		*/
	uint	salloc : 1;		/* This structure allocated.	*/

	uint	must_be_displayed : 1;	/* Don't hide on detach().	*/
	uint	is_displayed : 1;	/* Currently is on the screen.	*/
	uint	nobuf : 1;		/* No image buffer.		*/

	uint	wpage;			/* Window is in this page	*/

	} gwin;


/****************************************************************************
			Global variables
****************************************************************************/

/* == Yes if keyboard BIOS has extended ( 10, 11, 12 ) functions	*/
extern bool	__GW_ext_keyboard;	



/****************************************************************************
				Prototypes
****************************************************************************/



bool	gwinit( void );
bool	gwreset( void );

void	gwfatal( char *msg );


void	gwnobuf( gwin *window );

gwin	*gwcreate( int x, int y, int xsize, int ysize );
bool	gwregister( gwin *window );
bool	gwdestroy( gwin *window );


bool	gwon( gwin *window );
bool	gwoff( gwin *window );
bool	gwpermoff( gwin *window );

bool	gwattach( gwin *window );
bool	gwdetach( void );
bool	gwpermdetach( void );

bool	gwdigout( gwin *window  );
bool	gwdigin( void );


/*	errmsg.c	*/

int	warnmsg( va_list arg_list, ...);
int	errmsg( va_list arg_list, ...);
bool    confirm( char *msg );
void	press_any_key( void );

void	outofmem_msg( void );
void	ioerr_msg( void );

/*	bar.c		*/


void	push_bar( char *bar_text );
void	pop_bar( void );

/*	tools.c		*/

bool	gwclear( char color );
bool	gwborder( int width, char color );
bool	gwborder3d( int bwidth, char bcolor, int swidth, char scolor, char ccolor );
bool	gwshadow( int bwidth, char bcolor, int swidth, char scolor, char ccolor );

/*	edits.c		*/

bool	gwedits( int x, int y, int len, char *str, char *header, char *help );

extern int	Gresult;		/* Result of last graphisc op.	*/


/*	dir.c		*/


bool	dirmenu( char *mask, char *outname, char *help );
bool	sfdirmenu( int super_file_dsc, int block_dsc, char *outname, char *help );

/*	HELP.C		*/

void	help( char *topic );


/*	SELCOLOR.C	*/

bool	sel_color( int *color_ptr );

/*	ZONE.C		*/

bool	select_zone( int *xbase, int *ybase, int *xsize, int *ysize, bool fix );


/*	ASKFNAME.C	*/

bool	infname(  int x, int y, int len, char *mask, char *out, char *head, char *help );
bool	outfname( int x, int y, int len, char *mask, char *out, char *head, char *help );
bool	newfname( int x, int y, int len, char *mask, char *out, char *head, char *help );

/*	XORLINE.C	*/

void	xorline( int x1, int y1, int x2, int y2, uchar color );


/*	VPAGE.C		*/

bool	gwapage( int page );
bool	gwvpage( int page );


#endif GW_H