head	1.15;
access;
symbols
	Beta_12:1.3;
locks;
comment	@ *      @;


1.15
date	97.03.26.11.00.20;	author dz;	state Exp;
branches;
next	1.14;

1.14
date	97.03.22.16.05.16;	author dz;	state Exp;
branches;
next	1.13;

1.13
date	97.03.17.08.03.42;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	97.03.05.18.12.38;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.07.31.07.26.25;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.07.31.07.00.23;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.07.22.02.48.05;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.01.22.04.36.41;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.01.13.09.48.05;	author dz;	state Exp;
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


1.15
log
@showbase
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	RIP Database class
 *
 *      $Log: base.h $
 *      Revision 1.14  1997/03/22 16:05:16  dz
 *      datetime
 *
 *      Revision 1.13  1997/03/17 08:03:42  dz
 *      get_timed_out_routes( vector <base_data> &b ) const
 *
 *      Revision 1.12  1997/03/05 18:12:38  dz
 *      Ver. 34
 *
 *      Revision 1.11  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.10  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.9  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.8  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.7  1996/07/31 07:00:23  dz
 *      FripMail written
 *
 *      Revision 1.6  1996/07/22 02:48:05  dz
 *      ver 17
 *
 *      Revision 1.5  1996/01/22 04:36:41  dz
 *      C Set & STL support
 *
 *      Revision 1.4  1996/01/13 09:48:05  dz
 *      *** empty log message ***
 *
 *      Revision 1.3  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#ifndef BASE_H
#define BASE_H

//#include "frip.h"
#include "style.h"
#include "rip.h"
#include "conf.h"
#include "except.h"
#include <vector.h>

class base_data
    {
      //tm       time_to_go_v;
    long     time_to_go_v;                   // it is measured in seconds, though not datetime, since origin is real zero
    int      hops_v;
    addr     route_for_v;
    addr     route_via_v;
    //time_t   time_created_v;       // When this database record was created
    datetime time_created_v;       // When this database record was created
    flags    fplus_v, fminus_v, finfo_v;     // RIP flags
    
public:
        
    base_data( const rip &, const datetime &now );
    base_data( string ); // from base
    base_data( 
              const addr &rofor, 
              const addr &rovia, 
              long time_to_go,
              int hops,
              datetime time_created,
              const flags &fplus, const flags &fminus, const flags &finfo
             ) :
        time_to_go_v( time_to_go ), time_created_v( time_created ),
        route_for_v( rofor ), route_via_v( rovia ), hops_v( hops ),
        fplus_v(fplus), fminus_v(fminus), finfo_v(finfo)
            {};
    
    base_data( const base_data &b
             ) :
    time_to_go_v( b.time_to_go_v ), time_created_v( b.time_created_v ),
        route_for_v( b.route_for_v ), route_via_v( b.route_via_v ), 
        hops_v( b.hops_v ),
        fplus_v(b.fplus_v), fminus_v(b.fminus_v), finfo_v(b.finfo_v)
        {};
    
    base_data()
        :
    time_created_v( 0, datetime::UTC ),
    time_to_go_v( 0 ),
    hops_v( 1024 )
            {};
        
    int operator    == ( const base_data & ) const;
    
    operator        string () const;
      //operator        const tm & () const { return time_to_go_v; };
    void            showbase(); // user-oriented display
    
    int             hops() const { return hops_v; }
    
    const addr &    route_for() const { return route_for_v; };
    const addr &    route_via() const { return route_via_v; };
    
    bool            too_old( datetime laziness ) const;
    
    const flags &   fplus() const { return fplus_v; }
    const flags &   fminus() const { return fminus_v; }
    const flags &   finfo() const { return finfo_v; }

    datetime        created() const { return time_created_v; }
    long            time_to_go() const { return time_to_go_v; }
    };

class base
    {
private:
    
    vector            <base_data>        b;
    bool                                 loaded_ok;
    bool                                 enabled;
    bool                                 changed;
    
public:
    base() { loaded_ok = No; enabled = No; changed = No; }
    ~base() { save(); }

    void      enable() { enabled = Yes; }            // Let 'em use base
    void      disable() { enabled = No; save(); }
    
    void      load();
    void      save(); // const 

    void      showbase(); // user-oriented display

    
    int       count() const { if(!enabled) throw Ex_Fail("base::count","disabled",""); return b.size(); };

    void      add( string );
    bool      check( const rip &it );
    void      update( const rip &r );
    bool      better( const base_data &b, const rip &r, const datetime t ) const;
    bool      match( bool &b_in_r, bool &r_in_b, const base_data &bd, const addr &r ) const;
    
    void      foreach( void (*func)( base_data ) );

    addr      find_route( const addr &to, bool for_rif );

    enum db_off { Off_Not_Last, Off_Last };
    db_off    process_off( const rip &r, bool forced );

    void      get_timed_out_routes( vector <base_data> &b ) const;
    };


#endif // BASE_H
@


1.14
log
@datetime
@
text
@d8 3
d104 2
a105 1
    //operator        const tm & () const { return time_to_go_v; };
d141 3
@


1.13
log
@get_timed_out_routes( vector <base_data> &b ) const
@
text
@d8 3
d57 2
a58 1
    tm       time_to_go_v;
d62 2
a63 1
    time_t   time_created_v;       // When this database record was created
d68 2
a69 1
    base_data( const rip &, tm now );
d73 1
a73 1
              const tm &time_to_go,
d75 1
a75 1
              time_t time_created,
d91 5
a95 2
    base_data() :
        time_created_v( 0 ), hops_v( 1024 )
d101 1
a101 1
    operator        const tm & () const { return time_to_go_v; };
d108 1
a108 1
    bool            too_old( time_t laziness ) const;
d114 2
a115 1
    time_t          created() const { return time_created_v; }
d142 1
a142 1
    bool      better( const base_data &b, const rip &r, const time_t t ) const;
@


1.12
log
@Ver. 34
@
text
@d8 3
d104 2
a105 1
    
d142 1
@


1.11
log
@Version 25
@
text
@d8 3
d46 1
d109 3
d114 1
a114 1
    base() {}
d116 4
a119 1
        
d121 1
a121 1
    void      save() const;
d123 1
a123 1
    int       count() const { return b.size(); };
@


1.10
log
@Exceptions added, not compiled
@
text
@d8 3
d108 1
d115 1
a115 1
    bool      add( string );
d117 1
a117 1
    bool      update( const rip &r );
d121 1
a121 1
    bool      foreach( void (*func)( base_data ) );
d123 1
a123 1
    bool      find_route( addr &via, const addr &to, bool for_rif );
a128 3


extern base db;
@


1.9
log
@Before exceptions.
@
text
@d8 3
d106 2
a107 2
    bool      load();
    bool      save() const;
d121 1
a121 1
    enum db_off { Off_Err, Off_Not_Last, Off_Last };
@


1.8
log
@Clean up of watcom classlib based code
@
text
@d8 3
@


1.7
log
@FripMail written
@
text
@d8 3
a33 1
#if USE_STL
a34 3
#else
#include <wcvector.h>
#endif
@


1.6
log
@ver 17
@
text
@d8 3
a95 1
#if USE_STL
a96 3
#else
    WCPtrOrderedVector<base_data>        b;
#endif
d103 1
a103 1
#if USE_STL
d105 1
a105 3
#else
    int       count() const { return b.entries(); };
#endif
d114 5
a118 1
    bool      find_route( addr &via, const addr &to );
@


1.5
log
@C Set & STL support
@
text
@d8 3
d113 1
a113 1
    bool      match( bool &b_in_r, bool &r_in_b, const base_data &bd, const rip &r ) const;
d116 2
@


1.4
log
@*** empty log message ***
@
text
@d8 3
d25 3
d29 1
d54 8
d63 4
a66 1

d90 3
d94 1
d97 2
d101 3
d105 1
@


1.3
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d31 1
d41 2
a42 1
              time_t time_created
d45 2
a46 1
        route_for_v( rofor ), route_via_v( rovia ), hops_v( hops )
d60 6
a65 1
    bool            too_old() const;
@


1.2
log
@before COW strings
@
text
@d1 11
d68 1
a68 1
    bool      check( const rip &it ) const;
d70 2
a71 2
    bool      better( const base_data &b, const rip &r ) const;
    bool      match( const base_data &bd, const rip &r ) const;
@


1.1
log
@Initial revision
@
text
@d20 1
a20 1
    base_data( rip &, tm now );
d33 1
a33 1
    int operator == ( const base_data & ) const;
d35 2
a36 2
    operator string () const;
    operator const tm & () const { return time_to_go_v; };
d38 1
a38 1
    int       hops() const { return hops_v; }
d40 4
a43 2
    const addr &    route_for() { return route_for_v; };
    const addr &    route_via() { return route_via_v; };
d54 2
a55 2
    bool      save();
    int       count() { return b.entries(); };
d57 4
a60 4
    bool      check( rip &it );
    bool      update( rip &r );
    bool      better( base_data &b, rip &r );
    bool      match( base_data &bd, rip &r );
@
