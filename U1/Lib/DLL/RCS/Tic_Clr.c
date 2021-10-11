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
date	95.03.11.18.00.54;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@debug
@
text
@/************************ UU2 FILE2UU ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Clear container
 *
 *      $Log: Tic_Clr.c $
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 *      
 *         Rev 1.1   07 Jun 1993 17:39:04   dz
 *      Checks for empty container before flushing it
 *      
 *         Rev 1.0   29 Dec 1992 01:31:38   dz
 *      Initial revision.
 *
 *
\*/

#include    "tic.h"


void tic_file::
clear( void )
	{
	from_v.clear();
	origin_v.clear();
	receipt_v.clear();
	send_receipt_v = No;

	if( path_v.getItemsInContainer() != 0 )
		path_v.flush();

	if( seenby_v.getItemsInContainer() != 0 )
		seenby_v.flush();

	date_v = (time_t)0;
	size_v = 0L;
	crc_v  = 0L;
	passwd_v[0] = '\0';

	source_v.clear();
	area_v[0] = '\0';
	name_v[0] = '\0';

	if( replaces_v.getItemsInContainer() != 0 )
		replaces_v.flush();

	release_v = (time_t)0;

	if( desc_v.getItemsInContainer() != 0 )
		desc_v.flush();

	}

















@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/fido/vcs/tic_clr.c_v  $
@
