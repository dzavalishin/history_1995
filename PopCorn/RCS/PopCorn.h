head	1.5;
access;
symbols
	Ver_075:1.5;
locks;
comment	@ * @;


1.5
date	96.09.17.23.20.43;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.09.15.07.06.18;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.09.14.19.37.27;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.13.12.38.48;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.13.07.24.48;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@First completely working POP3/ESMTP version
@
text
@/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Dmitry Zavalishin
 *
 *	Module 	: Main header
 *
 *      $Log: PopCorn.h $
 *      Revision 1.4  1996/09/15 07:06:18  dz
 *      *** empty log message ***
 *
 *
 *
 *
 *
\*/

#ifndef POPCORN_H
#define POPCORN_H

#include "strng.h"
#include <style.h>

#include "version.h"
#include "passwd.h"
#include "except.h"

class minfo
    {
    private:
        string fn_v;
    
        int    size_v;
        //string id_v;
        //string subj_v;
        bool   killed_v;

        void   load();
    
    public:
        minfo() { killed_v = No; }
        minfo( const string & filename ) : fn_v(filename)
            {
            load();
            killed_v = No;
            }

        minfo( const minfo &v )
            {
            fn_v = v.fn_v;
            size_v = v.size_v;
            killed_v = v.killed_v;
            }
        
        int            size()   const { return size_v; }
        //const string & id()     const { return id_v; }
        //const string & subj()   const { return subj_v; }
        bool           killed() const { return killed_v; }

        void           kill()         { killed_v = Yes; }
        void           unroll()       { killed_v = No;  }

        const string & fn() const     { return fn_v;    }
        
        void           really_kill() { if(killed_v) unlink(fn_v); }
    };

class MailBox
    {
    int            lock_v;         // Is locked
    int            transaction_v;  // We have something to commit/unroll
    string         user_v;
    int            last_msg_no;

    vector <minfo> mi_v;
    
    void           update_last_msg_no( const string &fn );
    string         gen_new_msg_fname();
    void           parse_uupc_box();
        
    public:
        MailBox(const string &user);  //{ lock_v = 0; }
        ~MailBox(); //{ if(lock_v) unlock(); if(transaction_v) unroll(); }
        
        void      lock();
        void      unlock();
        int       is_locked() const { return lock_v; }

        void      kill( int msg );
        
        void      commit(); // Changes are confirmed. Do them.
        void      unroll(); // Changes must be thrown away

        int       messages() const;

        const minfo & info( int no ) { return mi_v[no]; }
    };

class MailBoxes : public passwd
    {
    public:
        MailBoxes();
        MailBox & operator[] ( const char *username );
    };





  // -----------------------------------------------------------------------
  // externs
  // -----------------------------------------------------------------------

extern MailBoxes mailboxes;
extern string domain;
extern bool debug;

  // -----------------------------------------------------------------------
  // global funcs
  // -----------------------------------------------------------------------


void run_rmail(
               const vector<string> &msg,
               const string &from,
               const vector<string> &to,
               const string &peer
              );


string uupc_mailbox_file_name( const string &user );
string uupc_passwd_file_name();

void   do_move( string from, string to );


#endif // POPCORN_H

@


1.4
log
@*** empty log message ***
@
text
@d7 4
a10 1
 *      $Log: MpChan.c $
d17 3
d25 41
d69 11
a79 2
    int           lock_v;         // Is locked
    int           transaction_v;  // We have something to commit/unroll
d81 1
a81 1
        MailBox();  //{ lock_v = 0; }
d92 4
d115 19
d136 1
@


1.3
log
@*** empty log message ***
@
text
@d1 12
a44 1
extern MailBoxes mailboxes;
a46 9
class TCP_Server
    {
    private:
        bool    connected_v;
        int     socket_v;
    
    public:
        TCP_Server( int sock );
        virtual ~TCP_Server();
a47 100
        void hangup(); // { connected_v = No; }
        bool connected() const { return connected_v; }
        
        void send( const string& s );
        void send_nl(); // { send("\r\n"); }
        void sendline( const string& s );
        /*
            {
            if( s[0] == '.' ) send(".");
            send(s);
            send_nl();
            }
        */
        string getline();
    };

class Pop3Server : public TCP_Server
    {
    protected:
        bool    valid_user_v;
        string  username_v;
        string  password_v;

        MailBox *mailbox;
        
        void    reply( bool success, const char* text );

        void    cmd_quit();
        void    cmd_user( const string& name );
        void    cmd_pass( const string& pass );
        void    cmd_stat();
        void    cmd_list( const string& msg );
        void    cmd_retr( const string& msg );
        void    cmd_dele( const string& msg );
        void    cmd_noop();
        void    cmd_rset();
        // optional
        void    cmd_top( const string& msg );
        void    cmd_uidl( const string& msg );
        void    cmd_apop( const string& auth );
        void    cmd_help();

        
    public:
        Pop3Server( int port ); // : TCP_Server( port );
            /*
            {
            valid_user_v = No;
            reply( Yes, "PopCorn " VERSION_STR "POP3 server ready" );
            mailbox = 0;
            }
            */
        virtual ~Pop3Server();

        void server_loop();
    
    };


class SMTPServer : public TCP_Server
    {
    protected:
        //bool    valid_user_v;
        //string  username_v;
        //string  password_v;
        
        //MailBox *mailbox;
        
        void    reply( bool success, const char* text );
        
        void    cmd_quit();
        //void    cmd_user( const string& name );
        //void    cmd_pass( const string& pass );
        //void    cmd_stat();
        //void    cmd_list( const string& msg );
        //void    cmd_retr( const string& msg );
        //void    cmd_dele( const string& msg );
        void    cmd_noop();
        //void    cmd_rset();
          // optional
        //void    cmd_top( const string& msg );
        //void    cmd_uidl( const string& msg );
        //void    cmd_apop( const string& auth );
        void    cmd_help();
        
        
    public:
        SMTPServer( int port ); // : TCP_Server( port );
          /*
           {
           valid_user_v = No;
           reply( Yes, "PopCorn " VERSION_STR "POP3 server ready" );
           mailbox = 0;
           }
           */
        virtual ~SMTPServer();
        
        void server_loop();
        
    };
d49 3
d53 2
@


1.2
log
@tcp works somehow
@
text
@d6 1
d26 1
a26 1
class MailBoxes
d29 1
a29 2
        int       valid_user ( const char *username );
        int       valid_pass ( const char *username, const char *password );
d86 1
d105 41
@


1.1
log
@Initial revision
@
text
@d12 2
a13 2
        MailBox() { lock_v = 0; }
        ~MailBox() { if(lock_v) unlock(); if(transaction_v) unroll(); }
d89 2
a90 1
        Pop3Server( int port ) : TCP_Server( port )
d96 1
@
