/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Dmitry Zavalishin
 *
 *	Module 	: HTTP Proto impl.
 *
 *      $Log: smtpproto.c $
 *
 *
 *
\*/

#include "popcorn.h"
#include "http.h"
#include "except.h"
#include "recode.h"

HTTPServer::HTTPServer( int port ) : TCP_Server( port )
    {
    }

HTTPServer::~HTTPServer()
    {
    }


void HTTPServer::server_loop()
    {
    string cmdl;
    try { cmdl = getline(); }
    catch( XEOF )
        {
        printf("HTTP connection closed by peer\n");
        return;
        }
    string cmd, arg;
        
    cmdl.strip_ws();
    cmdl.parse( cmd, arg );
    cmd.lcase();
        
    printf("HTTP got '%s'\n", cmdl.c_str() );
        
    try
        {
        if( cmd == "get" )         cmd_get(arg);
        else
            send_error("Wrong command", "I'm stupid and the only command I know is GET");
        }
    catch( ... )
        {
        send_error("Fail","Server failed (exception)");
        }
    }

void HTTPServer::send_error( const string & title, const string & text )
    {
    html_head( "PopCorn Server Error" );
    h(1,title);
    send(text);
    html_tail();
    }

void HTTPServer::html_head(
                           string title,
                           int bgcolor, int textcolor,
                           int linkcolor, int vlinkcolor,
                           string backimage
                          )
    {
    send("<HTML>\r\n<HEAD>\r\n<TITLE>");
    send(title);
    send("</TITLE>\r\n</HEAD>\r\n<BODY ");
    if( !backimage.is_empty() ) send("BACKGROUND=\"Back1.GIF\" ");
    string cols;
    cols.print(
               "BGCOLOR=\"#%06x\" TEXT=\"#%06x\" LINK=\"#%06x\" VLINK=\"#%06x\">\r\n",
               bgcolor, textcolor, linkcolor, vlinkcolor
              );
    send(cols);
    }

void HTTPServer::html_tail(
                          )
    {
    send("</BODY>\r\n</HTML>\r\n");
    }

void HTTPServer::h( int level, string header )
    {
    char hh[20];
    sprintf(hh,"<H%d>", level );  send(hh);
    send( header );
    sprintf(hh,"</H%d>", level ); send(hh); 
    }
void HTTPServer::p()
    {
    send( "<P>" );
    }


// /?LoginInput=&PasswordInput=&ComeIn=+Come+In+

void HTTPServer::cmd_get( const string & arg )
    {
    string dname, _;
    arg.parse( dname, _ );
    if( dname == "/" ) send_root_page();
    if( dname[0] == '/' && dname[1] == '?' ) login( dname.substr(2) );
    else send_error("Document not found","\r\nSorry, there's no requested page here\r\n");
    }

void HTTPServer::send_root_page()
    {
    html_head( "PopCorn Login Page at "+domain );
      //send("<CENTER>");h(1,"Welcome to PopCorn mail service");send("</CENTER>");
    h(1,"<CENTER>Welcome to PopCorn mail service</CENTER>");
    p();
    send("Mail server at "+domain+" is ready to service you\r\n");
    p();
    send(
         "To access your mailbox please enter your user name"
         " (login) and password into the following fields and"
         " press button below.<P>\r\n"
         "\r\n"
         "<FORM>\r\n"
         "\r\n"
         "<HR ALIGN=CENTER>\r\n"
         "Login: <INPUT NAME=\"LoginInput\" SIZE=35><P>\r\n"
         "Password: <INPUT NAME=\"PasswordInput\" SIZE=31><P>\r\n"
         "<HR ALIGN=CENTER>\r\n"
         "<CENTER><INPUT TYPE=\"SUBMIT\" NAME=\"ComeIn\" VALUE=\" Come In \"></CENTER>\r\n"
         "\r\n"
         "</FORM>\r\n"
        );

    html_tail();
    }


  // LoginInput=&PasswordInput=&ComeIn=+Come+In+

void HTTPServer::login( string arg )
    {

    string login, password, elem, tail = arg, key, val;

    while( !tail.is_empty() )
        {
        tail.parse(elem, '&', tail );
        elem.parse( key, '=', val );
        if( key == "LoginInput" ) login = val;
        if( key == "PasswordInput" ) password = val;
        }

    
    if(mailboxes.valid_pass(login, password))
        {
        start_session(login, password);
        return;
        }

    
    html_head( "PopCorn Login Failure at "+domain );
    h(1,"<CENTER>You are not logged in</CENTER>");
    p();
    send("Mail server at "+domain+" does not know user "+login+" with password you specified, sorry.\r\n");
    p();
    html_tail();
    }

void HTTPServer::start_session(string login, string password)
    {
    html_head( "PopCorn "+login+"'s Page at "+domain );
    h(1,"<CENTER>List of messages for "+login+"</CENTER>");
    send("<P>\r\nYour mail at "+domain+":<P>\r\n");

    html_tail();
    }

