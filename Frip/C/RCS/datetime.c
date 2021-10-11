head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	97.03.26.10.58.18;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	97.01.21.08.53.51;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	97.01.01.16.08.42;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@localtime_ctime_locale_string()
@
text
@#include "datetime.h"
#include "except.h"
#include <stdlib.h>



string datetime::gmtime_rip_string() const
    {
    tm c = gmtime_tm();

    char b[30];
    sprintf( b, "%d/%d/%d %d:%d:%d 0",
        c.tm_mday, c.tm_mon+1, c.tm_year,
        c.tm_hour, c.tm_min, c.tm_sec
       );

    return b;
    }

string datetime::gmtime_time_t_string() const
    {
    char b[30];
    sprintf( b, "%d", value );
    return b;
    }

string datetime::localtime_rip_string() const
    {
    tm c = localtime_tm();

    char b[30];
    sprintf( b, "%d/%d/%d %d:%d:%d %d",
        c.tm_mday, c.tm_mon+1, c.tm_year,
        c.tm_hour, c.tm_min, c.tm_sec,
        _timezone
       );

    return b;
    }


  // Sat Jul 16 02:03:55 1994
string datetime::localtime_ctime_locale_string() const
    {
    const tm c = localtime_tm();

    char b[30];
    strftime( b, sizeof b, "%a %b %d %H:%M:%S %Y", &c );

    return b;
    }


// Via 2:5020/251.81@@fidonet @@19970107.143738.UTC+3 T-Mail 2599.OS2.K

string datetime::localtime_via_string() const
    {
    tm c = localtime_tm();

    int y = c.tm_year;

    if( y > 80 ) y += 1900;
    else         y += 2000;

    char tzs[45];

    int tz = _timezone;

    char sign;
    if( _timezone < 0 )
        {
        sign = '+'; // ???
        tz = -tz;
        }
    else
        sign = '-'; // ???

    sprintf( tzs, "%d", tz / (60*60) );
    tz %= (60*60);

    if( tz )
        {
        char tmp[15];
        sprintf( tmp, ":%d", tz / 60 );
        tz %= 60;
        strcat( tzs, tmp );
        if( tz )
            {
            sprintf( tmp, ":%d", tz );
            strcat( tzs, tmp );
            }
        }

    char b[30];
    sprintf( b, "%04d%02d%02d.%02d%02d%02d.UTC%c%s",
            y, c.tm_mon+1, c.tm_mday,
            c.tm_hour, c.tm_min, c.tm_sec,
            sign, tzs
           );

    return b;
    }

void datetime::load_tm( const tm &tt )
    {
    tm temp = tt;
    value = mktime( &temp );
    if( value == -1 ) throw Ex_Arg( "datetime::load_tm", string(asctime(&tt)) );
    }

datetime datetime::from_rip_string( string s )
    {
    string date, time, tz, s1;

    if( !s.parse( date, s1 ) )
        throw Ex_Arg( "datetime::datetime", "RIP \"created\" line format incorrect", s );

    int tzsec = 0;
    if( s1.parse( time, tz ) )
        tzsec = atoi( tz );

    tm tt;
    
    if( 3 != sscanf( date, "%d/%d/%d", &tt.tm_mday, &tt.tm_mon,  &tt.tm_year ) )
        throw Ex_Arg( "rip::created_s", "RIP date format incorrect", s );

    tt.tm_mon--; // we store month as 1-12, but tm counts it as 0-11

    if( 3 != sscanf( time, "%d:%d:%d", &tt.tm_hour, &tt.tm_min, &tt.tm_sec ) )
        throw Ex_Arg( "rip::created_s", "RIP time format incorrect", s );


    return datetime( tt, tzsec );
    }


datetime datetime::from_time_t_string( string s, tztype tzt  )
    {
    time_t t = atoi(s);

    return datetime( t, tzt );
    }





@


1.2
log
@*** empty log message ***
@
text
@d41 13
@


1.1
log
@Initial revision
@
text
@d7 1
a7 1
string datetime::gmtime_string() const
d20 8
a27 1
string datetime::localtime_string() const
d41 50
d98 1
a98 1
datetime::datetime( string s )
d120 1
a120 1
    *this = datetime( tt, tzsec );
d124 6
@
