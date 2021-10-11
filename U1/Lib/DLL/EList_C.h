/************************ UU2 EchoList ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	EchoList cache header
 *
 *      $Log: EList_C.h $
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *         Rev 1.1   07 Jun 1993 17:57:34   dz
 *      update
 *      
 *         Rev 1.0   15 Nov 1992 02:07:34   dz
 *      Initial revision.
 *
 *
\*/

#ifndef ELIST_C_H
#define ELIST_C_H

#include	<stdlib.h>
#include	<style.h>


#include	"elist.h"

#pragma pack(1)


class _Export el_cache: public echo_def
	{
	int					took_size;
	void				take_size( int s );
	void				take_size( void ) { take_size( echo_def::size() ); }
	void				release_size();

	friend bool			get_area_for_group( char *area, const char *group );
	friend bool			get_group_for_area( char *group, const char *area );

	el_cache			*next;			// Next in double-linked list
	el_cache			*prev;			// Previous

	static el_cache		*first;			// Beginning of list
	static el_cache		*last;			// Ok, got it? :)

	bool				inserted;		// This el. is in the chain

	bool				attach_to_end();// To the end of chain
	bool				detach();		// From the chain

	static bool			release_one();	// Drop last one

	static long			mem_to_use;		// How many bytes we can use
	static long			mem_used;		// And really used

	bool				reinsert();		// Move to beginning of chain

public:
						el_cache();
						~el_cache();

	bool				valid() const;		// Has good data

	static el_cache		*get_by_group( const char *group );
	static el_cache		*get_by_area ( const char *area  );

	bool				have_in_list() const;		// Do we have such a record in a list?
	bool				will_fit() const;			// Do we have free memory for this element?
	bool				try_to_insert();			// If we have unused memory, and this element is not in list yet, insert it into the end of the list
	bool				attach();					// To the beginning of chain

	};


#pragma pack(1)

#endif
