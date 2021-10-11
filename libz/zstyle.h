/*\
 *	Copyright (C) 1989-1996 by Infinity Soft
 *
 *	Module:	Global types
 *
 *      $Log: zstyle.h $
 *      Revision 1.2  1996/09/23 03:32:01  dz
 *      *** empty log message ***
 *
 *      Revision 1.1  1996/09/23 03:19:42  dz
 *      Initial revision
 *
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

#if (!defined(ZSTYLE_H)) && (!defined(STYLE_H))
#define ZSTYLE_H

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
	Yes  	= 1,	No    	= 0
	};

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
	Yes	= 1,	No	= 0
	} bool;

#endif //	__cplusplus


typedef unsigned int	uint;
typedef unsigned long	ulong;
typedef unsigned short	ushort;
typedef unsigned char	uchar;





#endif // ZSTYLE_H
