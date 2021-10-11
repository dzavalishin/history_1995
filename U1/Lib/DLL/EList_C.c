/************************ UU2 EchoList ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	EchoList cache members
 *
 *      $Log: EList_C.c $
 * Revision 1.3  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.2  1995/04/06  19:46:16  dz
 * Echolist cache size limit removed, so it is not
 * a cache anymore - we just loading all the descriptions
 * at once and use 16Kb buffer when loading - such a strategy
 * speeds things up to twice.
 *
 * Revision 1.2  1995/04/06  19:46:16  dz
 * Echolist cache size limit removed, so it is not
 * a cache anymore - we just loading all the descriptions
 * at once and use 16Kb buffer when loading - such a strategy
 * speeds things up to twice.
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *
 *
\*/

#include	<stdio.h>
#include	<stdlib.h>
#include	<share.h>
#include	<style.h>

#include	"setup.h"
#include	"log.h"

#include	"elist.h"
#include	"elist_c.h"


el_cache	*el_cache::first	= NULL;
el_cache	*el_cache::last		= NULL;

/**
 *			C'tor/D'tor
**/


el_cache::el_cache( void )
	{
	inserted = No;
	next = prev = NULL;
	}


el_cache::~el_cache( void )
	{
	if( inserted )
		detach();
	}


bool
el_cache::valid() const
	{
	return Yes;
	}

bool
el_cache::will_fit() const
	{
	return Yes;
	}


/**
 *			Insertion/deletion
**/


bool
el_cache::attach()							// To the beginning of chain
	{
	if( inserted || (!valid()) )			// Already in chain or bad one
		return Err;

	take_size();

	inserted = Yes;

	if( first == NULL )						// We are the first
		{
		first = this;
		last = this;
		return Ok;
		}

	next = first;
	prev = NULL;
	first->prev = this;
	first = this;

	return Ok;
	}

bool
el_cache::attach_to_end()					// To the end of chain
	{
	if( inserted || (!valid()) )			// Already in chain or bad one
		return Err;

	take_size();

	inserted = Yes;

	if( first == NULL )						// We are the first
		{
		first = this;
		last = this;
		return Ok;
		}

	next = NULL;
	prev = last;
	last->next = this;
	last = this;

	return Ok;
	}


bool
el_cache::detach()							// Delete from chain
	{
	if( !inserted )
		return Err;

	if( first == NULL || last == NULL )
		{
		fatal( EC_Got_Insane, EI_None, "EchoList cache got insane..." );
		// ...no return
		}

	inserted = No;

	if( first == this && last == this )
		{
		first = last = NULL;
		}
	else if( first == this )
		{
		first = next;
		first->prev = NULL;
		}
	else if( last == this )
		{
		last = prev;
		last->next = NULL;
		}
	else
		{
		next->prev = prev;
		prev->next = next;
		}

	next = prev = NULL;
	return Ok;
	}


bool
el_cache::release_one()
	{
	return Ok;
	}

/**
 *			Misc.
**/

bool
el_cache::reinsert()
	{
	detach();
	return attach();
	}

bool
el_cache::try_to_insert()
	{
	if( have_in_list() )
		{
//        debug("Elc: Already in cache");
		return Err;
		}

//    debug("Elc: Trying to add to end");
	return attach_to_end();
	}

/**
 *			Search
**/


el_cache *
el_cache::get_by_group( const char *a_group )
	{
	el_cache	*p;

	for( p = first; p; p = p->next )
		{
		if( stricmp( a_group, p->group().c_str() ) == 0 )
			{
			p->reinsert();							// Reorder list
			return p;
			}
		}

	return NULL;									// Not found
	}


el_cache *
el_cache::get_by_area( const char *a_area )
	{
	el_cache	*p;

	for( p = first; p; p = p->next )
		{
		if( stricmp( a_area, p->area().c_str() ) == 0 )
			{
			p->reinsert();							// Reorder list
			return p;
			}
		}

	return NULL;									// Not found
	}


bool
el_cache::have_in_list() const
	{
	el_cache	*p;

	for( p = first; p; p = p->next )
		{
		if( stricmp( area().c_str(), p->area().c_str() ) == 0 &&
			stricmp( group().c_str(), p->group().c_str() ) == 0 )
			return Yes;
		}

	return No;										// Not found
	}



	/*************************************************************
						   Size
	*************************************************************/

void el_cache::
take_size(int )
	{
	}

void el_cache::
release_size()
	{
	}






