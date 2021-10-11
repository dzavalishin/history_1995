/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:	Class hero.
 *
 *      $Log:   D:/animate/vcs/hero.h_v  $
 *      
 *         Rev 1.0   18 Dec 1990 04:26:10   dz
 *      Initial revision.
 *
 *
\*/

#ifndef	HERO_H
#define	HERO_H

#ifndef	STYLE_H
#	include	<style.h>
#endif

#ifndef	ANIMATE_H
#	include	"animate.h"
#endif

#ifndef	SCENE_H
#	include	"scene.h"
#endif


class hero {

	friend	scene;


	image		*pic;		// Current picture
	int		x, y;		// Current position on the screen
	uint		prio;		// Current priority (deepest = 0)
	bool		visible;	// Is it visible now ?

	bool		changed;	// Something changed

public:
		/*********************************************
				C'tor/d'tor
		*********************************************/


	hero( uint priority = 0 );
	~hero( void );

		/*********************************************
				Set state
		*********************************************/

	int	get_x( void ) { return	x; }
	int	get_y( void ) { return	y; }
	int	get_xs( void ) { return	pic->xs; }
	int	get_ys( void ) { return	pic->ys; }

	bool	is_visible( void ) { return visible; }

	void set_image( image *new_img ) {	
		pic = new_img;	
		changed = Yes;	
		if( pic == NULL )		// Can't be inline because
			visible = No;		// of this if() !!
		}

	void set_position( int new_x, int new_y ) {	
		x = new_x; 
		y = new_y;	
		changed = Yes;	
		}

	void set_priority( int new_prio ) {	
		prio = new_prio;
		changed = Yes;	
		}

	int	get_priority( void ) {
		return	prio;
		}

	void set_visibility_status( bool is_visible ) {
		changed = Yes;	
		if( pic == NULL ) {		// Can't be inline also ...
			visible = No;
			return;
			}
		visible = is_visible;
		}


		/*********************************************
				Display
		*********************************************/

	void	draw( void );

	};

#endif	HERO_H
