head	1.11;
access;
symbols
	Beta_12:1.1;
locks;
comment	@ * @;


1.11
date	97.03.26.10.57.57;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	97.03.22.15.48.26;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	97.03.05.18.12.38;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	97.01.31.16.09.43;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.09.08.19.05.31;	author dz;	state Exp;
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


1.11
log
@showbase
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	RIP Database I/O
 *
 *      $Log: Base_rw.C $
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



@


1.10
log
@datetime
@
text
@d8 3
d244 5
a248 1

@


1.9
log
@Ver. 34
@
text
@d8 3
d48 1
d51 1
a51 1
    string hops_s, rfor_s, rvia_s, date_s, time_s;
d53 1
a53 6
    if( 
       (!s.parse( rfor_s, s )) ||
       (!s.parse( rvia_s, s )) ||
       (!s.parse( date_s, s )) ||
       (!s.parse( time_s, s ))
      )
a55 3
    tm       togo;
    time_t   line_created;
    int      hops = 0;
d57 26
a82 8
    
    if( 3 != sscanf( date_s, "%d/%d/%d", 
                    &togo.tm_mday, &togo.tm_mon, &togo.tm_year ) )
        throw Ex_Arg( "base::add", "Base date format incorrect", orig );
    
    if( 3 != sscanf( time_s, "%d:%d:%d", 
                    &togo.tm_hour, &togo.tm_min, &togo.tm_sec ) )
        throw Ex_Arg( "base::add", "Base time format incorrect", orig );
d87 21
a107 14
    if( !s.parse( hops_s, s ) )   // last part is hop count?
        line_created = time(NULL); // assume record was created right now
    else
        line_created = atol( s );
    
    hops = atoi( hops_s );
    
    // here we have line_created as the first token in s.
    // eat it now.
    
        {
        string _;
        s.parse( _, s );
        }
d109 5
d120 3
a122 1
    b.push_back( base_data( rofor, rovia, togo, hops, line_created, fplus, fminus, finfo ) );
@


1.8
log
@Changed order of file processing to ensure
fail-safe operation.
@
text
@d8 4
d43 2
d100 2
d124 2
d137 2
d150 1
a150 1
        try { add(l); }
d154 5
d166 2
a167 1
void base::save() const
d169 4
d183 3
d187 1
d189 3
d194 4
d203 2
d208 1
@


1.7
log
@Version 25
@
text
@d8 3
d94 9
a102 2
    FILE *bf = fopen( conf.dir()+"/RIP_Base", "r" );
    if( bf == NULL ) bf = fopen( conf.dir()+"/RIP_Base.Tmp", "r" );
d107 18
a124 10
        chmod( conf.dir()+"/RIP_Base.S2", 0666 );
        unlink( conf.dir()+"/RIP_Base.S2" );
        chmod( conf.dir()+"/RIP_Base.S1", 0666 );
        rename( conf.dir()+"/RIP_Base.S1", conf.dir()+"/RIP_Base.S2" );
        chmod( conf.dir()+"/RIP_Base.S0", 0666 );
        rename( conf.dir()+"/RIP_Base.S0", conf.dir()+"/RIP_Base.S1" );
        chmod( conf.dir()+"/RIP_Base.Old", 0666 );
        rename( conf.dir()+"/RIP_Base.Old", conf.dir()+"/RIP_Base.S0" );
        
        throw Ex_Errno("base::load()","Can't open base", errno );
d139 1
a139 1
        catch( General_Ex ex ) { ex.print(); }
d151 8
a158 8
    chmod( conf.dir()+"/RIP_Base.Old", 0666 );
    unlink( conf.dir()+"/RIP_Base.Old" );
    chmod( conf.dir()+"/RIP_Base", 0666 );
    rename( conf.dir()+"/RIP_Base", conf.dir()+"/RIP_Base.Old" );
    unlink( conf.dir()+"/RIP_Base" );
    
    FILE *bf = fopen( conf.dir()+"/RIP_Base.Tmp", "w" );
    
a160 1

a162 1

d165 8
a172 5
    chmod( conf.dir()+"/RIP_Base.Tmp", 0666 );
    chmod( conf.dir()+"/RIP_Base", 0666 );
    unlink( conf.dir()+"/RIP_Base" );
    if( rename( conf.dir()+"/RIP_Base.Tmp", conf.dir()+"/RIP_Base" ) )
        throw Ex_Errno("base::save","Can't rename "+conf.dir()+"/RIP_Base.Tmp to "+conf.dir()+"/RIP_Base", errno );
d174 5
@


1.6
log
@Exceptions added, not compiled
@
text
@d8 3
d41 4
a44 4
       (!parse( s, rfor_s, s )) ||
       (!parse( s, rvia_s, s )) ||
       (!parse( s, date_s, s )) ||
       (!parse( s, time_s, s ))
d64 1
a64 1
    if( !parse( s, hops_s, s ) )   // last part is hop count?
d76 1
a76 1
        parse( s, _, s );
d110 1
a110 1
    while( l.load( bf ) == Ok )
d112 2
d145 1
a145 4
        {
        if( ((string)b[i]).save( bf ) != Ok )
            throw Ex_Errno("base::save","Can't write base", errno );
        }
@


1.5
log
@Before exceptions.
@
text
@d8 3
d31 1
a31 1
bool base::add( string s )
d43 1
a43 4
        {
        Error( "Base format incorrect: "+orig );
        return Err;
        }
d52 1
a52 4
        {
        Error( "Base date format incorrect: "+orig );
        return Err;
        }
d56 2
a57 6
        {
        Error( "Base time format incorrect: "+orig );
        return Err;
        }
    
    
d82 1
a82 4
    b.push_back( 
                base_data( rofor, rovia, togo, hops, line_created, fplus, fminus, finfo ) 
               );
    return Ok;
d86 1
a86 1
bool base::load()
d89 1
d93 11
a103 2
        perror("Can't open base");
        return Err;
a106 1
    
d115 2
a116 6
        if( add(l) == Err )
            {
            perror("Can't read base");
            fclose( bf );
            return Err;
            }
a117 1
	
a119 1
    return Ok;
d126 1
a126 1
bool base::save() const
d128 1
d130 1
d134 1
a134 1
    FILE *bf = fopen( conf.dir()+"/RIP_Base", "w" );
d137 1
a137 4
        {
        perror("Can't create base");
        return Err;
        }
d142 1
a142 5
            {
            perror("Can't read base");
            fclose( bf );
            return Err;
            }
d146 6
a151 1
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
 *      $Log: Base_rw.c $
a81 24
#if 0
    string fs;
    
    s.strip_ws();
    if( s[0] == '+' )
        {
        parse( s, fs, s );
        fplus = flags( fs.substr( 1 ) );
        }
    
    s.strip_ws();
    if( s[0] == '-' )
        {
        parse( s, fs, s );
        fminus = flags( fs.substr( 1 ) );
        }
    
    s.strip_ws();
    if( s[0] == '!' )
        {
        parse( s, fs, s );
        finfo = flags( fs.substr( 1 ) );
        }
#else
a84 1
#endif
a85 1
#if USE_STL
a88 9
#else
    base_data *l = new base_data( 
                                 rofor, rovia, 
                                 togo, hops, line_created,
                                 fplus, fminus, finfo
                                );
    
    b.insert( l );
#endif
a145 1
#if USE_STL
a146 3
#else
        if( ((string)(*(b[i]))).save( bf ) != Ok )
#endif
@


1.2
log
@Beta 15
@
text
@d8 3
d108 5
d120 1
d178 3
d182 1
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:$
d66 44
a109 1
    base_data *l = new base_data( rofor, rovia, togo, hops, line_created );
@
