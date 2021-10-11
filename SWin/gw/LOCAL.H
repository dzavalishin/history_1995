/**--------------------   Graphics Windows   -------------------**\
 *                                                               
 *     Copyright (C) 1989 by DZ - Moscow, 110.6728		 
 *                                                               
 *     Module : Local definitions for graphics windows           
 *                                                               
 *	$Log:   C:/c/gw/vcs/local.h_v  $
 * 
 *    Rev 1.0   29 Apr 1990 16:21:14   dz
 * Initial revision.
 *
 *	   Rev 1.0     20.08.89                                 
 *	Initial revision.                                       
 *                                                               
 *                                                               
\**-------------------------------------------------------------**/

#ifndef LOCAL_H
#define LOCAL_H


#ifndef GW_H
#include "gw.h"
#endif

/*	extern char far	*XCbuf[4];	*/

#define	PLANEBUF_SIZE	28000

extern char far		*planebuf;	/* One plane ( 28000 ) buffer	*/

extern bool	gw_inited;
extern int	Gresult;

extern int	Xbase,
		Ybase,
		Xend,
		Yend;

extern int	_GW_vpage;
extern int	_GW_apage;

extern int	_GW_vpb;	/*	Visual page base	*/
extern int	_GW_apb;	/*	Active page base	*/



#define	assert_init		if( !gw_inited ) return Err
#define assert_xy( x, y )	if( check_xy( x, y ))	return Err
#define	assert_page( np )	if( (np)<0 || (np)>1 )	return Err


void	gw_swap_images( gwin *window, bool restore_only );
void	gw_set_win_pars( gwin *window );




#endif LOCAL_H