head	1.3;
access;
symbols
	Ver_075:1.3;
locks;
comment	@ * @;


1.3
date	96.09.17.23.20.43;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.15.10.05.15;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.15.07.06.18;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@First completely working POP3/ESMTP version
@
text
@/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Dmitry Zavalishin
 *
 *	Module 	: TCP/IP wrappers
 *
 *      $Log: tcp.h $
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


#ifndef TCP_H
#define TCP_H

#include <types.h>
#include <sys\socket.h>
#include <netdb.h>
#include <netinet\in.h>
#include <sys\un.h>
#include <nerrno.h>


typedef void  (* _Optlink threadf_t)( void * );


class TCP_Listener
    {
    private:
        int            lsocket;
        string         pname;
        //int            pport;
        threadf_t      server_thread;
        
    public:
        TCP_Listener(
                     const char* name,
                       //int port,
                     threadf_t thread, int qsize = 4 );
        virtual void listen();
        virtual ~TCP_Listener();
        
    };


  // -----------------------------------------------------------------------



class TCP_Server
    {
    private:
        bool    connected_v;
        int     socket_v;
        
    public:
        TCP_Server( int sock );
        virtual ~TCP_Server();
        
        void hangup(); 
        bool connected() const { return connected_v; }
        
        void send( const string& s );
        void send_nl(); 
        void sendline( const string& s );
        string getline();

        void peer_addr( sockaddr_in &a, int &len  );
    };


#endif // TCP_H
@


1.2
log
@smtp proto mostly done
@
text
@d8 3
d29 1
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: MpChan.c $
d21 6
d36 1
a36 1
        int            pport;
d40 4
a43 1
        TCP_Listener( const char* name, int port, threadf_t thread, int qsize = 4 );
d64 1
a64 1
        void hangup(); // { connected_v = No; }
d68 1
a68 1
        void send_nl(); // { send("\r\n"); }
a69 7
          /*
           {
           if( s[0] == '.' ) send(".");
           send(s);
           send_nl();
           }
           */
d71 2
@
