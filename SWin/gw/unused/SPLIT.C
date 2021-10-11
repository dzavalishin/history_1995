/**-------------------   Graphics Windows   -------------------**\
 *                                                              *
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728		*
 *                                                              *
 *	Module :  Split screen support.        			*
 *	
 *	$Log:   C:/c/gw/vcs/split.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:20:06   dz
 *      Initial revision.
 *                                                             
 *	Revision 1.1     28 Dec 89                              *
 *		Initial revision.				*
 *                                                              *
 *								*
\**------------------------------------------------------------**/

#include <style.h>

#include "local.h"
#include "ega.h"
#include "gw.h"

#include <dos.h>

/*#include <.h>			*/
/*#include <.h>			*/
/*#include <.h>			*/
/*#include <.h>			*/
/*#include <.h>			*/



void
gwsplit( uchar row ) {



	/*	Set line compare register	*/

	outportb( 0x3D4, 0x18 );
	outportb( 0x3D5, row );
	}









































