/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Dmitry Zavalishin
 *
 *	Module 	: HTTP proto
 *
 *      $Log: smtp.h $
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


class HTTPServer : public TCP_Server
    {
    protected:
          // session data
        //bool         open_v; // they gave correct HELO or EHLO
        //string       peer_v;

        //sockaddr_in  peer_addr_v;
        //int          peer_addr_len_v;
        //string       peer_domain_v;
        //string       peer_dotaddr_v;

        void cmd_get( const string & arg );

          // Sends a page
        void send_error( const string & title, const string & text ); 
        void send_root_page();
        void login( string arg );
        void start_session(string login, string password);

        // HTML syntax
        void h( int level, string header );
        void p();
        
        void html_tail();
        void html_head(
                       string title,
                       int bgcolor      = 0x808080,
                       int textcolor    = 0x000000,
                       int linkcolor    = 0x00FFFF,
                       int vlinkcolor   = 0x008080,
                       string backimage = ""
                      );
        
        
    public:
        HTTPServer( int port );
        virtual ~HTTPServer();
        
        void server_loop();
        
    };

