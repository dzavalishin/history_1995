head	1.1;
access;
symbols
	Ver_075:1.1;
locks;
comment	@ * @;


1.1
date	96.09.17.23.20.43;	author dz;	state Exp;
branches;
next	;


desc
@@


1.1
log
@Initial revision
@
text
@/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Dmitry Zavalishin
 *
 *	Module 	: UUPC config access
 *
 *      $Log:  $
 *
 *
 *
 *
\*/

#include "popcorn.h"

#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#include <os2.h>

static const string & uupc_setup_dir()
    {
    static string dir;

    if( dir.is_empty() )
        {
        string s_sysrc = getenv("UUPCSYSRC"); // We MUST create our own copy
        const char * sysrc = s_sysrc;
        if( !sysrc )
            {
            printf("set UUPCSYSRC first!\n");
            throw Fail("uupc_setup_dir","UUPCSYSRC is not set","");
            }

        char *cp;

        while((cp = strchr( sysrc, '\\' )) != 0 )
            *cp = '/';
    
        cp = strrchr( sysrc, '/' );
        if( !cp )
            {
            printf("UUPCSYSRC value (%s) is not a full file name\n", sysrc);
            throw Fail("uupc_setup_dir","UUPCSYSRC value is not a full file name",sysrc);
            }

        *cp = 0;
        dir = sysrc;
        
        printf("uupc conf dir = '%s'\n", sysrc);
        }

    return dir;
    }


string uupc_mailbox_file_name( const string &user )
    {
    return uupc_setup_dir() + "/mail/" + user + ".spb";
    }

string uupc_passwd_file_name()
    {
    return uupc_setup_dir() + "/passwd";
    }







void do_move( string from, string to )
    {
    printf("moving %s to %s\n", from.c_str(), to.c_str() );
    if( rename( from, to ) )
        {
        if( NO_ERROR != DosCopy( from, to, 0 ) )
            throw("do_move","Can't rename or copy "+from+" to "+to,"");
        if( unlink( from ) )
            throw("do_move","Can't delete",from);
        }
    }






@
