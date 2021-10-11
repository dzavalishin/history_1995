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
date	96.09.14.19.37.27;	author dz;	state Exp;
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
 *	Module 	: UUPC passwd access
 *
 *      $Log: passwd.h $
 *      Revision 1.2  1996/09/15 07:06:18  dz
 *      *** empty log message ***
 *
 *
 *
 *
 *
\*/

#ifndef PASSWD_H
#define PASSWD_H

#include <vector.h>

class passwd_entry
    {
    public:
        passwd_entry( ) {}
        
        passwd_entry( string s1, string s2 )
            : user(s1), pass(s2)
            {}
        
        string user;
        string pass;
    };

class passwd
    {
    vector <passwd_entry> pwd;
    
    public:
        void load( const char * fn );
        int valid_user( const string &user );
        int valid_pass( const string &user, const string &pass );
    };

#endif // PASSWD_H

@


1.2
log
@*** empty log message ***
@
text
@d7 4
a10 1
 *      $Log: MpChan.c $
d17 3
d44 2
@


1.1
log
@Initial revision
@
text
@d1 13
@
