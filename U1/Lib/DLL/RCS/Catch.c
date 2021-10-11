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
@/************************ UUCP to FIDO convertor ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Signals catcher
 *
 *      $Log: Catch.c $
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *         Rev 1.2   23 Oct 1992 15:14:14   dz
 *      
 *         Rev 1.1   05 Mar 1992 21:13:08   dz
 *      
 *         Rev 1.0   12 Sep 1991 01:31:06   dz
 *      Initial revision.
 *
 *
\*/


#include	"log.h"
#include	<stdio.h>
#include	<uu2io.h>
#include	<fcntl.h>
#include	<stdlib.h>
#include	<time.h>
#include	<errno.h>
#include	<string.h>

#include	"setup.h"
#include	"stat.h"
#include	<signal.h>



void
sig_catch( int sig )
	{
	fatal( EC_Signal_Caught, EI_None, "Signal %d caught!", sig );
	}


void
sig_skip( int )
	{
	// Just ignore it
	printf("\nCatch: ^C ignored\n");
	}


void
set_traps( void )
	{

	signal( SIGABRT, sig_catch );
	signal( SIGSEGV, sig_catch );
	signal( SIGFPE,  sig_catch );
	signal( SIGILL,  sig_catch );

	signal( SIGTERM, sig_skip );
	signal( SIGINT,  sig_skip );
	}


//#prag ma start up set_traps 64
@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/fido/vcs/catch.c_v  $
@
