/**--------------------   Graphics Windows   -------------------**\
 *                                                               
 *     Copyright (C) 1989 by DZ - Moscow, 110.6728		 
 *                                                               
 *     Module : Font structure and file layout.                  
 *                                                               
 *	$Log:   C:/c/gw/vcs/font.h_v  $
 * 
 *    Rev 1.0   29 Apr 1990 16:21:40   dz
 * Initial revision.
 *
 *	   Rev 1.0     22.08.89                                 
 *	Initial revision.                                       
 *                                                               
 *                                                               
\**-------------------------------------------------------------**/


#ifndef FONT_H
#define FONT_H


#ifndef STYLE_H
#include "style.h"
#endif

#include	<stdarg.h>

/****************************************************************************
		Infinity Soft Symbol - See 8x16t font
****************************************************************************/

#define	ISOFT_LOGO	0xF0



/****************************************************************************
			Font file format definitions
****************************************************************************/


					/* Current version of format	*/
#define	FONT_FORMAT_VER		2
#define	FONT_MAGIC              'fI'

enum Font_item_type {
	Font_layout,
	Font_prop_table
	};


struct Font_item_header {
	long			size;
	enum Font_item_type	type;
	};





/**Doc************************************************************************
 *	Type: font 				Header: font.h
 *
 *	Layout:
 *		int		hsize	- Horizontal size of character
 *		int		vsize	- Vertical size of character
 *		int		bpl	- Number of bytes per letter
 *		char *		fontptr	- Pointer to the font array
 *
 *	Description:
 *		This type is used to keep fonts.
 *
 *	See also:
 *		load_font(),	sf_load_font(),
 *		save_font(),	sf_save_font(),
 *		cstyle.
 *
**/


typedef struct Font_struct {	/* --------------- Description -----------------	      --- Format version --- In File - In Memory ---	*/
	int		hsize;		/* Horizontal letter size, pixels				0		Y	Y		*/
	int		vsize;		/* Vertical letter size, ---""---				0		Y	Y		*/
	int		bpl;		/* Bytes per letter.						0		Y	Y		*/
	uint		non0v : 1;	/* Version is not zero - compartibility problems		1		Y			*/
	uint		allc : 1;	/* Fontptr allocated.						0			Y		*/
	uint		pallc : 1;	/* Proportional table allocated					1			Y		*/
	uint		vers : 10;	/* File format version number					1		Y			*/

	char far	*fontptr;	/* Has no meaning in file					0			Y		*/
	uchar far	*propptr;	/* Proportional spacing table pointer				1			Y		*/

	uint		pack_meth : 5;	/* Compression method. ( 0 - None )						Y			*/

	uint		prop : 1;	/* Proportional font, propptr points to the width table		1			Y		*/

	uint		id;		/* Magic number.						1		Y			*/
	uint		n_item;		/* Number of items.						1		Y			*/

	uchar		baseline;	/* Base line position						2		Y	Y		*/

	} font;


#define	GET_LL( fontp, ch )	( ((char huge *)(fontp)->fontptr) + (ch) * (fontp)->bpl )

#define FONT_HEADER_SIZE	sizeof( struct Font_struct )
#define FONT_LL_POS		64



/**********************************
 *
 *	Character style type
 *
**/


/**Doc************************************************************************
 *	Type: cstyle 				Header: font.h
 *
 *	Layout:
 *		font *		s_font	- Pointer to the font
 *		uchar		c_color	- Character color
 *
 *		uchar		s_color	- Shadow color
 *      	int		s_xs	- Shadow X displacement
 *		int		s_ys	- Shadow Y displacement
 *
 *		uchar           b_width	- Bold character line width
 *
 *              signed char	i_step	- Italic step
 *
 *              uchar		u_dist	- Underline distance
 *		uchar		u_height- Underline height
 *
 *              uchar		o_color	- Outline color
 *		uchar		o_thick	- Outline thickness
 *
 *              uchar		b_color	- Background color
 *
 *              uint : 1	s_flag	- Shadow
 *		uint : 1	b_flag	- Bold
 *		uint : 1        i_flag	- Italic
 *		uint : 1        u_flag	- Underline
 *		uint : 1        o_flag	- Outline
 *
 *              uint : 1        w_copy	- Copy put/ Or put
 *		uint : 1        b_fill	- Background fill
 *
 *	Description:
 *		This type is used to keep character style.
 *
 *	See also:
 *		gwsputs(),	gwsprintf(),
 *		gwswidth(),	gwsheight().
 *
**/
/*	*NoDoc*		*/

typedef struct Character_style_struct {
	font	*s_font;	/*	Which font to use		*/
	uchar	c_color;	/*	Character color			*/

	uchar	s_color;	/*	Shadow color			*/
	int	s_xs;		/*	Shadow horizontal size (-left)	*/
	int	s_ys;		/*	Shadow vertical size   (-up)	*/

	uchar	b_width;	/*	Bold factor.			*/

	signed char i_step;	/*	Italic factor.			*/

	uchar	u_dist;		/*	Underline distance.		*/
	uchar	u_height;	/*	Underline height.		*/

	uchar	o_color;	/*	Outline color			*/
	uchar	o_thick;	/*	Outline thickness		*/

	uchar	b_color;	/*	Background color		*/

	int	dummy[9];	/*	For future expansions		*/


	uint	s_flag : 1;	/*	Shadow ?			*/
	uint	b_flag : 1;	/*	Bold ?				*/
	uint	i_flag : 1;	/*	Italic ?			*/
	uint	u_flag : 1;	/*	Underline ?			*/
	uint	o_flag : 1;	/*	Outline ?			*/

	uint	w_copy : 1;	/*	Copy/Or	write			*/
	uint	b_fill : 1;	/*	Background fill			*/
	} cstyle;



/**Doc***
 *	Global variable:	_8x16t_font	Header: font.h
 *
 *	Type:			font
 *
 *	Description:
 *		This variable contains user-accessible
 *		standard font which size is 8 points hor.
 *		and 16 points ver.
 *
 */


/**Doc***
 *	Global variable:	_8x8_font	Header: font.h
 *
 *	Type:			font
 *
 *	Description:
 *		This variable contains user-accessible
 *		standard font which size is 8 points hor.
 *		and 8 points ver.
 *
 */


extern cstyle	def_style;
extern font	_8x16t_font;	/*	Default font 			*/
extern font	_8x8_font;
extern font	_8x14t_font;	/*	8x14 font			*/


/*		Generic	( FontRd.C & FontWr.C )		*/
bool	g_load_font( int (*readf)(  int len, long base, void *buf ), font *font );
bool	g_save_font( int (*writef)( int len, long base, void *buf ), font *font );

/*		File ( Font.C )				*/
bool	load_font(    font *font, char *file_name );
bool	save_font(    font *font, char *file_name );

bool	load_beta_font( font *font, char *file_name );
bool	creat_font(      font *font );

/*		Superfile - by number ( BdFont.C )		*/
bool	sf_load_font_bd( int sfd, int bd, font *font );
bool	sf_save_font_bd( int sfd, int bd, font *font );

/*		Superfile - by name ( SfFont.C )		*/
bool	sf_load_font( int sfd, font *font, char *name );
bool	sf_save_font( int sfd, font *font, char *name );

/****************************************************************************
			Printer support utilitis
****************************************************************************/

void	gp_sputs( cstyle *style, char *string );
void	gp_puts( char *string );
void	gp_sprintf( cstyle *style, va_list arg_list, ... );
void	gp_printf( va_list arg_list, ... );
void	gp_hskip( int skip );
void	gp_def_style( cstyle *style );

/****************************************************************************

****************************************************************************/


void	linegen( int x, int y, char *string, int count, cstyle *character_style );
void	putsymbol( int x, int y, font *f, uchar sym, int op, uchar color );


/*		Generic style text support		*/

#include <stdarg.h>

void	gwprintf( va_list arg_list, ... );

void	gwsprintf( cstyle *style, va_list arg_list, ...);
void	gwsputs( cstyle *style, char *text );

void	gwscenter( cstyle *style, char *text );

int	gwsheight( cstyle *style, char *text );
int	gwswidth( cstyle *style, char *text );

void	gwsgotoxy( cstyle *style, int x, int y );
void	gwsnl( cstyle *style, int xpos );

/*		Standard style ( def_style ) support	*/

#define	gwputs( txt )		gwsputs( &def_style, txt )
#define	gwcenter( txt )		gwscenter( &def_style, txt )
#define	gwheight( txt )		gwsheight( &def_style, txt )
#define	gwwidth( txt )		gwswidth( &def_style, txt )
#define	gwgotoxy( x, y )	gwsgotoxy( &def_style, x, y )
#define	gwnl( x )		gwsnl( &def_style, x )




void	gwtextcolor( uint color );
void	gwtextback( uint back );
void	gwtextattr( uint attr );

void	gwtextfill( bool );



/*		Special support				*/


void     putbitline( int, int, int, char *, int, int );





#endif FONT_H
