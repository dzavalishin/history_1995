/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Dmitry Zavalishin
 *
 *	Module 	: SMTP proto
 *
 *      $Log: smtp.h $
 *      Revision 1.3  1996/09/17 23:20:43  dz
 *      First completely working POP3/ESMTP version
 *
 *      Revision 1.2  1996/09/15 10:05:15  dz
 *      smtp proto mostly done
 *
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
  //#include "passwd.h"


class SMTPServer : public TCP_Server
    {
    protected:
          // session data
        bool         open_v; // they gave correct HELO or EHLO
        string       peer_v;

        sockaddr_in  peer_addr_v;
        int          peer_addr_len_v;
        string       peer_domain_v;
        string       peer_dotaddr_v;

          // Mail transaction data
        string          from_v;
        vector <string> to_v;
        vector <string> data_v;
        
        
        enum completeness { NotLast = 0, Last = 1 };
        enum status
            {
            ROK = 250, RForward = 251,
            
            RHelp = 211, RStatus = 211, RUserHelp = 214,
            
            RReady = 220, RClose = 221,
            
            RStartSend = 354,
            
            RShutDown = 421,
            
            RMailboxBusy = 450, RLocalError = 451, RNoStorage = 452,
            
            RSyntax = 500, RArgSyntax = 501, RNotImplemented = 502,
            RBadSequence = 503, RArgNotImplemented = 504,
            
            RMailboxUnavailable = 550, RUserNotLocal = 551,
            RNoStorage2 = 552, RBadMailboxName = 553, RFailed = 554
            };

          /*
           211 System status, or system help reply
           214 Help message
           [Information on how to use the receiver or the meaning of a
           particular non-standard command; this reply is useful only
           to the human user]
           220 <domain> Service ready
           221 <domain> Service closing transmission channel
           250 Requested mail action okay, completed
           251 User not local; will forward to <forward-path>
           
           354 Start mail input; end with <CRLF>.<CRLF>
           
           421 <domain> Service not available,
           closing transmission channel
           [This may be a reply to any command if the service knows it
           must shut down]
           450 Requested mail action not taken: mailbox unavailable
           [E.g., mailbox busy]
           451 Requested action aborted: local error in processing
           452 Requested action not taken: insufficient system storage
           
           500 Syntax error, command unrecognized
           [This may include errors such as command line too long]
           501 Syntax error in parameters or arguments
           502 Command not implemented
           503 Bad sequence of commands
           504 Command parameter not implemented
           550 Requested action not taken: mailbox unavailable
           [E.g., mailbox not found, no access]
           551 User not local; please try <forward-path>
           552 Requested mail action aborted: exceeded storage allocation
           553 Requested action not taken: mailbox name not allowed
           [E.g., mailbox syntax incorrect]
           554 Transaction failed
           
           */
        
        void    reply( status s, completeness c, const char* text );

        bool    correct_peer_name( const string &n );
        void    cleanup(); // Prepare to a new mail transaction
        
        void    cmd_quit();
        void    cmd_helo( const string& id );
        void    cmd_ehlo( const string& id );
        void    cmd_noop();
        void    cmd_help();

        void    cmd_turn();
        
        void    cmd_mail( const string&  );
        void    cmd_rcpt( const string&  );
        void    cmd_data();
        void    cmd_rset();
          //void    cmd_retr( const string& msg );
          // optional
          //void    cmd_top( const string& msg );
          //void    cmd_uidl( const string& msg );
          //void    cmd_apop( const string& auth );
        
        
    public:
        SMTPServer( int port );
        virtual ~SMTPServer();
        
        void server_loop();
        
    };

