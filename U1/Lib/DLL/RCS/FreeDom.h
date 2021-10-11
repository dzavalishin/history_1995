head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	95.11.06.02.33.43;	author dz;	state Exp;
branches;
next	1.2;

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


1.3
log
@Fixes & cleanups caused by FTN_Msg introduction
@
text
@/************************ UU2 Library ***************************\
 *
 *	Copyright (C) 1991,1992 by Infinity Soft
 *
 *	Module 	:	Free domains checker
 *
 *      $Log: FreeDom.h $
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
 *         Rev 1.0   07 Jun 1993 17:57:44   dz
 *      Initial revision.
 *
 *
\*/


#ifndef	FREEDOM_H
#define	FREEDOM_H

#ifndef	STYLE_H
#	include	<style.h>
#endif


bool _Export			is_free_address( const string adr_a );


#endif

@


1.2
log
@debug
@
text
@d8 3
d33 1
a33 1
bool _Export			is_free_address( char *adr_a );
@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/uu2/vcs/freedom.h_v  $
d30 1
a30 1
extern bool			is_free_address( char *adr_a );
@
