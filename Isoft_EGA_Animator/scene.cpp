/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:	Scene.
 *
 *      $Log:$
 *
 *
\*/

#ifndef	STYLE_H
#	include	<style.h>
#endif

#include	"scene.h"

#define		CP	page[currpage]


bool
scene::rest_back( void ) {
	hero	*t;
	bool	found = No;

	t = curr.top();

	//
	//	Find background
	do {
		if( t->prio == 0 ) {
			found = Yes;
			break;		
			}

		t = curr.next();
		} while( t != NULL );

	if( found != Yes )
		return	Err;

	if( bads[currpage] == 0 ) {
		t->draw();
/*		ega::active_page( !currpage );
		t->draw();
		ega::active_page( currpage );
*/		return	Ok;
		};

	for( int i = 0; i < bads[currpage]; i++ )
		t->pic->draw( 0, 0, 	CP[i].x1, CP[i].y1, 
					CP[i].x2, CP[i].y2 );

	return	Ok;
	}


void
scene::draw( void ) {
	int	level;
	hero	*t;
	bool	d = Yes;

	ega::active_page( currpage );

	if( rest_back() == Err )
		return;				// Background not found!!!

	level = 0;
	bads[currpage]  = 0;

	while( d == Yes ) {
		t = curr.top();
		d = No;
		level++;

		do {
			if( t->get_priority() != level )
				continue;

			if( t->is_visible() == No )
				continue;

			//
			//	Found, display it and change
			//	background's bad list.
			//

			CP[bads[currpage]].x1 = t->get_x();
			CP[bads[currpage]].x2 = t->get_xs();
			CP[bads[currpage]].y1 = t->get_y();
			CP[bads[currpage]].y2 = t->get_ys();

			bads[currpage]++;

			t->draw();
			d = Yes;
			} while( (t = curr.next()) != NULL );

		}

	ega::visual_page( currpage );
	currpage = !currpage;
	}