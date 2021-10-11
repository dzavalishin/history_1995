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
@/************************ UU2 library ***************************\
 *
 *  Copyright (C) 1991, 1992 by Infinity Soft
 *
 *  Module  :   OutNewsSpool mgr
 *
 *      $Log: Spool.h $
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *         Rev 1.4   07 Jun 1993 17:59:18   dz
 *      Update
 *      BC++ for OS/2 pack pragma used to 
 *      ensure struct. fields alignment
 *      
 *         Rev 1.3   17 Nov 1992 04:04:36   dz
 *      Based on Object
 *      
 *         Rev 1.2   17 Nov 1992 03:45:26   dz
 *      Rewritten in C++ :)
 *      
 *         Rev 1.0   24 Oct 1992 18:01:00   dz
 *      Initial revision.
 *
\*/

#ifndef _SPOOL_H_
#define _SPOOL_H_

#include	<style.h>
#include    <stdio.h>
#include    <errno.h>
#include    <time.h>
#include    <string.h>

#include <uu2io.h>

#include    "setup.h"
#include    "log.h"

//#include	<object.h>
#include	"objtypes.h"


#ifdef __OS2__
#pragma pack(1)
#endif

class _Export news_out_spool_file //: Object
	{
	char		name[90];				// Name of spool file
	FILE		*fp_v;					// Pointer to spool file
	bool		valid;					// Yeah, we made valid file
	bool		posted;					// And it was inserted into the list
	bool		created;				// We had a file

public:
				news_out_spool_file( void );
				~news_out_spool_file( void );

	bool        create( void );
	bool		close( void );

	bool		abort( void );
	bool		post( const char *list_info, const char *path = NULL );

	FILE *		fp( void ) const { return fp_v; }

public:		// From Object

//	virtual classType		isA() const { return news_out_spool_fileClass; }
//	virtual char _FAR		*nameOf() const { return "news_out_spool_file"; }
//	virtual hashValueType 	hashValue() const { return 0; }
//	virtual int 			isEqual( const Object _FAR & ) const;
//	virtual void 			printOn( ostream _FAR & ) const;

	};

#ifdef __OS2__
#pragma pack()              // restore default packing
#endif

#endif // _SPOOL_H_

@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/uu2/vcs/spool.h_v  $
d53 1
a53 1
class news_out_spool_file //: Object
@
