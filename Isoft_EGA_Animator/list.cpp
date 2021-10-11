/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:	List of herous.
 *
 *      $Log:$
 *
 *
\*/

#ifndef	STYLE_H
#	include	<style.h>
#endif

#include	"scene.h"


#include	"list.h"


void
list::ins( hero *h ) {
	lhero	*temp;
	lhero	*t1;

	temp = new lhero( h );
	if( temp == NULL )
		exit(1);			// BUG!!!!!

	if( curr == NULL ) {			// This is a first.
		curr = first = last = temp;
		return;
		}

	temp->prev = curr->prev;
	temp->next = curr;
	curr->prev = temp;
	
	if( curr == first )
		first = temp;

	curr = temp;
	}

void
list::del( hero *h ) {
	lhero	*t;

	if( first == NULL )
		return;

	curr = first;

	do {
		if( curr->data == h ) {
			del();
			return;
			}

		} while( curr );
	}


void
list::del( void ) {
	lhero	*temp = curr;

	if( curr == NULL )
		return;

	if( curr == first && curr == last ) {
		delete	curr;
		curr = first = last = NULL;
		return;
		}

	if( curr == first ) {
		curr->next->prev = NULL;
		first = temp	 = curr->next;
		delete	curr;
		curr = temp;
		return;
		} 

	if( curr == last ) {
		curr->prev->next = NULL;
		last = temp	 = curr->prev;
		delete	curr;
		curr = temp;
		return;
		} 

	temp = curr->next;

	curr->prev->next = curr->next;
	curr->next->prev = curr->prev;
	delete	curr;
	curr = temp;
	}

list::~list( void ) {
	lhero	*t;

	if( first == NULL )
		return;

	curr = first;

	do {
		t = curr->next;
		delete	curr;
		curr = t;
		} while( curr );
	}