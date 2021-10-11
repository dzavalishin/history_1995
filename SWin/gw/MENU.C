/*********************   Graphics Windows   *******************\
 *                                                               
 *	Module : Menu handler.                                    
 *                     
 *	$Log:   C:/c/gw/vcs/menu.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:20:22   dz
 *      Initial revision.
 *                                         
 *	   Revision 1.0     13.08.89                                 
 *	Initial revision.                                       
 *                                                               
 *                                                               
\***********/


#include <stdlib.h>
#include <graphics.h>
#include <bios.h>

#include "gw.h"
#include <conio.h>

#include <ctype.h>

#include "menu.h"
#include "key.h"
#include "mouse.h"
#include "font.h"


static cstyle	men_style = {
	&_8x16t_font, LIGHTGRAY,
	RED|LIGHTGREEN,1,1,	/* SHcol, SHh, SHv	*/
	1,		/* Bold factor		*/
	1,		/* Italic factor	*/
	0,0,		/* Udist, Uheight	*/
	0,0,		/* Ocol, Othick		*/
	0,		/* Back color */
	{0},
	0,0,0,0,0,	/* S, B, I, U, O	*/
	0,		/* Copy = 1		*/
	1		/* Background fill	*/
	};







#define	local	men->lcl


char std_menu_help[] = " F1 - ®¬®ιμ                                                       F10 - ‚λε®¤ ";


static void	menu_on_enter( menu *men );
static void	menu_draw( menu *men );
static void	menu_on_key( menu *men, unsigned char scan  );
static void	menu_disp_current( menu *men );
static void	menu_next( menu *men );
static void	menu_prev( menu *men );
static int	select_on_letter( int c, items *item, int max );
static void	menu_mouse_action( menu *men );


/*
 *      Following function is that one, which implements
 *      the most of menu-driven user interface.
 *
 *      The only argument is a pointer to a menu descripting
 *      structure.
 */

static bool     go_up_right = False;
static bool     go_up_left  = False;

char    menu_F10_exit = 0;

extern void	help( char *topic );


/**Doc************************************************************************
 *	Function: sel_menu()			Header: menu.h
 *
 *	Parameters:
 *		menu *			menu_ptr - Pointer to user-defined
 *						   menu descriptor.
 *
 *	Return value:			None.
 *
 *	Description:
 *		This function invokes a menu handler, which
 *		lets user to select an item from the given menu.
 *
 *
**/



void
sel_menu( menu *men ) {
	items   *item = men->item;      /* To speed up display  	*/

	local.exitkey = No;    		/* ESC or Return pressed ?  	*/
	local.escaped = No;
	menu_F10_exit = No;		/* Exit from all menus		*/

	local.horizontal = (( men->nitems > 1 ) && ( item[0].y == item[1].y  )) ? True : False;
	local.redraw = 1;			/* To enforce redraw	*/
	local.oldsel = local.select;	/* to redraw on unselection */

	local.wd = gwcreate( men->begx, men->begy, men->sizx, men->sizy );
	if( local.wd == NULL ) {
		outofmem_msg();
		local.exitkey = Yes;
		return;
		}


	if( gwattach( local.wd ) == Err ) {
		outofmem_msg();
		gwdestroy( local.wd );
		local.exitkey = Yes;
		return;
		}

/*	gwclear(  (men->colors.text >> 4) & 0x0F );	*/
	gwborder3d( 2, men->colors.border & 0x0F,
		4, DARKGRAY,
		(men->colors.text >> 4) & 0x0F );


	reset_latches();	/*	Clear old mouse info.		*/

/**
 *      Main loop : get a key, act on it, redisplay selection.
**/
	do {
		int     key;
		int     dselect;

		if( local.redraw ) {
			local.redraw = 0;
			menu_draw( men );
			}

		if( bioskey(1) == 0 ) {
			menu_mouse_action( men );
			continue;
			}



		key = getch();		/* get next char withno echo */

		if( key == 0 ) {	/* F? or ALT-? or cursor keys */

			key = getch();

			switch( key ) { /* extended code */
				case F1:
					help( men->helpitem );
					continue;


				case DOWN:
				case RIGHT:
					if( local.horizontal && key == DOWN) {
						menu_on_enter( men );
						break; /* not continue !!! */
						}

					if((!local.horizontal) && key == RIGHT) {
						local.exitkey = 1;
						local.escaped = True;
                                                go_up_right = True;
                                                break;
						}

                                        menu_next( men );
                                        break;

                                case UP:
                                case LEFT:
					if(local.horizontal && key == UP)
						continue; /* Ignore */

					if((!local.horizontal) && key == LEFT) {
						local.exitkey = 1;
						local.escaped = True;
                                                go_up_left = True;
                                                break;
						}

                                        menu_prev( men );
                                        break;

                                case F10:
                                case ALT_X:              /* Alt-X */
                                        menu_F10_exit = 1;      /* Exit from all menus  */
					local.exitkey = 1;
					local.escaped = Yes;
					break;

                                default:
					menu_on_key( men, key );
                                        break; /* not continue !!! */
                        } /* switch     */
                } /* if( key == 0 ) */

                else {                  /* ASCII character */
                        switch( key ) {
                                case ESC:
					local.exitkey = 1;
					local.escaped = 1;
                                        break;

                                default:
                                        dselect = select_on_letter(key,item,
                                        men->nitems);
                                        if( (dselect < men->nitems)
                                                && (dselect >= 0))
						local.select = dselect;
                                        else
                                                break;

                                        /* O'k, display it.  */

					menu_disp_current( men );


                                /* Fall through ...       */

                                case ENTER:
					menu_on_enter( men );
                                        break; /* not continue !!! */


                                } /* switch     */

                        } /* if( key == 0 )     */

		menu_disp_current( men );

		} while( !local.exitkey );

	gwpermdetach();
	gwdestroy( local.wd );

	reset_latches();	/*	Clear mouse info.	*/

	}


static void
menu_next( menu *men ) {
	int	psel = local.select;
	do {

		if( ++local.select >= men->nitems)
			local.select = 0;

		if( local.select == psel ) {
			local.exitkey = True;
			local.escaped = True;
			return;
			}

		} while( men->item[local.select].flags.disable );
        }





static void
menu_prev( menu *men ) {
	int	psel = local.select;
	do {
		--local.select;

		if( local.select < 0 || local.select >= men->nitems )
			local.select = (men->nitems) - 1;

		if( local.select == psel ) {
			local.exitkey = True;
			local.escaped = True;
			return;
			}

		} while( men->item[local.select].flags.disable );
        }


static void
menu_on_enter( menu *men ) {

        items   *item = men->item;

        while( 1 ) {
		menu_disp_current( men );

		if( item[ local.select ].flags.offmenu )
			gwdetach();

		if( item[ local.select ].invoke )
			(item[local.select].invoke) ( &(item[ local.select ].value) );

		if( item[ local.select ].menu )
			sel_menu( item[ local.select ].menu );

		if( item[ local.select ].flags.offmenu ) {
			if( gwattach( local.wd ))
				errmsg("Menu : can't reattach window");
			}

		if( item[ local.select ].flags.leave || menu_F10_exit ) {
			local.exitkey = 1;
			return;
			}

                if(go_up_right || go_up_left) {
			if( ! local.horizontal) {
				local.exitkey = 1;
				return;
                                }

                        if( go_up_right ) {
                                go_up_right = False;
                                menu_next( men );
				if( ( local.exitkey ) || ! item[ local.select ].menu )
					return;
                                continue;
                                }

                        if( go_up_left ) {
                                go_up_left = False;
                                menu_prev( men );
				if( ( local.exitkey ) || ! item[ local.select ].menu )
					return;
                                continue;
                                }
                        }

                        break;
                }

	return;
        }



static void
menu_on_key( menu *men, unsigned char scan ) {

        int     nkey = men->nkeys;
        int     i;

        keys    *key = men->key;
        items   *item = men->item;

        for( i = 0; i < nkey; i++ )
                if( key[i].scan == scan )
                        break;

	if( i >= nkey )
		return;

        if( key[ i ].flags.disable )    /* Disabled key.                */
		return;


	menu_disp_current( men );

        if( key[ i ].flags.offmenu )    /* Turn menu off                */
		gwdetach();

        if( key[ i ].invoke )
		(key[i].invoke) ( &(item[ local.select ].value) );

        if( key[ i ].menu )
                sel_menu( key[ i ].menu );

        if( key[ i ].flags.offmenu )    /* Turn menu off                */
		gwattach( local.wd );


	if( key[ i ].flags.leave || menu_F10_exit ) {
		local.exitkey = 1;
		return;
		}

        go_up_right = go_up_left = 0;

	menu_disp_current( men );

        }



static void
menu_disp_current( menu *men ) {

        items   *item = men->item;

	mouse_off();

	if( local.oldsel != local.select ) {
		moveto( item[local.oldsel].x, item[local.oldsel].y );
		men_style.b_color = (
			men->item[local.oldsel].flags.disable ?
			men->colors.disable : men->colors.text ) >> 4;
		men_style.c_color = ((men->item[local.oldsel].flags.disable) ? men->colors.disable : men->colors.text) & 0xF;
		gwsprintf( &men_style, item[local.oldsel].text, item[local.oldsel].value, item[local.oldsel].user );
		}

	local.oldsel = local.select;

	moveto( item[local.select].x, item[local.select].y );
	men_style.b_color = ( men->colors.bar ) >> 4;
	men_style.c_color = ((men->item[local.select].flags.disable) ? men->colors.disable : men->colors.text) & 0xF;
	gwsprintf( &men_style, item[local.select].text, item[local.select].value, item[local.select].user );

	mouse_on();

        }




static void
menu_draw( menu *men ) {

	int	i;

/*
 *      Next part of code drawing out
 *      menu itself.
 */

	mouse_off();

	for(i = 0; i < men->nitems; i++) {
		moveto( men->item[i].x, men->item[i].y );
		men_style.b_color = ( i == local.select ?
                        men->colors.bar :
			(men->item[i].flags.disable ?
                        men->colors.disable : men->colors.text)
			) >> 4;
		men_style.c_color = ((men->item[i].flags.disable) ? men->colors.disable : men->colors.text) & 0xF;
		gwsprintf( &men_style, men->item[i].text, men->item[i].value, men->item[i].user );
		}

	mouse_on();
	}



static int
select_on_letter( int c, items *item, int max ) {
        register int    i;
        register char   *cp;

        if( islower( c ))               /* Ignore case when comparing */
                c = toupper( c );

        for( i = 0; i < max; i++ ) {
                cp = item[i].text;

                while(*cp == ' ' || *cp == '\t')
                        cp++;

                if( c == *cp )
                        return i;
        }
        return -1;                      /* Nothing found  */
} /* select_on_letter   */



void
exit_menu( void ) {
        menu_F10_exit = 1;
        }


#define	mitem	(men->item)

static void
menu_mouse_action( menu *men ) {

	register int	i;
	int		x, y, b;


	if( !Mouse_attached )
		return;

	if( mpress( 2, &x, &y, &b )) {
		local.exitkey = Yes;
		local.escaped = True;
		return;
		}



	mgetpos( &x, &y, &b );
	if( b & 0x1) {
		x -= men->begx;
		y -= (men->begy-12);


		for( i = men->nitems-1; i >= 0; i-- ) {
			if( local.horizontal ) {
				if( mitem[i].x > x )	continue;
				if( i != (men->nitems-1) && mitem[i+1].x <= x )	continue;

				local.select = i;
				break;
				}
			else {
				if( mitem[i].y > y )	continue;
				if( i != (men->nitems-1) && mitem[i+1].y <= y )	continue;

				local.select = i;
				break;
				}
			}

		if( local.select >= men->nitems)
			local.select = men->nitems-1;

		if( local.select < 0 )
			local.select = 0;


		if( mitem[local.select].invoke == NULL &&
		    mitem[local.select].menu ) {
			menu_on_enter( men );
			if( local.oldsel != local.select )
				menu_disp_current( men );

			return;
			}


		if( local.oldsel != local.select )
			menu_disp_current( men );

		}




	if( mpress( 0, &x, &y, &b ) ) {
		static long	prevtime;
		long		currtime;


		x -= men->begx;
		y -= (men->begy-12);


		if( x < 0 || y < 0 || x > men->sizx || y > men->sizy ) {
			local.exitkey = Yes;
			return;
			}


		currtime = biostime( 0, 0 );

		if( (currtime - prevtime) < 9 ) {
			menu_on_enter( men );
			menu_disp_current( men );
			}
		else
			prevtime = currtime;

		reset_latches();



		}
	}






