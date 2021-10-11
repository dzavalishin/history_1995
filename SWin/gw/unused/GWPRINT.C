/**-------------------   Graphics window   -------------------**\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *				Dmitry	Zavalishin	[DZ],
 *                            	Yury	Tolmachev	[UA],
 *			      	Serge	Mednov		[MS],
 *				Gregory	Nickonov	[GN].
 *
 *	Module 	:	Printer support
 *
 *	$Log:   C:/c/gw/vcs/gwprint.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:19:14   dz
 *      Initial revision.
 *
 *	Revision 1.1	19 Apr. 1990 [GN]
 *		Overlook corrected.
 *
 *	Revision 1.0	16 Apr. 1990 [GN]
 *		Initial revision.
 *
\***********/

#include	"style.h"
#include	<bios.h>
#include	"font.h"
#include	<string.h>
#include	<stdio.h>


/**Doc***********************************************************************
			Graphic printer support.

	Revision	1.02	19 Apr. 1990 	by [GN]
****************************************************************************/


/****************************************************************************
				Static Variables
****************************************************************************/

static char	gp_buf[200];
static char	*gp_bp = gp_buf;
static cstyle	*Current_style = &def_style;
static char	*buffer;
static int	hskip = 0;


/****************************************************************************
			Global	Functions
****************************************************************************/

void	gp_sputs( cstyle *style, char *string );
void	gp_puts( char *string );
void	gp_sprintf( cstyle *style, va_list arg_list, ... );
void	gp_printf( va_list arg_list, ... );
void    gp_hskip( int skip );
void    gp_def_style( cstyle *style );


/****************************************************************************
			Static functions
****************************************************************************/

static void	graph_print( char *line, int len );
static void	graph_hskip( int n );
static void	release_buf( void );
static void	print_char( uchar chr );
static bool	Print_string( cstyle *style, char *string );
static void	Graph_crlf( void );


/**Doc************************************************************************
 *	Name:	gp_def_style		Header:		GP.H
 *
 *	Synopsis: ()
 *
 *	Parameters:	User style.
 *
 *
 *	Return value:	None.
 *
 *	Description:	Define user style as current.
 *
 *
 *	See also:	gp_sputs, gp_sprintf.
 *
**/

void
gp_def_style( cstyle *style ) {
	Current_style = style;
	}


/**Doc************************************************************************
 *	Name:	gp_puts			Header:		GP.H
 *
 *	Synopsis: ()
 *
 *	Parameters:	String for print.
 *
 *
 *	Return value:	None.
 *
 *	Description:	Print string witj current style.
 *
 *
 *	See also:	gp_sputs.
 *
**/

void
gp_puts( char *string ) {
	gp_sprintf( Current_style, string );
	}


/**Doc************************************************************************
 *	Name:	gp_sputs		Header:		GP.H
 *
 *	Synopsis: ()
 *
 *	Parameters:	Users style and string.
 *
 *
 *	Return value:	None.
 *
 *	Description:	Print string with style.
 *
 *
 *	See also:	gp_puts.
 *
**/

void
gp_sputs( cstyle *style, char *string ) {
	buffer = string;
	while( Print_string( style, buffer ) )
		release_buf();
	buffer = '\0';
	}


/**Doc************************************************************************
 *	Name:	gp_sprintf		Header:		GP.H
 *
 *	Synopsis: ()
 *
 *	Parameters:	User style, other see in PRINTF.
 *
 *
 *	Return value:	None.
 *
 *	Description:	Print string with style and format.
 *
 *
 *	See also:	gp_printf.
 *
**/

void
gp_sprintf( cstyle *style, va_list arg_list, ... ) {
	va_list	arg_ptr;
	char	*format;
	char	output[200];

	va_start( arg_ptr, arg_list );
	format = arg_list;
	vsprintf( output, format, arg_ptr );
	gp_sputs( style, output );
	}


/**Doc************************************************************************
 *	Name:	gp_printf		Header:		GP.H
 *
 *	Synopsis: ()
 *
 *	Parameters:	See PRINTF function.
 *
 *
 *	Return value:	None.
 *
 *	Description:	Print string with current style and user format.
 *
 *
 *	See also:	gp_sprintf.
 *
**/

void
gp_printf( va_list arg_list, ... ) {
	va_list	arg_ptr;
	char	*format;
	char	output[200];

	va_start( arg_ptr, arg_list );
	format = arg_list;
	vsprintf( output, format, arg_ptr );
	gp_puts( output );
	}


/**Doc************************************************************************
 *	Name:	gp_hskip		Header:		GP.H
 *
 *	Synopsis: ()
 *
 *	Parameters:	Horizontal skip in dot.
 *
 *
 *	Return value:	None.
 *
 *	Description:	Set horizontal skip for print functions.
 *
 *
 *	See also:	gp_puts, gp_sputs, gp_printf, gp_sprintf.
 *
**/

void
gp_hskip( int skip ) {
	hskip = skip;
	}


/****************************************************************************
			Print string using style
****************************************************************************/

static bool
Print_string( cstyle *style, char *string ) {
	char	*ll;
	char	b[50];
	font	*Font;
	int	i, j, fhsize;
	int	x, y, y8;
	uchar	h8;
	uchar	chr;
	uchar	*chr1, *chr2;
	int	char16;
	bool	Cr_Lf = No;
	bool	Second_pass = No;


	Font = style->s_font;
	chr1  = chr2 = string;

	j = (style->b_flag == No) ? 1 : (style->b_width == 0) ? 1 : style->b_width;

	for( y8 = 0; y8 < Font->vsize; y8 += 8 ) {
		for( i = 1; i <= j; i++ ) {
			chr1 = chr2;

			for( char16 = 0; char16 <= strlen(string); char16++ ) {
				chr = *chr1++;
				if( chr == '\0' ) {
					Cr_Lf = No;
					break;
					}

				if( chr == '\n' || chr == '\r' ) {
					Cr_Lf = Yes;
					buffer = chr1;
					break;
					}

				fhsize = Font->prop == Yes ? Font->propptr[(uint) chr] : Font->hsize;
				ll = Font->fontptr + Font->bpl * chr;

				for( x = 0; x < fhsize; x++ ) {
					b[x] = 0;
					for( y = 0; y < 8 && y8+y < Font->vsize; y++ ) {
						h8 = *( ll + ((y8+y)*Font->hsize/8) + x/8 );
						if( ( h8 << (x%8)) & 0x80 )
							b[x] |= (0x80U >> y);
						}
					}
				/* Print it out	*/
				graph_print( b, fhsize );
				graph_hskip( hskip );
				}
			release_buf ( );
			print_char( 0x0d );
			}

		if( Second_pass == No ) {
			Graph_crlf();
			Second_pass = Yes;
			}
		else
			Second_pass = No;
		}
	if( Cr_Lf )
		Graph_crlf();
	if( Cr_Lf )
		return Yes;
	else
		return No;
	}


/****************************************************************************
			Print one character
****************************************************************************/


static void
print_char( uchar c ) {
	biosprint( 0, c, 0 );
	}


/****************************************************************************
			Release data buffer
****************************************************************************/

static void
release_buf( void ) {
	int len = (int)(gp_bp - gp_buf);

	if( len >= 0 ) {
		print_char( 27  );
		print_char( 42  );
		print_char( 5   );
		print_char( len );
		print_char( 0   );
		for( gp_bp = gp_buf; len--; gp_bp++ ) {
			print_char( *gp_bp );
			}
		}
	gp_bp = gp_buf;
	}


/****************************************************************************
			Print data
****************************************************************************/

static void
graph_print( char *data, int len ) {
	if( ( gp_bp-gp_buf + len ) >= 200 )
		release_buf();

	while( len-- )
		*gp_bp++ = *data++;
	}


/****************************************************************************
			Make horizontal skip
****************************************************************************/

static void
graph_hskip( int n ) {
	while( n-- )
		graph_print( "\0", 1 );
	}


/****************************************************************************
			Make new line at printer
****************************************************************************/

static void
Graph_crlf( void ) {
	release_buf();

	print_char( 27 );
	print_char( 74 );
	print_char( 24 );
	print_char( '\r' );
	}

