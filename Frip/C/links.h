/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Links class
 *
 *      $Log: links.h $
 *      Revision 1.14  1997/03/26 11:01:01  dz
 *      datetime fixes
 *
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
