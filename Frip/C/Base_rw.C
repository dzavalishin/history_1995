/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	RIP Database I/O
 *
 *      $Log: Base_rw.C $
 *      Revision 1.11  1997/03/26 10:57:57  dz
 *      showbase
 *
 *      Revision 1.10  1997/03/22 15:48:26  dz
 *      datetime
 *
 *      Revision 1.9  1997/03/05 18:12:38  dz
 *      Ver. 34
 *
 *      Revision 1.8  1997/01/31 16:09:43  dz
 *      Changed order of file processing to ensure
 *      fail-safe operation.
 *
 *      Revision 1.7  1996/09/08 19:05:31  dz
 *      Version 25
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


// format of line: route-for route-via date time hops [time_t date-of-line]
void base::add( string s )
    {
    if(!enabled) throw Ex_Fail("base::add","disabled","");
    
#if 0
    string orig = s;
    
    string rfor_s, rvia_s, date_s, time_s;
    
    if( (!s.parse( rfor_s, s )) || (!s.parse( rvia_s, s )) || (!s.parse( date_s, s )) )
        throw Ex_Arg( "base::add", "Base format incorrect", orig );
    
    addr     rofor( rfor_s ), rovia( rvia_s );

    long     togo_long;
        {
        tm       togo;
        if( 3 == sscanf( date_s, "%d/%d/%d", &togo.tm_mday, &togo.tm_mon, &togo.tm_year ) )
            {
            if(!s.parse( time_s, s ))
                throw Ex_Arg( "base::add", "Base format incorrect", orig );
            if( 3 != sscanf( time_s, "%d:%d:%d",
                            &togo.tm_hour, &togo.tm_min, &togo.tm_sec ) )
                throw Ex_Arg( "base::add", "Base time format incorrect", orig );

              // This conversion is not exact, but we will apply it only once -
              // on the first run base will be written in a new format entirely
            togo_long  = togo.tm_year;            togo_long *= 12;
            togo_long += togo.tm_mon;             togo_long *= 31;
            togo_long += togo.tm_mday;            togo_long *= 24;
            togo_long += togo.tm_hour;            togo_long *= 60;
            togo_long += togo.tm_min;             togo_long *= 60;
            togo_long += togo.tm_sec;
            }
        else
            if( 1 != sscanf( date_s, "%d", &togo_long ) )
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
    int hops = atoi( hops_s );
    
    string created_s;
    if(!s.parse( created_s, s ))
        throw Ex_Arg( "base::add", "Base format incorrect", orig );
    datetime line_created( datetime::from_time_t_string( created_s, datetime::UTC ) );

    flags fplus, fminus, finfo;
    
    fplus. extract( s, '+' );
    fminus.extract( s, '-' );
    finfo. extract( s, '!' );
    
    b.push_back( base_data( rofor, rovia, togo_long, hops, line_created, fplus, fminus, finfo ) );
#endif
    b.push_back( base_data( s ) );
    }


void base::load()
    {
    if(!enabled) throw Ex_Fail("base::load","disabled","");

    string f_dat = conf.dir()+"/RIP_Base";
    string f_tmp = conf.dir()+"/RIP_Base.Tmp";
    string f_old = conf.dir()+"/RIP_Base.Old";
    string f_s0  = conf.dir()+"/RIP_Base.S0";
    string f_s1  = conf.dir()+"/RIP_Base.S1";
    string f_s2  = conf.dir()+"/RIP_Base.S2";


    FILE *bf = fopen( f_dat, "r" );
    
    if( bf == NULL )
        {
        // Try to save old base if someone killed main
        chmod ( f_s2, 0666 );
        unlink( f_s2 );
        chmod ( f_s1, 0666 );
        rename( f_s1, f_s2 );
        chmod ( f_s0, 0666 );
        rename( f_s0, f_s1 );
        chmod ( f_old, 0666 );
        rename( f_old, f_s0 );

        changed = Yes; // something is wrong. Save it anyway.
        
          // maybe we can pick up a previous base, at least?
        bf = fopen( f_s0, "r" );
        if( bf == NULL )
            {
              //throw Ex_Errno("base::load()","Can't open base", errno );
            Warning("base::load(): Can't open base or it's backup copy");
            return;
            }
        Warning("base::load(): Can't open base, using backup (now in RIP_Base.S0)");
        }

    int ndone = 0;
    
    string l;
    while( 1 )
        {
        try{ l.load( bf ); } catch(Ex_EOF) { break; }
        
        l.strip_leading_ws();
        l.strip_crlf();
        
        if(l[0] == ';' || l.is_empty() )
            continue;

        try { add(l); ndone++; }
        catch( General_Ex ex ) { ex.print(); throw; }
        }

    string s;
    s.print("base::load did %d", ndone );
    debug(s);

    
    fclose( bf );
    }





// const
void base::save() 
    {

    if(!changed) return;
    if(!enabled) throw Ex_Fail("base::save","disabled","");
    
    string f_dat = conf.dir()+"/RIP_Base";
    string f_tmp = conf.dir()+"/RIP_Base.Tmp";
    string f_old = conf.dir()+"/RIP_Base.Old";

    chmod( f_tmp, 0666 );
    unlink( f_tmp );

    FILE *bf = fopen( f_tmp, "w" );
    if( bf == NULL )
        throw Ex_Errno("base::save","Can't create base", errno );

    int ndone = 0;

    for( int i = 0; i < count(); i++ )
        {
        string(b[i]).save( bf );
        ndone++;
        }
    
    fclose( bf );

    string s;
    s.print("base::save had to write %d, did %d", count(), ndone );
    debug(s);
    
    chmod( f_old, 0666 );
    unlink( f_old );
    chmod( f_dat, 0666 );
    rename( f_dat, f_old );
    unlink( f_dat );

    changed = No; // can't do anything more
    
    if( rename( f_tmp, f_dat ) )
        throw Ex_Errno("base::save","Can't rename "+f_tmp+" to "+f_dat, errno );

    }



void base::showbase()
    {
    for( int i = 0; i < count(); i++ )
        b[i].showbase();
    }



