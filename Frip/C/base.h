/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	RIP Database class
 *
 *      $Log: base.h $
 *      Revision 1.15  1997/03/26 11:00:20  dz
 *      showbase
 *
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
