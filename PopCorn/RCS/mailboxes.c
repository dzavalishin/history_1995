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
date	96.09.15.07.06.18;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.13.12.38.48;	author dz;	state Exp;
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
 *	Module 	: mailbox access impl.
 *
 *      $Log: mailboxes.c $
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


@


1.2
log
@*** empty log message ***
@
text
@d7 4
a10 1
 *      $Log: MpChan.c $
a16 1
#include "except.h"
d72 2
a73 1
    load("D:/net/UUPC/Conf/passwd ");
d79 1
a79 1
    throw Fail("MailBoxes::operator[]", "invalid user name", username );
@


1.1
log
@Initial revision
@
text
@d1 11
d16 3
d21 1
a21 1
int MailBoxes::valid_user ( const char *username )
d23 33
d59 1
a59 1
int MailBoxes::valid_pass ( const char *username, const char *password )
d61 3
d66 7
@
