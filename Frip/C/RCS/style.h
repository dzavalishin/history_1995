head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.07.31.07.00.23;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
log
@Version 25
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Global types
 *
 *      $Log: style.h $
 *      Revision 1.3  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.2  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.1  1996/07/31 07:00:23  dz
 *      Initial revision
 *
 *      Revision 1.3  1996/01/24 22:33:07  dz
 *      class success
 *
 *      Revision 1.2  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

/**----------------  Infinity Soft C toolkit -------------------**\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:  Common style header.
 *
 *	Revision 1.2	13 Mar	1990
 *		New 'bool' data types was added [MS]
 *	Revision 1.1	14 Aug	1989
 *		Initial revision.
 *
\**------------------------------------------------------------**/

#ifndef STYLE_H
#define STYLE_H

/*
	This header file written for TC C++ compiler
*/


/**
 *	Data types
**/


#if defined(__cplusplus)

#if defined(__GNUC__)
#  define bool dz_bool
#endif //	GNUC

enum bool {
	On   	= 1,	Off   	= 0,
//	True 	= 1,	False 	= 0,
//	Err  	= -1,	Ok    	= 0,
	Yes  	= 1,	No    	= 0
	};

//enum success    {    Err = -1, Ok = 0    };

/*

inline bool operator && ( bool l, bool r )
    { return (bool) ::operator && (int(l), int(r)); }

inline bool operator || ( bool l, bool r )
    { return (bool) ( ::operator || ( int(l), int(r) )); }

inline bool operator ! ( bool l )
    { return l ? No : Yes; }

*/
    
/*
inline operator bool ( int a )
    { return a ? Yes : No; }

inline operator int ( bool a )
    { return (int)a; }
    */

    
#else

typedef enum {
	On	= 1,	Off	= 0,
	True	= 1,	False	= 0,
//	Err	= -1,	Ok	= 0,
	Yes	= 1,	No	= 0
	} bool;

#endif //	__cplusplus


typedef unsigned int	uint;
typedef unsigned long	ulong;
typedef unsigned short	ushort;
typedef unsigned char	uchar;





#endif // STYLE_H
@


1.3
log
@Exceptions added, not compiled
@
text
@d8 3
d68 22
@


1.2
log
@Before exceptions.
@
text
@d8 3
d59 1
a59 1
	Err  	= -1,	Ok    	= 0,
d70 1
a70 1
	Err	= -1,	Ok	= 0,
@


1.1
log
@Initial revision
@
text
@d8 3
@
