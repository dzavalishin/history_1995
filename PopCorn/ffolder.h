/************************ PopCorn server ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :       File Folder class
 *
 *      $Log: ffolder.h $
 *      Revision 1.1  1996/09/17 23:20:43  dz
 *      Initial revision
 *
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
