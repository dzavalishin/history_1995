/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Dmitry Zavalishin
 *
 *	Module 	: Main header
 *
 *      $Log: popcorn.h $
 *      Revision 1.5  1996/09/17 23:20:43  dz
 *      First completely working POP3/ESMTP version
 *
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

