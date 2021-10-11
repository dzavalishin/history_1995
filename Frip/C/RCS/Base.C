head	1.18;
access;
symbols
	Beta_12:1.4;
locks;
comment	@ *      @;


1.18
date	97.03.22.15.48.26;	author dz;	state Exp;
branches;
next	1.17;

1.17
date	97.03.17.07.59.10;	author dz;	state Exp;
branches;
next	1.16;

1.16
date	97.03.05.18.12.38;	author dz;	state Exp;
branches;
next	1.15;

1.15
date	97.01.31.10.13.18;	author dz;	state Exp;
branches;
next	1.14;

1.14
date	96.12.28.03.11.55;	author dz;	state Exp;
branches;
next	1.13;

1.13
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.07.31.07.26.25;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.07.31.07.00.23;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.07.22.02.48.05;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.01.22.19.23.05;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.01.22.04.35.34;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.01.13.09.48.05;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.12.05.04.23.40;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.11.26.13.58.02;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.12.13.12.27;	author dz;	state Exp;
branches;
next	;


desc
@@


1.18
log
@datetime
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	RIP Database
 *
 *      $Log: Base.C $
 *      Revision 1.17  1997/03/17 07:59:10  dz
 *      get_timed_out_routes written - for fatal timeout support
 *
 *      Revision 1.16  1997/03/05 18:12:38  dz
 *      Ver. 34
 *
 *      Revision 1.15  1997/01/31 10:13:18  dz
 *      Silence support
 *
 *      Revision 1.14  1996/12/28 03:11:55  dz
 *      new string/istring/domain_s comparison ops
 *
 *      Revision 1.13  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.12  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.11  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.10  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.9  1996/07/31 07:00:23  dz
 *      FripMail written
 *
 *      Revision 1.8  1996/07/22 02:48:05  dz
 *      ver 17
 *
 *      Revision 1.7  1996/01/22 19:23:05  dz
 *      C Set, mostly works
 *
 *      Revision 1.6  1996/01/22 04:35:34  dz
 *      C Set & STL support
 *
 *      Revision 1.5  1996/01/13 09:48:05  dz
 *      *** empty log message ***
 *
 *      Revision 1.4  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#include "frip.h"
#include "stat.h"


//
// Возвpащаем Yes если пpедложеный RIP стоит помещеня в нашу базу
// и pассылки далее. Пpичины:
//     - База пуста
//     - В базе нет записей с адpесом, pавным rip.ad
//     - Такие записи есть, но они дают худшее качество pоутинга
//     - Записи есть, но очень стаpые
//



bool base::check( const rip &it )
    {

    if(!enabled) throw Ex_Fail("base::check","disabled","");

    // if base is empty - anything looks great
    if(count() == 0) 
        {
        stats.rip_new_addr();
        return Yes;
        }

    changed = Yes;
    
      //time_t tt = time(NULL);
    datetime now = datetime::now();
    //tm  t = *localtime( &tt );
    //time_t rip_creation_time = mktime( &((tm&)it) );
      //time_t rip_travel_time = tt - rip_creation_time;
    datetime rip_creation_time = it.created();
    datetime rip_travel_time   = now - rip_creation_time;
    
    bool count_as_new = Yes; // Add to statistics as a new record
    bool got_a_match = No;   // Is matched by some non-deleted record in base
    bool is_better = No;     // Is better than matched record

    for( int i = 0; i < count(); i++ )
        {
        base_data &bb = b[i];
        bool r_in_b, b_in_r;
        
	if( match( b_in_r, r_in_b, bb, it.announce() ) )
            {
            string s1 = bb.route_for();
            string s2 = bb.route_via();
            
            if(conf.debug()) Msg("Match ("+s1+" > "+s2+")");
            
            count_as_new = No;
            bool repl = No;
            if( b_in_r && bb.too_old(rip_travel_time) )
                {
                BoldMsg("Route ("+s1+" > "+s2+") timed out");
                repl = Yes;
                stats.rip_timeout_repl();
                }
            else
                {
                if( r_in_b ) got_a_match = Yes;
                if( b_in_r && better( bb, it, now ) )
                    {
                    if(conf.debug()) BoldMsg("better than old ("+s1+" > "+s2+")");
                    repl = Yes;
                    is_better = Yes;
                    stats.rip_better_repl();
                    }
                }
            
            // base record is equal to announce or
            // contained in it, so throw it out
            if( repl && b_in_r )
                {
                if(conf.debug()) BoldMsg("deleting ("+s1+" > "+s2+")");
                
                b.erase( b.begin()+i );
                // removed = Yes; newone = No;
                i--;
                }
            // NB! 'i' is invalidated here!
            }
        }
    
    if( count_as_new ) 
        {
        stats.rip_new_addr();
        if(conf.debug()) BoldMsg("New route");
        }

    return is_better ? Yes : (got_a_match ? No : Yes);
    }


bool base::better( const base_data &b, const rip &r, const datetime t ) const
    {

    if(!enabled) throw Ex_Fail("base::better","disabled","");
    
    bool r_minus_empty = r.fminus().is_empty();
    bool b_minus_empty = b.fminus().is_empty();
    
    // rip has -flags, and database record - no
    if( (!r_minus_empty) && b_minus_empty )
       {
       if(!conf.silence()) Msg("Rip has minus flag (but db record has not), forced to be worse");
       return No;
       }
    
    // rip has no -flags, and database record has some
    if( r_minus_empty && (!b_minus_empty) )
       {
       if(!conf.silence()) Msg("Rip has no minus flag (but db record has), forced to be better");
       return Yes;
       }
    
    if( (!r_minus_empty) && (!b_minus_empty) )
        {
        // -NA has max. prio
        if( r.fminus().has("NA") && (!b.fminus().has("NA")) )
            {
            if(!conf.silence()) Msg("Rip has -NA flag (but db record has not), forced to be worse");
            return No;
            }
        if( (!r.fminus().has("NA")) && (b.fminus().has("NA")) )
            {
            if(!conf.silence()) Msg("Rip has no -NA flag (but db record has), forced to be better");
            return Yes;
            }
            
        }
    
    signed long tb = b.time_to_go();
    signed long tr = r.created();

    if( ((long)tb) < ((signed long)t) - ((long)tr) ) return No;

    if( r.hops() > b.hops() ) return No;

    return Yes;
    }









void base::update( const rip &r )
    {
    if(!enabled) throw Ex_Fail("base::update","disabled","");

    changed = Yes;
    
    //datetime now = time(NULL);
    //tm  t = *localtime( &tt );
    if(conf.debug()) Msg("Adding to database");
    b.push_back( base_data( r, datetime::now() ) );
    }






void split_ftn_domain( const string &in, string out1, string out2 );


// match a rip against a base record.
// Assumptions:
//    if domain is wild - address is a big '*' et al.
//    if zone is wild, node, net & point are wild too
//    if net is wild node & point are wild
//    if node is wild - point is wild.
//
// Return:
//    if addresses are equal or any of them matches other
//    function value is 'Yes' and flags 'base_in_rip' and
//    'rip_in_base' values are set accordingly. Else return
//    value is 'No' and flags are not changed.
//

// NB! To implement flags correctly, rip must not match
// db record, which has differrent - and + flags.
// This need forces us to use set, not a vector, as it is now, to
// keep flags in class flags.

bool base::match(
                 bool &base_in_rip, bool &rip_in_base,
                 const base_data &bd, const addr &r ) const
    {
    if(!enabled) throw Ex_Fail("base::match","disabled","");

    addr b( bd.route_for());
    //addr r( ri.announce());

    if( r.net() !=b.net()  && b.net() !=WILD && r.net() !=WILD ) return No;
    if( r.node()!=b.node() && b.node()!=WILD && r.node()!=WILD ) return No;
    if( r.zone()!=b.zone() && b.zone()!=WILD && r.zone()!=WILD ) return No;
    
    /* node is allways a default route for its points */
    if( b.point() != WILD && b.point() != 0 &&
       r.point() != WILD && r.point() != 0 &&
       r.point() != b.point() ) return No;
    
    int b_neq_e = b.domain() != r.domain();
    bool b_wild = b.domain().is_wild();
    bool r_wild = r.domain().is_wild();
    
    if(
       b_neq_e && (!b_wild) && (!r_wild)
      )
        {
        string rd1, rd2, bd1, bd2;
	r.domain().parse( rd1, '.', rd2 );  // rd1.rd2
	b.domain().parse( bd1, '.', bd2 );

	if( rd2.is_empty() )  bd2="";
	if( bd2.is_empty() )  rd2="";

	if( rd2 != bd2 || rd1 != bd1 ) return No;
        }
    
    if( b == r )                   
        {
        base_in_rip = rip_in_base = Yes;
        return Yes;
        }
    
    
    if( our_aka_matches(r) )
        {
        if( r.domain()==string("*")  && b.domain() != string("*")  ) return No;
        if( r.zone()==WILD   && b.zone() != WILD   ) return No;
        if( r.net()==WILD    && b.net() != WILD    ) return No;
        if( r.node()==WILD   && b.node() != WILD   ) return No;
        if( r.point()==WILD  && b.point() != WILD  ) return No;
        }
    
    if( our_aka_matches(b) )
        {
        if( b.domain()==string("*")  && r.domain() != string("*")  ) return No;
        if( b.zone()==WILD   && r.zone() != WILD   ) return No;
        if( b.net()==WILD    && r.net() != WILD    ) return No;
        if( b.node()==WILD   && r.node() != WILD   ) return No;
        if( b.point()==WILD  && r.point() != WILD  ) return No;
        }
    
    base_in_rip = rip_in_base = No;
    
    if( b.domain().is_wild()) rip_in_base = Yes;
    else if( r.domain().is_wild()) base_in_rip = Yes;
    else if( b.zone() == WILD  )   rip_in_base = Yes;
    else if( r.zone() == WILD  )   base_in_rip = Yes;
    else if( b.net() == WILD   )   rip_in_base = Yes;
    else if( r.net() == WILD   )   base_in_rip = Yes;
    else if( b.node() == WILD  )   rip_in_base = Yes;
    else if( r.node() == WILD  )   base_in_rip = Yes;
    else if( b.point() == WILD )   rip_in_base = Yes;
    else if( r.point() == WILD )   base_in_rip = Yes;
    
    return Yes;
    }


  // -----------------------------------------------------------------


  // маразм. пересортировать базу, к ядрене фене,
  // а лучше - переделать на дерево или map

addr base::find_route( const addr &to, bool for_rif )
    {
    if(!enabled) throw Ex_Fail("base::find_route","disabled","");
    
    changed = Yes; // strange. isn't fingd route should be const?
    
    int best = 0;
    addr guess;

    //Msg("Looking for route to "+string(to));
    
    for( int i = 0; i < count(); i++ )
        {
        base_data &bb = b[i];

        if( for_rif && bb.fminus().has("RIF") )
            {
            //debug(" - Route via "+string(bb.route_via())+" skipped because of -RIF");
            continue;
            }
        
        //Msg("    bb.route_for() "+string(bb.route_for()));
        if( bb.route_for() == to )
            return bb.route_via();

        bool r_in_b, b_in_r;
        
        if( match( b_in_r, r_in_b, bb, to ) )
            {
            if( !r_in_b ) continue;
            
            //string rfor = bb.route_for();
            string rvia = bb.route_via();
            addr rfor = bb.route_for();
            
            if(conf.debug()) Msg("Match ("+string(to)+" > "+rvia+")");

            int level = 0;
            if( rfor.net() == to.net() ) level = 3;
            else if( rfor.zone() == to.zone() ) level = 2;
            else if( rfor.domain() == to.domain() ) level = 1;

            if( level > best )
                {
                best = level;
                guess = bb.route_via();
                }
            }
        }
    
    if( best == 0 ) throw Ex_EOF("base::find_route", "No route to " + string(to) );
    return guess;
    }


// BUG! 
// Тут нужно рекурсивно генерить off-ы по мере выяснения того, кто 
// у нас рутится через означеный узел. Или не тут? Или не обязательно 
// off-ы, а можно переручивать? Вряд ли, так как "переручка" может пойти 
// вразрез с политикой окружающих.

base::db_off base::process_off( const rip &r, bool forced )
    {
    if(!enabled) throw Ex_Fail("base::process_off","disabled","");

    changed = Yes;
    
    bool removed = No; // Removed some routes
    bool remain = No;  // Something still alive
      //Msg("Looking for route to "+string(to));

    addr off_dest = r.off();

#define via_is_bad( a ) (forced || r.seenby_matches(a))
    
    for( int i = 0; i < count(); i++ )
        {
        base_data &bb = b[i];
        //Msg("Looking at route to "+string(bb.route_for())+" via "+string(bb.route_via()) );

#if 0
          //Msg("    bb.route_for() "+string(bb.route_for()));
        if( bb.route_for() == off_dest )
            {
            if( via_is_bad( bb.route_via() ) )
                {
                BoldMsg("Removing route to "+string(bb.route_for())+" via "+string(bb.route_via()) );
                b.erase( b.begin()+i );
                i--; // for will increment it now, but we need to continue just here
                removed = Yes;
                continue;
                }
            remain = Yes;
            continue;
            }
#endif
        
        bool r_in_b, b_in_r;
        
        if( match( b_in_r, r_in_b, bb, off_dest ) )
            {
            if( (!b_in_r) && r_in_b )
                {
                BoldMsg("Will keep route to "+string(bb.route_for())+" via "+string(bb.route_via()) );
                //Msg("remain = yes");
                  // we have some aggregate, that includes requested route
                remain = Yes;
                }
            if( b_in_r )
                {
                if( via_is_bad( bb.route_via() ) )
                    {
                    BoldMsg("Removing route to "+string(bb.route_for())+" via "+string(bb.route_via()) );
                    b.erase( b.begin()+i );
                    i--; // for will increment it now, but we need to continue just here
                    removed = Yes;
                    continue;
                    }
                remain = Yes;
                continue;
                }
            }
        }

    if( removed )
        {
        if( !remain )
            BoldMsg("Lost all routes to "+string(r.off()) );
        else
            BoldMsg("Lost some routes to "+string(r.off()) );
        }
    
    if( remain )
        return Off_Not_Last;
    
    return Off_Last;
    }



void base::get_timed_out_routes( vector <base_data> &out ) const
    {
    if(!enabled) throw Ex_Fail("base::get_timed_out_routes","disabled","");

    int fatal_timeout = conf.db_fatal_timeout();
    
    for( int i = 0; i < count(); i++ )
        {
        const base_data &bb = b[i];

        if( time(0) - bb.created() > fatal_timeout )
            out.push_back( b[i] );
        }
    }



@


1.17
log
@get_timed_out_routes written - for fatal timeout support
@
text
@d8 3
d83 7
a89 4
    time_t tt = time(NULL);
    tm  t = *localtime( &tt );
    time_t rip_creation_time = mktime( &((tm&)it) );
    time_t rip_travel_time = tt - rip_creation_time;
d118 1
a118 1
                if( b_in_r && better( bb, it, tt ) )
d151 1
a151 1
bool base::better( const base_data &b, const rip &r, const time_t t ) const
a155 4
    signed long tb = mktime( &((tm&)b) );
    signed long tr = mktime( &((tm&)r) );
    
    
d189 3
d193 1
a193 2
    
    
d213 2
a214 2
    time_t tt = time(NULL);
    tm  t = *localtime( &tt );
d216 1
a216 1
    b.push_back( base_data( r, t ) );
@


1.16
log
@Ver. 34
@
text
@d8 3
d462 17
@


1.15
log
@Silence support
@
text
@d8 3
d66 2
d75 2
d144 2
d202 4
d243 2
d325 4
d386 4
@


1.14
log
@new string/istring/domain_s comparison ops
@
text
@d8 3
d148 1
a148 1
       Msg("Rip has minus flag (but db record has not), forced to be worse");
d155 2
a156 2
       Msg("Rip has no minus flag (but db record has), forced to be better");
       return No;
d164 1
a164 1
            Msg("Rip has -NA flag (but db record has not), forced to be no good");
d169 2
a170 2
            Msg("Rip has no -NA flag (but db record has), forced to be better");
            return No;
d354 1
a354 1
    if( best == 0 ) throw Ex_EOF("base::find_route", "Route not found");
@


1.13
log
@Version 25
@
text
@d8 3
d266 1
a266 1
        if( r.domain()=="*"  && b.domain() != "*"  ) return No;
d275 1
a275 1
        if( b.domain()=="*"  && r.domain() != "*"  ) return No;
@


1.12
log
@Exceptions added, not compiled
@
text
@d8 3
d40 1
d245 2
a246 2
	parse( r.domain(), rd1, '.', rd2 );  // rd1.rd2
	parse( b.domain(), bd1, '.', bd2 );
d348 1
a348 1
    if( best == 0 ) throw Ex_EOF("base::find_route");
@


1.11
log
@Before exceptions.
@
text
@d8 3
d138 1
a138 1
       Msg("Rip has minus flag (but db record has not), forced to be no good");
d181 1
a181 1
bool base::update( const rip &r )
a184 1

a185 1
    
a186 1
    return Ok;
d298 1
a298 1
bool base::find_route( addr &via, const addr &to, bool for_rif )
d317 1
a317 4
            {
            via = bb.route_via();
            return Ok;
            }
d344 2
a345 3
    if( best == 0 ) return Err;
    via = guess;
    return Ok;
@


1.10
log
@Clean up of watcom classlib based code
@
text
@d8 3
@


1.9
log
@FripMail written
@
text
@d8 3
a64 1
#if USE_STL
a65 3
#else
        base_data &bb = *(b[i]);
#endif
a100 1
#if USE_STL
a101 4
#else
                b[i].~base_data();
                b.removeAt( i );
#endif
a121 2
    //tm tmb = (tm&)b;
    
a181 1
#if USE_STL
a182 6
#else
    base_data  *rec = new base_data( r, t /* *localtime( &tt ) */ );
    
    if( !b.insert( rec ) )
        return Err;
#endif
@


1.8
log
@ver 17
@
text
@d8 3
d47 1
a47 1
        stats.new_addr();
d82 1
a82 1
                stats.timeout_repl();
d92 1
a92 1
                    stats.better_repl();
d117 1
a117 1
        stats.new_addr();
d310 1
a310 1
bool base::find_route( addr &via, const addr &to )
d321 6
d364 79
@


1.7
log
@C Set, mostly works
@
text
@d7 4
a10 1
 *      $Log: Base.c $
d66 1
a66 1
	if( match( b_in_r, r_in_b, bb, it ) )
d227 1
a227 1
                 const base_data &bd, const rip &ri ) const
d230 1
a230 1
    addr r( ri.announce());
d298 55
@


1.6
log
@C Set & STL support
@
text
@d8 3
d238 1
a238 1
    bool b_neq_e = b.domain() != r.domain();
@


1.5
log
@*** empty log message ***
@
text
@d8 3
d53 3
d57 1
d93 3
d98 1
d181 3
d188 1
a188 1
    
@


1.4
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d41 2
d62 1
a62 1
            if( b_in_r && bb.too_old() )
a106 2
//    time_t tt = time(NULL);
//    tm  t = *localtime( &tt );
d108 1
a108 4
#if 1
//    tmb.tm_isdst = -1;
//    tmr.tm_isdst = -1;
    tm tmb = (tm&)b;    
a110 12
    /*
    signed long tb =
        ((
          ((
            ((
              (((tmb.tm_year * 12) + tmb.tm_mon) * 31) +
              tmb.tm_mday) * 24 ) +
            tmb.tm_hour) * 60 ) +
          tmb.tm_min) * 60 )
        + tmb.tm_sec;
        */
        
d113 34
a148 9
#else
    tm tmb = (tm&)b; tm tmr = (tm&)r;
    
    if( tmb.tm_year <  tmr.tm_year-t.tm_year ) return No;
    if( tmb.tm_mon  <  tmr.tm_mon -t.tm_mon  ) return No;
    if( tmb.tm_mday <  tmr.tm_mday-t.tm_mday ) return No;
    if( tmb.tm_hour <  tmr.tm_hour-t.tm_hour ) return No;
    if( tmb.tm_min  <= tmr.tm_min -t.tm_min  ) return No;
#endif
a167 40
#if 0
    bool removed, newone = Yes;
    
    do
        {
        removed = No;
        for( int i = 0; i < count(); i++ )
            {
            base_data &br = *(b[i]);
            
            if( match( br, r ) ) /* remove lines that worse than ours */
                {
                bool repl = No;
                if( br.too_old() )
                    {
                    repl = Yes;
                    stats.timeout_repl();
                    }
                if( better( br, r, t ) )
                    {
                    repl = Yes;
                    stats.better_repl();
                    }
                if( repl )
                    {
                    b[i].~base_data();
                    b.removeAt( i );
                    removed = Yes; newone = No;
                    break;
                    }
                }
            }
        } while( removed );
    
    if( newone )
        stats.new_addr();
    
//    time_t tt = time(NULL);
#endif
    
a182 45


#if 0


// create base line from rip
base_data::base_data( const rip &r, tm now )
    {
    const tm &rt = r.created();
    tm        bt;
    
    bt.tm_sec  = now.tm_sec -rt.tm_sec;
    if( bt.tm_sec  < 0 ) { bt.tm_sec  += 60; now.tm_min --; }
    
    bt.tm_min  = now.tm_min -rt.tm_min;
    if( bt.tm_min  < 0 ) { bt.tm_min  += 60; now.tm_hour--; }
    
    bt.tm_hour = now.tm_hour-rt.tm_hour;
    if( bt.tm_hour < 0 ) { bt.tm_hour += 24; now.tm_mday--; }
    
    bt.tm_mday = now.tm_mday-rt.tm_mday;
    if( bt.tm_mday < 0 ) { bt.tm_mday += 24; now.tm_mon --; }
    
    bt.tm_mon  = now.tm_mon -rt.tm_mon;
    if( bt.tm_mon  < 0 ) { bt.tm_mon  += 24; now.tm_year--; }
    
    bt.tm_year = now.tm_year-rt.tm_year;
/*
    if( bt.tm_year < 0 ) 
        {
        Error("Crazy... Over the rainbow I'm crazy...");
        }
*/    
    time_to_go_v = bt;
    time_created_v = time(NULL);
    route_for_v = r.ad();
    route_via_v = r.from();
    hops_v = r.hops();
    }


#endif



d200 5
d220 5
a224 1

d226 1
a226 2
       b.domain() != r.domain() && 
       (!b.domain().is_wild()) && (!r.domain().is_wild())
a280 196
#if 0

/*
// match a rip against a base record - old case

bool base::match(
                 bool &base_in_rip, bool &rip_in_base,
                 const base_data &bd, const rip &ri ) const
    {
    addr b( bd.route_for());
    addr r( ri.announce());

    if( b.zone() != WILD && r.zone() != b.zone() ) return No;
    if( b.net()  != WILD && r.net()  != b.net()  ) return No;
    if( b.node() != WILD && r.node() != b.node() ) return No;

    // node is allways a default route for its points
    if( b.point() != WILD && b.point() != 0 & r.point() != b.point() ) return No;

    if (((const string&)b.domain()) != string("*") )
        {
        string rd1, rd2, bd1, bd2;
	parse( r.domain(), rd1, '.', rd2 );  // rd1.rd2
	parse( b.domain(), bd1, '.', bd2 );

	if( rd2.is_empty() )  bd2="";
	if( bd2.is_empty() )  rd2="";

	if( rd2 != bd2 || rd1 != bd1 ) return No;
        }

    if( our_aka_matches(r) )
        {
        if( r.domain()=="*"  && b.domain() != "*"  ) return No;
        if( r.zone()==WILD   && b.zone() != WILD   ) return No;
        if( r.net()==WILD    && b.net() != WILD    ) return No;
        if( r.node()==WILD   && b.node() != WILD   ) return No;
        if( r.point()==WILD  && b.point() != WILD  ) return No;
        }
    
    if( our_aka_matches(b) )
        {
        if( b.domain()=="*"  && r.domain() != "*"  ) return No;
        if( b.zone()==WILD   && r.zone() != WILD   ) return No;
        if( b.net()==WILD    && r.net() != WILD    ) return No;
        if( b.node()==WILD   && r.node() != WILD   ) return No;
        if( b.point()==WILD  && r.point() != WILD  ) return No;
        }

    return Yes;
    }
 
 */


/*
 * format of base line: route-for route-via date time hops base_line_created
    tm       time_to_go_v;
    int      hops_v;
    addr     route_for_v;
    addr     route_via_v;
 */

base_data::operator string () const
    {
    char line[1024];
    
    sprintf( line, "%s %s %d/%d/%d %d:%d:%d %d %ld + - !",
            (const char*)((string)route_for_v),
            (const char*)((string)route_via_v),
            time_to_go_v.tm_mday,
            time_to_go_v.tm_mon,
            time_to_go_v.tm_year,
            time_to_go_v.tm_hour,
            time_to_go_v.tm_min,
            time_to_go_v.tm_sec,
            hops_v,
            time_created_v
            );
    
    return line;
    }


base_data::operator == ( const base_data &his ) const
    {
    return
        time_to_go_v.tm_mday == his.time_to_go_v.tm_mday &&
        time_to_go_v.tm_mon  == his.time_to_go_v.tm_mon  &&
        time_to_go_v.tm_year == his.time_to_go_v.tm_year &&
        time_to_go_v.tm_hour == his.time_to_go_v.tm_hour &&
        time_to_go_v.tm_min  == his.time_to_go_v.tm_min  &&
        time_to_go_v.tm_sec  == his.time_to_go_v.tm_sec  &&
        
        hops_v       == his.hops_v       &&
        route_for_v  == his.route_for_v  &&
        route_via_v  == his.route_via_v;
    }


bool base::foreach( void (*func)( base_data ) )
    {
    for( int i = 0; i < count(); i++ )
        func( *(b[i]) );
    
    return Ok;
    }

bool base_data::too_old() const
    {
    time_t diff = time(0) - time_created_v;
    //fprintf( stderr, "timediff = %ld, threshold = %ld\n", diff, conf.db_timeout());
    return (diff > conf.db_timeout()) ? Yes : No;
    }


#endif


#if 0


// format of line: route-for route-via date time hops [time_t date-of-line]
bool base::add( string s )
    {
    string orig = s;
    
    string hops_s, rfor_s, rvia_s, date_s, time_s;
    
    if( 
       (!parse( s, rfor_s, s )) ||
       (!parse( s, rvia_s, s )) ||
       (!parse( s, date_s, s )) ||
       (!parse( s, time_s, s ))
      )
        {
        Error( "Base format incorrect: "+orig );
        return Err;
        }
    
    tm       togo;
    time_t   line_created;
    int      hops = 0;
    addr     rofor( rfor_s ), rovia( rvia_s );
    
    if( 3 != sscanf( date_s, "%d/%d/%d", 
                    &togo.tm_mday, &togo.tm_mon, &togo.tm_year ) )
        {
        Error( "Base date format incorrect: "+orig );
        return Err;
        }
    
    if( 3 != sscanf( time_s, "%d:%d:%d", 
                    &togo.tm_hour, &togo.tm_min, &togo.tm_sec ) )
        {
        Error( "Base time format incorrect: "+orig );
        return Err;
        }
    
    
    // Last, we'll try to handle old base format, which ends with
    // hop count only
    
    if( !parse( s, hops_s, s ) )   // last part is hop count?
        line_created = time(NULL); // assume record was created right now
    else
        line_created = atol( s );
    
    hops = atoi( hops_s );
    
    base_data *l = new base_data( rofor, rovia, togo, hops, line_created );
    
    b.insert( l );
    return Ok;
    }


bool base::load()
    {
    FILE *bf = fopen( conf.dir()+"/RIP_Base", "r" );
    
    if( bf == NULL )
        {
        perror("Can't open base");
        return Err;
        }

    string l;
    
    while( l.load( bf ) == Ok )
        {
        l.strip_leading_ws();
        l.strip_crlf();
        
        if(l[0] == ';' || l.is_empty() )
            continue;
a281 44
        if( add(l) == Err )
            {
            perror("Can't read base");
            fclose( bf );
            return Err;
            }
        }
	

    fclose( bf );
    return Ok;
    }





bool base::save() const
    {
    unlink( conf.dir()+"/RIP_Base.Old" );
    rename( conf.dir()+"/RIP_Base", conf.dir()+"/RIP_Base.Old" );
    unlink( conf.dir()+"/RIP_Base" );
    
    FILE *bf = fopen( conf.dir()+"/RIP_Base", "w" );
    
    if( bf == NULL )
        {
        perror("Can't create base");
        return Err;
        }

    for( int i = 0; i < count(); i++ )
        {
        if( ((string)(*(b[i]))).save( bf ) != Ok )
            {
            perror("Can't read base");
            fclose( bf );
            return Err;
            }
        }

    fclose( bf );
    return Ok;
    }
a282 1
#endif
@


1.3
log
@before COW strings
@
text
@d1 11
d15 9
a23 53
// format of line: route-for route-via date time hops [time_t date-of-line]
bool base::add( string s )
    {
    string orig = s;
    
    string hops_s, rfor_s, rvia_s, date_s, time_s;
    
    if( 
       (!parse( s, rfor_s, s )) ||
       (!parse( s, rvia_s, s )) ||
       (!parse( s, date_s, s )) ||
       (!parse( s, time_s, s ))
      )
        {
        Error( "Base format incorrect: "+orig );
        return Err;
        }
    
    tm       togo;
    time_t   line_created;
    int      hops = 0;
    addr     rofor( rfor_s ), rovia( rvia_s );
    
    if( 3 != sscanf( date_s, "%d/%d/%d", 
                    &togo.tm_mday, &togo.tm_mon, &togo.tm_year ) )
        {
        Error( "Base date format incorrect: "+orig );
        return Err;
        }
    
    if( 3 != sscanf( time_s, "%d:%d:%d", 
                    &togo.tm_hour, &togo.tm_min, &togo.tm_sec ) )
        {
        Error( "Base time format incorrect: "+orig );
        return Err;
        }
    
    
    // Last, we'll try to handle old base format, which ends with
    // hop count only
    
    if( !parse( s, hops_s, s ) )   // last part is hop count?
        line_created = time(NULL); // assume record was created right now
    else
        line_created = atol( s );
    
    hops = atoi( hops_s );
    
    base_data *l = new base_data( rofor, rovia, togo, hops, line_created );
    
    b.insert( l );
    return Ok;
    }
d26 1
a26 1
bool base::load()
d28 3
a30 3
    FILE *bf = fopen( conf.dir()+"/RIP_Base", "r" );
    
    if( bf == NULL )
d32 2
a33 2
        perror("Can't open base");
        return Err;
d36 2
a37 1
    string l;
d39 5
a43 1
    while( l.load( bf ) == Ok )
d45 2
a46 2
        l.strip_leading_ws();
        l.strip_crlf();
d48 1
a48 4
        if(l[0] == ';' || l.is_empty() )
            continue;

        if( add(l) == Err )
d50 37
a86 3
            perror("Can't read base");
            fclose( bf );
            return Err;
d89 6
a94 1
	
d96 1
a96 2
    fclose( bf );
    return Ok;
d100 1
a100 4



bool base::save() const
d102 27
a128 3
    unlink( conf.dir()+"/RIP_Base.Old" );
    rename( conf.dir()+"/RIP_Base", conf.dir()+"/RIP_Base.Old" );
    unlink( conf.dir()+"/RIP_Base" );
d130 6
a135 1
    FILE *bf = fopen( conf.dir()+"/RIP_Base", "w" );
d137 1
a137 5
    if( bf == NULL )
        {
        perror("Can't create base");
        return Err;
        }
d139 1
a139 12
    for( int i = 0; i < count(); i++ )
        {
        if( ((string)(*(b[i]))).save( bf ) != Ok )
            {
            perror("Can't read base");
            fclose( bf );
            return Err;
            }
        }

    fclose( bf );
    return Ok;
a142 8
//
// Возвpащаем Yes если пpедложеный RIP стоит помещеня в нашу базу
// и pассылки далее. Пpичины:
//     - База пуста
//     - В базе нет записей с адpесом, pавным rip.ad
//     - Такие записи есть, но они дают худшее качество pоутинга
//     - Записи есть, но очень стаpые
//
a145 2
bool base::check( const rip &it ) const
    {
a146 2
    // if base is empty - anything looks great
    if(count() == 0) return Yes;
a147 1
    bool got_a_match = No;
a148 24
    for( int i = 0; i < count(); i++ )
        {
        base_data &bb = *(b[i]);
        
	if( match( bb, it ) )
            {
            got_a_match = Yes;
            if( bb.too_old() )
                {
                string s1 = bb.route_for();
                string s2 = bb.route_via();
                BoldMsg("Route ("+s1+" > "+s2+") replaced");
                return Yes;
                }
            
            if( better( bb, it ) )
                return Yes;
            }
        }

//    debug("got_a_match " + got_a_match );

    return got_a_match ? No : Yes;
    }
d152 4
a155 1
    
d173 1
a173 1
                if( better( br, r ) )
d192 6
a197 2
    time_t tt = time(NULL);
    base_data  *rec = new base_data( r, *localtime( &tt ) );
d212 1
a212 17

bool base::better( const base_data &b, const rip &r ) const
    {
    time_t tt = time(NULL);
    tm  t = *localtime( &tt );
    
    if( ((tm)b).tm_year < ((tm)r).tm_year-t.tm_year ) return No;
    if( ((tm)b).tm_mon  < ((tm)r).tm_mon -t.tm_mon )  return No;
    if( ((tm)b).tm_mday < ((tm)r).tm_mday-t.tm_mday ) return No;
    if( ((tm)b).tm_hour < ((tm)r).tm_hour-t.tm_hour ) return No;
    if( ((tm)b).tm_min  < ((tm)r).tm_min -t.tm_min )  return No;

    if( r.hops() > b.hops() ) return No;

    return Yes;
    }

d251 1
a251 1

d258 13
a270 1
// match a rip against a base record
d272 3
a274 1
bool base::match( const base_data &bd, const rip &ri ) const
d279 31
a309 1
    bool rip_mask_mode = Yes;
d312 5
a316 2
        // Say 'Ad 'rip.ad" matches our aka -> will'nt be treated as mask"
        rip_mask_mode = No;
d318 1
a318 2

    bool base_mask_mode = Yes;
d321 5
a325 2
        // Say 'Base line 'b" matches our aka -> will'nt be treated as mask"
        base_mask_mode = No;
d327 36
d364 1
a364 1
    if (b.domain() != string("*") )
d376 1
a376 8
    if( b.zone() != WILD && r.zone() != b.zone() ) return No;
    if( b.net()  != WILD && r.net()  != b.net()  ) return No;
    if( b.node() != WILD && r.node() != b.node() ) return No;

    /* node is allways a default route for its points */
    if( b.point() != WILD && b.point() != 0 & r.point() != b.point() ) return No;

    if( rip_mask_mode==0 )
d385 1
a385 1
    if( base_mask_mode==0 )
d396 2
a397 2


d459 129
@


1.2
log
@beta 9
@
text
@d172 1
a172 1
    bool removed;
d198 1
a198 1
                    removed = Yes;
d205 1
a205 1
    if( !removed )
a392 1

@


1.1
log
@Initial revision
@
text
@a2 8
/*
 
 #include "base.h"

#include <io.h>
#include <stdio.h>
#include <time.h>
 */
d96 1
a96 1
bool base::save()
d125 10
d136 1
a136 1
bool base::check( rip &it )
d146 3
a148 1
	if( match( *(b[i]), it ) )
d151 9
a159 1
            if( better( *(b[i]), it ) )
d169 48
d218 8
a225 1
bool base::better( base_data &b, rip &r )
d229 1
a229 1

d244 1
a244 1
base_data::base_data( rip &r, tm now )
d265 1
a268 1
        exit( 2356 );
d270 1
a270 1
    
a279 32
bool base::update( rip &r )
    {
    
    bool removed;
    
    do
        {
        removed = No;
        for( int i = 1; i < count(); i++ )
            {
            if( match( *(b[i]), r ) ) /* remove lines that worse than ours */
                if( better( *(b[i]), r ) )
                    {
                    // kill( i ); // mark base record as deleted
                    b[i].~base_data();
                    b.removeAt( i );
                    removed = Yes;
                    break;
                    }
            }
        } while( removed );

    time_t tt = time(NULL);
    base_data  *rec = new base_data( r, *localtime( &tt ) );
    
    if( !b.insert( rec ) )
        return Err;
    
    return Ok;
    
            
    }
d288 1
a288 1
bool base::match( base_data &bd, rip &ri )
d362 1
a362 1
    sprintf( line, "%s %s %d/%d/%d %d:%d:%d %d %ld",
d404 6
@
