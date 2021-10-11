head	1.2;
access;
symbols;
locks;
comment	@ * @;


1.2
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@Exceptions added, not compiled
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	OS Dependent Thingies - Win NT/ Win 95
 *
 *      $Log: dep_nt_i.c $
 *      Revision 1.1  1996/08/04 08:18:43  dz
 *      Initial revision
 *
 *      Revision 1.2  1996/01/14 01:06:31  dz
 *      beta 14
 *
 *      Revision 1.1  1996/01/07 20:21:01  dz
 *      Initial revision
 *
 *
 *
\*/

/*

void OS_Type( void )
{
    OSVERSIONINFO VerInfo;

    GetVersionEx( &VerInfo );
    if( GetVersionEx( VerInfo ) && ( VerInfo.dwPlatformId == VER_PLATFORM WIN32_NT ));
}

*/

@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: dep_nt.ic $
@
