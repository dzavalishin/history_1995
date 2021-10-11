head	1.6;
access;
symbols;
locks;
comment	@ * @;


1.6
date	97.03.22.16.05.27;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	97.03.05.18.12.38;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	97.01.21.08.53.51;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	97.01.01.21.31.39;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	97.01.01.16.26.19;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	97.01.01.16.08.42;	author dz;	state Exp;
branches;
next	;


desc
@@


1.6
log
@*** empty log message ***
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	datetime class
 *
 *      $Log: datetime.h $
 *      Revision 1.5  1997/03/05 18:12:38  dz
 *      Ver. 34
 *
 *      Revision 1.4  1997/01/21 08:53:51  dz
 *      *** empty log message ***
 *
 *      Revision 1.3  1997/01/01 21:31:39  dz
 *      msg received time/date support
 *
 *      Revision 1.2  1997/01/01 16:26:19  dz
 *      msgtime support
 *
 *      Revision 1.1  1997/01/01 16:08:42  dz
 *      Initial revision
 *
 *
 *      
 *      
\*/

#ifndef DATETIME_H
#define DATETIME_H

#include "style.h"
#include "strng.h"
#include <time.h>

#ifdef __WATCOMC__
#define _timezone timezone
#endif


// --------------------------------- *.msg date/time strucuture --------------------------------------------------

struct msgtime
    {
    unsigned  day   : 5;
    unsigned  month : 4;
    unsigned  year  : 7;
    unsigned  tsec  : 5;
    unsigned  min   : 6;
    unsigned  hour  : 5;
    };



// --------------------------------- basic date/time class --------------------------------------------------



  //
  // It is supposed that value stored in GMT/UTC only. We're trying to convert it to UTC
  // on input.


class datetime
    {
    public:
        enum tztype { GMT = 0, UTC = 0, LocalTime = 1, Local = 1 };

    private:
        time_t      value;

    public:

          // generate

        static datetime now();
        
          // construct/set
        
        datetime( time_t t, tztype tzt );
        datetime( time_t t, long tz ); // tz is in seconds, according to _timezone c set var

        datetime( const tm &, tztype tzt );
        datetime( const tm &, long tz );

          // "named constructors"
        static datetime from_rip_string( string );
        static datetime from_time_t_string( string s, tztype tzt );

          // helper
        void      load_tm( const tm & );

          // get

        //operator  time_t () const; // GMT!
        //operator  tm     () const; // GMT!
        //operator  string () const; // GMT!
        //operator msgtime () const;

        operator  signed long() const { return value; }

        time_t    localtime() const;
        string    localtime_rip_string() const;
        string    localtime_via_string() const;
        string    localtime_ctime_locale_string() const;
        tm        localtime_tm() const;
        msgtime   localtime_msgtime() const;

        time_t    gmtime() const;
        string    gmtime_rip_string() const;
        string    gmtime_time_t_string() const;
        tm        gmtime_tm() const;
        msgtime   gmtime_msgtime() const;


          // convert

          // from UTC/GMT to given tz
        time_t    convert_to_tz( int tz );


          // compare

        int       operator >  ( const datetime & ) const;
        int       operator <  ( const datetime & ) const;
        int       operator == ( const datetime & ) const;
        int       operator != ( const datetime & ) const;

        datetime  operator +  ( const datetime & ) const;
        datetime  operator += ( const datetime & );
        datetime  operator -  ( const datetime & ) const;
        datetime  operator -= ( const datetime & );
        
    };







inline datetime::datetime( time_t t, tztype tzt )
    {
    value = t;
    if( tzt != UTC ) value += _timezone;
    }

inline datetime::datetime( time_t t, long tz ) // tz is in seconds, according to _timezone c set var
    {
    value = t+tz;
    }


inline datetime::datetime( const tm &tt, tztype tzt )
    {
    load_tm( tt );
    if( tzt != UTC ) value += _timezone;
    }

inline datetime::datetime( const tm &tt, long tz )
    {
    load_tm( tt );
    value += tz;
    }







  // get

/*
inline datetime::operator time_t () const
    {
    return value;
    }

inline datetime::operator tm     () const
    {
#ifdef unix
    return *::gmtime( &value );       // in unix gmtime does not correct tz
#else
    return *::localtime( &value );    // in dos/os2/windows localtime does not correct tz
#endif
    }

inline datetime::operator string () const
    {
    return gmtime_string();
    }
*/




inline time_t datetime::localtime() const
    {
    return value-_timezone;
    }
    
    
inline tm     datetime::localtime_tm() const
    {
    time_t tt = value-_timezone;
#ifdef unix
    return *::gmtime( &tt );       // in unix gmtime does not correct tz
#else
    return *::localtime( &tt );    // in dos/os2/windows localtime does not correct tz
#endif
    }




inline time_t datetime::gmtime() const
    {
    return value;
    }

inline tm     datetime::gmtime_tm() const
    {
#ifdef unix
    return *::gmtime( &value );       // in unix gmtime does not correct tz
#else
    return *::localtime( &value );    // in dos/os2/windows localtime does not correct tz
#endif
    }

inline msgtime datetime::gmtime_msgtime() const
    {
    tm tt = this->gmtime_tm();
    msgtime mt;

    mt.day   = tt.tm_mday;
    mt.month = tt.tm_mon;
    mt.year  = tt.tm_year;
    mt.tsec  = tt.tm_sec / 2;
    mt.min   = tt.tm_min;
    mt.hour  = tt.tm_hour;

    return mt;
    }

inline msgtime datetime::localtime_msgtime() const
    {
    tm tt = this->localtime_tm();
    msgtime mt;

    mt.day   = tt.tm_mday;
    mt.month = tt.tm_mon;
    mt.year  = tt.tm_year;
    mt.tsec  = tt.tm_sec / 2;
    mt.min   = tt.tm_min;
    mt.hour  = tt.tm_hour;

    return mt;
    }

/*
inline datetime::operator msgtime () const
    {
    tm tt = gmtime_tm();
    msgtime mt;

    mt.day   = tt.tm_mday;
    mt.month = tt.tm_mon;
    mt.year  = tt.tm_year;
    mt.tsec  = tt.tm_sec / 2;
    mt.min   = tt.tm_min;
    mt.hour  = tt.tm_hour;
    return mt;
    }
*/





          // convert

          // from UTC/GMT to given tz
inline time_t datetime::convert_to_tz( int tz )
    {
    return value-tz;
    }



          // compare

inline int datetime::operator >  ( const datetime &v ) const { return value > v.value; }
inline int datetime::operator <  ( const datetime &v ) const { return value < v.value; }
inline int datetime::operator == ( const datetime &v ) const { return value == v.value; }
inline int datetime::operator != ( const datetime &v ) const { return value != v.value; }

inline datetime  datetime::operator +  ( const datetime &v ) const { return datetime(time_t(value + v.value), UTC); }
inline datetime  datetime::operator += ( const datetime &v )       { value += v.value; return *this; }
inline datetime  datetime::operator -  ( const datetime &v ) const { return datetime(time_t(value - v.value), UTC); }
inline datetime  datetime::operator -= ( const datetime &v )       { value -= v.value; return *this; }




  // generators

inline datetime datetime::now()
    {
#ifdef unix
    return datetime( time(0), datetime::UTC );          // in unix time() returns UTC time
#else
    return datetime( time(0), datetime::LocalTime );    // in dos/os2/win time() returns local time
#endif
    }









#endif // DATETIME_H





@


1.5
log
@Ver. 34
@
text
@d8 3
d98 2
a99 1
        
d104 1
@


1.4
log
@*** empty log message ***
@
text
@d8 3
d32 3
@


1.3
log
@msg received time/date support
@
text
@d8 3
a72 2
        datetime( string );

d76 5
d85 5
a89 3
        operator  time_t () const; // GMT!
        operator  tm     () const; // GMT!
        operator  string () const; // GMT!
d92 2
a93 1
        string    localtime_string() const;
d98 2
a99 1
        string    gmtime_string() const;
a109 2
        operator msgtime () const;
        
d162 1
d181 1
a181 16


inline datetime::operator msgtime () const
    {
    tm tt = this->operator tm();
    msgtime mt;

    mt.day   = tt.tm_mday;
    mt.month = tt.tm_mon;
    mt.year  = tt.tm_year;
    mt.tsec  = tt.tm_sec / 2;
    mt.min   = tt.tm_min;
    mt.hour  = tt.tm_hour;
    return mt;
    }

a201 16
inline msgtime datetime::localtime_msgtime() const
    {
    tm tt = this->localtime_tm();
    msgtime mt;

    mt.day   = tt.tm_mday;
    mt.month = tt.tm_mon;
    mt.year  = tt.tm_year;
    mt.tsec  = tt.tm_sec / 2;
    mt.min   = tt.tm_min;
    mt.hour  = tt.tm_hour;

    return mt;
    }


d233 32
@


1.2
log
@msgtime support
@
text
@d8 3
d86 1
d91 1
d176 1
a176 1
    tm tt = this->operator tm;
d185 1
d219 2
d252 2
d286 1
a286 1
static inline datetime now()
d288 5
a292 1
    return datetime( time(0), datetime::UTC );
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: app.h $
d24 19
d95 2
a127 1
//inline datetime::datetime( string );
a140 1
//inline datetime::load_tm( const tm & );
d142 6
a147 1
          // get
d168 18
a190 1
//inline string datetime::localtime_string() const
d202 17
a223 2
  //inline string datetime::gmtime_string() const;

d233 15
d258 1
a279 5





@
