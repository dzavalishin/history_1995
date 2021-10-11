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
 *	Module 	:	.TIC file keywords parser header
 *
 *      $Log: Tic_kw.h $
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   19 Jan 1993 20:58:18   dz
 *      Initial revision.
 *
 *
\*/

#ifndef	TIC_KW_H
#define	TIC_KW_H

#include    "style.h"

#pragma pack(1)

class _Export tic_kw
	{

public:
	enum tic_token {
		no_token,
		app,		area,		crc,		created,		date,
		desc,		file,		from,		origin,			path,
		pw,			release,	replaces,	seenby,		size
		};

private:
	static bool	in_use;		// Only one instance of class may be used at a time
	bool		valid;

	public:
				tic_kw();
				~tic_kw();

	bool		invalid( void ) { return (bool) !valid; }

	tic_token	parse( char *&keyw );
	bool		finish( void );
	};

#pragma pack()

#endif

@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/file/file2uu/vcs/tic_kw.h_v  $
d28 1
a28 1
class tic_kw
@
