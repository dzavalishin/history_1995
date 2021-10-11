/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Flags class
 *
 *      $Log: flag.h $
 *      Revision 1.8  1997/01/21 08:53:51  dz
 *      clear
 *
 *      Revision 1.7  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.6  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.5  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.4  1996/01/22 04:36:41  dz
 *      C Set & STL support
 *
 *      Revision 1.3  1996/01/14 01:06:31  dz
 *      beta 14
 *
 *      Revision 1.2  1996/01/13 09:48:05  dz
 *      *** empty log message ***
 *
 *      Revision 1.1  1996/01/12 01:08:49  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#ifndef FLAG_H
#define FLAG_H

#ifdef __WATCOMC__
//#pragma warning 621 9
#pragma warning 549 9
#pragma warning 297 9
#endif

#include "strng.h"
#include "addr.h"

#include <set.h>


class flags
    {
    set<string, less<string> >    v;
    
public:
    
    flags() {};

      //flags( const flags &f );
    flags( const string &f );
    ~flags();
    
    // NB! It will clean extracted part out of s
    void extract( string &s, char key );
    
    //flags & operator = (flags &f );
    
    operator string () const;
    
    flags & operator += ( string s );
    flags & operator -= ( string s );
    
    int operator == ( const flags &f ) const;
    
    bool has( string s ) const ;
    
    bool is_empty() const { return v.empty() /* ? Yes : No */ ; }
    void clear();
    };


#endif // FLAG_H
