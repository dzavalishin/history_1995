head	1.15;
access;
symbols
	Beta_12:1.6;
locks;
comment	@ *      @;


1.15
date	97.01.31.10.14.51;	author dz;	state Exp;
branches;
next	1.14;

1.14
date	97.01.21.08.53.51;	author dz;	state Exp;
branches;
next	1.13;

1.13
date	96.12.28.03.11.55;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.07.31.07.26.25;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.07.31.07.00.23;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.01.18.00.56.53;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	95.12.05.04.23.40;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.11.26.13.58.02;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.11.16.00.54.44;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.11.13.06.46.09;	author dz;	state Exp;
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
@Silence support
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Route file generator
 *
 *      $Log: Routes.C $
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
        sprintf( out, "%s@@%s", znn, (const char *)a.domain() );
        
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
@


1.14
log
@Dead.
@
text
@d8 3
d81 1
a81 1
    BoldMsg( "Creating route file..." );
@


1.13
log
@new string/istring/domain_s comparison ops
@
text
@d8 3
d38 2
d257 5
d263 1
@


1.12
log
@Version 25
@
text
@d8 3
d113 1
a113 1
    if( rf.point() == 0 || rf.point() == WILD || rf.node() == WILD || rf.net() == WILD || rf.zone() == WILD || rf.domain() == "*" )
d123 1
a123 1
       rf.node() == WILD || rf.net() == WILD || rf.zone() == WILD || rf.domain() == "*"
d134 1
a134 1
       rf.node() != WILD || rf.net() == WILD || rf.zone() == WILD || rf.domain() == "*"
d145 1
a145 1
       rf.node() != WILD || rf.net() != WILD || rf.zone() == WILD || rf.domain() == "*"
d156 1
a156 1
       rf.node() != WILD || rf.net() != WILD || rf.zone() != WILD || rf.domain() == "*"
d167 1
a167 1
       rf.node() != WILD || rf.net() != WILD || rf.zone() != WILD || rf.domain() != "*"
@


1.11
log
@Exceptions added, not compiled
@
text
@d8 3
d64 1
a64 1
void create_routes()
d178 1
a178 1
static rs ( string &out_s, const addr &a )
@


1.10
log
@Before exceptions.
@
text
@d8 3
d40 1
a40 1
bool suck_in( FILE *to, const string &fn )
d42 1
a42 2
    if( fn.is_empty() )
        return Ok;
d45 1
a45 6
    
    if( !from )
        {
        Error("Can't open "+fn );
        return Err;
        }
a54 2
    
    return Ok;
d61 1
a61 1
bool create_routes()
d65 1
a65 1
        return Ok;
a73 1
    
d75 1
a75 4
        {
        Error("Can't create "+ron );
        return Err;
        }
d77 1
a77 2
    if(conf.debug()) Msg("  Prefix...");
    suck_in( route_file, conf.route_file_pref() );
d81 6
a86 12
        if(conf.debug()) Msg("  Points...");
        db.foreach( no_wild );
        if(conf.debug()) Msg("  Nodes...");
        db.foreach( wild_point );
        if(conf.debug()) Msg("  Nets...");
        db.foreach( wild_node );
        if(conf.debug()) Msg("  Zones...");
        db.foreach( wild_net );
        if(conf.debug()) Msg("  Domains...");
        db.foreach( wild_zone );
        if(conf.debug()) Msg("  World...");
        db.foreach( wild_domain );
d90 6
a95 12
        if(conf.debug()) Msg("  World...");
        db.foreach( wild_domain );
        if(conf.debug()) Msg("  Domains...");
        db.foreach( wild_zone );
        if(conf.debug()) Msg("  Zones...");
        db.foreach( wild_net );
        if(conf.debug()) Msg("  Nets...");
        db.foreach( wild_node );
        if(conf.debug()) Msg("  Nodes...");
        db.foreach( wild_point );
        if(conf.debug()) Msg("  Points...");
        db.foreach( no_wild );
a96 1
    
d98 1
a98 2
    if(conf.debug()) Msg("  Suffix...");
    suck_in( route_file, conf.route_file_suff() );
a100 2
    
    return Ok;
d107 1
a107 9
    
    if(
       rf.point() == 0 ||
       rf.point() == WILD ||
       rf.node() == WILD ||
       rf.net() == WILD ||
       rf.zone() == WILD ||
       rf.domain() == "*"
      )
a108 1
       
a114 1
    
d116 2
a117 7
       (
        rf.point() != 0 &&
        rf.point() != WILD ) ||
       rf.node() == WILD ||
       rf.net() == WILD ||
       rf.zone() == WILD ||
       rf.domain() == "*"
a119 1
       
a125 1
    
d127 2
a128 7
       (
        rf.point() != 0 &&
        rf.point() != WILD ) ||
       rf.node() != WILD ||
       rf.net() == WILD ||
       rf.zone() == WILD ||
       rf.domain() == "*"
a130 1
       
a136 1
    
d138 2
a139 7
       (
        rf.point() != 0 &&
        rf.point() != WILD ) ||
       rf.node() != WILD ||
       rf.net() != WILD ||
       rf.zone() == WILD ||
       rf.domain() == "*"
a141 1
       
a147 1
    
d149 2
a150 7
       (
        rf.point() != 0 &&
        rf.point() != WILD ) ||
       rf.node() != WILD ||
       rf.net() != WILD ||
       rf.zone() != WILD ||
       rf.domain() == "*"
a152 1
       
a158 1
    
d160 2
a161 7
       (
        rf.point() != 0 &&
        rf.point() != WILD ) ||
       rf.node() != WILD ||
       rf.net() != WILD ||
       rf.zone() != WILD ||
       rf.domain() != "*"
a163 1
       
d171 2
a172 4
    if( n == WILD )
        strncpy( s, conf.wild_word(), 10 );
    else
        sprintf( s, "%d", n );
@


1.9
log
@Clean up of watcom classlib based code
@
text
@d8 3
d70 1
a70 1
        return;
@


1.8
log
@FripMail written
@
text
@d8 3
a234 6
/*
void route_lineout( const string &s )
    {
    s.save(route_file);
    }
*/
a268 15
/*
string route_line(const addr &rovia, const addr &rofor)
    {
    string svia = rs(rovia);
    string sfor = rs(rofor);
    
    string out;
    
    out = conf.route_line();
    
    out.insert( &svia, &sfor );
    
    return out;
    }
*/
a312 1
    // route_lineout( out );
@


1.7
log
@Beta 15
@
text
@d7 4
a10 1
 *      $Log: Routes.c $
d63 3
@


1.6
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d290 1
a290 1
    if( rofor.matches(rovia) ) return;
@


1.5
log
@before COW strings
@
text
@d1 11
d67 1
a67 1
        Error("Can' create "+ron );
d71 1
d76 1
d78 1
d80 1
d82 1
d84 1
d86 1
d91 1
d93 1
d95 1
d97 1
d99 1
d101 1
d106 1
d223 1
a223 1

d228 1
d230 1
a230 2

static void pone( char *s, int n )
d238 1
a238 1
static string rs (const addr &a )
d259 1
a259 1
    return out;
d262 2
a268 15
    /*
    if( conf.strip_domain() )
        {
        string _;
        parse( svia, svia, '@@', _ );
        parse( sfor, sfor, '@@', _ );
        if( !conf.point_routing() )
            {
            parse( svia, svia, '.', _ );
            parse( sfor, sfor, '.', _ );
            }

        }
    */
    
a270 11
    /*
    if( !conf.route_line_pref().is_empty() )
        out = conf.route_line_pref() + " " + svia + " " + sfor;
    else
        out = svia + " " + sfor;

    if( !conf.route_line_suff().is_empty() )
        out += " " + conf.route_line_suff();
    
    */
    
d277 1
a277 1

d312 11
a322 1
    route_lineout( route_line(rovia,rofor) );
@


1.4
log
@beta 9
@
text
@d288 2
@


1.3
log
@beta 7
@
text
@d221 1
a221 1
    if( conf.point_routing() )
@


1.2
log
@b6
@
text
@d14 27
d48 1
a48 1
    string ron( conf.dir()+"/routes.inc" );
d60 2
d82 2
d256 8
d265 10
a274 1
    return svia + " " + sfor;
d285 2
a286 1
    if( match_addr(rofor,rovia) ) return;
d292 4
a295 2
        if( match_addr(rofor,conf.aka[i]) ) return;
        if( match_addr(rovia,conf.aka[i]) ) return;
@


1.1
log
@Initial revision
@
text
@d33 1
a33 1
    if( /* route_wild_after */ 1 )
d173 33
d208 19
a226 1
    return ((string)rovia) + " " + ((string)rofor);
d245 11
@
