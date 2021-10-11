/************************ UU2 News Batcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Feeds file interface.
 *
 *      $Log: feeds.h $
 *      Revision 1.4  1996/05/21 00:21:33  dz
 *      cosmetical changes
 *
 *      Revision 1.3  1995/04/10  02:08:14  dz
 *      IBM C Set version
 *
 *      Revision 1.2  1995/03/11  18:26:07  dz
 *      snews=host mode
 *
 *      Revision 1.1  1995/03/06  22:35:30  dz
 *      Initial revision
 *
 *
 *
\*/

#ifndef	FEEDS_H
#define	FEEDS_H

#include	<style.h>
#include	<stdio.h>

#define         MAXHOST         40	// Maximum length of host address
#define         MAXALIAS        200
#define         MAXGRLIST       400	// Maximum length of groups list
#define         FEED_REC_LEN	600	// Maximum line length in feed file
#define         MAXCODENAME     40	// Maximum code table name length

struct feed
    {

    enum f_mode { NoMode, ViaUux, ViaMail, ViaRsh, ViaFilt };
    enum f_comp { NoComp, Batch,  Comp12,  Comp16 /*, Zip */ };

    f_mode		mode;
    f_comp		comp;

    bool		parse_mode( char *kw, int len );
    bool		parse_comp( char *kw, int len );

    char		codetab_name[MAXCODENAME+1];

    bool		add_lines;								// Count lines, add "Lines" headline

    char		dest[MAXHOST+1];
    char		snews[MAXHOST+1];

    char                aliases[MAXALIAS+1];
    char		groups[MAXGRLIST+1];
    };


class feeds
    {
    FILE		*ff;									// Feeds list file

public:
                feeds();
                ~feeds();

    bool	open( const char *filename );	       // Open feeds file
    void	close();							   // Close feeds file

    void	rewind();							   // Go to first feed
    bool	next( feed& );                         // Get next feed
    };



#endif
