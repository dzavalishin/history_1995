/*************************   Graphics Windows   **********************\
 *								
 *     Module : Directory menu.					
 *								
 *	$Log:   C:/c/gw/vcs/dir.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:20:28   dz
 *      Initial revision.
 *
 *	Revision 1.3	12 Mar 90 [DZ]				
 *		Static file list buffer now allocated.		
 *								
 *	Revision 1.2	16 Dec 89 [DZ]				
 *		Mouse support added				
 *								
 *	Revision 1.1     16 Sep 89				
 *		Modified to work with new GWindows.		
 *								
 *	Revision 1.0     05 Aug 89				
 *		Initial revision - extracted from phantom.	
 *								
 *								
\**********/


#include <stdlib.h>
#include <graphics.h>
#include <dir.h>
#include <errno.h>
#include <string.h>
#include <dos.h>
#include <conio.h>
#include <bios.h>

#include "gw.h"
#include "local.h"
#include "key.h"
#include "font.h"
#include "mouse.h"


uint	DIR_BORD =	RED;
uint	DIR_BOX  =	DARKGRAY;

uint	DIR_BACK =	CYAN;
uint	DIR_TEXT =	WHITE;
uint	DIR_SHAD =	CYAN;

uint	DIR_BARB =	BLACK;
uint	DIR_BART =	WHITE;
uint	DIR_BARS =	LIGHTBLUE;


#define	MORE_UP_X	501
#define MORE_UP_Y	3
#define MORE_DN_X	501
#define MORE_DN_Y	91




cstyle	dir_style = {
	&_8x16t_font, 0/*DIR_TEXT*/,
	0/*DIR_SHAD*/,1,1,	/* SHcol, SHh, SHv	*/
	1,		/* Bold factor		*/
	0,		/* Italic factor	*/
	0,0,		/* Udist, Uheight	*/
	0,0,		/* Ocol, Othick		*/
	0/*DIR_BACK*/,		/* Back color */
	{0},
	1,0,0,0,0,	/* S, B, I, U, O	*/
	0,		/* Copy = 1		*/
	1		/* Background fill	*/
	};

extern font	_8x8_font;

static cstyle	scroll_mark_style = {
	&_8x8_font, WHITE,
	0,0,0,		/* SHcol, SHh, SHv	*/
	0,		/* Bold factor		*/
	0,		/* Italic factor	*/
	0,0,		/* Udist, Uheight	*/
	0,0,		/* Ocol, Othick		*/
	MAGENTA,	/* Back color */
	{0},
	0,0,0,0,0,	/* S, B, I, U, O	*/
	0,		/* Copy = 1		*/
	1		/* Background fill	*/
	};



static char	dir_helpbar[] = " \x18\x19\x1B\x1A - Выбор      Enter - Взять                             ESC - Отказ";







#define MAX_DIR 150




struct ffnames {
	char  name[15];
	char  attrib;
	};

static struct ffnames	*files;

static int   		nfile = 0;


static int	width, height;
static int	inwin;




static void	get_directory( char *mask );
static int	sel_directory( char *path, char *out, char *help_n );

static void	get_sf_directory( int sfd, int bd );
static void	sort_dir( void );





void
dir_bar( unsigned pos, char *name, int color, char mark ) {
	char line[20];

	strncpy( line, name, 13 );
	line[13] = '\0';
	moveto( ((pos&3)*16 + 3)*width,   ((pos >> 2)+1)*height+12 );

	if( color ) {
		dir_style.c_color = DIR_BART;
		dir_style.b_color = DIR_BARB;
		dir_style.s_color = DIR_BARS;
		}
	else {
		dir_style.c_color = DIR_TEXT;
		dir_style.b_color = DIR_BACK;
		dir_style.s_color = DIR_SHAD;
		}


	mouse_off();

	gwsputs( &dir_style, line );
	if( mark ) {
		char cc[2];
		cc[0] = mark;
		cc[1] = '\0';

		gwsputs( &dir_style, cc );
		}
	mouse_on();
	}



void
disp_dir( int shift ) {
	int i;
	int break_count = 4;
	char  line[ 20 ];

	register int  x = 3, y = 0;

	dir_style.c_color = DIR_TEXT;
	dir_style.b_color = DIR_BACK;
	dir_style.s_color = DIR_SHAD;


	mouse_off();

	moveto( MORE_UP_X, MORE_UP_Y+6 );
	gwsputs( &scroll_mark_style, shift ? "More" : "    " );

	moveto( MORE_DN_X, MORE_DN_Y+6 );
	gwsputs( &scroll_mark_style, nfile > shift+inwin ? "More" : "    " );

	for( i = shift; i < nfile && i < shift + inwin; i++, break_count++ ) {

		if( !(break_count % 4) ) {
			y++;
			x = 3;
			moveto( x*width, y*height+12 );
			setfillstyle( SOLID_FILL, DIR_BACK );
			bar( x*width, y*height/*+12*/, (x+60)*width, (y+1)*height/*+12*/ );
			}

		strncpy( line, files[i].name, 13 );
		line[13] = '\0';

		moveto( x*width, y*height+12 );
		x += 16;

		gwsputs( &dir_style, line );
		if( files[i].attrib & FA_DIREC )
			gwsputs( &dir_style, "\\");

		}

	mouse_on();

	}





static int	current;
static int	oldcur;
static int	shift;
static int	old_shift;

static bool     run;
static bool	escape;



static void
redisp_dir( void ) {
	reset_latches();


	if( current < 0 )   current = 0;
	if( current >= nfile )    current = nfile - 1;

	while( current - shift >= inwin ) shift += 4;
	while( current < shift )    shift -= 4;

	if( shift != old_shift ) {
		disp_dir( shift );
		old_shift = shift;
		}
	else if( oldcur != current )
		dir_bar( oldcur - shift, files[oldcur].name, 0, 0 );


	dir_bar( current - shift, files[current].name, 1, 0 );
	oldcur = current;

        }

static void
scroll_up( void ) {
	if( shift ) {
		shift -= 4;
		current -= 4;
		}
	}

static void
scroll_dn( void ) {
	if( nfile > shift+inwin ) {
		shift += 4;
		current += 4;
		}
	}


static bool
track_mouse( int x, int y ) {
	bool	good;
	int	i;
	int break_count = 4;
	register int  xx = 3, yy = /*12*/0;

	good = No;
	for(
			break_count = 4,i = shift;
			i < nfile && i < shift + inwin;
			i++, break_count++, xx += 16 ) {

		if( !(break_count % 4) ) {
			yy++;
			xx = 3;
			}
		if(
				x >= xx*width &&
				x <= (xx+14)*width &&
				y >= yy*height &&
				y <= (yy+1)*height
				) {
			if( i != current ) {
				current = i;
				redisp_dir();
				}
			good = Yes;
			break;
			}
		} /* for */
	return good;
	}


static bool
mouse_action( int wx, int wy ) {
	int	x, y, b;
	bool	act = No;

	if( !Mouse_attached )		/* No mouse ?			*/
		return No;		/* Nothing to do at all		*/




	if( mpress( 0, &x, &y, &b ) ) {
		static long	prevtime;
		long		curtime;

		x -= wx;		/*	Window - relative.	*/
		y -= (wy);

/*warnmsg("X %d, Y %d", x, y );*/
		if(
				x >= MORE_UP_X && x <= MORE_UP_X+4*8 &&
				y >= MORE_UP_Y && y <= MORE_UP_Y+8
				) {
/*warnmsg("PgUp");*/
			scroll_up();
			redisp_dir();
			return Yes;
			}

		if(
				x >= MORE_DN_X && x <= MORE_DN_X+4*8 &&
				y >= MORE_DN_Y && y <= MORE_DN_Y+8
				) {
/*warnmsg("PgDn");*/
			scroll_dn();
			redisp_dir();
			return Yes;
			}




		if( !track_mouse( x, y ))
			return No;

		curtime = biostime( 0, 0 );
		if( curtime-prevtime < 9 ) {
			escape = run = No;
			act = Yes;
			}
		else {
			prevtime = curtime;
			while( mgetpos( &x, &y, &b ), b )
				track_mouse( x-wx, y-wy );
			}
		reset_latches();
		}

	if( mpress( 2, &x, &y, &b ) ) {
		act = escape = Yes;
		run = No;
		}

	return act;
	}




static bool
keyb_action( char *help_n ) {
	uint	key;

	if( !bioskey(1) )	/*	No key in buffer ?		*/
		return No;	/*	Nothing to do here.		*/

	key = getch();

	if( key ) {     /* Not a func. key  */
		switch( key ) {
			case ESC:
				run = No;
				escape = Yes;
				break;

			case ENTER:
				run = No;
				break;

			default:
				return No;

			}

		return Yes;
		}

	key = getch();
	switch( key ) {
		case UP:
			current -= 4;
			break;

		case DOWN:
			current += 4;
			break;

		case RIGHT:
			current++;
			break;

		case LEFT:
			current--;
			break;


		case PGUP:
			current -= 20;
			break;

		case PGDN:
			current += 20;
			break;


		case HOME:
			current = 0;
			break;


		case END:
			current = nfile - 1;
			break;


		case F10:   /*  Exit    */
			escape = Yes;
			run = No;
			break;

		case F1:
			help( help_n );
			break;

		default:
			return No;

		}
	return Yes;
	}



int
sel_directory( char *path, char *out, char *help_n ) {
        gwin dir_win = { 6*8, 6*16, 68*8+3 , 7*16+3 };


	current = 0;
	shift   = 0;
	oldcur = 0;
	old_shift = 0;

	path = path;

	dir_win.xs = 68 * width;
	dir_win.ys = 7  * height;

	dir_style.c_color = DIR_TEXT;
	dir_style.s_color = DIR_SHAD;
	dir_style.b_color = DIR_BACK;


	if( gwregister( &dir_win ) || gwattach( &dir_win )) {
		outofmem_msg();
		return Err;
		}

	gwborder3d( 2, DIR_BORD, 3, DIR_BOX, DIR_BACK );

	run = Yes;
	escape = No;

	reset_latches();	/* Forget old mouse state.		*/

	disp_dir( shift );
	redisp_dir();	/* This will redisplay the current selection	*/
	while( run ) {

		if( keyb_action( help_n ) ) {
			redisp_dir();
			continue;
			}

		if( mouse_action( dir_win.xb, dir_win.yb ) ) {
			redisp_dir();
			continue;
			}

		}

	if( !escape ) {
		strncpy( out, files[current].name, 13 );
		out[13] = '\0';
		}


	gwpermdetach();
	gwdestroy( &dir_win );

	reset_latches();

	return !escape;
	}





/**Doc************************************************************************
 *	Function: dirmenu()      		Header: gw.h
 *
 *	Parameters:
 *		char *		mask	- File name mask ( e.g. *.c )
 *		char *		out	- User choosed file name.
 *		char *		help	- Help item name.
 *
 *	Return value on error:		No
 *
 *	Normal return value:		Yes
 *
 *	Description:
 *		Displays a directory list and lets user to
 *		pick a file name from this list.
 *
**/

bool
dirmenu( char *mask, char *out, char *help ) {
	char    path[ 80 ], dr[4], dn[10];
	char    filename[14];
	int	good;


	width = gwswidth( &dir_style, "W" )-1;
	height = gwsheight( &dir_style, "H" )-1;


	inwin = 20;

	files = calloc( sizeof( struct ffnames ), MAX_DIR );
	if( files == NULL ) {
		outofmem_msg();
		return Err;
		}


	get_directory( mask );
	if( nfile == 0 ) {
		free( files );
		return No;
		}

	fnsplit( mask, dr, path, dn, dn );

	push_bar( dir_helpbar );

	good = sel_directory( path, filename, help );
	if( good )
		fnmerge( out, dr, path, filename, "" );

	pop_bar();

	free( files );

	return good;
	}


/*
#define ATTR  FA_DIREC|FA_RDONLY|FA_HIDDEN|FA_SYSTEM|FA_ARCH

*/

#define ATTR  FA_RDONLY|FA_HIDDEN|FA_SYSTEM|FA_ARCH


void
get_directory( char *mask ) {
	int res;
	struct ffblk  buf;

	nfile = 0;
	errno = 0;

	res = findfirst( mask, &buf, ATTR );

	files[ nfile ].attrib = buf.ff_attrib;
	memcpy( files[ nfile ].name, buf.ff_name, 13 );

	( files[ nfile ].name )[13] = '\0';

	if( res && errno ) {
		if( errno == ENOENT )
			errmsg("Каталог пуст");
		else
			errmsg( sys_errlist[ errno ] );
		return;
		}

	nfile++;

	while(1) {
		errno = 0;
		res = findnext( &buf );

		files[ nfile ].attrib = buf.ff_attrib;
		memcpy( files[ nfile ].name, buf.ff_name, 13 );

		( files[ nfile ].name )[13] = '\0';

		if( res && errno && errno != ENOENT ) {
			errmsg( sys_errlist[ errno ] );
			return;
			}

		if( errno ) {
			sort_dir();
			return;
			}

		nfile++;

		}

	}





#undef DIRECTORY

#include "sfile.h"



/**Doc************************************************************************
 *	Function: sfdirmenu()      		Header: gw.h
 *
 *	Parameters:
 *		int		sfd	- SuperFile descriptor.
 *		int		bd	- Block descriptor.
 *		char *		out	- User-choosen name.
 *		char *		help	- Help item name.
 *
 *	Return value on error:		No
 *
 *	Normal return value:		Yes
 *
 *	Description:
 *		Displays a SuperFile directory list and lets user to
 *		pick a name from this list.
 *
**/


bool
sfdirmenu( int sfd, int bd, char *out, char *help ) {
	char    filename[14];
	int	good;


	width = gwswidth( &dir_style, "W" )-1;
	height = gwsheight( &dir_style, "H" )-1;


	inwin = 20;

	files = calloc( sizeof( struct ffnames ), MAX_DIR );
	if( files == NULL ) {
		outofmem_msg();
		return Err;
		}

	get_sf_directory( sfd, bd );
	if( nfile == 0 ) {
		free( files );
		return No;
		}

	push_bar( dir_helpbar );

	good = sel_directory( "Fonts", filename, help );
	if( good )
		strncpy( out, filename, 14 );

	pop_bar();

	free( files );

	return good;
	}




static void
get_sf_directory( int sfd, int bd ) {
	struct Bnames	buf;	/* SF directory entry			*/

	long		sf_pos;


	nfile = 0;
	errno = 0;

	for( sf_pos = 0L; ; sf_pos += sizeof( struct Bnames ) ) {

		if( sfread( sfd, bd, sizeof( struct Bnames ), sf_pos, &buf  ) != sizeof( struct Bnames ))
			break;

		if( buf.numdscr == NO_NAME )	continue;

		files[ nfile ].attrib = 0;
		memcpy( files[ nfile ].name, buf.bname, 14 );

		files[ nfile ].name[14] = '\0';

		nfile++;

		}

	if( nfile == 0 )
		errmsg("Каталог пуст");
	else
		sort_dir();

	}


static int
compare_ent( struct ffnames *e1, struct ffnames *e2 ) {
	return strcmp( e1->name, e2->name );
	}


static void
sort_dir( void ) {
	qsort( files, nfile, sizeof( struct ffnames ), compare_ent );
	}