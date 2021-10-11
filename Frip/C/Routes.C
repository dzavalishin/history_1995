/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Route file generator
 *
 *      $Log: Routes.C $
 *      Revision 1.15  1997/01/31 10:14:51  dz
 *      Silence support
 *
 *      Revision 1.14  1997/01/21 08:53:51  dz
 *      Dead.
 *
 *      Revision 1.13  1996/12/28 03:11:55  dz
 *      new string/istring/domain_s comparison ops
 *
 *      Revision 1.12  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.11  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.10  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.9  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.8  1996/07/31 07:00:23  dz
 *      FripMail written
 *
 *      Revision 1.7  1996/01/18 00:56:53  dz
 *      Beta 15
 *
 *      Revision 1.6  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#include "frip.h"

#if EXT_ROUTER

void no_wild( base_data );
void wild_point( base_data );
void wild_node( base_data );
void wild_net( base_data );
void wild_zone( base_data );
void wild_domain( base_data );
void wild_all( base_data );

void put_route( base_data &);


static void suck_in( FILE *to, const string &fn )
    {
    if( fn.is_empty() ) return;

    ifstream from( fn );
    if( !from )    throw Ex_Fail("suck_in","Can't open", fn );
    
    const bs = 2048;
    char buf[bs];
    
    while( from.getline( buf, bs ), !from.eof() )
        {
        fputs( buf, to );
        fputs( "\n", to );
        }
    }
    


static FILE *route_file;

void FripApp::create_routes()
    {

    if(conf.route_file_path().is_empty() || conf.route_file_name().is_empty())
        return;
    
    if(!conf.silence()) BoldMsg( "Creating route file..." );
    
    string ron( conf.route_file_path()+"/"+conf.route_file_name() );
    unlink( ron );
//    ofstream ro( ron );
    
    route_file = fopen( ron, "w" );
    if( route_file == 0 )
        throw Ex_Fail("suck_in","Can't create", ron );
    
    debug("  Prefix...");    suck_in( route_file, conf.route_file_pref() );
    
    if( conf.route_wild_after() )
        {
        debug("  Points...");        db.foreach( no_wild );
        debug("  Nodes...");         db.foreach( wild_point );
        debug("  Nets...");          db.foreach( wild_node );
        debug("  Zones...");         db.foreach( wild_net );
        debug("  Domains...");       db.foreach( wild_zone );
        //debug("  World...");         db.foreach( wild_domain );
        }
    else
        {
        debug("  World...");         db.foreach( wild_domain );
        debug("  Domains...");       db.foreach( wild_zone );
        debug("  Zones...");         db.foreach( wild_net );
        debug("  Nets...");          db.foreach( wild_node );
        debug("  Nodes...");         db.foreach( wild_point );
        debug("  Points...");        db.foreach( no_wild );
        }

    debug("  Suffix...");    suck_in( route_file, conf.route_file_suff() );
    
    fclose(route_file);
    }
    
    
void no_wild( base_data b )
    {
    const addr &rf = b.route_for();
    if( rf.point() == 0 || rf.point() == WILD || rf.node() == WILD || rf.net() == WILD || rf.zone() == WILD || rf.domain() == string("*") )
        return;
    put_route( b );
    }

void wild_point( base_data b )
    {
    const addr &rf = b.route_for();
    if(
       ( rf.point() != 0 && rf.point() != WILD ) ||
       rf.node() == WILD || rf.net() == WILD || rf.zone() == WILD || rf.domain() == string("*")
      )
        return;
    put_route( b );
    }

void wild_node( base_data b )
    {
    const addr &rf = b.route_for();
    if(
       ( rf.point() != 0 && rf.point() != WILD ) ||
       rf.node() != WILD || rf.net() == WILD || rf.zone() == WILD || rf.domain() == string("*")
      )
        return;
    put_route( b );
    }

void wild_net( base_data b )
    {
    const addr &rf = b.route_for();
    if(
       ( rf.point() != 0 && rf.point() != WILD ) ||
       rf.node() != WILD || rf.net() != WILD || rf.zone() == WILD || rf.domain() == string("*")
      )
        return;
    put_route( b );
    }

void wild_zone( base_data b )
    {
    const addr &rf = b.route_for();
    if(
       ( rf.point() != 0 && rf.point() != WILD ) ||
       rf.node() != WILD || rf.net() != WILD || rf.zone() != WILD || rf.domain() == string("*")
      )
        return;
    put_route( b );
    }

void wild_domain( base_data b )
    {
    const addr &rf = b.route_for();
    if(
       ( rf.point() != 0 && rf.point() != WILD ) ||
       rf.node() != WILD || rf.net() != WILD || rf.zone() != WILD || rf.domain() != string("*")
      )
        return;
    put_route( b );
    }



static void inline pone( char *s, int n )
    {
    if( n == WILD )  strncpy( s, conf.wild_word(), 10 );
    else             sprintf( s, "%d", n );
    }

static void rs ( string &out_s, const addr &a )
    {
    char z[10], ne[10], no[10], p[10], znn[40], out[100];
    
    pone( z,  a.zone() );
    pone( ne, a.net() );
    pone( no, a.node() );
    
    if( conf.point_routing() && (a.point() != 0) )
        {
        pone( p,  a.point() );
        sprintf( znn, "%s:%s/%s.%s", z, ne, no, p );
        }
    else
        sprintf( znn, "%s:%s/%s",    z, ne, no );
    
    if( conf.strip_domain() )
        strcpy( out, znn );
    else
        sprintf( out, "%s@%s", znn, (const char *)a.domain() );
        
    out_s = out;
    }


    

void put_route( base_data &b )
    {
    addr rofor=b.route_for();
    addr rovia=b.route_via();

    /* don't be stupid and route node via itself */
    //if( match_addr(rofor,rovia) ) return;
    if( (!conf.viaself()) && rofor.matches(rovia) ) return;

    if( conf.strip_domain() && (rofor.domain() != conf.aka[0].domain()) && (rofor.zone() == WILD) )
        return;

    /* If 'route for' or 'route via'matches our address, don't put a route for it! */
    for( int i = 0; i < conf.aka.count(); i++ )
        {
        //if( match_addr(rofor,conf.aka[i]) ) return;
        if( rofor.matches(conf.aka[i]) ) return;
        //if( match_addr(rovia,conf.aka[i]) ) return;
        if( rovia.matches(conf.aka[i]) ) return;
        }
    
    
    // HACK! Will skip route if it is for or via point system
    if( !conf.point_routing() )
        {
        if(
           (rovia.point() != WILD && rovia.point() != 0) ||
           (rofor.point() != WILD && rofor.point() != 0)
          ) return;
        
        }

    string svia, sfor;
    
    rs( svia, rovia);
    rs( sfor, rofor);
    
    string out = conf.route_line();
    
    out.insert( &svia, &sfor );

    out.save(route_file);
    }

#else // EXT_ROUTER

void FripApp::create_routes()
    {
    }

#endif // EXT_ROUTER
