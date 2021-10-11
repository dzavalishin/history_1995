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
 *	Copyright (C) 1993 by Infinity Soft
 *
 *  Module  :   An portable replacement for DOS dir.h
 *
 *      $Log: UU2Dir.h $
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   07 Jun 1993 18:01:58   dz
 *      Initial revision.
 *
 *
\*/

#if defined(__MSDOS__) || defined(__BORLANDC__)
#include    <dir.h>
#else
#ifdef __IBMCPP__

#else
#include <unistd.h>
#endif
#endif


@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/uu2/vcs/uu2dir.h_v  $
d24 3
d28 1
@
