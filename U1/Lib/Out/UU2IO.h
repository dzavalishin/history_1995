/************************ UU2 library ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   An portable replacement for DOS io.h
 *
 *      $Log: UU2IO.h $
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
 *         Rev 1.0   07 Jun 1993 18:02:04   dz
 *      Initial revision.
 *
 *
\*/

#if defined(__MSDOS__) || defined(__OS2__) || defined(__EMX__)
#include    <io.h>
#else
#include <unistd.h>
#endif


