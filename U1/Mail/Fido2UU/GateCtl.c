/************************ FIDO to UUCP convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :	Gate control lines parser
 *
 *      $Log: GateCtl.c $
 *      Revision 1.4  1995/11/06 01:42:16  dz
 *      Using FTN_Msg! Seems to be working.
 *
 *      Revision 1.3  1995/08/08 13:22:34  dz
 *      App class used
 *
 * Revision 1.2  1995/04/12  21:20:45  dz
 * IBM C Set version seems to be working...
 *
 * Revision 1.1  1995/03/11  18:24:01  dz
 * Initial revision
 *
 *      
 *         Rev 1.2   07 Jun 1993 16:42:14   dz
 *      Copyright
 *      
 *         Rev 1.1   11 Nov 1992 05:57:00   dz
 *      error()
 *      
 *         Rev 1.0   22 Mar 1992 19:30:58   dz
 *      Initial revision.
 *
 *
 *
\*/

#include	"fido2uu.h"
#include	<ctype.h>





#define		GATECTL_L	"%Gate:"
#define		GATECTL_N	6


bool
Fido2uuApp::is_gate_control( string s )
    {
    if( strnicmp( (const char *)s, GATECTL_L, GATECTL_N ) )
        return No;
    s.strip_leading( GATECTL_N );
    s.strip_leading_ws();
    
    debug("Gate control line: '%s'", (const char *)s );
    
    /* Parsing to be written */
    error( EI_None, "Unknown control line: '%s'", (const char *)s );
    
    return Yes;
    }


