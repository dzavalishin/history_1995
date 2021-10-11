/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	RIF class
 *
 *      $Log: rif.h $
 *      Revision 1.7  1997/03/22 16:06:11  dz
 *      datetime
 *
 *      Revision 1.6  1997/01/21 08:53:51  dz
 *      Flags remade, msgid added.
 *
 *      Revision 1.5  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.4  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.3  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.2  1996/07/31 07:00:23  dz
 *      FripMail written
 *
 *      Revision 1.1  1996/07/22 02:48:05  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#ifndef RIF_H
#define RIF_H

#include "conf.h"
#include "addr.h"
#include "flag.h"
#include "datetime.h"
#include <time.h>
#include <fstream.h>

#include <vector.h>

//
//        Типы рифов
//
// F    - Просто файл. Принять и положить в инбаунд.
// Mail - NetMail. Принять и распаковать.
// 
// 

class rif_type
    {
public:
    enum rftype { NOP = 0, F = 1, File = 1, Mail = 2 };
    struct rtts { rftype val; const char *name; };
    
protected:
    rftype      val;
    
public:
    
    rif_type() : val(NOP) {};
    //rip_type( rtype t ) : val(t) {};
    rif_type( const rftype &t ) : val(t) {};
    rif_type( string );
    
    operator rftype () const { return val; }
    operator string () const;
    };

class path_elem
    {
    private:
        addr      addr_v;
        datetime  time_v;

    public:

        path_elem() : addr_v(conf.aka[0]), time_v(datetime::now())  {}
        path_elem( const addr &a, const datetime &t ) : addr_v(a), time_v(t)  {}
            
        path_elem( string s ) : time_v(0,datetime::UTC)
            {
            string s1, s2;
            s.parse( s1, s2 );
            time_v = datetime::from_time_t_string( s2, datetime::UTC );
            addr_v = s1;
            }

        operator string () const
            {
            return string(addr_v) + " " + time_v.gmtime_time_t_string();
            }
        operator const addr & () const       { return addr_v; }
        operator const datetime & () const   { return time_v; }

        const addr &      address() const    { return addr_v; }
        const datetime &  time() const       { return time_v; }


    };

class rif
    {
    datetime created_v;
    addr     from_v;
    addr     to_v;
    rif_type type_v;
    string   name_v;
    string   desc_v;
    string   msgid_v;
    flags    flags_v; // space-delimited flags, type-specific
    
    vector            <path_elem>     path_v;

    vector            <string>        comment_v;
    
    void         save_to_file( ostream &f );
public:
    rif() : created_v(0,datetime::UTC) {};
    
    void         from(const addr &a)          { from_v = a; }
    void         to(const addr &a)            { to_v = a; }
    void         created( datetime t)         { created_v = t; }
    void         type(const rif_type &t)      { type_v = t; }
    void         name(const string &n)        { name_v = n; }
    void         desc(const string &s)        { desc_v = s; }
    void         msgid(const string &s)       { msgid_v = s; }

    void         set_flags(const flags &s)    { flags_v = s; }
    void         set_rrq()                    { flags_v += "RRQ"; }
    
    const addr & from() const                 { return from_v; }
    const addr & to() const                   { return to_v; }
    datetime     created() const              { return created_v; }
    rif_type     type() const                 { return type_v; }
    const string &name() const                { return name_v; }
    const string &desc() const                { return desc_v; }
    const string &msgid() const               { return msgid_v; }

    const flags  &get_flags()const            { return flags_v; }
    const bool   rrq()                        { return flags_v.has("RRQ"); }
    
    const vector <path_elem> &path() const    { return path_v; }
    
    //void         put_path( ofstream & );
    //void         put_seenby( ofstream & );
    //void         put_unknown( ofstream &o );
    
    //void         add_seenby( const addr &a ) {seenby_v.push_back( a );}
    //void         add_path( const addr &a )   {path_v.insert( path_v.begin(), a );}
    void         add_seenby( const addr &a );
    void         add_path( const path_elem &a );
    
    void         load( const string & );
    void         create_in_dir( const string &dir, string &newname, const string &data, bool kill );

    };

void         unpack_rif( string const &s );
void         add_path_to_rif( string const &s );

#endif // RIF_H


