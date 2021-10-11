head	1.14;
access;
symbols
	Beta_12:1.3;
locks;
comment	@ *      @;


1.14
date	97.03.26.11.01.01;	author dz;	state Exp;
branches;
next	1.13;

1.13
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.07.31.07.26.25;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.07.31.07.00.23;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.07.22.02.48.05;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.01.22.19.23.58;	author dz;	state Exp;
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
date	96.01.15.03.50.54;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.12.05.04.23.40;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.12.13.12.27;	author dz;	state Exp;
branches;
next	;


desc
@@


1.14
log
@datetime fixes
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Links class
 *
 *      $Log: links.h $
 *      Revision 1.13  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.12  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.11  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.10  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.9  1996/07/31 07:00:23  dz
 *      FripMail written
 *
 *      Revision 1.8  1996/07/22 02:48:05  dz
 *      ver 17
 *
 *      Revision 1.7  1996/01/22 19:23:58  dz
 *      C Set, mostly works
 *
 *      Revision 1.6  1996/01/22 04:36:41  dz
 *      C Set & STL support
 *
 *      Revision 1.5  1996/01/18 00:58:45  dz
 *      Beta 15
 *
 *      Revision 1.3  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#ifndef LINKS_H
#define LINKS_H

#include "sglist.h"



class ad_filter
    {
    vector            <stopgo>     sg_v;

    wildness                       wild_v;
    datetime                       time_inc_v;
    
    friend class links;
    
    void add_sg( const stopgo &sg )
        { sg_v.push_back( sg ); }
    
    void set_wild( const string &s )
        { wild_v = wildness(s); }
    
    void set_time_inc( string s );
    
public:
    ad_filter() : time_inc_v( 0, datetime::UTC ), wild_v( wildness::None ) { }
        
    bool      pass( const rip &r ) const; // if this rip should pass this filter
    datetime  time_inc() const { return time_inc_v; }
    
    };


const  link_n_ping = 10;

class link
    {
        
    protected:
        addr                           myaddr_v;
        addr                           hisaddr_v;
    
        ad_filter                      in_v;
        ad_filter                      out_v;

          // Following items are saved in separate links database file,
          // not in user-editable one, and updated automatically by frip

          // Last received capas from that link.
          // Do we still ignore capas from ads?
        string                         capas;
        string                         flags;

        int                            ping_delay[link_n_ping];

        void                           clear( void );

          // time we got our last message from this link, 0 - unknown
        datetime                       last_heard_from;
    
    public:

          link( void );
          // link( const string & );  по хоpошему сюда надо пеpекинуть инициализацию
    
        const addr &   myaddr()  const { return myaddr_v;  };
        const addr &   hisaddr() const { return hisaddr_v; };
    
        bool    will_pass_out( const rip &r ) const  // if this outgoing rip should pass this link
            { return out_v.pass( r ); }
        bool    will_pass_in( const rip &r ) const   // if this incoming rip should pass this link
            { return in_v.pass( r ); }
        
        const ad_filter &       in()  const { return in_v; }
        const ad_filter &       out() const { return out_v; }
        
        int operator == ( const link &his ) const
            {
            return myaddr_v == his.myaddr_v && hisaddr_v == his.hisaddr_v;
            }
        
        friend class links;
    };


//class FripApp;


class links
    {
    private:
        vector            <link>       l;

        void  add_user( string  );
        void  add_base( string  );
        
        void  load_user(); // load user settings, must be done first
        void  load_base(); // load our info about links, just after load_user

        void  save_base();
        
        bool  loaded;
        
    public:
        links() { loaded = No; }
        ~links() { if( loaded ) save_base(); }
        
        void  load();

        const link operator [] (int i) const { return l[i]; }
        int  size() const { return l.size(); }

        //typedef void (FripApp::*link_foreach_t)(rip,const link&);
        //void  foreach( const rip &r, link_foreach_t );
        //void  foreach( const rip &r, void (*do_a_link)( rip r, const link &l ) );

        void  process_ping( const rip &r );
    
          //bool  check_in( const rip &r );
        bool  check_in( const rip &r, link &l ) const;
        bool  is_from_link( const rip &r ) const;
    };

//extern links llist;



void deliver( const addr &a, const string &fn_a );
//void do_a_link( rip r, const link &l );







#endif // LINKS_H
@


1.13
log
@Version 25
@
text
@d8 3
d54 1
a54 1
    time_t                         time_inc_v;
d67 1
a67 1
    ad_filter() { time_inc_v = 0; }
d69 2
a70 2
    bool    pass( const rip &r ) const; // if this rip should pass this filter
    time_t  time_inc() const { return time_inc_v; }
d100 1
a100 1
        time_t                         last_heard_from;
@


1.12
log
@Exceptions added, not compiled
@
text
@d8 3
d124 1
d146 8
a153 2
    void  load();
    bool  foreach( const rip &r, bool (*do_a_link)( rip r, const link &l ) );
d155 1
a155 1
    void  process_ping( const rip &r );
d157 3
a159 3
    //bool  check_in( const rip &r );
    bool  check_in( const rip &r, link &l ) const;
    bool  is_from_link( const rip &r ) const;
d162 1
a162 1
extern links llist;
d167 1
a167 1
void do_a_link( rip r, const link &l );
@


1.11
log
@Before exceptions.
@
text
@d8 3
d128 2
a129 2
        bool  add_user( string  );
        bool  add_base( string  );
d131 2
a132 2
        bool  load_user(); // load user settings, must be done first
        bool  load_base(); // load our info about links, just after load_user
d134 1
a134 1
        bool  save_base();
d142 1
a142 1
    bool  load();
d145 1
a145 1
    bool  process_ping( const rip &r );
d149 1
a149 1
    bool  not_from_link( const rip &r ) const;
d156 2
a157 2
bool deliver( const addr &a, const string &fn_a );
bool do_a_link( rip r, const link &l );
@


1.10
log
@Clean up of watcom classlib based code
@
text
@d8 3
d66 2
d70 4
a73 2
    addr                           myaddr_v;
    addr                           hisaddr_v;
d75 17
a91 2
    ad_filter                      in_v;
    ad_filter                      out_v;
d93 4
a96 7
    /*
    WCPtrOrderedVector<stopgo>     sg_out_v;
    WCPtrOrderedVector<stopgo>     sg_in_v;
    
    bool                           out_wildonly_v;
    bool                           in_wildonly_v;
    */
d98 2
a99 3
public:
    link( void ) {}
//    link( const string & );  по хоpошему сюда надо пеpекинуть инициализацию
d101 14
a114 17
    const addr &   myaddr()  const { return myaddr_v;  };
    const addr &   hisaddr() const { return hisaddr_v; };
    
    bool    will_pass_out( const rip &r ) const  // if this outgoing rip should pass this link
        { return out_v.pass( r ); }
    bool    will_pass_in( const rip &r ) const   // if this incoming rip should pass this link
        { return in_v.pass( r ); }
    
    const ad_filter &       in()  const { return in_v; }
    const ad_filter &       out() const { return out_v; }
    
    int operator == ( const link &his ) const
        {
        return myaddr_v == his.myaddr_v && hisaddr_v == his.hisaddr_v;
        }
    
    friend class links;
d122 8
a129 2
private:
    vector            <link>       l;
d131 7
a137 2
public:
    links() {}
a139 1
    bool  add( string  );
d141 2
d146 1
a146 1
    
@


1.9
log
@FripMail written
@
text
@d8 3
a38 1
#if USE_STL
d40 1
a40 3
#else
    WCPtrOrderedVector<stopgo>     sg_v;
#endif
a46 1
#if USE_STL
a47 3
#else
        { sg_v.insert( new stopgo( sg ) ); }
#endif
a107 1
#if USE_STL
a108 3
#else
    WCPtrOrderedVector<link>       l;
#endif
@


1.8
log
@ver 17
@
text
@d8 3
d135 2
@


1.7
log
@C Set, mostly works
@
text
@d8 3
d128 9
@


1.6
log
@C Set & STL support
@
text
@d8 3
d122 1
a122 1
    } llist;
d124 1
@


1.5
log
@Beta 15
@
text
@d8 3
d27 3
d31 1
d38 3
d42 1
d75 1
d103 3
d107 1
d110 2
@


1.4
log
@wildness filter
@
text
@d26 1
d36 2
d39 2
d42 1
d98 2
a99 1
    bool  check_in( const rip &r );
@


1.3
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d20 3
a22 2
/*
class stopgo
d24 11
d36 1
a36 4
    stopgo( string );
        
    enum sg { Stop, Go }   kind;
    addr                   mask;
a37 3
    
    int operator == ( stopgo const &a ) const
        { return mask==a.mask && kind == kind; };
a38 1
*/
d46 4
d52 5
a56 1

d63 7
a69 2
    bool    will_pass_out( const rip &r ) const;  // if this outgoing rip should pass this link
    bool    will_pass_in( const rip &r ) const;  // if this incoming rip should pass this link
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
@d4 3
d19 1
a19 1

d27 2
a28 1
    WCPtrOrderedVector<stopgo>     sg_v;
d36 2
a37 1
    bool    will_pass( const rip &r ) const;  // if this rip should pass this link
d59 3
a61 1
        
@
