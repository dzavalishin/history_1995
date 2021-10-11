head	1.6;
access;
symbols
	Ver_075:1.6;
locks;
comment	@ * @;


1.6
date	96.09.17.23.20.43;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.09.15.10.05.15;	author dz;	state Exp;
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


1.6
log
@First completely working POP3/ESMTP version
@
text
@/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Dmitry Zavalishin
 *
 *	Module 	: Main
 *
 *      $Log: main.c $
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

















@


1.5
log
@smtp proto mostly done
@
text
@d8 3
d127 1
d129 1
a129 1
int main()
d131 9
@


1.4
log
@*** empty log message ***
@
text
@d7 4
a10 1
 *      $Log: MpChan.c $
d110 1
a110 1
    TCP_Listener pop3l( "pop3", 110, pop3_server_thread );
d117 1
a117 1
    TCP_Listener smtpl( "smtp", 25, smtp_server_thread );
@


1.3
log
@*** empty log message ***
@
text
@d1 12
d17 4
a32 2
// extern 
typedef void  (* _Optlink threadf_t)( void * );
d34 15
d50 1
a50 1
class Listener
d52 16
a67 42
    private:
        int            lsocket;
        string         pname;
        int            pport;
        threadf_t      server_thread;

    public:
        Listener(
                 const char* name, int port,
                 threadf_t thread, int qsize = 4
                )
            :
        pname(name), pport(port), server_thread(thread)
            {
            int lsocket = socket(PF_INET, SOCK_STREAM, 0);
            
            if(lsocket < 0)
                {
                psock_errno("socket");
                throw Fail("Listener::Listener","Can't obtain socket",pname);
                }
            struct sockaddr_in myname;
            memset(&myname, 0, sizeof(myname));
            myname.sin_family = AF_INET;
            myname.sin_addr.s_addr = INADDR_ANY; /* all interfaces */
              //myname.sin_port = htons(serv->s_port);
            myname.sin_port = htons(port);
            
            int rc = bind(lsocket, (struct sockaddr *) &myname, sizeof(myname));
            if( rc )
                {
                psock_errno("bind");
                throw Fail("Listener::Listener","Can't bind socket",pname);
                }
            
            rc = ::listen( lsocket, qsize ); // Allow for up to 4 clients in queue
            if( rc )
                {
                psock_errno("listen");
                throw Fail("Listener::Listener","Can't switch socket to listen mode",pname);
                }
            }
a69 35
        void listen()
            {
            try
                {
                while( 1 )
                    {
                    printf("Waiting for %s connection\n", pname.c_str() );
                    fflush(stdout);
                    int client = accept(lsocket, (struct sockaddr *) 0, (int *) 0);
                    if( client < 0 )
                        psock_errno("accept");
                    else
                        {
                        printf("Got an %s connection\n", pname.c_str() );
                        int serv_tid = _beginthread( server_thread, 0, 16384, (void*)client );
                        
                        if( serv_tid < 0 )
                            {
                            printf("Can't run %s server thread\n", pname.c_str() );
                            shutdown(client,2);
                            soclose(client);
                            }
                        else
                            printf("Server thread (%s): %d\n", pname.c_str(), serv_tid );
                        }
                    
                    }
                }
            catch( ... )
                {
                printf("Exception, %s server is dead now :(\n", pname.c_str() );
                return;
                }
            
            }
a70 4
        virtual ~Listener()
            {
            soclose( lsocket );
            }
a71 1
    };
d107 1
a107 1
    Listener pop3l( "pop3", 110, pop3_server_thread );
d114 1
a114 1
    Listener smtpl( "smtp", 25, smtp_server_thread );
d120 2
d124 3
d132 18
d167 1
a167 1
    while( 1 )
d172 1
a172 1
           rc == ERROR_INTERRUPT ||
d177 3
a179 1
        printf("Thread %d ended\n", tid );
a194 133

#if 0


void _Optlink server_thread( void *arg )
    {
    int client = (int)arg; // socket
    int rc;
    
    linger lmode;
    lmode.l_onoff = 1;
    lmode.l_linger = 60; // 1 minute
      //rc = setsockopt( client, SOL_SOCKET, SO_LINGER, (char*)&lmode, sizeof(lmode) );
      //if( rc ) psock_errno("setsockopt(linger)");
    
    try
        {
        Pop3Server server( client );
        server.server_loop();
        }
    catch( ... )
        {
        printf("Exception, client thrown away\n");
        }
    shutdown( client, 2 );
    soclose( client );
    }


void _Optlink pop3_listen_thread( void * )
    {
    int so = socket(PF_INET, SOCK_STREAM, 0);
    
    if(so < 0)
        {
        psock_errno("socket");
        exit(2);
        }
    
    servent *serv = getservbyname("pop3", "tcp");
    if( serv == NULL )
        {
        psock_errno("getservbyname(\"pop3\")");
        exit(2);
        }
    
    struct sockaddr_in myname;
    memset(&myname, 0, sizeof(myname));
    myname.sin_family = AF_INET;
    myname.sin_addr.s_addr = INADDR_ANY; /* all interfaces */
      //myname.sin_port = htons(serv->s_port);
    myname.sin_port = htons(110);
    
    int rc = bind(so, (struct sockaddr *) &myname, sizeof(myname));
    if( rc )
        {
        psock_errno("bind");
        exit(2);
        }
    
    rc = listen( so, 4 ); // Allow for up to 4 clients in queue
    if( rc )
        {
        psock_errno("listen");
        exit(2);
        }
    
    
    try
        {
        while( 1 )
            {
            printf("Waiting for connection\n");
            fflush(stdout);
            int client = accept(so, (struct sockaddr *) 0, (int *) 0);
            if( client < 0 )
                psock_errno("accept");
            else
                {
                printf("Got a connection\n");
                int serv_tid = _beginthread( server_thread, 0, 16384, (void*)client );

                if( serv_tid < 0 )
                    {
                    printf("Can't run server thread\n");
                    shutdown(client,2);
                    soclose(client);
                    }
                else
                    printf("Server thread: %d\n", serv_tid );

                }
            
            }
        }
    catch( ... )
        {
        soclose( so );
        printf("Exception, server is dead now :(\n");
        }
    
    soclose( so );
    }

int main()
    {
    if( sock_init() )
        {
        psock_errno("Socket initialization");
        exit(1);
        }
    
    int pop3_listen_tid = _beginthread( pop3_listen_thread, 0, 16384, 0 );
    if( pop3_listen_tid < 0 )
        {
        printf("Can't start pop3 listener thread\n");
        exit( 4 );
        }
    printf("POP3 listener thread: %d\n", pop3_listen_tid );
    
    while( 1 )
        {
        TID tid = 0;
        int rc = DosWaitThread( &tid, DCWW_WAIT );
        if( rc == ERROR_INTERRUPT || (rc == NO_ERROR && tid == pop3_listen_tid) )
            break;
        printf("Thread %d ended\n", tid );
        }
    
    };


#endif
@


1.2
log
@tcp works somehow
@
text
@d3 1
d13 101
a113 1
void server_thread( void *arg )
d116 38
a158 1

d164 70
a234 1
    int pop3listener = socket(PF_INET, SOCK_STREAM, 0);
d236 5
a240 1
    if(pop3listener < 0)
d259 2
a260 2

    int rc = bind(pop3listener, (struct sockaddr *) &myname, sizeof(myname));
d266 2
a267 2

    rc = listen( pop3listener, 0 );
d273 2
a274 2


d279 1
a279 1
            printf("Waiting for connection...");
d281 1
a281 1
            int client = accept(pop3listener, (struct sockaddr *) 0, (int *) 0);
d286 4
a289 9
                printf(" Got a connection\n");
                linger lmode;
                lmode.l_onoff = 1;
                lmode.l_linger = 60; // 1 minute
                //rc = setsockopt( client, SOL_SOCKET, SO_LINGER, (char*)&lmode, sizeof(lmode) );
                if( rc )
                    psock_errno("setsockopt(linger)");
                
                try
d291 3
a293 2
                    Pop3Server server( client );
                    server.server_loop();
d295 3
a297 6
                catch( ... )
                    {
                    printf("Exception, client thrown away\n");
                    }
                shutdown( client, 2 );
                soclose( client );
d299 1
a299 1

d304 1
a304 1
        soclose( pop3listener );
d307 3
d311 24
a334 1
    soclose( pop3listener );
d339 1
a339 1

@


1.1
log
@Initial revision
@
text
@d12 5
d46 2
a47 1
    myname.sin_port = htons(serv->s_port);
d57 1
d68 2
d75 1
d79 1
a79 1
                rc = setsockopt( client, SOL_SOCKET, SO_LINGER, (char*)&lmode, sizeof(lmode) );
d92 1
@
