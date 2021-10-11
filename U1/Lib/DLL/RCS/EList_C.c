head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	95.04.13.11.06.28;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.06.19.46.16;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.04.17;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@debug
@
text
@/************************ UU2 EchoList ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	EchoList cache members
 *
 *      $Log: EList_C.c $
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






@


1.2
log
@Echolist cache size limit removed, so it is not
a cache anymore - we just loading all the descriptions
at once and use 16Kb buffer when loading - such a strategy
speeds things up to twice.
@
text
@d8 12
@


1.1
log
@Initial revision
@
text
@d7 3
a9 13
 *      $Log:   Q:/lib/uu2/vcs/elist_c.c_v  $
 *      
 *         Rev 1.3   07 Jun 1993 17:46:36   dz
 *      Partially rewritten
 *      
 *         Rev 1.2   15 Nov 1992 00:00:30   dz
 *      debugs disabled
 *      
 *         Rev 1.1   23 Oct 1992 15:49:06   dz
 *      error()
 *      
 *         Rev 1.0   16 Oct 1992 03:43:00   dz
 *      Initial revision.
d12 1
a29 12
#ifdef __OS2__
#define		MEM_2_USE	(100*1024)
#else
#define         MEM_2_USE       (10*1024)
#endif

long		el_cache::mem_to_use	= MEM_2_USE;
long		el_cache::mem_used		= 0;

const el_size	= sizeof( el_cache );


a36 2
	mem_used += el_size;
	took_size = 0;
a38 9


	while( mem_used > mem_to_use )
		if( release_one() == Err )
			{
			error( EI_None, "Elc: EchoList cache unable to fit into the given memory limits!");
			break;
			}

a43 1
	mem_used -= el_size;
a45 1

a57 2
	if( (el_size + echo_def::size()) >= (mem_to_use - mem_used) )
		return No;
a72 1
//	took_size = 0;		// Я не понимаю вообще, какого чоpта оно тут обнулялось
a159 4
	if( last == NULL )
		return Err;

	delete( last );
a182 6
	if( !will_fit() )
		{
//        debug("Elc: Will not fit in given memory");
		return Err;
		}

d250 1
a250 1
take_size(int s)
a251 3
	release_size();
	took_size = s;
	mem_used += took_size;
a256 2
	mem_used -= took_size;
	took_size = 0;
@
