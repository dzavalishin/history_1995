head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	95.11.06.01.42.16;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.08.13.22.34;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.12.21.20.45;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.24.01;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
log
@Using FTN_Msg! Seems to be working.
@
text
@/************************ FIDO to UUCP convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :	Gate control lines parser
 *
 *      $Log: GateCtl.c $
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


@


1.3
log
@App class used
@
text
@d8 3
d43 14
a56 15
Fido2uuApp::is_gate_control( uchar *s )
	{
	if( strnicmp( (const char *)s, GATECTL_L, GATECTL_N ) )
		return No;
	s += GATECTL_N;
	while( isspace( *s ) )
		s++;

	debug("Gate control line: '%s'", s );

	/* Parsing to be written */
	error( EI_None, "Unknown control line: '%s'", s );

	return Yes;
	}
@


1.2
log
@IBM C Set version seems to be working...
@
text
@d8 3
d40 1
a40 1
is_gate_control( uchar *s )
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/fido2uu/vcs/gatectl.c_v  $
d39 1
a39 1
	if( strnicmp( s, GATECTL_L, GATECTL_N ) )
@
