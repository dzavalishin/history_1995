/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	RIP Database
 *
 *      $Log: Base.C $
 *      Revision 1.18  1997/03/22 15:48:26  dz
 *      datetime
 *
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


static int hops_penalty( int h )
    {
    int p = 0;
    
    if( h > 2 ) p += ( conf.hop_penalty() * (h-2) );
    if( h > 1 ) p += conf.direct_priority();

    return p;
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

    // time to go as in base
    signed long tb = b.time_to_go();
    // time to go as in rip
    signed long tr = ((signed long)t) - ((signed long)r.created());

    int hb = b.hops();
    int hr = r.hops();
    
    if( hb == 0 && hr != 0 ) return No; // Never route ourself via others
    //if( r.hops() == 0 ) return Yes; // Do we need to prioritize 'em?

    if( hr != hb )
        {
        tb += hops_penalty( hb );
        tr += hops_penalty( hr );
        }
    
    if( tb < tr ) return No;

    if( hr > hb ) return No;

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

    if( fatal_timeout < (24l*60l*60l * 20))
        {
        Error("Fatal timeout can't be < 20 days! default of 20 days used.");
        fatal_timeout = (24l*60l*60l * 20);
        }

    datetime now = datetime::now();
    
    for( int i = 0; i < count(); i++ )
        {
        const base_data &bb = b[i];
        int timediff = (now - bb.created());
        if( timediff > fatal_timeout )
            {
            string d;
            d.print("get timed out routes: timediff %d < f_to %d", timediff, fatal_timeout );
            debug(d);
            d.print(
                    "get timed out routes: now %s, route created %s",
                    now.localtime_ctime_locale_string().c_str(),
                    bb.created().localtime_ctime_locale_string().c_str()
                   );
            debug(d);
            out.push_back( b[i] );
            }
        }
    }



