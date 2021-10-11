/************************ U1 ***************************\
 *
 *  Copyright (C) 1991-1995 by Infinity Soft
 *
 *  Module  :   Rexx subcommand handler for GRemote
 *
 *      $Log: RexxGr.c $
 *      Revision 1.3  1995/11/06 14:28:09  dz
 *      Current
 *
 *      Revision 1.2  1995/08/02 13:58:28  dz
 *      cleanup
 *
 *      Revision 1.1  1995/08/02 13:57:57  dz
 *      Initial revision
 *
 *
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
#include "gremote.h"

#include <log.h>


APIRET  EXPENTRY Gr_Command(PRXSTRING cmd, PUSHORT flags, PRXSTRING ret );

bool GRemoteApp::
Register_Gr_Rexx_Subcommand_Handler( void )
	{

	RexxRegisterSubcomExe("GRemote", (PFN)Gr_Command, NULL);

	return Ok;
	}


#define TEST( v, s ) ( strnicmp( v, s, sizeof( s ) - 1 ) == 0 )

#define	SC_SUCCESS		{ strcpy(ret->strptr, "0"); ret->strlength = 1; return 0; }
#define	SC_FAILURE(code)	{ *flags = RXSUBCOM_FAILURE; strcpy(ret->strptr, code); ret->strlength = 1; return 0; }
#define	SC_ERROR(code)		{ *flags = RXSUBCOM_ERROR; strcpy(ret->strptr, code); ret->strlength = 1; return 0; }

#define CMD( tail )		{ if( (rc = sc_##tail( cmd )) != 0 ) { sprintf( rcs, "%d", rc ); SC_ERROR(rcs); } }

extern int	sc_GetLetterLine( string s );
extern int	sc_PutLetterLine( string &s );


APIRET  EXPENTRY 
Gr_Command(PRXSTRING r_cmd, PUSHORT flags, PRXSTRING ret )
	{
	string cmd( r_cmd->strptr );
	const char *p1, *p2;
	const maxv = 25;
	char  verb[maxv];

	// for CMD macro
	int	rc;
	char	rcs[10];


	p1 = cmd.c_str();
	p2 = strpbrk( p1, " \t" );
	strncpy( verb, p1, min( maxv, p2-p1 ) );
	verb[min( maxv, p2-p1 )] = '\0';

	strlwr( verb );

	while( *p2 == ' ' || *p2 == '\t' )
		p2++;

	cmd = p2;

	debug( "Rexx cmd got verb '%s' and tail '%s'", verb, cmd.c_str() );

	if( TEST( verb, "getletterline" ) )	CMD( GetLetterLine ) else
	if( TEST( verb, "putletterline" ) )	CMD( PutLetterLine ) else
		{
		error( EI_None, "GRemote rexx subcommand: unknown verb '%s'", verb );
		SC_FAILURE("33");
		}

	SC_SUCCESS;
	}










