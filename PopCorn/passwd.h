/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Dmitry Zavalishin
 *
 *	Module 	: UUPC passwd access
 *
 *      $Log: passwd.h $
 *      Revision 1.3  1996/09/17 23:20:43  dz
 *      First completely working POP3/ESMTP version
 *
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

