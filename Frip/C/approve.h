/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	approve class
 *
 *      $Log: approve.h $
 *      Revision 1.6  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.5  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.4  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.3  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.2  1996/01/22 04:36:41  dz
 *      C Set & STL support
 *
 *      Revision 1.1  1996/01/13 09:48:05  dz
 *      Initial revision
 *
 *
 *
 *
\*/


#ifndef APPROVE_H
#define APPROVE_H

#include "style.h"
#include "strng.h"
#include "addr.h"
#include "conf.h"

#include <set.h>

class app_elem
    {
public:
    addr  who;
    addr  what;
    
    app_elem( const addr & awho, const addr & awhat )
        : who( awho ), what( awhat ) {};

    app_elem() {};
    app_elem( const app_elem &a ) : who(a.who), what(a.what) {};
    
    app_elem & operator = ( const app_elem &a )
        { who = a.who; what = a.what; return *this; }
    int operator == ( const app_elem &a ) const
        { return who == a.who && what == a.what; }
    
    int operator <  ( const app_elem &a ) const
        { return who < a.who || ( who == a.who && what < a.what); }
    
    };


class approve
    {
    bool   done_load;
    
    void   load();
    void   load_file( const string &file );
    void   load_line( const string &line );
    
    void   insert( const app_elem & );
    bool   lookup( const app_elem & );
    
    set <app_elem, less<app_elem> >   set_v;
    
public:
    approve() { done_load = No; };
    approve( const approve &a ) : set_v(a.set_v), done_load(a.done_load) {}
    //~approve() { release(); }
    
    bool is_approved( const addr &who, const addr &what );
    };




#endif // APPROVE_H
