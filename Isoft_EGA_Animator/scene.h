/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:	The scene header.
 *
 *      $Log:$
 *
 *
\*/

#ifndef	SCENE_H
#define	SCENE_H

#ifndef	STYLE_H
#	include	<style.h>
#endif

#ifndef	AREA_H
#	include	"area.h"
#endif

#ifndef	LIST_H
#	include	"list.h"
#endif

#ifndef	EGA_H
#	include	"ega.h"
#endif


#define	MAXHEROUS	265

class	scene {
//	friend	list;

	area		page[2][MAXHEROUS];
	list		curr;
	int		currpage;		// Current video page
	int		bads[2];

	bool		rest_back( void );
public:

	scene( void );
	
	void	add( hero *h );
	void	del( hero *h );
	void	draw( void );
	};


inline	void	scene::add( hero *h ) {	curr.ins( h ); }
inline	void	scene::del( hero *h ) {	curr.del( h ); }


inline	scene::scene( void ) {

	bads[0] = bads[1]	= 0;
	currpage        	= 1;
	}

#endif	SCENE_H