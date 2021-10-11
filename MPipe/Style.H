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

#ifdef __cplusplus

#ifdef __GNUC__

#define On     1
#define Off    0
#define True   1
#define False  0
//#define Err   -1
//#define Ok     0
#define Yes    1
#define No     0

#else

enum bool {
	On   	= 1,	Off   	= 0,
	True 	= 1,	False 	= 0,
//	Err  	= -1,	Ok    	= 0,
	Yes  	= 1,	No    	= 0
	};
#endif // GNUC

#else

typedef enum {
	On	= 1,	Off	= 0,
	True	= 1,	False	= 0,
//	Err	= -1,	Ok	= 0,
	Yes	= 1,	No	= 0
	} bool;

#endif // __cplusplus


typedef unsigned int	uint;
typedef unsigned long	ulong;
typedef unsigned short	ushort;
typedef unsigned char	uchar;





#endif // STYLE_H
