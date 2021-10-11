head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.01.18.00.58.45;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@Exceptions added, not compiled
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Lock class
 *
 *      $Log: osname.h $
 *      Revision 1.2  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.1  1996/01/18 00:58:45  dz
 *      Initial revision
 *
 *      Revision 1.1  1996/01/13 09:48:05  dz
 *      Initial revision
 *
 *
 *
 *
\*/


#ifndef OSNAME_H
#define OSNAME_H


#ifndef OS_NAME
#  ifdef __OS2__
#    define OS_NAME "OS/2"
#  endif // __OS2__
#endif // OS_NAME

#ifndef OS_NAME
#  ifdef __NT__
#    define OS_NAME "NT"
#  endif // __NT__
#endif // OS_NAME

#ifndef OS_NAME
#  ifdef __DOS__
#    ifdef __386__
#      define OS_NAME "DOS/32"
#    else // __386__
#      define OS_NAME "DOS/16"
#    endif // __386__
#  endif // __DOS__
#endif // OS_NAME

#ifndef OS_NAME
#  ifdef unix
#    define OS_NAME "Unix"
#  endif // unix
#endif // OS_NAME


#endif // OSNAME_H
@


1.2
log
@Before exceptions.
@
text
@d8 3
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: lock.h $
@
