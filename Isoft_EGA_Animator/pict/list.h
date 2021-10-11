/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:	list_root / list_elem definition.
 *
 *      $Log:   D:/animate/vcs/list.h_v  $
 *      
 *         Rev 1.0   18 Dec 1990 05:44:48   dz
 *      Initial revision.
 *
 *
\*/

#ifndef	STYLE_H
#	include	<style.h>
#endif

class list_root;

class list_elem {

	friend		list_root;

private:
	list_elem	*next;
	list_elem	*prev;

	list_root	*root;

public:
	list_elem	*get_next( void )	{ return next; }
	list_elem	*get_prev( void )	{ return prev; }

	list_elem( list_root *root );
	~list_elem( void );

	};



class list_root {

	friend		list_elem;

private:
	list_elem	*first;
	list_elem	*last;
	list_elem	*curr;

public:
	list_root( void ) { first = curr = last = NULL;	}
	~list_root( void );


public:
	void		go_first( void )	{ curr = first;		}
	void		go_last( void )		{ curr = last;		}

	void		go_next( void );
	void		go_prev( void );

	list_elem	*get_current( void )	{ return curr;		}

	void		for_each( void (*func)( list_elem * ) );

	void		ins( list_elem *new_elem );	// Before current
	bool		del( list_elem *old_elem );	// Err/Ok
	void		del( void );			// Current

	bool		find( list_elem *search_for );	// Yes - found
	};


inline
list_elem::list_elem( list_root *l_root ) {
	root = l_root;

	l_root->ins( this );		// ins() checks for NULL
	}
















