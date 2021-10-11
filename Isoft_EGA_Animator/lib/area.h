/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:	Class area.
 *
 *      $Log:   D:/animate/vcs/area.h_v  $
 *      
 *         Rev 1.0   18 Dec 1990 04:25:48   dz
 *      Initial revision.
 *
 *
\*/

#ifndef	AREA_H
#define	AREA_H

#ifndef	STYLE_H
#	include	<style.h>
#endif


class area {

public:
	int	x1, x2;
	int	y1, y2;

	area	operator +	( area &__z2 );
	area &	operator +=	( area &__z2 );

	bool	operator &&	( area &__z2 );

	};



#endif	AREA_H