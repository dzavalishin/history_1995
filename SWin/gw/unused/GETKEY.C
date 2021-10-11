/**-------------------   Graphics Windows   -------------------**\
 *                                                              
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728		
 *                                                              
 *	Module :	Keyboard access function.		
 *                                                              
 *	$Log:   C:/c/gw/vcs/getkey.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:20:12   dz
 *      Initial revision.
 *
 *	Revision 1.1	19 Mar 90            			
 *		Initial revision.				
 *                                                              
 *								
\***********/


#include <style.h>
#include "gw.h"
#include <dos.h>





/**Doc************************************************************************
 *	Name:	gwkey			Header:		gw.h
 *
 *	Synopsis: uint	gwkey( void )
 *
 *	Parameters:
 *			None.
 *
 *	Return value:
 *		Lower byte	- ASCII code or 0 or 0xE0.
 *		Upper byte	- Scan code.
 *
 *	Description:
 *		  This function checks for a key pressed and returns
 *		0 if no key code available, ASCII & scan code for
 *		ASCII keys, 0 + scan for functional and Alt keys
 *		and 0x0E + scan for extended keys on extended keyboards.
 *
 *	See also:
 *		gwskey
 *
**/


uint
gwkey( void ) {
	union REGS	r;

	r.h.ah = __GW_ext_keyboard ? 0x11 : 0x01;
	int86( 0x16, &r, &r );
	return r.x.ax;
	}

uint
gwskey( void ) {
	union REGS	r;

	r.h.ah = 0x01;			/* Use standard service only	*/
	int86( 0x16, &r, &r );
	return r.x.ax;
	}


/************
 *
 *	BUG!!	It's possible to get more info
 *		about standard keyboard.
 *		eg. CapsLock, etc.
**/


#define	STBYTE	( peekb( 0, 0x418 ) )
#define	SBYTE	( peekb( 0, 0x417 ) )

bool		sh_left_ctrl,
		sh_right_ctrl,

		sh_left_alt,
		sh_right_alt,

		sh_left_shift,
		sh_right_shift,

		sh_caps_lock,
		sh_num_lock,
		sh_scroll_lock,

		sh_caps,
		sh_num,
		sh_scroll,
		sh_sys_req,
		sh_ins;



void
gwgetshift( void ) {
	union REGS	r;

	r.h.ah = __GW_ext_keyboard ? 0x12 : 0x02;
	int86( 0x16, &r, &r );

	if( !__GW_ext_keyboard ) {
		r.h.ah = 0;

		if( STBYTE & 0x01 ) 	/*	Left Ctrl	*/
			{
			sh_left_ctrl = On;
			sh_right_ctrl = Off;
			}

		else if( r.h.al & 0x04 )
			{
			sh_right_ctrl = On;
			sh_left_ctrl = Off;
			}

		else
			sh_left_ctrl = sh_right_ctrl = Off;


		if( STBYTE & 0x02 )	/*	Left Alt	*/
			{
			sh_left_alt = On;
			sh_right_alt = Off;
			}

		else if( r.h.al & 0x08 )
			{
			sh_left_alt = Off;
			sh_right_alt = On;
			}

		else
			{
                        sh_left_alt = Off;
			sh_right_alt = Off;
			}



		if( STBYTE & 0x04 )	/*	SysReq		*/

			sh_sys_req = On;

		else
			sh_sys_req = Off;


		if( STBYTE & 0x10 )	/*	Scrol key pressed	*/

			sh_scroll = On;

		else
			sh_scroll = Off;


		if( STBYTE & 0x20 )	/*	Num key pressed		*/

			sh_num = On;

		else
			sh_num = Off;


		if( STBYTE & 0x40 )	/*	Caps key pressed	*/

			sh_caps = On;

		else
			sh_caps = Off;

		if( STBYTE & 0x80 )	/*	Ins key pressed		*/

			sh_ins = On;

		else
			sh_ins = Off;


		if( SBYTE & 0x40 )	/*	CapsLock Locked		*/

			sh_ins = On;

		else
			sh_ins = Off;

		}

	}









































