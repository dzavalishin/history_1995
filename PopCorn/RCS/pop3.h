head	1.2;
access;
symbols
	Ver_075:1.2;
locks;
comment	@ * @;


1.2
date	96.09.17.23.20.43;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.15.07.06.18;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@First completely working POP3/ESMTP version
@
text
@/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Dmitry Zavalishin
 *
 *	Module 	: POP3 proto
 *
 *      $Log: pop3.h $
 *      Revision 1.1  1996/09/15 07:06:18  dz
 *      Initial revision
 *
 *
 *
 *
 *
\*/

#include "strng.h"
#include <style.h>

#include "version.h"
#include "tcp.h"
#include "recode.h"




class Pop3Server : public TCP_Server
    {
    protected:
        volatile bool  valid_user_v;
        string         username_v;
        string         password_v;
        
        MailBox *      mailbox;
        
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

        void    send_file( int mno, int nlines = -1 );

        void    cmd_xtnd( const string& arg );
        void    xtnd_xmit();

        
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

@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: MpChan.c $
d22 1
a22 1
  //#include "passwd.h"
d30 3
a32 3
        bool    valid_user_v;
        string  username_v;
        string  password_v;
d34 1
a34 1
        MailBox *mailbox;
d52 6
a57 1
        
@
