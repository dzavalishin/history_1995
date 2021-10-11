/**-------------------   Graphics Windows   -------------------**\
 *                                                              
 *     Copyright (C) 1989 by DZ - Moscow, 110.6728		
 *                                                              
 *	$Log:   C:/c/gw/vcs/vpage.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:19:56   dz
 *      Initial revision.
 *
 *     Module :   Video Page support.                           
 *                                                              
 *     Revision 1.1     27 Dec 89                               
 *       Initial revision.                                      
 *                                                              
 *								
\**------------------------------------------------------------**/


#include "local.h"
#include "ega.h"
#include "gw.h"
#include "mouse.h"

/*#include <.h>			*/
/*#include <.h>			*/
/*#include <.h>			*/
/*#include <.h>			*/


#define	P0_BASE		0xA000
#define P1_BASE		0xA800

#define	page_base( p )	( p ? P1_BASE : P0_BASE );

int	_GW_vpage	= 0;
int	_GW_apage	= 0;

int	_GW_vpb		= P1_BASE;	/* Visual page base address	*/
int	_GW_apb		= P0_BASE;	/* Active page base address	*/



/**Doc************************************************************************
 *		NOT IMPLEMENTED !!!
 *
 *	Name:	gwvpage			Header: gw.h
 *
 *	Synopsis: bool gwvpage( int npage )
 *
 *	Parameters:
 *		int		npage	- Number of the visual page, 0 - 1.
 *
 *	Return value:		Err/Ok
 *
 *	Description:
 *		Sets visual graphics page. Page number is in range
 *		0 to 1.
 *
 *	See also:
 *		gwapage
**/



bool
gwvpage( int npage ) {
	assert_init;
	assert_page( npage );

	mouse_off();

	_GW_vpage	= npage;
	_GW_vpb		= page_base( npage );
	setvisualpage( npage );

	mouse_on();
	return Ok;
	}


/**Doc************************************************************************
 *		NOT IMPLEMENTED !!!
 *
 *	Name:	gwapage			Header: gw.h
 *
 *	Synopsis: bool gwapage( int npage )
 *
 *	Parameters:
 *		int		npage	- Number of the active page, 0 - 1.
 *
 *	Return value:		Err/Ok
 *
 *	Description:
 *		Sets active graphics page. Page number is in range
 *		0 to 1. Subsequent graphics output goes to given page.
 *		This may not be a current visual page.
 *
 *	See also:
 *		gwvpage
**/


bool
gwapage( int npage ) {
	assert_init;
	assert_page( npage );

	_GW_apage	= npage;
	_GW_apb		= page_base( npage );

	setactivepage( npage );
	return Ok;
	}









































