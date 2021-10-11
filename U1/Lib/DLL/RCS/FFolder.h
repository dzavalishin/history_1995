head	1.2;
access;
symbols;
locks;
comment	@ * @;


1.2
date	95.11.05.18.50.12;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.05.13.52.48;	author dz;	state Exp;
branches;
next	;


desc
@FTN Folder
@


1.2
log
@RFC_Msg used
@
text
@/************************ Gate remote control ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :       FTN Folder class
 *
 *      $Log: FFolder.h $
 *      Revision 1.1  1995/11/05 13:52:48  dz
 *      Initial revision
 *
 *      Revision 1.2  1995/08/02 14:03:07  dz
 *      working version
 *
 *  Revision 1.1  1995/07/01  21:49:36  dz
 *  Initial revision
 *
 *
\*/

#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#include	<os2.h>

#include	<strng.h>
#include	<ftn_msg.h>
#include	<log.h>

class _Export ffolder
	{
	bool		restart_flag;
	bool		opened_flag;
	const string	dir_v;
	HDIR		h;

	bool		do_restart( string &name );
	bool		do_continue( string &name );
public:
			ffolder( const string &dir );
			~ffolder();

	void		restart();
	bool		next( FTN_Msg &fm );
	bool		close();


	};



@


1.1
log
@Initial revision
@
text
@d8 3
d25 1
a25 1
#include	<fido_msg.h>
d42 1
a42 1
	bool		next( fido_msg &fm );
@
