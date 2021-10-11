head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	95.11.04.18.32.53;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.11.04.01.21.46;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.11.02.14.25.20;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.08.14.02.51.21;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.08.08.13.22.34;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@Now returning message to sender in a case of
address conversion errors.
Some debugging done.
@
text
@/************************ FIDO2UU ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	RFC Message Class
 *
 *      $Log: RFC_Msg.h $
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


class RFC_envelope
    {
    string        from_v;
public:
//    env_from_Bag  from;
//    env_to_Bag    to;

    void          removeAll() { /*from.removeAll(); to.removeAll();*/ }
    
    void          add_from( const string &v ) { from_v = v; };
    string        from_string() { return from_v; };
    };


class RFC_text : public txt_Bag
    {
    };

class RFC_headlines : public hl_Bag
    {
public:
    hl *             have_key( const string &key );
    hl *             add_or_find_key( const string &key );
    void             add_hl( const string &key, const string &val );
    
    string operator  [] (const string &key);
    bool             get( string &val, const string key );
    };


class RFC_Msg
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
    bool     save( FILE *fp );
    bool     save( RFC_Msg &dest );
    
    // returns !!! estimated !!! size of message in bytes
    long     size()  
        { return size_v; }
    
    void     set_loader_recoder( recoder *r ) { lr = r; }
    void     set_saver_recoder( recoder *r ) { sr = r; }
    
//    bool     get_headline( string &val, const string key );
    //   string   headline( const string key ); // returns empty string, if no such headline found
    
    time_t   get_gm_time()
             {
             string date;
             if( Ok != headline.get( date, "date" ) ) return -1;
             return getindate(date);
             }
    
    string           from_realname() { if(froms_invalid_v) parse_froms(); return from_realname_v; }
    string           reply_address() { if(froms_invalid_v) parse_froms(); return reply_address_v; }
    string           errors_reply_address()  { if(froms_invalid_v) parse_froms(); return errors_reply_address_v; }
    
    
    };





#endif
@


1.4
log
@First RFC_Msg-based version, that compiles and does something
@
text
@d8 3
d100 1
@


1.3
log
@Before switching to rfc_msg
@
text
@d8 3
d34 1
d39 1
d41 2
a42 2
    env_from_Bag  from;
    env_to_Bag    to;
d44 4
a47 1
    void          removeAll() { from.removeAll(); to.removeAll(); }
d50 1
d61 3
d70 4
d76 10
a85 2


d91 30
a120 13


   RFC_Msg( void );
   ~RFC_Msg( void );

   void     clear() { envelope.removeAll(); headline.removeAll(); text.removeAll(); }
   bool     load( FILE *fp );
   bool     save( FILE *fp );

   void     set_loader_recoder( recoder *r ) { lr = r; }
   void     set_saver_recoder( recoder *r ) { sr = r; }

   };
@


1.2
log
@Intermediate
@
text
@d8 3
d21 3
d28 25
d56 5
a60 15
   {
/*
   string   dest_addr_v;
   string   from_path_v;

   hl_Bag   hl_v;

   char     *text_buf;
   int      text_buf_len;
   int      text_len;

   void     init_buf() { text_buf_len = text_len = 0; text_buf = 0; }
   int      free_in_buf() { return text_buf_len - text_len; }
   void     expand_buf();
*/
d63 5
d72 1
d74 4
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: UUEncode.c $
d15 3
d20 4
d26 1
a27 1
   string   from_addr_v;
d30 2
d36 1
a36 1
   int      init_buf() { text_buf_len = text_len = 0; text_buf = 0; }
d38 2
a39 1
   int      expand_buf();
d46 2
d53 2
@
