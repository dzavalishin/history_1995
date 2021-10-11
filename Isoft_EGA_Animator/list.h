/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:	List - header file
 *
 *      $Log:$
 *
 *
\*/

#ifndef	LIST_H
#define	LIST_H

#ifndef	STYLE_H
#	include	<style.h>
#endif

#ifndef	__STDLIB_H
#	include	<stdlib.h>
#endif

#ifndef	HERO_H
#	include	"hero.h"
#endif

class	list;

class	lhero {
	friend	list;
	friend hero;

public:
	lhero		*next;			// Next hero			
	lhero		*prev;			// Previous hero
	hero		*data;			// The hero

	lhero( hero *h );
	lhero( hero *h, lhero *p, lhero *n );
	};

inline	
lhero::lhero( hero *h ) {
	next = prev = NULL;
	data = h;
	}

inline
lhero::lhero( hero *h, lhero *p, lhero *n ) {

	next	= n;
	prev	= p;
	data	= h;
	}


class	list {

private:

	lhero		*first;
	lhero		*last;
	lhero		*curr;

public:

	list( void )	{ first = last = curr = NULL; }
	~list( void );



	void	ins( hero *h );			// Insert hero before curr
	void	del( void );			// Delete current hero
	void	del( hero *h );
	hero	*get( void );			// Get current hero
	void	put( hero *h );			// Change hero

	hero	*next( void );			// Get next hero
	hero	*prev( void );			// Get prev hero

	hero	*top( void );			// Go first hero
	hero	*bottom( void );		// Go last  hero
	};

inline	hero *
list::get( void ) {
	return	curr->data;
	}

inline	void
list::put( hero *h ) {
	if( curr != NULL )
		curr->data = h;
	else
		ins( h );
	}

inline	hero *
list::next( void ) {
	if( curr == last )
		return	NULL;

	curr = curr->next;
	return	curr->data;
	}

inline	hero *
list::prev( void ) {
	if( curr == first )
		return	NULL;

	curr = curr->prev;
	return	curr->data;
	}

inline	hero *
list::top( void ) {
	curr = first;
	return	curr == NULL ? NULL : curr->data;
	}

inline	hero *
list::bottom( void ) {
	curr = last;
	return	curr == NULL ? NULL : curr->data;
	}

#endif	LIST_H