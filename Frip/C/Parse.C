/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Generic parsers
 *
 *      $Log: Parse.C $
 *      Revision 1.6  1996/09/08 19:05:31  dz
 *      Version 25
 *
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
    
