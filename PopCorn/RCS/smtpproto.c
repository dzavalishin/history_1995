head	1.4;
access;
symbols
	Ver_075:1.4;
locks;
comment	@ * @;


1.4
date	96.09.17.23.20.43;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.09.15.10.05.15;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.15.07.06.18;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.14.19.37.27;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
log
@First completely working POP3/ESMTP version
@
text
@/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Dmitry Zavalishin
 *
 *	Module 	: SMTP Proto impl.
 *
 *      $Log: smtpproto.c $
 *      Revision 1.3  1996/09/15 10:05:15  dz
 *      smtp proto mostly done
 *
 *      Revision 1.2  1996/09/15 07:06:18  dz
 *      *** empty log message ***
 *
 *
 *
 *
\*/

#include "PopCorn.h"
#include "smtp.h"
#include "except.h"
#include "recode.h"

SMTPServer::SMTPServer( int port ) : TCP_Server( port )
    {
    open_v = No;

    try { peer_addr( peer_addr_v, peer_addr_len_v ); }
    catch( Fail )
        {
        reply( RShutDown, Last, domain+" PopCorn " VERSION_STR ": unable to get peer address" );
        throw Fail("SMTPServer::SMTPServer","unable to get peer address","");
        }

    peer_dotaddr_v = string("[")+inet_ntoa( peer_addr_v.sin_addr )+string("]");
    
        {
        struct hostent * h = gethostbyaddr( (char *)&(peer_addr_v.sin_addr), sizeof(peer_addr_v.sin_addr), AF_INET );
        if( h != 0 )
            peer_domain_v = h->h_name;
        else
            peer_domain_v = peer_dotaddr_v;
        }

    printf("SMTP peer is %s, %s\n", peer_domain_v.c_str(), peer_dotaddr_v.c_str() );
    
    reply( ROK, NotLast, domain+" PopCorn " VERSION_STR ": SMTP server ready" );
    reply( ROK, Last, "ESMTP spoken here" );
    }

SMTPServer::~SMTPServer()
    {
    }


void SMTPServer::server_loop()
    {
    while(connected())
        {
        string cmdl;
        try { cmdl = getline(); }
        catch( XEOF )
            {
            printf("SMTP connection closed by peer\n");
            break;
            }
        string cmd, arg;
        
        cmdl.strip_ws();
        cmdl.parse( cmd, arg );
        cmd.lcase();
        
        printf("SMTP got '%s'\n", cmdl.c_str() );
        
        try
            {
            if( cmd == "quit" ) cmd_quit();
            else if( cmd == "helo" ) cmd_helo(arg);
            else if( cmd == "ehlo" ) cmd_ehlo(arg);
            else if( cmd == "turn" ) cmd_turn();
            else if( cmd == "mail" ) cmd_mail(arg);
            else if( cmd == "rcpt" ) cmd_rcpt(arg);
            else if( cmd == "data" ) cmd_data();
            else if( cmd == "noop" ) cmd_noop();
            else if( cmd == "rset" ) cmd_rset();
            
            //else if( cmd == "top"  ) cmd_top(arg);
            //else if( cmd == "uidl" ) cmd_uidl(arg);
            //else if( cmd == "apop" ) cmd_apop(arg);
            else if( cmd == "help" ) cmd_help();
            else
                {
                string s;
                s.print( "Command '%s' is unknown", cmd.c_str() );
                reply( RSyntax, Last, s );
                }
            }
        catch( ... )
            {
            reply( RLocalError, Last, "This command failed (exception)" );
            }
        
        }
    }

void SMTPServer::reply( status s, completeness c, const char* text )
    {
    char code[30];
    sprintf( code, "%d%c", s, c ? ' ' : '-' );
    send( code );
    send( text );
    send_nl();
    }

void SMTPServer::cmd_quit()
    {
    reply( RClose, Last, domain+" PopCorn SMTP server signs off, see you later." );
    hangup();
    }

void SMTPServer::cmd_noop()
    {
    reply( ROK, Last,  "Did nothing. Successfully." );
    }

void SMTPServer::cmd_help()
    {
    reply( RUserHelp, NotLast, "Here you go:" );
    reply( RUserHelp, NotLast, "quit        -  disconnect");
    reply( RUserHelp, NotLast, "ehlo domain -  open channel");
    reply( RUserHelp, NotLast, "helo domain -  open channel, old way");
    reply( RUserHelp, NotLast, "noop        -  do nothing");
    reply( RUserHelp, NotLast, "help        -  this text");
    reply( RUserHelp, Last, "End of help");
    }


bool SMTPServer::correct_peer_name( const string &n )
    {
    struct hostent * h = gethostbyname((char *)(n.c_str()));
    if( h == 0 )
        {
        printf("Can't gethostbyname( %s )\n", n.c_str() );
        return No;
        }

    if( h->h_addrtype != AF_INET )
        return No;

    //if( h->h_length != peer_addr_len_v )  return No;

    if( memcmp( h->h_addr, &(peer_addr_v.sin_addr), sizeof(peer_addr_v.sin_addr) ) != 0 )
        return No;
        
    return Yes;
    }


void SMTPServer::cmd_helo( const string& id )
    {
    if(open_v)
        {
        reply( RBadSequence, Last, "Again?" );
        return;
        }
    
    if( correct_peer_name( id ) )
        {
        reply( ROK, Last,  domain+" Hello "+id+", pleased too meet you." );
        peer_v = id;
        printf("SMTP: %s came in\n", peer_v.c_str() );
        }
    else
        {
        reply( ROK, Last,  domain+" Hello "+peer_domain_v+", why do you call yourself "+id+"?" );
        peer_v = peer_domain_v;
        printf("SMTP: %s came in, called himself %s\n", peer_domain_v.c_str(), id.c_str() );
        }
    open_v = Yes;
    }


void SMTPServer::cmd_ehlo( const string& id )
    {
    if(open_v)
        {
        reply( RBadSequence, Last, "Again?" );
        return;
        }
    if( correct_peer_name( id ) )
        {
        reply( ROK, NotLast,  domain+" Hello "+id+", pleased too meet you." );
        peer_v = id;
        printf("SMTP: %s came in\n", peer_v.c_str() );
        }
    else
        {
        reply( ROK, NotLast,  domain+" Hello "+peer_domain_v+", why do you call yourself "+id+"?" );
        peer_v = peer_domain_v;
        printf("SMTP: %s came in, called himself %s\n", peer_domain_v.c_str(), id.c_str() );
        }
    reply( ROK, NotLast,  "8BITMIME" );
    reply( ROK, Last,     "HELP" );
    open_v = Yes;
    }


/*
void SMTPServer::cmd_vrfy()
    {
      //reply( ROK, Last,  "Full Name <addr>" );
      //reply( RForward, Last,  "User not local, will forward to <addr>" );
      //reply( RMailboxUnavailable, Last,  "String does not match anything." );
    }
    */

void SMTPServer::cmd_turn()
    {
    reply( RNotImplemented, Last, "Oh, no." );
    }

void SMTPServer::cmd_rset()
    {
    cleanup();
    reply( ROK, Last, "OK" );
    }


// Cleans up settings for previous transaction
void SMTPServer::cleanup()
    {
    from_v.clear();
    for( int i = to_v.size(); i--; )
        to_v.pop_back();
    for( i = data_v.size(); i--; )
        data_v.pop_back();
    }


void SMTPServer::cmd_mail( const string& arg )
    {
    string _, from;
    arg.parse( _, ':', from );
    _.ucase();

    if( _ != string("FROM") )
        {
        reply( RArgSyntax, Last, "Syntax: MAIL FROM:<path>" );
        return;
        }

    cleanup();

    from_v = from.substr(1,from.length()-2);
    reply( ROK, Last, "OK" );
    }

void SMTPServer::cmd_rcpt( const string& arg )
    {
    string _, to;
    arg.parse( _, ':', to );
    _.ucase();
    
    if( _ != string("TO") )
        {
        reply( RArgSyntax, Last, "Syntax: RCPT TO:<path>" );
        return;
        }

    if( from_v.is_empty() )
        {
        reply( RBadSequence, Last, "MAIL FROM goes first!" );
        return;
        }
    
    to_v.push_back( to.substr(1,to.length()-2) );
    reply( ROK, Last, "OK" );
    }

void SMTPServer::cmd_data()
    {
    
    if( from_v.is_empty() )
        {
        reply( RBadSequence, Last, "MAIL FROM goes first!" );
        return;
        }
    if( to_v.size() == 0 )
        {
        reply( RBadSequence, Last, "No recipients given" );
        return;
        }
    
    reply( RStartSend, Last, "Send data, end with single '.'" );
    
    recoder r( code_to_koi8() );

    while(1)
        {
        if(!connected())
            return;
        
        string dl = getline();
        if( dl == "." )
            break;

        if( dl.length() > 1 && dl[0] == '.' )
            dl.strip_leading(1);

        r.rs( dl );
        data_v.push_back( dl );
        }

    try
        {
        run_rmail( data_v, from_v, to_v, peer_v );
        reply( ROK, Last, "OK" );
        }
    catch( ... )
        {
        reply( RFailed, Last, "rmail failed" );
        }

    }
@


1.3
log
@smtp proto mostly done
@
text
@d8 3
d22 1
d60 7
a66 1
        string cmdl = getline();
d73 2
d254 1
a254 1
    from_v = from;
d276 1
a276 1
    to_v.push_back( to );
d296 2
d309 2
a310 1
        
d314 10
a323 2
    reply( ROK, Last, "OK" );
    
@


1.2
log
@*** empty log message ***
@
text
@d7 4
a10 1
 *      $Log: MpChan.c $
d18 1
d23 20
d68 4
a71 4
            //else if( cmd == "stat" ) cmd_stat();
            //else if( cmd == "list" ) cmd_list(arg);
            //else if( cmd == "retr" ) cmd_retr(arg);
            //else if( cmd == "dele" ) cmd_dele(arg);
d73 1
a73 1
            //else if( cmd == "rset" ) cmd_rset();
d126 21
d149 19
a167 4
    // BUG! Should chech peer here
    reply( ROK, Last,  domain+" Hello "+id+", pleased too meet you." );
    peer_v = id;
    printf("SMTP: %s came in\n", peer_v.c_str() );
d173 17
a189 3
      // BUG! Should chech peer here
    reply( ROK, NotLast,  domain+" Hello "+id+", pleased too meet you." );
    reply( ROK, NotLast,  "HELP" );
d191 2
a192 2
    peer_v = id;
    printf("SMTP: %s came in\n", peer_v.c_str() );
d196 8
d205 97
@


1.1
log
@Initial revision
@
text
@d1 11
d14 1
d18 3
a20 3
    //valid_user_v = No;
    reply( Yes, "PopCorn " VERSION_STR ": SMTP server ready" );
    //mailbox = 0;
d42 2
a43 2
            //else if( cmd == "user" ) cmd_user(arg);
            //else if( cmd == "pass" ) cmd_pass(arg);
d59 1
a59 1
                reply( No, s );
d64 1
a64 1
            reply( No, "This command failed (exception)" );
d70 1
a70 1
void SMTPServer::reply( bool success, const char* text )
d72 3
a74 1
    send( success ? "+OK " : "-ERR " );
d81 1
a81 1
    reply( Yes, "PopCorn SMTP server signs off, see you later." );
d87 1
a87 1
    reply( Yes, "Did nothing. Successfully." );
d92 27
a118 18
    reply( Yes, "Here you go:" );
    sendline("quit        -  disconnect");
    //sendline("user name   -  set username");
    //sendline("pass pwd    -  set password");
    //sendline("stat        -  get mailbox stats");
    //sendline("list [msg]  -  list messages");
    //sendline("retr msg    -  get message");
    //sendline("dele msg    -  delete message");
    sendline("noop        -  do nothing");
    //sendline("rset        -  undo deletions");
    //sendline("top  msg n  -  *show top n lines of message");
    //sendline("uidl [msg]  -  *get unique ids");
    //sendline("apop id     -  *alternative user identification");
    sendline("help        -  this text");
    sendline("");
    //sendline("commands marked with * are not implemented");
    send(".");
    send_nl();
d120 1
@
