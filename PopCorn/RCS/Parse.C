head	1.2;
access;
symbols
	Ver_075:1.2;
locks;
comment	@ * @;


1.2
date	96.09.17.23.20.43;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.13.07.24.48;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@First completely working POP3/ESMTP version
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Generic parsers
 *
 *      $Log: Parse.C $
 *      Revision 1.1  1996/09/13 07:24:48  dz
 *      Initial revision
 *
 *
 *
\*/

#include "strng.h"

bool string::parse( string &out1, char c, string &out2 ) const
    {
    
    for( int i = 0; i < length(); i++ )
        if( (*this)[i] == c )
            {
            if( i == 0 ) out1 = "";
            else out1 = substr( 0, i );
            out2 = substr( i+1 );
            out2.strip_ws();
            return Yes;
            }
    
    out1=(*this);
    out2 = "";
    return No;
    }


bool string::parse( string &out1, string &out2 ) const
    {
    
    for( int i = 0; i < length(); i++ )
        if( (*this)[i] == ' ' || (*this)[i] == '\t' )
            {
            out1 = substr( 0, i );
            out2 = substr( i );
            out2.strip_ws();
            return Yes;
            }
    
    out1=(*this);
    out2 = "";
    return No;
    }
    
@


1.1
log
@Initial revision
@
text
@d8 1
a8 1
 *      Revision 1.1  1996/05/22 19:46:04  dz
@
