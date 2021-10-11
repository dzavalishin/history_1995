/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Header
 *
 *      $Log: batch.h $
 *      Revision 1.5  1996/05/21 00:21:33  dz
 *      cosmetical changes
 *
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


