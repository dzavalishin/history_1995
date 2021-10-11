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
 *	Module 	: POP3 Proto impl.
 *
 *      $Log: pop3proto.c $
 *      Revision 1.4  1996/09/15 07:06:18  dz
 *      *** empty log message ***
 *
 *
 *
 *
\*/

#include "PopCorn.h"
#include "pop3.h"
#include "except.h"

Pop3Server::Pop3Server( int port ) : TCP_Server( port )
    {
    valid_user_v = No;
    reply( Yes, "PopCorn " VERSION_STR ": POP3 server ready" );
    mailbox = 0;
    }

Pop3Server::~Pop3Server()
    {
    if(mailbox) delete mailbox;
    }


void Pop3Server::server_loop()
    {
    while(connected())
        {
        string cmdl;
        try { cmdl = getline(); }
        catch( XEOF )
            {
            printf("POP3 connection closed by peer\n");
            break;
            }
        string cmd, arg;

        cmdl.strip_ws();
        cmdl.parse( cmd, arg );
        cmd.lcase();

        printf("POP3 got '%s'\n", cmdl.c_str() );

        if(
           (!valid_user_v) &&
           (cmd != string("quit")) &&
           (cmd != string("help")) &&
           (cmd != string("noop")) &&
           (cmd != string("user")) &&
           (cmd != string("pass")) &&
           (cmd != string("apop"))
           )
            {
            reply( No, "Log in first!" );
            continue;
            }

        try
            {
            if( cmd == "quit" ) cmd_quit();
            else if( cmd == "user" ) cmd_user(arg);
            else if( cmd == "pass" ) cmd_pass(arg);
            else if( cmd == "stat" ) cmd_stat();
            else if( cmd == "list" ) cmd_list(arg);
            else if( cmd == "retr" ) cmd_retr(arg);
            else if( cmd == "dele" ) cmd_dele(arg);
            else if( cmd == "noop" ) cmd_noop();
            else if( cmd == "rset" ) cmd_rset();
        
            else if( cmd == "top"  ) cmd_top(arg);
            else if( cmd == "uidl" ) cmd_uidl(arg);
            else if( cmd == "apop" ) cmd_apop(arg);
            else if( cmd == "help" ) cmd_help();
            else if( cmd == "xtnd" ) cmd_xtnd(arg);
            else
                {
                string s;
                s.print( "Command '%s' is unknown", cmd.c_str() );
                reply( No, s );
                }
            }
        catch( ... )
            {
            reply( No, "This command failed (exception)" );
            }

        }
    }


void Pop3Server::reply( bool success, const char* text )
    {
    send( success ? "+OK " : "-ERR " );
    send( text );
    send_nl();
    }






void Pop3Server::cmd_xtnd( const string& arg )
    {
    string xcmd, xarg;
    arg.parse( xcmd, xarg );
    xcmd.lcase();

    if( xcmd == "xmit" ) xtnd_xmit();
    else
        {
        string s;
        s.print( "Extended command '%s' is unknown", xcmd.c_str() );
        reply( No, s );
        }
    }

void Pop3Server::xtnd_xmit()
    {
    reply( No, "Not implemented yet, sorry." );
    }












void Pop3Server::cmd_quit()
    {
    if(!valid_user_v)
        reply( Yes, "Unidentified user said byebye. Tried to pick a lock?" );
    else
        {
        if( mailbox != 0 )
            mailbox->commit();
              
        reply( Yes, "PopCorn signs off, see you later." );
        }
    hangup();
    }


void Pop3Server::cmd_user( const string& name )
    {
    if(valid_user_v)
        {
        reply( No, "Oh no, don't log in twice!" );
        return;
        }

    if(!mailboxes.valid_user(name))
        {
        reply( No, "Sorry, dunno who you are :(" );
        return;
        }
    
    username_v = name;
    reply( Yes, "- \"What's da passwoid?\" - \"Ken sent me!\"" );
    }

void Pop3Server::cmd_pass( const string& pass )
    {
    if(valid_user_v)
        {
        reply( No, "Oh no, don't log in twice!" );
        return;
        }
    
    if(!mailboxes.valid_pass(username_v, pass))
        {
        reply( No, "Oh. You stolen this password from a wrong user." );
        return;
        }

    password_v = pass;


    mailbox = &(mailboxes[username_v]);

    try { mailbox->lock(); }
    catch( ... )
        {
        reply( No, "Can't lock mailbox." );
        return;
        }

    valid_user_v = Yes;

    reply( Yes, "Welcome!" );
    }

void Pop3Server::cmd_noop()
    {
    reply( Yes, "Did nothing. Successfully." );
    }

void Pop3Server::cmd_help()
    {
    reply( Yes, "I don't have to help you, but..." );
    sendline("quit        -  stop/commit changes");
    sendline("user name   -  set username");
    sendline("pass pwd    -  set password");
    sendline("stat        -  get mailbox stats");
    sendline("list [msg]  -  list messages");
    sendline("retr msg    -  get message");
    sendline("dele msg    -  delete message");
    sendline("noop        -  do nothing");
    sendline("rset        -  undo deletions");
    sendline("top  msg n  -  show top n lines of message");
    sendline("uidl [msg]  -  *get unique ids");
    sendline("apop id     -  *alternative user identification");
    sendline("xtnd xmit   -  *upload a message");
    sendline("help        -  this text");
    sendline("");
    sendline("commands marked with * are not implemented");
    send("."); send_nl();
    }


void Pop3Server::cmd_stat()
    {
    int size = 0;
    for( int i = mailbox->messages(); i--; )
        size += (mailbox->info(i)).size();

    string s;
    s.print( "%d %d", mailbox->messages(), size );
    
    reply( Yes, s );
    }

void Pop3Server::cmd_list( const string& msg )
    {

    if( !msg.is_empty() )
        {
        string s;
        int i = atoi(msg) - 1;
        if( i < 0 || i >= mailbox->messages() )
            {
            reply( No, "No such message" );
            return;
            }
        if((mailbox->info(i)).killed())
            {
            reply(No,"Message is deleted");
            return;
            }
        s.print( "%d %d", i+1, (mailbox->info(i)).size() );
        reply(Yes,s);
        return;
        }
    
    int nmess = mailbox->messages();
    
    int size = 0;
    for( int i = nmess; i--; )
        if(!((mailbox->info(i)).killed()))
            size += (mailbox->info(i)).size();
    
    string s;
    s.print( "%d messages (%d octets)", mailbox->messages(), size );
    
    reply( Yes, s );

    for( i = 0; i < nmess; i++ )
        if(!((mailbox->info(i)).killed()))
            {
            s.print( "%d %d", i+1, (mailbox->info(i)).size() );
            sendline(s);
            }
    send("."); send_nl();
    }

void Pop3Server::cmd_dele( const string& msg )
    {
    int mn = atoi(msg) - 1;
    if( mn < 0 || mn >= mailbox->messages() )
        {
        reply( No, "No such message" );
        return;
        }
    mailbox->kill(mn);
    string s;
    s.print("Killed message number %d", mn+1 );
    reply( Yes, s );
    }

void Pop3Server::cmd_rset()
    {
    mailbox->unroll();
    reply( Yes, "Changed your mind? Ok, ok." );
    }



void Pop3Server::cmd_top( const string& tail )
    {
    string mno_s, lines_s;
    tail.parse( mno_s, lines_s );
    mno_s.strip_ws();

    if( mno_s.length() == 0 )
        reply( No, "No message number given" );
    else
        send_file( atoi(mno_s)-1, lines_s.length() == 0 ? 10 : atoi(lines_s) );
    }

void Pop3Server::cmd_retr( const string& tail )
    {
    string mno_s, _;
    tail.parse( mno_s, _ );
    mno_s.strip_ws();
    
    if( mno_s.length() == 0 )
        reply( No, "No message number given" );
    else
        send_file( atoi(mno_s)-1 );
    }



void Pop3Server::cmd_uidl( const string&  )
    {
    reply( No, "Did nothing. Unsuccessfully." );
    }

void Pop3Server::cmd_apop( const string&  )
    {
    if( valid_user_v )
        {
        reply( No, "You're already logged in!" );
        return;
        }
    reply( No, "Did nothing. Unsuccessfully." );
    }



void Pop3Server::send_file( int mno, int nlines )
    {
    if( mno < 0 || mno >= mailbox->messages() )
        {
        reply( No, "No such message" );
        return;
        }
    
    string fn_v = (mailbox->info(mno)).fn();
    
    FILE *fp = fopen(fn_v, "r" );
    //if( fp == NULL ) throw Fail( "Pop3Server::send_file", "can't open file", fn_v );
    if( fp == NULL )
        {
        reply( No, "Can't open message" );
        return;
        }

    reply( Yes, "Here it goes:" );
    
    bool in_header = Yes;
    int counter = 0;

    recoder r( code_from_koi8() );
    
    string line;
    while(1)
        {
        try { line.load(fp); } catch( XEOF ) { break; }
        
        line.strip_crlf();

        if( !in_header ) counter++;

        if( nlines >= 0 && counter > nlines )
            break;
        
        if( in_header && line.length() == 0 )
            in_header = No; // end of header
        
        r.rs( line );
        sendline( line );
        }

    send("."); send_nl();
    
    fclose(fp);
    }

@


1.4
log
@*** empty log message ***
@
text
@d7 4
a10 1
 *      $Log: MpChan.c $
d18 1
d29 1
d37 7
a43 1
        string cmdl = getline();
d50 16
d82 1
d98 1
d106 36
d191 1
a191 1
#if 0
d202 1
a202 2
#endif
    
d223 1
a223 1
    sendline("top  msg n  -  *show top n lines of message");
d226 1
d230 1
a230 2
    send(".");
    send_nl();
d236 8
a243 1
    reply( No, "Not implemented yet." );
a247 2
    reply( No, "Not implemented yet." );
    }
d249 38
a286 3
void Pop3Server::cmd_retr( const string& msg )
    {
    reply( No, "Not implemented yet." );
d291 6
a296 1
    int mn = atoi(msg);
d299 1
a299 1
    s.print("Killed message number %d", mn );
d311 1
a311 1
void Pop3Server::cmd_top( const string&  )
d313 20
a332 1
    reply( No, "Did nothing. Unsuccessfully." );
d335 2
d344 5
d352 50
@


1.3
log
@*** empty log message ***
@
text
@d1 11
d14 1
@


1.2
log
@tcp works somehow
@
text
@d7 1
a7 1
    reply( Yes, "PopCorn " VERSION_STR "POP3 server ready" );
d42 1
d46 1
a46 1
                s.print( "This command (%s) is not known to me, sorry", cmd );
d114 1
d125 1
d133 22
@


1.1
log
@Initial revision
@
text
@d4 12
d42 6
d171 4
a174 3
        {
        reply( No, "Did nothing. Unsuccessfully." );
        }
d176 3
a178 3
            {
            reply( No, "Did nothing. Unsuccessfully." );
            }
@
