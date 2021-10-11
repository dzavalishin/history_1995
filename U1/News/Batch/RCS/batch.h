head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	96.05.21.00.21.33;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.05.20.23.58.57;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.07.05.15.02;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.10.02.08.14;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.26.07;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@cosmetical changes
@
text
@/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Header
 *
 *      $Log: batch.h $
 *      Revision 1.4  1996/05/20 23:58:57  dz
 *      current
 *
 *      Revision 1.3  1995/08/07 05:15:02  dz
 *      Using App class
 *
 *      Revision 1.2  1995/04/10  02:08:14  dz
 *      IBM C Set version
 *
 *      Revision 1.1  1995/03/11  18:26:07  dz
 *      Initial revision
 *      
 *      Rev 1.0   21 Feb 1992 11:22:38   dz
 *      Initial revision.
 *      
 *
\*/

#ifndef BATCH_H
#define BATCH_H

#include	<style.h>
#include	<app.h>
#include	<ftn_msg.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<ctype.h>
#include	<errno.h>
#include	<time.h>

#include	"setup.h"
#include	"log.h"

#include	"feeds.h"



class BatchApp : public U1App
    {
    bool    sent_something;

    void	scan();
    void	scan_feeds( feeds &f, FILE *fp );
    void	scan_arts_nc( const feed &fe, FILE *fp );
    void	scan_arts_c( const feed &fe, FILE *fp );

    bool	send( feed::f_mode feed_mode, const char *dest, const char *snews, const char *fn );

    bool	compress( bool m12, int in_fd, int out_fd );

    bool	put_art( char *art, int batch_fd, const char *codetab );
    bool	do_put_art( const char *code, int ofd, int ifd );

    bool	recode( char *art, int ofd, const char *codetab );

    bool	group_in_list( const char *group, const char *g_list );

    bool	dest_in_path( const char *path, const char *aliases );

    bool	mk_temp_batch( char *o_fn, int *o_fd );


public:
    int	Run();
    };


#endif // BATCH_H


@


1.4
log
@current
@
text
@d8 3
d14 2
a15 5
 * Revision 1.2  1995/04/10  02:08:14  dz
 * IBM C Set version
 *
 * Revision 1.1  1995/03/11  18:26:07  dz
 * Initial revision
d17 2
d20 1
a20 1
 *         Rev 1.0   21 Feb 1992 11:22:38   dz
d26 3
d46 2
a47 2
	{
	bool    sent_something;
d49 4
a52 4
	void	scan();
	void	scan_feeds( feeds &f, FILE *fp );
	void	scan_arts_nc( const feed &fe, FILE *fp );
	void	scan_arts_c( const feed &fe, FILE *fp );
d54 1
a54 1
	bool	send( feed::f_mode feed_mode, const char *dest, const char *snews, const char *fn );
d56 1
a56 1
	bool	compress( bool m12, int in_fd, int out_fd );
d58 2
a59 2
	bool	put_art( char *art, int batch_fd, const char *codetab );
	bool	do_put_art( const char *code, int ofd, int ifd );
d61 1
a61 1
	bool	recode( char *art, int ofd, const char *codetab );
d63 1
a63 1
	bool	group_in_list( const char *group, const char *g_list );
d65 1
a65 1
	bool	dest_in_path( const char *path, const char *aliases );
d67 1
a67 1
	bool	mk_temp_batch( char *o_fn, int *o_fd );
d71 2
a72 3
	int	Run();
	};

d75 1
@


1.3
log
@Using App class
@
text
@d8 3
d26 1
a26 1
#include	<fido_msg.h>
@


1.2
log
@IBM C Set version
@
text
@d8 3
d22 1
a35 3
/****************************************************************************
						Global variables
****************************************************************************/
d37 23
a59 1
extern bool            sent_something;
a60 3
/****************************************************************************
							Functions
****************************************************************************/
d62 3
a65 1
void	scan();
a66 1
bool	send( feed::f_mode feed_mode, const char *dest, const char *snews, const char *fn );
a67 3
bool	compress( bool m12, int in_fd, int out_fd );
bool	put_art( char *art, int batch_fd, const char *codetab );
bool	recode( char *art, int ofd, const char *codetab );
a68 2
bool	group_in_list( const char *group, const char *g_list );
bool	dest_in_path( const char *path, const char *aliases );
a69 1
bool	mk_temp_batch( char *o_fn, int *o_fd );
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/uurnews/unbatch/vcs/unbatch.h_v  $
d44 1
a44 1
//bool	send( feed::f_mode feed_mode, const char *dest, const char *file_to_send );
@
