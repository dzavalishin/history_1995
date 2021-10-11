head	1.10;
access;
symbols
	Beta_12:1.1;
locks;
comment	@ * @;


1.10
date	97.03.28.22.02.29;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	97.03.26.10.57.46;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	97.03.22.15.48.26;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	97.03.05.18.12.38;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.07.31.07.26.25;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.01.22.04.35.34;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.01.18.00.58.05;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	;


desc
@@


1.10
log
@laziness was counted in with opposite sign :(
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	RIP Database record
 *
 *      $Log: BaseData.C $
 *      Revision 1.9  1997/03/26 10:57:46  dz
 *      showbase
 *
 *      Revision 1.8  1997/03/22 15:48:26  dz
 *      datetime
 *
 *      Revision 1.7  1997/03/05 18:12:38  dz
 *      Ver. 34
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
 *      Revision 1.2  1996/01/18 00:58:05  dz
 *      Beta 15
 *
 *      Revision 1.1  1996/01/07 20:21:01  dz
 *      Initial revision
 *
 *
 *
\*/

#include "frip.h"





// create base line from rip
base_data::base_data( const rip &r, const datetime &now )
    :
time_to_go_v( ((long)now.gmtime()) - ((long)r.created().gmtime()) ),
time_created_v( datetime::now() )
    {
    //datetime rt = r.created();
    //tm        bt;
    
    //bt.tm_sec  = now.tm_sec -rt.tm_sec;
    //if( bt.tm_sec  < 0 ) { bt.tm_sec  += 60; now.tm_min --; }
    
    //bt.tm_min  = now.tm_min -rt.tm_min;
    //if( bt.tm_min  < 0 ) { bt.tm_min  += 60; now.tm_hour--; }
    
    //bt.tm_hour = now.tm_hour-rt.tm_hour;
    //if( bt.tm_hour < 0 ) { bt.tm_hour += 24; now.tm_mday--; }
    
    //bt.tm_mday = now.tm_mday-rt.tm_mday;
    //if( bt.tm_mday < 0 ) { bt.tm_mday += 31; now.tm_mon --; }
    
    //bt.tm_mon  = now.tm_mon -rt.tm_mon;
    //if( bt.tm_mon  < 0 ) { bt.tm_mon  += 12; now.tm_year--; }
    
    //bt.tm_year = now.tm_year-rt.tm_year;
/*
    if( bt.tm_year < 0 ) 
        {
        Error("Crazy... Over the rainbow I'm crazy...");
        }
*/    
    route_for_v = r.ad();
    route_via_v = r.from();
    hops_v = r.hops();
    fplus_v = r.fplus();
    fminus_v = r.fminus();
    finfo_v = r.finfo();
    }



// format of line: route-for route-via date time hops [time_t date-of-line]
base_data::base_data( string s )
    :
time_created_v( datetime::now() )
    {
    string orig = s;

    string date_s;
        {
        string rfor_s, rvia_s;
    
        if( (!s.parse( rfor_s, s )) || (!s.parse( rvia_s, s )) || (!s.parse( date_s, s )) )
            throw Ex_Arg( "base::add", "Base format incorrect", orig );

        route_for_v = rfor_s;
        route_via_v = rvia_s;
        }

        {
        tm       togo;
        if( 3 == sscanf( date_s, "%d/%d/%d", &togo.tm_mday, &togo.tm_mon, &togo.tm_year ) )
            {
            string time_s;
            if(!s.parse( time_s, s ))
                throw Ex_Arg( "base::add", "Base format incorrect", orig );
            if( 3 != sscanf( time_s, "%d:%d:%d",
                            &togo.tm_hour, &togo.tm_min, &togo.tm_sec ) )
                throw Ex_Arg( "base::add", "Base time format incorrect", orig );

              // This conversion is not exact, but we will apply it only once -
              // on the first run base will be written in a new format entirely
            int togo_long;
            togo_long  = togo.tm_year;            togo_long *= 12;
            togo_long += togo.tm_mon;             togo_long *= 31;
            togo_long += togo.tm_mday;            togo_long *= 24;
            togo_long += togo.tm_hour;            togo_long *= 60;
            togo_long += togo.tm_min;             togo_long *= 60;
            togo_long += togo.tm_sec;

            time_to_go_v = togo_long;
            }
        else
            if( 1 != sscanf( date_s, "%d", &time_to_go_v ) )
                throw Ex_Arg( "base::add", "Base date/time format incorrect", orig );

        }

    // Last, we'll try to handle old base format, which ends with
    // hop count only
    
    //datetime line_created;
    //if( !s.parse( hops_s, s ) )   // last part is hop count?
    //    line_created = datetime::now(); // assume record was created right now
    //else
    //    line_created = atol( s );

    //datetime line_created( s.parse( hops_s, s ) ? datetime(atol( s ),datetime::UTC) : datetime::now() );
    /*
    datetime line_created(
                          s.parse( hops_s, s )
                          ?
                          datetime::from_time_t_string( s, datetime::UTC )
                          :
                          datetime::now()
                         );
    */

    string hops_s;
    if(!s.parse( hops_s, s ))
        throw Ex_Arg( "base::add", "Base format incorrect", orig );
    hops_v = atoi( hops_s );
    
    string created_s;
    if(!s.parse( created_s, s ))
        throw Ex_Arg( "base::add", "Base format incorrect", orig );
    time_created_v = datetime::from_time_t_string( created_s, datetime::UTC );

    fplus_v. extract( s, '+' );
    fminus_v.extract( s, '-' );
    finfo_v. extract( s, '!' );
    }


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

    sprintf( line, "%s %s %d %d %ld +%s -%s !%s",
            (const char*)((string)route_for_v),                 (const char*)((string)route_via_v),
            time_to_go_v,  hops_v,  time_created_v,
            (const char*)((string)fplus_v), (const char*)((string)fminus_v), (const char*)((string)finfo_v)
            );
    
    return line;
    }


void base_data::showbase() // user-oriented display
    {
    char line[1024];
    int i, days, hours, minutes;

    string os1 = route_for_v.short_string() + " > " + route_via_v.short_string();

    i = time_to_go_v;

    days    = i / (24*60*60);
    i       = i % (24*60*60);
    hours   = i / (60*60);
    i       = i % (60*60);
    minutes = i / 60;

    if( days > 0 )
        sprintf( line, "tm %2dd %2dh %2dh", days, hours, hops_v );
    else
        sprintf( line, "tm %2d:%02d   %2dh", hours, minutes, hops_v );
    //Msg( line );
    string os2 =  line;

    

    i = datetime::now() - time_created_v;
    days    = i / (24*60*60);
    i       = i % (24*60*60);
    hours   = i / (60*60);
    i       = i % (60*60);
    minutes = i / 60;

    if( days > 0 )
        sprintf(
                line, " cr. %2dd %2dh ago", days, hours
               );
    else
        sprintf(
                line, " cr. %2d:%02d ago", hours, minutes
               );
    //Msg( line );
    os2 += line;

    sprintf( line, "%-35s %s", (const char *)os1, (const char *)os2 );
    cout << line;
    
    if(!fplus_v.is_empty())  cout << " +"<< (const char*)((string)fplus_v);
    if(!fminus_v.is_empty()) cout << " -"<< (const char*)((string)fminus_v);
    if(!finfo_v.is_empty())  cout << " !"<< (const char*)((string)finfo_v);

    cout << "\n";
    }


base_data::operator == ( const base_data &his ) const
    {
    return
        time_to_go_v == his.time_to_go_v &&
        
        hops_v       == his.hops_v       &&
        route_for_v  == his.route_for_v  &&
        route_via_v  == his.route_via_v  &&
        
        fplus_v      == his.fplus_v      &&
        fminus_v     == his.fminus_v     &&
        finfo_v      == his.finfo_v
        ;
    }


void base::foreach( void (*func)( base_data ) )
    {
    if(!enabled) throw Ex_Fail("base::foreach","disabled","");
    changed = Yes;
    for( int i = 0; i < count(); i++ )
        func( b[i] );
    }

    
#if 0
bool base_data::too_old() const
    {
    time_t diff = time(0) - time_created_v;
    //fprintf( stderr, "timediff = %ld, threshold = %ld\n", diff, conf.db_timeout());
    return (diff > conf.db_timeout()) ? Yes : No;
    }
#endif
    
//
// Предполагается, что laziness - это время прохождения рипа-претендента
// в секундах. Чем дольше он шел, тем у него меньше шансов срубить эту запись
// в базе. Иначе выходит бардак - всякая шваль иногда берет верх, а это
// не в кайф.
//

bool base_data::too_old( datetime laziness ) const
    {
    // laziness /= 2; // Иначе тестировать невозможно :)
    
    signed long now = datetime::now();
    signed long diff = now - long(time_created_v) - laziness;
#if 1
    if( conf.debug() )
        {
        char buf[200];
        sprintf( buf, "if timeout %ld < (timediff %ld = now %ld - created %ld - laziness %ld)",
                conf.db_timeout(), diff, now, time_created_v, laziness );
        debug(buf);
        }
#endif
    return (diff > ((signed long)conf.db_timeout())) ? Yes : No;
    }
    
    @


1.9
log
@showbase
@
text
@d8 3
d290 1
a290 1
    signed long diff = now - long(time_created_v - laziness);
@


1.8
log
@datetime
@
text
@d8 3
d45 1
a45 1
time_to_go_v( now - r.created() ),
d185 53
@


1.7
log
@Ver. 34
@
text
@d8 3
d40 4
a43 1
base_data::base_data( const rip &r, tm now )
d45 2
a46 2
    const tm &rt = r.created();
    tm        bt;
d48 2
a49 2
    bt.tm_sec  = now.tm_sec -rt.tm_sec;
    if( bt.tm_sec  < 0 ) { bt.tm_sec  += 60; now.tm_min --; }
d51 2
a52 2
    bt.tm_min  = now.tm_min -rt.tm_min;
    if( bt.tm_min  < 0 ) { bt.tm_min  += 60; now.tm_hour--; }
d54 2
a55 2
    bt.tm_hour = now.tm_hour-rt.tm_hour;
    if( bt.tm_hour < 0 ) { bt.tm_hour += 24; now.tm_mday--; }
d57 2
a58 2
    bt.tm_mday = now.tm_mday-rt.tm_mday;
    if( bt.tm_mday < 0 ) { bt.tm_mday += 31; now.tm_mon --; }
d60 2
a61 2
    bt.tm_mon  = now.tm_mon -rt.tm_mon;
    if( bt.tm_mon  < 0 ) { bt.tm_mon  += 12; now.tm_year--; }
d63 1
a63 1
    bt.tm_year = now.tm_year-rt.tm_year;
a69 2
    time_to_go_v = bt;
    time_created_v = time(NULL);
d80 81
d174 5
a178 15
    
    sprintf( line, "%s %s %d/%d/%d %d:%d:%d %d %ld +%s -%s !%s",
            (const char*)((string)route_for_v),
            (const char*)((string)route_via_v),
            time_to_go_v.tm_mday,
            time_to_go_v.tm_mon,
            time_to_go_v.tm_year,
            time_to_go_v.tm_hour,
            time_to_go_v.tm_min,
            time_to_go_v.tm_sec,
            hops_v,
            time_created_v,
            (const char*)((string)fplus_v),
            (const char*)((string)fminus_v),
            (const char*)((string)finfo_v)
d188 1
a188 6
        time_to_go_v.tm_mday == his.time_to_go_v.tm_mday &&
        time_to_go_v.tm_mon  == his.time_to_go_v.tm_mon  &&
        time_to_go_v.tm_year == his.time_to_go_v.tm_year &&
        time_to_go_v.tm_hour == his.time_to_go_v.tm_hour &&
        time_to_go_v.tm_min  == his.time_to_go_v.tm_min  &&
        time_to_go_v.tm_sec  == his.time_to_go_v.tm_sec  &&
d226 1
a226 1
bool base_data::too_old( time_t laziness ) const
d230 3
a232 3
    signed long now = time(0);
    signed long diff = now - time_created_v - laziness;
#if 0
@


1.6
log
@Exceptions added, not compiled
@
text
@d8 3
d133 2
@


1.5
log
@Before exceptions.
@
text
@d8 3
d128 1
a128 1
bool base::foreach( void (*func)( base_data ) )
a131 2
    
    return Ok;
@


1.4
log
@Clean up of watcom classlib based code
@
text
@d8 3
@


1.3
log
@C Set & STL support
@
text
@d7 4
a10 1
 *      $Log: BaseData.c $
a124 1
#if USE_STL
a125 3
#else
        func( *(b[i]) );
#endif
@


1.2
log
@Beta 15
@
text
@d8 3
d122 3
d126 1
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:$
d54 3
d75 1
a75 1
    sprintf( line, "%s %s %d/%d/%d %d:%d:%d %d %ld + - !",
d85 4
a88 1
            time_created_v
d107 6
a112 1
        route_via_v  == his.route_via_v;
d124 2
d132 28
@
