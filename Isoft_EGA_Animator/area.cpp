/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:	Class area support.
 *
 *      $Log:   D:/animate/vcs/area.cpv  $
 *      
 *         Rev 1.0   18 Dec 1990 04:25:30   dz
 *      Initial revision.
 *
 *
\*/

#include	<style.h>
#include	<stdlib.h>

#include	"area.h"

area
area::operator + ( area &z2 ) {
	area	r;

	r.x1 = x1 < z2.x1 ? x1 : z2.x1;
	r.x2 = x2 > z2.x2 ? x2 : z2.x2;

	r.y1 = y1 < z2.y1 ? y1 : z2.y1;
	r.y2 = y2 > z2.y2 ? y2 : z2.y2;

	return r;
	}


area &
area::operator += ( area &z2 ) {
	if( z2.x1 < x1 )	x1 = z2.x1;
	if( z2.x2 > x2 )	x2 = z2.x2;

	if( z2.y1 < y1 )	y1 = z2.y1;
	if( z2.y2 > y2 )	y2 = z2.y2;

	return *this;
	}

bool	
area::operator && ( area & z2 ) {
	if(

	x1 > z2.x2	||	x2 < z2.x1	||
	y1 > z2.y2	||	y2 < z2.y1

	)
		return No;

	return Yes;
	}


