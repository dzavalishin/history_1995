head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	96.05.21.00.21.33;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.04.10.02.08.14;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.03.11.18.26.07;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.06.22.35.30;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
log
@cosmetical changes
@
text
@/************************ UU2 News Batcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Feeds file interface.
 *
 *      $Log: feeds.h $
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
@


1.3
log
@IBM C Set version
@
text
@d8 2
a9 2
 * Revision 1.2  1995/03/11  18:26:07  dz
 * snews=host mode
d11 5
a15 2
 * Revision 1.1  1995/03/06  22:35:30  dz
 * Initial revision
@


1.2
log
@snews=host mode
@
text
@d8 3
d24 5
a28 5
#define		MAXHOST			40		// Maximum length of host address
#define         MAXALIAS                200
#define		MAXGRLIST		400		// Maximum length of groups list
#define		FEED_REC_LEN	600		// Maximum line length in feed file
#define		MAXCODENAME		40		// Maximum code table name length
d31 1
a31 1
	{
d33 2
a34 2
	enum f_mode { NoMode, ViaUux, ViaMail, ViaRsh, ViaFilt };
	enum f_comp { NoComp, Batch,  Comp12,  Comp16 /*, Zip */ };
d36 2
a37 2
	f_mode		mode;
	f_comp		comp;
d39 2
a40 2
	bool		parse_mode( char *kw, int len );
	bool		parse_comp( char *kw, int len );
d42 1
a42 1
	char		codetab_name[MAXCODENAME+1];
d44 1
a44 1
	bool		add_lines;								// Count lines, add "Lines" headline
d51 2
a52 1
	};
d55 2
a56 2
	{
	FILE		*ff;									// Feeds list file
d59 1
a59 1
				feeds();
d62 2
a63 2
	bool		open( const char *filename );	       // Open feeds file
	void		close();							   // Close feeds file
d65 3
a67 3
    void		rewind();							   // Go to first feed
	bool		next( feed& );						   // Get next feed
	};
@


1.1
log
@Initial revision
@
text
@d7 3
a9 1
 *      $Log:$
d12 1
d44 2
@
