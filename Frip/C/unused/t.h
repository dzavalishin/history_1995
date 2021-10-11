/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	time class
 *
 *      $Log: t.h $
 *      Revision 1.2  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.1  1996/08/04 08:18:43  dz
 *      Initial revision
 *
 *
 *
\*/

#ifndef T_H
#define T_H

class time
    {
    protected:
        time_t      t_v;    // in GMT!
        static      tz_off; // what should we add to local time to get GMT
        static bool got_tz;
        
    public:

        time( time_t t ) { t_v = t; }
        time( ) { t_v = ::time(0); }
        
        operator time_t() const { return t_v; }

        bool     from_rfc( string s, bool gmt = No );
        bool     from_ftn( string s, bool gmt = No );

        string   to_rfc( bool gmt = No );
        string   to_ftn( bool gmt = No );

        time_t   to_local() const;
        time_t   to_gmt() const { return t_v; }

        void     from_local(time_t t)  { if(!got_tz) get_tz(); t_v = t+tz_off; }
        void     from_gmt(time_t t) { t_v = t; }
        
    };

extern const time Program_start_time;

#endif // T_H

