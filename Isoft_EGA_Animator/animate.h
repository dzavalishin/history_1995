/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	: class image definition.
 *
 *      $Log:   D:/animate/vcs/animate.h_v  $
 *      
 *         Rev 1.0   15 Dec 1990 11:20:40   dz
 *      Initial revision.
 *
 *
\*/

#ifndef	ANIMATE_H
#define	ANIMATE_H

#ifndef	STYLE_H
#	include	<style.h>
#endif

#ifndef	ISG_FMT_H
#	include	"isg_fmt.h"
#endif



struct	image {
	int		xs;
	int		ys;
	Summpl	huge	*data;

	image::image( void ) 	{	data = 0;	}
	image::~image( void )	{	remove();	}

	bool	load( char *name );
	void	draw( int x, int y, int xb, int yb, int xsz, int ysz );
	void	draw( int x, int y ) {	draw( x, y, 0, 0, xs, ys ); }
	void	thru( image &see, int sx, int sy );
	void	remove( void );

	};


#endif	ANIMATE_H