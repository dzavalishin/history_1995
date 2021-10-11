/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:	List support.
 *
 *      $Log:   D:/animate/vcs/list.cpv  $
 *      
 *         Rev 1.0   18 Dec 1990 05:45:14   dz
 *      Initial revision.
 *
 *
\*/

#include	<style.h>
#include	<stdlib.h>

#include	"animate.h"
#include	"list.h"


/****************************************************************************
			Class `list_root'
****************************************************************************/



		/*********************************************
				Positioning
		*********************************************/


void
list_root::go_next( void ) {
	list_elem	*n;

	if( (n = curr->next) == NULL )
		return;

	curr = n;
	}


void
list_root::go_prev( void ) {
	list_elem	*n;

	if( (n = curr->prev) == NULL )
		return;

	curr = n;
	}


		/*********************************************
				Search
		*********************************************/

bool		
list_root::find( list_elem *search_for ) {
	list_elem	*pp;

	for( pp = first; pp; pp = pp->next ) {
		if( pp == search_for ) {
			curr = pp;
			return Yes;
			}
		}
	return No;
	}


		/*********************************************
				Insert/Delete
		*********************************************/


void
list_root::ins( list_elem *new_el ) {

	if( new_el == NULL )
		return;

	if( first == NULL ) {
		
		first	=
		curr	=
		last	= new_el;

		return;
		}

	if( curr == NULL )
		curr = first;

	new_el->next	= curr;
	new_el->prev	= curr->prev;
	curr->prev	= new_el;

	if( curr->prev )	
		curr->prev->next	= new_el;
	}



void
list_root::del( void ) {		// Delete current

	if( first == NULL )		// Nothing to do ...
		return;			// ... oh, baby, stay in bed.

	if( curr == NULL )		// No current ??
		return;

	if( first == last ) {		// Only one
		curr->next = NULL;
		curr->prev = NULL;

		first	=
		curr	=
		last	= NULL;

		return;
		}

	if( curr->next )	curr->next->prev = curr->prev;
	if( curr->prev )	curr->prev->next = curr->next;

	curr->next = NULL;
	curr->prev = NULL;
	}

bool
list_root::del( list_elem *to_delete ) {
	list_elem	*save = curr;

	if( find( to_delete ) != Yes )
		return Err;

	del();

	if( save != to_delete )
		curr = save;

	return Ok;
	}


		/*********************************************
				Desctructor
		*********************************************/

list_root::~list_root( void ) {
	}




/****************************************************************************
			Class `list_elem'
****************************************************************************/


		/*********************************************
				Desctructor
		*********************************************/

list_elem::~list_elem( void ) {
	if( root ) {
		root->del( this );
		root = NULL;
		}
	}



