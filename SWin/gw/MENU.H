/**-------------------   Graphics Windows   -------------------**\
 *
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728
 *
 *	Module : Menu support header.
 *
 *	$Log:   C:/c/gw/vcs/menu.h_v  $
 * 
 *    Rev 1.0   29 Apr 1990 16:22:00   dz
 * Initial revision.
 *
 *
 *
\******/

#include "gw.h"


/* Really it can be 16 ( 2**4 ) items, 1 bit - sign	*/

#define MAXITEMS	10


#define ON( back )	| ( back << 4 )

/**Doc************************************************************************
 *	Type:	items  				Header: menu.h
 *
 *	Layout:
 *		int		x		- Item position, win-relative.
 *		int		y
 *		char *		text		- Item text ( printf format )
 *		char *		value		- First arg. for printf
 *		long		user		- User-defined field
 *
 *		void		(*invoke)( char **value );
 *						- function to invoke
 *						  when item selected
 *
 *		menu *		menu		- Menu to activate
 *						  when item selected
 *
 *	struct {
 *		unsigned : 1	leave		- Selection of this item
 *						  causes exit from this menu
 *
 *		unsigned : 1	disable		- Disabled entry
 *
 *		unsigned : 1	offmenu		- Turn off menu while running
 *						  this item
 *
 *		} flags;
 *
 *
 *	Description:
 *		Array of items represents a menu elements. Each menu
 *		structure must have a pointer to such array.
 *
 *	See also:
 *		menu
 *
**/


struct ITEMS {
	int	x,
		y;
	char	*text;		/* format		*/
	char	*value;
	long	user;		/* User-defined field - use how you wish	*/
	void	(*invoke)( char **value ); /* function to invoke when item selected	*/
	struct MENU *menu;
	struct {
		unsigned	leave : 1;	/* Selection of this item causes exit from this menu */
		unsigned	disable : 1;	/* Disabled entry	*/
		unsigned	offmenu : 1;	/* Turn off menu while running this item */
		} flags;
	};


struct KEYS {
	unsigned char	scan;				/* Key scan-code				*/
	void		(*invoke)( char **value );	/* Function to invoke when this key is pressed.	*/
	struct MENU	*menu;
	struct {
		unsigned	leave : 1;	/* Selection of this item causes exit from this menu */
		unsigned	disable : 1;	/* Disabled entry	*/
		unsigned	offmenu : 1;	/* Turn off menu while running this item */
		} flags;
	};

/**Doc************************************************************************
 *	Type:	menu			Header: menu.h
 *
 *	Layout:
 *		int		nitems		- Number of items in menu
 *		int		nkeys		- Number of key definitions
 *
 *		char *		helpstring	- Help string
 *		char *		helpitem   	- Help-file item
 *
 *		int		border		- have border ?
 *
 *		int		begx		- Position of window
 *		int		begy
 *		int		sizx		- Size of window
 *		int		sizy
 *
 *	struct {                	NB! The next 7 must be zeroed.
 *		gwin *		wd		- Must be 0.
 *		int : 6		select		- Currently selected item
 *						  ( Read-write )
 *		int : 6		oldsel		- Don't care.
 *		unsigned : 1	exitkey		- Don't care.
 *		unsigned : 1	redraw		- Don't care.
 *		unsigned : 1	horizontal	- Don't care.
 *		unsigned : 1	escaped		- User have escaped from menu
 *						  ( Read-only )
 *		} lcl;
 *
 *	struct {
 *		int		text		- Non-selected entry color
 *		int		bar		- Color of selection bar
 *		int		border		- Color of border
 *		int		disable		- Color of disabled entry
 *		} colors;
 *
 *		items *		item		- Pointer to items array
 *		keys *		key		- Pointer to keys array
 *
 *	Description:
 *		Such a structure must be prepared for each menu.
 *		The best method is to declare a static structure,
 *		initialized with a parameters you need. You also
 *		can create menus dynamically, e.g. allocating it
 *		on the heap.
 *
 *
 *	See also:
 *		sel_menu()
 *
 *
 *
**/


struct MENU {
	int	nitems;		/* Number of items in menu			*/
	int	nkeys;		/* Number of key definitions			*/

	char	*helpstring;	/* String to put on 25-th line			*/
	char	*helpitem;	/* What to search for in help-file		*/

	int	border;		/* have border ?				*/
	int	begx,		/* for vsave					*/
		begy,
		sizx,
		sizy;

	struct {
		gwin		*wd;				/* Window descriptor		*/
		int		select	: 6;	/* Currently selected item	*/
		int		oldsel	: 6;	/* Previously selected item	*/
		unsigned	exitkey	: 1;	/* Exit from menu		*/
		unsigned	redraw	: 1;	/* Redraw all the menu		*/
		unsigned	horizontal : 1;	/* Horizontally layed menu	*/
		unsigned	escaped	: 1;	/* User have escaped from menu	*/
		} lcl;

	struct {
		int	text,		/* Non-selected entry color	*/
			bar,		/* Color of selection bar	*/
			border,		/* Color of border and header	*/
			disable;	/* Color of disabled entry	*/
		} colors;

	struct ITEMS 	*item;
	struct KEYS	*key;

	};



typedef struct ITEMS	items;
typedef struct KEYS	keys;
typedef struct MENU	menu;


extern char std_menu_help[];



void	exit_menu( void );	/* Causes exiting from the whole chain of nested menus	*/
void	sel_menu( menu *menu );	/* Menu driver itself.					*/