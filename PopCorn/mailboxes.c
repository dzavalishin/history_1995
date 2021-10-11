/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Dmitry Zavalishin
 *
 *	Module 	: mailbox access impl.
 *
 *      $Log: mailboxes.c $
 *      Revision 1.3  1996/09/17 23:20:43  dz
 *      First completely working POP3/ESMTP version
 *
 *      Revision 1.2  1996/09/15 07:06:18  dz
 *      *** empty log message ***
 *
 *
 *
 *
\*/

#include "PopCorn.h"

#include <stdio.h>


MailBoxes mailboxes;

void passwd::load( const char * fn )
    {
    FILE *fp = fopen(fn, "r" );
    if( fp == NULL ) throw Fail( "passwd::load", "can't open file", fn );

    string line;
    while(1)
        {
        try { line.load(fp); } catch( XEOF ) { break; }

        line.strip_crlf();
        line.strip_ws();

        if( line[0] == '#' || line.is_empty() ) continue;
        
        string user, temp, pass;
        line.parse( user, ':', temp );
        temp.parse( pass, ':', temp );

        if( user.is_empty() || pass.is_empty() )
            {
            printf("Invalid passwd entry: %s\n", line.c_str() );
            continue;
            }
        pwd.push_back( passwd_entry(user,pass) );
        }

    fclose(fp);
    }

int passwd::valid_user( const string &user )
    {
    for( int i = 0; i < pwd.size(); i++ )
        if( user == pwd[i].user )
            return 1;
    return 0;
    }

int passwd::valid_pass( const string &user, const string &pass )
    {
    for( int i = 0; i < pwd.size(); i++ )
        if( user == pwd[i].user && pass == pwd[i].pass )
            return 1;
    return 0;
    }


MailBoxes::MailBoxes()
    {
      //load("D:/net/UUPC/Conf/passwd");
    load(uupc_passwd_file_name());
    }


MailBox & MailBoxes::operator[] ( const char *username )
    {
    return *(new MailBox(username));
    }


