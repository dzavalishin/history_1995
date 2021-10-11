head	1.2;
access;
symbols;
locks;
comment	@ *  @;


1.2
date	95.08.02.14.03.07;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.07.01.21.49.36;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@working version
@
text
@/************************ Gate remote control ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :       FTN Folder class
 *
 *      $Log: FFolder.h $
 *  Revision 1.1  1995/07/01  21:49:36  dz
 *  Initial revision
 *
 *
\*/

#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#include	<os2.h>

//#define INCL_BSE
//#include <BSEDOS.H>

#include	<strng.h>
#include	<fido_msg.h>
#include	<log.h>

class ffolder
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
	bool		next( fido_msg &fm );
	bool		close();


	};



@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: Main.c $
d14 7
d23 1
d27 4
d32 2
d40 1
@
