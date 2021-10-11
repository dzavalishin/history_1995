/**--------------------   Graphics Windows   -------------------**\
 *                                                               
 *	Module : errmsg() function.                               
 *      
 *	$Log:   C:/c/gw/vcs/errmsg.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:20:04   dz
 *      Initial revision.
 *                                                          
 *	   Rev 1.0     06.08.89                                 
 *	Initial revision.                                       
 *                                                               
 *                                                               
\*********/


#include <stdlib.h>
#include <graphics.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <bios.h>


#include "style.h"

#include "gw.h"
#include "local.h"

#include "key.h"
#include "font.h"

#include "mouse.h"



#define	ENABLE_FATALS	0

#define	ERR_FOR	WHITE
#define	ERR_BKG RED


cstyle  errmsg_style = {
	&_8x16t_font, ERR_FOR,
	ERR_BKG, 2, 2,		/*      Shadow parameters       */
	0,			/*      Bold factor             */
	0,			/*      Italics skew            */
	0, 0,
	0, 0,
	ERR_BKG,		/*      Back color              */
	{ 0 },
	/* Shadow */1, /*Bold*/0, /*Italics*/0, 0, 0,
	0,			/*      copy put                */
	0			/*	Back fill off.		*/
	};





static bool
put_errmsg( char *buf, int warn ){

	int 	len;
	int	delta;


	gwin errwin = {
		0, 157, 0, /*22*/ 42
		};

	delta = warn ? 13 : 11;



	if ( ( len = strlen( buf ) ) > 68 ) {
		buf[68] = '\0';		/*	Cut text		*/
		len = 68;
		}


	errwin.xb = ( 80 - len - delta ) * 4 - 2;
	errwin.xs = ( len + delta ) * 8 + 4;


	if( gwregister( &errwin )) {
#if ENABLE_FATALS
		char	msg[100];
		sprintf( msg, "Errmsg: gwregister failed - `%.30s'", buf );
		gwfatal( msg );
#endif
		return Err;
		}

	mouse_off();

	if( gwattach( &errwin )) {
		mouse_on();
#if ENABLE_FATALS
		char	msg[100];
		sprintf( msg, "Errmsg: gwattach failed - `%.30s'", buf );
		gwdestroy( &errwin );
		gwfatal( msg );
		return Err;
#else
		gwdestroy( &errwin );
		return Err;
#endif
		}

	gwborder3d( 2, LIGHTGRAY, 2, RED, ERR_BKG  );

	moveto( 2, 4+12 );
	gwscenter( &errmsg_style, warn ? " Внимание : " : " Ошибка : " );
	moveto( 2, 18+12 );
	gwscenter( &errmsg_style, buf );


	push_bar("Нажмите 'Enter' ...");
	mouse_on();
	press_any_key();
	mouse_off();
	pop_bar();

	gwpermdetach();
	gwdestroy( &errwin );

	mouse_on();

	return Ok;
	}

/**Doc************************************************************************
 *	Function: errmsg()      		Header: gw.h
 *
 *	Parameters:			as printf.
 *
 *	Return value:			None.
 *
 *	Description:
 *		Displays error message.
 *
**/


int
errmsg( va_list arg_list, ...) {

	va_list arg_ptr;
	char *format;

	char	output[ 80 ];

	va_start(arg_ptr, arg_list);
	format = arg_list;
	vsprintf(output, format, arg_ptr);

	return put_errmsg( output, 0 );

	}

/**Doc************************************************************************
 *	Function: warnmsg()      		Header: gw.h
 *
 *	Parameters:			as printf.
 *
 *	Return value:			None.
 *
 *	Description:
 *		Displays warning message.
 *
**/


int
warnmsg( va_list arg_list, ...) {

	va_list arg_ptr;
	char *format;

	char	output[ 80 ];

	va_start(arg_ptr, arg_list);
	format = arg_list;
	vsprintf(output, format, arg_ptr);

	return put_errmsg( output, 1 );

	}


/**Doc************************************************************************
 *	Function: outofmem_msg()      		Header: gw.h
 *
 *	Parameters:			None.
 *
 *	Return value:			None.
 *
 *	Description:
 *		Displays message: "Out of memory".
 *
**/


void
outofmem_msg( void ) {
	errmsg("Недостаточно памяти");
	}

/**Doc************************************************************************
 *	Function: ioerr_msg()      		Header: gw.h
 *
 *	Parameters:			None.
 *
 *	Return value:			None.
 *
 *	Description:
 *		Displays message: "Input/Output error".
 *
**/


void
ioerr_msg( void ) {
	errmsg("Ввод/вывод закончился неуспешно");
	}


/**Doc************************************************************************
 *	Function: bool confirm()      		Header: .h
 *
 *	Parameters:
 *		char *		msg	- Message.
 *
 *	Description:
 *		Displays given message with the '?' sign,
 *		returns 'Yes' if user pressed "Y" or right
 *		mouse button, and 'No' if user pressed "N"
 *		or middle mouse button.
 *
**/

#define	CNF_COL		WHITE
#define CNF_BKG		CYAN


cstyle  confirm_style = {
	&_8x16t_font, CNF_COL,
	CNF_BKG, 2, 2,		/*      Shadow parameters       */
	0,			/*      Bold factor             */
	0,			/*      Italics skew            */
	0, 0,
	0, 0,
	CNF_BKG,		/*      Back color              */
	{ 0 },
	/* Shadow */1, /*Bold*/0, /*Italics*/0, 0, 0,
	0,			/*      copy put                */
	0			/*	Back fill off.		*/
	};


bool
confirm( char *msg ) {
	int	len;
	uchar	key;
	char	buf[80];

	gwin win = {
		0, 157, 0, 42
		};


	if ( ( len = strlen( msg )+8 ) > 68 )
		return Err;

	if( len < 14 )	len = 14;	/* 1st line	*/

	win.xb = ( 80 - len ) * 4 - 2;
	win.xs = len * 8 + 8;


	if( gwregister( &win ))
		return Err;

	if( gwattach( &win )) {
		gwdestroy( &win );
		return Err;
		}

	gwshadow( 2, LIGHTRED, 2, LIGHTGRAY, CNF_BKG );

	moveto( 4, 4+12 );

	gwscenter( &confirm_style, "Подтвердите (Y/N):" );
	gwsnl( &confirm_style, 4 );

	strcpy( buf, msg );
	strcat( buf, " ?" );

	gwscenter( &confirm_style, buf );


	push_bar("Нажмите 'Y' или 'N'");

	reset_latches();

	key = 0;

	do {
		int d;

		if( mpress( 2, &d, &d, &d )) {
			key = 'n';
			break;
			}

		if( mpress( 1, &d, &d, &d )) {
			key = 'y';
			break;
			}

		if( bioskey(1) )
			key = bioskey( 0 ) & 0xFF;

		} while(
			key != 'y' &&
			key != 'Y' &&
			key != 'n' &&
			key != 'N' &&
			key != 0x1B
			);

	pop_bar();

	reset_latches();

	gwpermdetach();
	gwdestroy( &win );

	return (key == 'y' || key == 'Y') ? Yes : No;
	}

/**Doc************************************************************************
 *	Function: void press_any_key() 		Header: gw.h
 *
 *	Parameters:			None.
 *
 *	Return value:			None.
 *
 *	Description:
 *		Waits until user press a key or
 *		mouse button.
 *
**/


void
press_any_key( void ) {
	int i,d;

	reset_latches();

	while(1) {
		if( bioskey(1)) {
			bioskey(0);
			return;
			}
		for( i = 0; i < 3; i++ )
			if( mpress( i, &d, &d, &d ))
				return;

		}
	}
