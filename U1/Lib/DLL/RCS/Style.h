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
@/**----------------  Infinity Soft C toolkit -------------------**\
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

#ifdef __cplusplus

enum bool {
	On   	= 1,	Off   	= 0,
//	True 	= 1,	False 	= 0,
	Err  	= -1,	Ok    	= 0,
	Yes  	= 1,	No    	= 0
	};


#else

typedef enum {
	On	= 1,	Off	= 0,
	True	= 1,	False	= 0,
	Err	= -1,	Ok	= 0,
	Yes	= 1,	No	= 0
	} bool;

#endif //	__cplusplus


typedef unsigned int	uint;
typedef unsigned long	ulong;
typedef unsigned short	ushort;
typedef unsigned char	uchar;





#endif // STYLE_H
@


1.1
log
@Initial revision
@
text
@d30 1
a30 1
	True 	= 1,	False 	= 0,
d45 1
a45 1
#endif	__cplusplus
d57 1
a57 1
#endif STYLE_H
@
