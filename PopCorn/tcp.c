/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Dmitry Zavalishin
 *
 *	Module 	: TCP/IP wrappers impl.
 *
 *      $Log: tcp.c $
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
#include "tcp.h"

#define INCL_DOSPROCESS
#include <os2.h>

#include <types.h>
#include <netinet\in.h>
#include <sys\socket.h>

TCP_Server::TCP_Server( int sock ) : socket_v(sock)
    {
    connected_v = Yes;
    }

TCP_Server::~TCP_Server()
    {
    shutdown( socket_v, 2 );
    soclose( socket_v );
    }
        
void TCP_Server::hangup()
    {
    connected_v = No;
    shutdown( socket_v, 2 );
    DosSleep( 1000 ); // wait a second, let last message to pass through
    }
        
void TCP_Server::send( const string& s )
    {

    const char *p = s;
    int len = s.length();

    while( len )
        {
        if( !connected_v )
            throw Fail("TCP_Server::send", "disconnect", "");

        int sent = ::send( socket_v, (char *)p, len, 0 );
        if( sent <= 0 )
            throw Fail("TCP_Server::send", "send error", "");

        len -= sent;
        p += sent;
        }
    
    }

void TCP_Server::send_nl()
    {
    send("\r\n");
    }

void TCP_Server::sendline( const string& s )
    {
    if( s[0] == '.' ) send(".");
    send(s);
    send_nl();
    }
        
string TCP_Server::getline()
    {
    const maxb = 1000;
    char buf[maxb+1];
    int toread;

    while(1)
        {
    
        int peek = ::recv( socket_v, buf, maxb, MSG_PEEK );

        if( peek <= 0 )
            if( sock_errno() == SOCECONNRESET )
                throw XEOF("TCP_Server::getline", "connection reset", "");
            else
                throw Fail("TCP_Server::getline", "peek error", "");

        buf[peek] = 0;
        
        char *seek = strchr( buf, '\r' );

        if( peek >= (maxb-1) && seek == NULL )
            throw Fail("TCP_Server::getline", "line too long", "");

        if( seek == 0 )
            {
            DosSleep( 128 );
            continue;
            }
        
        if( seek[0] == '\r' && seek[1] == '\n' )
            toread = (seek+2)-buf;
        else toread = (seek+1)-buf;
        break;
        };

    int nread = ::recv( socket_v, buf, toread, 0 );
    if( nread <= 0 )
        {
        if( sock_errno() == SOCECONNRESET )
            throw XEOF("TCP_Server::getline", "connection reset", "");
        else
            throw Fail("TCP_Server::getline", "recv error", "");
        }
    buf[nread] = 0;

    string s = buf;

    s.strip_crlf();
    return s;
    }
                           

void TCP_Server::peer_addr( sockaddr_in &a, int &len )
    {
    len = sizeof(a);
    if( getpeername( socket_v, (sockaddr *)&a, &len ) )
        throw Fail( "TCP_Server::peer_addr", "getpeername failure", "" );
    }






  // -----------------------------------------------------------------------



TCP_Listener::TCP_Listener(
                       const char* name, // int port,
                       threadf_t thread, int qsize
                      )
    :
pname(name),
//pport(port),
server_thread(thread)
    {
    lsocket = socket(PF_INET, SOCK_STREAM, 0);
    
    if(lsocket < 0)
        {
        psock_errno("socket");
        throw Fail("Listener::Listener","Can't obtain socket",pname);
        }

    servent *serv = getservbyname( (char *)(pname.c_str()), "tcp" );
    if( !serv )
        {
        psock_errno("getservbyname");
        throw Fail("Listener::Listener","Unknown service",pname);
        }
    
    struct sockaddr_in myname;
    memset(&myname, 0, sizeof(myname));
    myname.sin_family = AF_INET;
    myname.sin_addr.s_addr = INADDR_ANY; /* all interfaces */
    myname.sin_port = serv->s_port;
    //myname.sin_port = htons(port);
    
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


TCP_Listener::~TCP_Listener()
    {
    soclose( lsocket );
    }


void TCP_Listener::listen()
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







