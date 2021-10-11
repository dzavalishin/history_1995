head	1.6;
access;
symbols
	Beta_12:1.3;
locks;
comment	@ *      @;


1.6
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.12.05.04.23.40;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.12.13.12.27;	author dz;	state Exp;
branches;
next	;


desc
@@


1.6
log
@Version 25
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Generic parsers
 *
 *      $Log: Parse.C $
 *      Revision 1.5  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.4  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.3  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#include "frip.h"

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


1.5
log
@Exceptions added, not compiled
@
text
@d8 3
d23 1
a23 1
bool parse( const string &in, string &out1, char c, string &out2 )
d26 2
a27 2
    for( int i = 0; i < in.length(); i++ )
        if( in[i] == c )
d30 2
a31 2
            else out1 = in.substr( 0, i );
            out2 = in.substr( i+1 );
d36 1
a36 1
    out1=in;
d42 1
a42 1
bool parse( const string &in, string &out1, string &out2 )
d45 2
a46 2
    for( int i = 0; i < in.length(); i++ )
        if( in[i] == ' ' || in[i] == '\t' )
d48 2
a49 2
            out1 = in.substr( 0, i );
            out2 = in.substr( i );
d54 1
a54 1
    out1=in;
@


1.4
log
@Before exceptions.
@
text
@d8 3
@


1.3
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
@


1.2
log
@before COW strings
@
text
@d1 11
@


1.1
log
@Initial revision
@
text
@d12 1
a12 1
            out2.strip_leading_ws();
d30 1
a30 1
            out2.strip_leading_ws();
@
