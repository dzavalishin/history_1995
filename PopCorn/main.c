/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Dmitry Zavalishin
 *
 *	Module 	: Main
 *
 *      $Log: main.c $
 *      Revision 1.6  1996/09/17 23:20:43  dz
 *      First completely working POP3/ESMTP version
 *
 *      Revision 1.5  1996/09/15 10:05:15  dz
 *      smtp proto mostly done
 *
 *      Revision 1.4  1996/09/15 07:06:18  dz
 *      *** empty log message ***
 *
 *
 *
 *
 *
\*/

#include "PopCorn.h"
#include "except.h"

#include "pop3.h"
#include "smtp.h"
#include "http.h"


#include <stdio.h>

#include <types.h>
#include <sys\socket.h>
#include <netdb.h>
#include <netinet\in.h>
#include <sys\un.h>

#define INCL_DOSPROCESS
#define INCL_DOSERRORS
#include <os2.h>


#include <signal.h>
//#include <malloc.h>
//#include <ctype.h>



bool volatile Stop_PopCorn = No;

static void sig_handler( int sig );

#ifdef __IBMCPP__
#  define SIG_FUNC_T (_SigFunc)
#else
#  define SIG_FUNC_T
#endif

static void sig_setter()
    {
    signal( SIGINT,   SIG_FUNC_T sig_handler );   //     an interactive attention (CTRL/C on keyboard) is signalled
    signal( SIGBREAK, SIG_FUNC_T sig_handler );   //     an interactive attention (CTRL/BREAK on keyboard) is signalled
#if 0
    signal( SIGTERM,  SIG_FUNC_T sig_handler );   //     a termination request is sent to the program
    signal( SIGUSR1,  SIG_IGN );                  //     OS/2 process flag A via DosFlagProcess
#endif
    signal( SIGUSR2,  SIG_IGN );                  //     OS/2 process flag B via DosFlagProcess
    signal( SIGUSR3,  SIG_IGN );                  //     OS/2 process flag C via DosFlagProcess
    }

static void sig_handler( int  )
    {
      //sleep(1);
    sig_setter();
    Stop_PopCorn = Yes;
    }








void _Optlink pop3_server_thread( void *arg )
    {
    int client = (int)arg; // socket
    
    try
        {
        Pop3Server server( client );
        server.server_loop();
        }
    catch( ... )
        { printf("Exception, POP3 client thrown away\n"); }
    
    }

void _Optlink smtp_server_thread( void *arg )
    {
    int client = (int)arg; // socket
    
    try
        {
        SMTPServer server( client );
        server.server_loop();
        }
    catch( ... )
        { printf("Exception, SMTP client thrown away\n"); }
    
    }


void _Optlink http_server_thread( void *arg )
    {
    int client = (int)arg; // socket
    
    try
        {
        HTTPServer server( client );
        server.server_loop();
        }
    catch( ... )
        { printf("Exception, HTTP client thrown away\n"); }
    
    }






void _Optlink pop3_listen_thread( void *arg )
    {
    TCP_Listener pop3l( "pop3", /* 110, */ pop3_server_thread );

    pop3l.listen();
    }

void _Optlink smtp_listen_thread( void * )
    {
    TCP_Listener smtpl( "smtp", /* 25, */ smtp_server_thread );
    
    smtpl.listen();
    }


void _Optlink http_listen_thread( void * )
    {
    TCP_Listener httpl( "www-http", /* 25, */ http_server_thread );
    
    httpl.listen();
    }


string domain;
bool debug = Yes;

int main( int ac, char **av)
    {

    while(++av,--ac)
        {
        string a = *av;
        if( a == "-d" ) debug = Yes;
        else if( a == "-d-" ) debug = No;
        else if( a == "-?" || a == "-h" )
            printf("-d\t- debug\n-d-\t- debug off\n");
        }

    sig_setter();
    
    if( sock_init() )
        {
        psock_errno("Socket initialization");
        exit(1);
        }

        {
        const bs=128;
        char buf[bs];
        if( gethostname(buf,bs-1) )
            {
            printf("Can't get host name\n");
            exit( 64 );
            }
        domain = buf;
        struct hostent * h = gethostbyname(buf);
        if( h != 0 )
            {
            domain = h->h_name;
            }
        }

    printf("PopCorn " VERSION_STR " on %s starting up\n", domain.c_str() );
    
    int pop3_listen_tid = _beginthread( pop3_listen_thread, 0, 16384, 0 );
    if( pop3_listen_tid < 0 )
        {
        printf("Can't start pop3 listener thread\n");
        exit( 4 );
        }
    printf("POP3 listener thread: %d\n", pop3_listen_tid );
    
    int smtp_listen_tid = _beginthread( smtp_listen_thread, 0, 16384, 0 );
    if( smtp_listen_tid < 0 )
        {
        printf("Can't start smtp listener thread\n");
        exit( 4 );
        }
    printf("SMTP listener thread: %d\n", smtp_listen_tid );

#if NEED_HTTP
    int http_listen_tid = _beginthread( http_listen_thread, 0, 16384, 0 );
    if( http_listen_tid < 0 )
        {
        printf("Can't start http listener thread\n");
        exit( 4 );
        }
    printf("HTTP (experimental) listener thread: %d\n", http_listen_tid );
#endif
    
    while( !Stop_PopCorn )
        {
        TID tid = 0;
        int rc = DosWaitThread( &tid, DCWW_WAIT );
        if(
           //rc == ERROR_INTERRUPT ||
           (rc == NO_ERROR && tid == pop3_listen_tid) ||
           (rc == NO_ERROR && tid == smtp_listen_tid)
          )
            break;

        if( rc == NO_ERROR )
            printf("Thread %d ended\n", tid );
        }
    
    };

















