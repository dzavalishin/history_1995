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
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :       File Folder class
 *
 *      $Log: Ffolder.H $
 *
 *
 *
\*/

#ifndef FFOLDER_H
#define FFOLDER_H

#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#include <os2.h>

#include <strng.h>
//#include <ftn_msg.h>
//#include <log.h>


class _Export ffolder
    {
    protected:
        bool		restart_flag;
        bool		opened_flag;
        const string	dir_v;
        HDIR		h;

        void		do_restart( string &name );
        void		do_continue( string &name );
    public:
        ffolder( const string &dir );
        ~ffolder();

        void		restart();
        string		next();
	void		close();

	};



#endif // FFOLDER_H
@
