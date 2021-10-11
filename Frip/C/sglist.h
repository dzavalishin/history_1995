/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	+- lists
 *
 *      $Log: sglist.h $
 *      Revision 1.6  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.5  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.4  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.3  1996/01/22 04:36:41  dz
 *      C Set & STL support
 *
 *      Revision 1.2  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#ifndef SGLIST_H
#define SGLIST_H

#include "strng.h"
#include "addr.h"

#include <vector.h>

class stopgo
    {
public:
    enum sg { Stop, Go, Error }   kind;
    addr                          mask;
    
    stopgo( string );
    stopgo( const stopgo &s ) : mask(s.mask) { kind = s.kind; }
    stopgo()                                 { kind = Error;   }
        
    
    int operator == ( stopgo const &a ) const
        { return mask==a.mask && kind == kind; };
    };



class sglist
    {
    vector            <stopgo>     sg_v;

public:
    sglist( const sglist& s) : sg_v(s.sg_v) {}
    sglist() {}
        
    void    add_one( const string &s )
        {
        sg_v.push_back( stopgo( s ) );
        }
    
    void    add_all( string s );
    
    bool    match( const addr &r, bool default_mode ) const;
    
    int operator == ( const sglist &his ) const
        {
        return sg_v == his.sg_v;
        }
    };






#endif // SGLIST_H








