/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:	Class hero c'tor / d'tor
 *
 *      $Log:   D:/animate/vcs/hero_cd.cpv  $
 *      
 *         Rev 1.1   09 Jan 1991 06:49:08   dz
 *      hero::draw
 *      
 *         Rev 1.0   18 Dec 1990 04:25:10   dz
 *      Initial revision.
 *
 *
\*/

#include	<style.h>
#include	<stdlib.h>

#include	"animate.h"
#include	"hero.h"


hero::hero( uint priority ) {
	pic	= NULL;
	x = y	= 0;
	prio	= priority;
	visible	= No;
	changed	= No;
	}

hero::~hero( void ) {
	}

void
hero::draw( void ) {
	pic->draw( x, y, 0, 0, pic->xs, pic->ys );
	}