head	1.2;
access;
symbols;
locks;
comment	@ * @;


1.2
date	95.04.13.11.06.28;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.04.17;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@debug
@
text
@/********************** UU2 Rules sender interface *************************\
 *
 *	Copyright (C) 1991,1992 by Infinity Soft
 *
 *	Module 	:	Rules sender interface
 *
 *      $Log: New_Usr.h $
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   07 Jun 1993 17:58:10   dz
 *      Initial revision.
 *
 *
\*/


#include	<style.h>

class _Export rules_sender
	{
	public:

	static bool	active( void );

	static bool	add_address( const char *group, const char *address );
	static bool	for_each_address( bool (*func)( const char *group, const char *address) );
	};











@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/uu2/vcs/new_usr.h_v  $
d24 1
a24 1
class rules_sender
@
