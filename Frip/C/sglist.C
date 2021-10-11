/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	+- lists class
 *
 *      $Log: sglist.C $
 *      Revision 1.7  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.6  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.5  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.4  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.3  1996/01/22 04:35:34  dz
 *      C Set & STL support
 *
 *      Revision 1.2  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#include "frip.h"
#include "sglist.h"

stopgo::stopgo( string s )
    {
    s.strip_leading_ws();
    
    kind=Go;
    if( s[0] == '-' )
        kind = Stop;
    if( s[0] == '-' || s[0] == '+' )
        s.strip_leading(1);
    
    mask = addr( s );
    
    //debug( string("got ") + (kind==Go?"Go  : ":"Stop: ") + ((string)mask) );
    }


bool sglist::match( const addr &r, bool default_mode ) const
    {
    stopgo::sg result = stopgo::Stop;
    if(default_mode) result = stopgo::Go;
    
    for( int i = 0; i < sg_v.size(); i++ )
        if( r.matches( sg_v[i].mask ) )
            result = sg_v[i].kind;

    return ( result == stopgo::Go ) ? Yes : No;
    }




void sglist::add_all( string s )
    {
    string l, r;
    
    while( s.parse( l, r ) )
        {
        if( !l.is_empty() )
            add_one( l );
        s = r;
        }
    
    if( !l.is_empty() )
        add_one( l );
    }




