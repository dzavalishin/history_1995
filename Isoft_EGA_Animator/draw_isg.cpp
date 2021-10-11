/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:	Display part of picture.
 *
 *      $Log:   D:/animate/vcs/draw_isg.cpv  $
 *      
 *         Rev 1.2   09 Jan 1991 06:41:18   gn
 *      Conditional EGA reset.
 *      
 *         Rev 1.1   18 Dec 1990 02:31:04   dz
 *      EGA reset & write mode set moved
 *      from ega::wline.
 *      
 *         Rev 1.0   11 Dec 1990 05:28:58   dz
 *      Initial revision.
 *
 *
\*/

#include	<style.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<graphics.h>

#include	"animate.h"
#include	"ega.h"

void
image::draw( int x, int y, int xb, int yb, int xsz, int ysz ) {
	int		ey = yb+ysz;
	char huge	*pp;
	static	bool	first_pass = Yes;


	if( first_pass == Yes ) {
		ega::reset();		// Reset EGA hardware registers
		ega::setm2();		// Select write mode 2
		first_pass = No;
		}

	pp = ((char huge *)data)+((long)xb+(yb*(long)xs));

	int	x_xb = x + xb;
	for( int cy = yb+y; cy < ey+y; cy++ ) {
		ega::wline( pp, xsz, x_xb, cy );
		pp += xs;
		}

	}

