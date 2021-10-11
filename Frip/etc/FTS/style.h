/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Global types
 *
 *      $Log: style.h $
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
	Err  	= -1,	Ok    	= 0,
	Yes  	= 1,	No    	= 0
	};

enum success    {    Err = -1, Ok = 0    };

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
