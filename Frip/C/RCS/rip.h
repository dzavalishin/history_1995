head	1.12;
access;
symbols
	Beta_12:1.3;
locks;
comment	@ *      @;


1.12
date	97.04.21.14.05.31;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	97.03.22.16.06.47;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.07.31.07.00.23;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.01.22.04.36.41;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.01.18.00.58.45;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.01.13.09.48.05;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.12.05.04.25.03;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.12.13.12.27;	author dz;	state Exp;
branches;
next	;


desc
@@


1.12
log
@*** empty log message ***
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	RIP class
 *
 *      $Log: rip.h $
 *      Revision 1.11  1997/03/22 16:06:47  dz
 *      *** empty log message ***
 *
 *      Revision 1.10  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.9  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.8  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.7  1996/07/31 07:00:23  dz
 *      FripMail written
 *
 *      Revision 1.6  1996/01/22 04:36:41  dz
 *      C Set & STL support
 *
 *      Revision 1.5  1996/01/18 00:58:45  dz
 *      Beta 15
 *
 *      Revision 1.4  1996/01/13 09:48:05  dz
 *      *** empty log message ***
 *
 *      Revision 1.3  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#ifndef RIP_H
#define RIP_H

#include "addr.h"
#include "flag.h"
#include "datetime.h"
#include <time.h>
#include <fstream.h>

#include <vector.h>
#include <map.h>

//
//        Типы рипов
//
// HI   - посылается pаз в неделю на холдовый линк чтобы
//        пpовеpить - не поpа ли pасхолдить. Пpи получении такого
//        pипа холд снимается, и в ответ посылается HIR.
// 
// HACK - ответ на HI. Обpабатывается так же, как HI , но HIR в ответ
//        уже не шлется.
// 
// RRQ  - Rescan Request - высылается всем новым линкам чтобы получить от
//        них накопленую инфоpмацию. Вызывает отсылку pескана и RAK.
// 
// RACK - Rescan Ack. - подтвеpждене того, что pескан сделан. Обpабатывается
//        так же, как и HIR.
// 
// OFF  - сообщене о потеpе pоутинга. Стpуктуpа pипа - такая же, как и у
//        обычного анонсного pипа, но вpеменнЫе поля необязательны и
//        вместо поля AD идет поле OFF (чтобы старые версии шугались).
//        Обpаботка: если pоутинг на данное напpавление у нас был выставлен на
//        узел, с котоpого мы получили OFF, pоутинг снимается, OFF
//        pассылается дальше, возможно - сообщаем сисопу. Если мы получили
//        OFF от линка, на котоpый у нас не стоит pоутинг для напpавления,
//        указаного в OFF, то надо послать ему анонс (pескан анонса) для
//        соответствующего напpавления, чтобы он пpозpел :).
// 
// PNG  - Ping. Служит для измеpения качества линка. Пеpедается pегуляpно.
//        to be defined in detail later
// 

class rip_type
    {
public:
    enum rtype { NOP, AD, HI, HACK, RRQ, RACK, OFF, PING, SEARCH };
    struct rtts { rtype val; const char *name; };
    
protected:
    rtype      val;
    
public:
    
    rip_type() : val(NOP) {};
    //rip_type( rtype t ) : val(t) {};
    rip_type( const rtype &t ) : val(t) {};
    rip_type( string );
    
    operator rtype () const { return val; }
    operator string () const;
    };

class rip
    {
    datetime created_v;
    int      hops_v;             // Нужен только для AD и OFF
    addr     announce_v;         // Только для типа AD
    addr     off_v;              // Только для типа OFF

      // Только для типа SEARCH - что ищем.
      // Формат: <search-type>:<mode> {-restriction,...} <object-name>
      // search-type: area|file
      // mode: get|notify
      // restriction: group:name, name ищется в именах файл-эх
    string   search_v;           
    addr     from_v;
    addr     to_v;
    addr     creator_v;
    rip_type type_v;

      // capabilities of last hop. kludge used to fight old versions.
      // mostly needed at the RIP reception moment to treat it
      // correctly and add/remove flags according to capas of prev.
      // frip in chain. Was introduced together with RIFs to
      // distinguish between RIF-capable and RIF-less FRIPs.
      // RIPs, passed RIF-less FRIPs will get -RIF flags on them.
    string   capas_v;            
    
    flags    fplus_v, fminus_v, finfo_v;     // RIP flags
    
      // main akas of nodes rip came through
    vector      <addr>                        path_v;
    map         <addr, string, less<addr> >   path_info;
    
      // like path, but unsorted & contains all AKAs
    vector            <addr>     seenby_v;

      // contatins comments
    vector            <string>   comment_v;
    
      // contatins unknown values
    vector            <string>   unknown_v;  
    
public:
    rip() : created_v(0,datetime::UTC) {};
    
    void         created_s( const string & );
    void         off(const addr &a)           { off_v = a; }
    void         ad(const addr &a)            { announce_v = a; }
    void         announce(const addr &a)      { announce_v = a; }
    void         from(const addr &a)          { from_v = a; }
    void         to(const addr &a)            { to_v = a; }
    void         hops(int i)                  { hops_v = i; }
    void         created(const datetime &t)   { created_v = t; }
    //void         created( time_t t );
    void         creator(const addr &a)       { creator_v = a; }
    void         type(const rip_type &t)      { type_v = t; }
    void         fplus( const flags &f )      { fplus_v = f; }
    void         fminus( const flags &f )     { fminus_v = f; }
    void         finfo( const flags &f )      { finfo_v = f; }
    void         search( const string & s )   { search_v = s; }
    void         add_comment(const string& s) { comment_v.push_back(s); }
        
    string       created_s() const;
    const addr & off() const                  { return off_v; }
    const addr & ad() const                   { return announce_v; }
    const addr & announce() const             { return announce_v; }
    const addr & from() const                 { return from_v; }
    const addr & to() const                   { return to_v; }
    int          hops() const                 { return hops_v; }
    datetime     created() const              { return created_v; }
    const addr & creator() const              { return creator_v; }
    //operator     const tm & () const          { return created_v; }
    rip_type     type() const                 { return type_v; }
    const string &capas() const               { return capas_v; }
    
    const flags& fplus()  const               { return fplus_v; }
    const flags& fminus() const               { return fminus_v; }
    const flags& finfo()  const               { return finfo_v; }
    
    flags      & fplus()                      { return fplus_v; }
    flags      & fminus()                     { return fminus_v; }
    flags      & finfo()                      { return finfo_v; }

    const string&search()                     { return search_v; }
    
    void         put_path( ofstream & );
    void         put_seenby( ofstream & );
    void         put_unknown( ofstream &o );
    
    void         add_seenby( const addr &a ) {seenby_v.push_back( a );}
    void         add_path( const addr &a )   {path_v.insert( path_v.begin(), a );}
    
    void         increment_hops() { hops_v++; };
    void         increment_hops( int add ) { hops_v += add; }
    void         increment_time( datetime t );
    
    void         load( const string & );
    void         save_to_dir( const string &dir );
    void         save_to_file( ofstream &f );
    

    bool         seenby_matches( const addr &a ) const;
    };


#endif


@


1.11
log
@*** empty log message ***
@
text
@d8 3
d135 4
d160 2
a161 1
    
@


1.10
log
@Version 25
@
text
@d8 3
d41 1
d100 1
a100 1
    tm       created_v;
d136 1
a136 1
    rip() {};
d145 2
a146 2
    void         created(const tm &t)         { created_v = t; }
    void         created( time_t t );
d161 1
a161 1
    const tm   & created() const              { return created_v; }
d163 1
a163 1
    operator     const tm & () const          { return created_v; }
d186 1
a186 1
    void         increment_time( time_t t );
@


1.9
log
@Exceptions added, not compiled
@
text
@d8 3
d42 1
d77 1
a77 1
    enum rtype { NOP, AD, HI, HACK, RRQ, RACK, OFF, PING };
d100 7
d123 3
a125 1
    vector            <addr>     path_v;
d148 1
d170 2
@


1.8
log
@Before exceptions.
@
text
@d8 3
d121 1
a121 1
    bool         created_s( const string & );
d168 3
a170 3
    bool         load( const string & );
    bool         save_to_dir( const string &dir );
    bool         save_to_file( ofstream &f );
@


1.7
log
@FripMail written
@
text
@d8 3
d144 1
@


1.6
log
@C Set & STL support
@
text
@d8 3
a31 1
#if USE_STL
a32 3
#else
#include <wcvector.h>
#endif
d94 8
d105 1
a105 1
#if USE_STL
d107 1
d109 2
a110 8
    
    vector            <string>   unknown_v;  // contatins unknown values
#else
    WCPtrOrderedVector<addr>     path_v;
    WCPtrOrderedVector<addr>     seenby_v;
    
    WCPtrOrderedVector<string>   unknown_v;  // contatins unknown values
#endif
a153 1
#if USE_STL
a155 4
#else
    void         add_seenby( const addr &a ) {seenby_v.insert( new addr( a ) );}
    void         add_path( const addr &a )   {path_v.insertAt( 0, new addr( a ) );}
#endif
@


1.5
log
@Beta 15
@
text
@d8 3
d28 4
d33 1
d77 2
a78 1
    rip_type( rtype t ) : val(t) {};
d98 6
d108 1
d152 4
d158 1
@


1.4
log
@*** empty log message ***
@
text
@d8 3
d141 1
@


1.3
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d19 1
d24 49
d76 3
a78 2
    int      hops_v;
    addr     announce_v;
d82 3
d95 1
d102 1
d104 4
d110 1
d119 9
d134 1
d141 2
@


1.2
log
@before COW strings
@
text
@d1 11
@


1.1
log
@Initial revision
@
text
@d52 1
@
