head	1.1;
access;
symbols;
locks;
comment	@ * @;


1.1
date	95.05.08.16.04.26;	author dz;	state Exp;
branches;
next	;


desc
@@


1.1
log
@Initial revision
@
text
@/************************ U1 ***************************\
 *
 *  Copyright (C) 1991-1995 by Infinity Soft
 *
 *  Module  :   Rexx functions handler
 *
 *      $Log: Main.c $
 *
**/

#define INCL_RXFUNC
#define INCL_RXSUBCOM
#define INCL_RXSHV
#define INCL_REXXSAA
#include <rexxsaa.h>                   /* needed for RexxStart()     */
#include <stdio.h>                     /* needed for printf()        */
#include <string.h>                    /* needed for strlen()        */
#include <strng.h>                    /* needed for strlen()        */

#include "rexx.h"
#include "dllinits.h"

#include <log.h>
#include <FidoAddr.h>
#include <FTN.h>






LONG   EXPENTRY MatchAKA(
  PSZ        name,                     /* function name              */
  LONG       argc,                     /* count of arguments         */
  PRXSTRING  argv,                     /* argument RXSTRINGs         */
  PSZ        queue,                    /* current Rexx queue         */
  PRXSTRING  retstr );                 /* returned string value      */


extern "SYSTEM" void
DeRegister_Rexx_Function_Handlers( void )
	{
	RexxDeregisterFunction("MatchAKA");
	}


bool
Register_Rexx_Function_Handlers( void )
	{

	atexit( DeRegister_Rexx_Function_Handlers );

	RexxRegisterFunctionExe("MatchAKA", (PFN)MatchAKA );

	return Ok;
	}



/*********************************************************************/
/*                                                                   */
/* MatchAKA - External Rexx function                                 */
/*                                                                   */
/*********************************************************************/


LONG   EXPENTRY MatchAKA(
  PSZ        name,                     /* function name              */
  LONG       argc,                     /* count of arguments         */
  PRXSTRING  argv,                     /* argument RXSTRINGs         */
  PSZ        queue,                    /* current Rexx queue         */
  PRXSTRING  retstr )                  /* returned string value      */
	{

	fido_addr	a;
	ftn_def		def;

	const char *in = RXSTRPTR(argv[0]); 
	a.aparse( in );

	ftn::match( def, a );

	const char *res = ((string)def.fido_a).c_str();

	strcpy(RXSTRPTR(*retstr), res);  // copy over current precision
                                       
	retstr->strlength = strlen(res); // set new length
	return 0;                              // completed successfully
	}

@
