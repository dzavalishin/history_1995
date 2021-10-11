/**********************   Graphics Windows   *********************\
 *                                                               
 *	Module : Line editor                                      
 *         
 *	$Log:   C:/c/gw/vcs/edits.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:20:26   dz
 *      Initial revision.
 *                                                      
 *	   Rev 1.1     18 Dec 89				 
 *	Partial mouse support added, auto-erase mode		 
 *	added.							 
 *								 
 *	   Rev 1.0     05.08.89                                 
 *	Initial revision.                                       
 *                                                               
 *                                                               
\*********/


#include <stdlib.h>
#include <graphics.h>

#include <string.h>
#include <dos.h>
#include <bios.h>

#include "local.h"
#include "key.h"

#include "font.h"
#include "mouse.h"


void	strins( char *string );
extern unsigned char	getch( void );

uint	EDS_BACK =	DARKGRAY;
uint	EDS_TEXT =	WHITE;
uint	EDS_BORD =	LIGHTRED;
uint	EDS_CURS =	LIGHTGRAY;
uint	EDS_HEAD =	LIGHTRED;


#define CHX	3
#define CHY	16
#define CHW	8
#define CHH	16



static cstyle  eds_style = {
	&_8x16t_font, WHITE,
	DARKGRAY, 1, 1,		/*      Shadow parameters       */
	0,			/*      Bold factor             */
	0,			/*      Italics skew            */
	0, 0,
	0, 0,
	LIGHTBLUE,		/*      Back color              */
	{ 0 },
	/* Shadow */1, /*Bold*/0, /*Italics*/1, 0, 0,
	0			/*      copy put                */
	};










static int	oldcurs;

static void
drawcurs( int pos ) {

	mouse_off();

	if( oldcurs >= 0 ) {
		/* Erase old cursor	*/
		setfillstyle( SOLID_FILL, EDS_BACK );
		bar( CHX+ (oldcurs-1)*CHW, CHY+CHH, CHX+ oldcurs*CHW, CHY+CHH );
		}
	/* Draw a cursor	*/
	setfillstyle( SOLID_FILL, EDS_CURS );
	bar( CHX+ (pos-1)*CHW, CHY+CHH, CHX+ pos*CHW, CHY+CHH );

	mouse_on();
	oldcurs = pos;
	}



static int
do_edits( int n, unsigned char *str, char *help_n )
{

	register int	xx;
	register char	*sp;
	int		len;
	bool		mod = True;
	char		ch;

	bool		first_keystroke = True;	/* Auto-erase logic	*/
	bool		go_keyb;

	str[n] = '\0';	/* Cut the string	*/

	oldcurs = -1;

	sp = str;
	xx = 1;
	len = 0;

	for(; 1; first_keystroke = False ){

		if( mod ) {
			mouse_off();
			setfillstyle( SOLID_FILL, EDS_BACK );
			bar( CHX, CHY, CHX + CHW*(n+1), CHY+CHH );
			moveto( CHX, CHY+12 );

			gwsputs( &eds_style, str );
			mouse_on();

			mod = False;
			}

		len = strlen( str );

		drawcurs( xx );


		go_keyb = No;
		while(1) {
			int	d, b;

			if( bioskey(1) ) {
				go_keyb = Yes;
				break;
				}
			if( mpress( 1, &d, &d, &d ) ) 	/* Like <Enter>	*/
				return No;

			if( mpress( 0, &d, &d, &d ) ) {
				while( mgetpos( &d, &d, &b ), b )
					if( mpress( 1, &d, &d, &d ) ) {
						while( mgetpos( &d, &d, &b ), b )
							;
						return Yes;	/* Esc */
						}
				}

			if( mpress( 2, &d, &d, &d ) )
				return Yes;			/* Esc	*/
			}

		if( !go_keyb )
			continue;

		ch = getch();

		switch( ch ) {
			char	*op;

			case BACKSPACE :
				if( sp == &str[0] )
					break;
				else if( sp > str ) {
					op = sp;
					while( *sp ) {
						*(sp - 1) = *sp;
						sp++;
						}
					sp--;
					*sp = '\0';
					sp = op - 1;
					xx--;
					mod = True;
					}
				break;
			case 0 :
				ch = getch();

				switch( ch ) {

					case F1:
						help( help_n );
						break;

					case DOWN :
						return No;

					case F10:
					case UP:
						return Yes;

					case HOME:
						sp = str;
						xx = 1;
						break;

					case END:
						sp = str + len;
						xx = 1 + len;
						break;

					case LEFT :
						if( sp == &str[0] )
							break;
						else 	if( sp > str ){
							sp--;
							xx--;
							}
						break;

					case RIGHT :
						if( *sp == 0 )
							break;
						else	if( sp < &str[len] ){
							sp++;
							xx++;
							}
						break;

					case DEL:
						if( *sp == 0 )
							break;
						else	if( sp < &str[len] ){
							op = sp;

							while( *sp ){
								*sp = *(sp + 1);
								sp++;
							}
							sp--;
							*sp = '\0';
							sp = op;
							mod = True;
							}
					       break;

					default :
						break;
			       }
			       break;

			case ENTER :
				return No;

			case ESC :
				return Yes;

			default:
				if( first_keystroke ) {
					str[0] = '\0';	/* Auto-erase	*/
					xx = 1;
					sp = str;
					}

				if( xx >= n+1  || ch < 32 )
					break;

				if( *sp == 0 )
					*(sp+1) = 0;

				str[n-1] = '\0';
				strins( sp );
				*sp = ch;
				sp++;
				xx++;

				mod = True;
				break;
			}
		}
/*
	return Yes;
*/
	}




void
strins( register char *str ) {

	register unsigned char	c1, c2 = ' ';

	while(1) {
		c1 = *str;
		*str++ = c2;
		if( c2 == '\0' )
			break;
		c2 = c1;
		}
	}

/**Doc************************************************************************
 *	Function: bool gwedits()      		Header: gw.h
 *
 *	Parameters:
 *		int		x	- X position of the window.
 *		int		y	- Y position of the window.
 *		int		len	- Maximal length of string.
 *		char *		str	- String buffer.
 *		char *		header	- Window header ( question ).
 *		char *		help	- Help item name.
 *
 *	Return value on error:	No.
 *
 *	Normal return value:	Yes.
 *
 *	Description:
 *		Lets user to edit given string. Returns 'Yes' if
 *		user pressed <Enter>, 'No' if <Esc>.
 *
**/

bool
gwedits( int x, int y, int len,
	char *str, char *header, char *help ) {

	gwin eds_win = {
		0, 0, 0, 36,
		};

	struct { int x; int y; }	pt[ 8 ];

	int	ret;
	int	headwid;	/* Header border width	*/
	int	headpos;	/* Header border base	*/

	assert_init;

	push_bar(" Enter - Accept        ESC - Refuse   F10 - Exit  ");

	eds_win.xb = x;
	eds_win.yb = y;
	eds_win.xs = len*CHW + 6 + CHW;

	headwid = ( strlen( header ) * CHW ) + 16;

	if( eds_win.xs < headwid )
		eds_win.xs = headwid+4;



	headpos = ( eds_win.xs - headwid ) / 2;

	if( headpos <= 0 )	return Err;


	pt[0].x = headpos;
	pt[0].y = 9;

	pt[1].x = headpos+7;
	pt[1].y = 3;

	pt[2].x = headpos+headwid-7;
	pt[2].y = 3;

	pt[3].x = headpos+headwid;
	pt[3].y = 9;

	pt[4].x = headpos+headwid;
	pt[4].y = 10;

	pt[5].x = headpos+headwid-7;
	pt[5].y = 15;

	pt[6].x = headpos+7;
	pt[6].y = 15;

	pt[7].x = headpos;
	pt[7].y = 10;


	if( gwregister( &eds_win ) == Err )
		return Err;

	if(gwattach( &eds_win ) == Err ) {
		gwdestroy( &eds_win );
		return Err;
		}

	mouse_off();

	setcolor( EDS_BORD );
	setfillstyle( SOLID_FILL, EDS_BACK );
	setlinestyle( SOLID_LINE, 0, 2 );
	bar( 0, 8, eds_win.xs-1, eds_win.ys-1 );
	rectangle( 1, 9, eds_win.xs-2, eds_win.ys-2 );

	setcolor( EDS_HEAD );
	setfillstyle( SOLID_FILL, EDS_BORD );
	fillpoly( 8, (int *)pt );

	gwtextcolor( EDS_TEXT );
	moveto( headpos+10, 2+12 );
	gwputs( header );

/*	gwtextcolor( EDS_TEXT );		*/

	eds_style.c_color = EDS_TEXT;
	eds_style.s_color = EDS_BACK;
	eds_style.b_color = EDS_BACK;


	mouse_on();

	reset_latches();
	ret = do_edits( len, str, help );
	reset_latches();

	gwpermdetach();
	gwdestroy( &eds_win );
	pop_bar();
	return ret ? No : Yes;
	}










