/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	RIP class
 *
 *      $Log: rip.h $
 *      Revision 1.12  1997/04/21 14:05:31  dz
 *      *** empty log message ***
 *
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
    void         put_comment( ofstream &o );
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


