/************************ FIDO2UU ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	RFC Message Class
 *
 *      $Log: RFC_Msg.h $
 *      Revision 1.5  1995/11/04 18:32:53  dz
 *      Now returning message to sender in a case of
 *      address conversion errors.
 *      Some debugging done.
 *
 *      Revision 1.4  1995/11/04 01:21:46  dz
 *      First RFC_Msg-based version, that compiles and does something
 *
 *      Revision 1.3  1995/11/02 14:25:20  dz
 *      Before switching to rfc_msg
 *
 *      Revision 1.2  1995/08/14 02:51:21  dz
 *      Intermediate
 *
 *      Revision 1.1  1995/08/08 13:22:34  dz
 *      Initial revision
 *
 *
 *
\*/

#ifndef RFC_MSG_H
#define RFC_MSG_H

#include <stdio.h>

#include <style.h>
#include <strng.h>

#include <hl.h>
#include <hl_Bag.h>
#include <RFC_Bags.h>

#include <recode.h>
#include <datec.h>


class _Export RFC_envelope
    {
    string        from_v;
public:
//    env_from_Bag  from;
//    env_to_Bag    to;

    void          removeAll() { /*from.removeAll(); to.removeAll();*/ }
    
    void          add_from( const string &v ) { from_v = v; };
    string        from_string() const { return from_v; };
    };


class _Export RFC_text : public txt_Bag
    {
    };

class _Export RFC_headlines : public hl_Bag
    {
    hl *             have_key( const string &key ) const;
public:
    hl *             add_or_find_key( const string &key );
    void             add_hl( const string &key, const string &val );
    
    string operator  [] (const string &key) const;
    bool             get( string &val, const string key ) const;

    bool             contains( const string &key ) const;
    };


class _Export RFC_Msg
    {
    void             add_headline( const string &hl );
    
    // BUG?
    // Better pass recoder as parameter to load/save functions?

    recoder          *lr;
    recoder          *sr;
    
    bool             froms_invalid_v;
    string           from_realname_v;
    string           reply_address_v;
    string           errors_reply_address_v;
    void             parse_froms();
    void             decode_from( string &realname_out, string &address_out, const string &in );
    
    long             size_v;
    
public:
    RFC_envelope     envelope;
    RFC_headlines    headline;
    RFC_text         text;

    RFC_Msg( void );
    ~RFC_Msg( void );
 
    void     clear() { envelope.removeAll(); headline.removeAll(); text.removeAll(); }
    bool     load( FILE *fp );
    bool     save( FILE *fp ) const;
    bool     save( RFC_Msg &dest ) const;
    
    // returns !!! estimated !!! size of message in bytes
    long     size() const
        { return size_v; }
    
    void     set_loader_recoder( recoder *r ) { lr = r; }
    void     set_saver_recoder( recoder *r ) { sr = r; }
    
//    bool     get_headline( string &val, const string key );
    //   string   headline( const string key ); // returns empty string, if no such headline found
    
    time_t   get_gm_time() const;
/*             {
             string date;
             if( Ok != headline.get( date, "date" ) ) return -1;
             return getindate(date);
             }
  */  
    string           from_realname() { if(froms_invalid_v) parse_froms(); return from_realname_v; }
    string           reply_address() { if(froms_invalid_v) parse_froms(); return reply_address_v; }
    string           errors_reply_address()  { if(froms_invalid_v) parse_froms(); return errors_reply_address_v; }
    
    
    };





#endif
