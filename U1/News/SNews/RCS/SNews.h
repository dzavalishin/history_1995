head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	95.08.07.13.57.10;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.08.07.13.28.26;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.02.13.39.21;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.09.23.24.27;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.31.14;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@Cleanup
@
text
@/************************ UU2 Snews ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	Snews header
 *
 *      $Log: SNews.h $
 *      Revision 1.4  1995/08/07 13:28:26  dz
 *      Using class App
 *
 *      Revision 1.3  1995/08/02 13:39:21  dz
 *      extended with RFC-* and REPLYADDR...
 *
 *      Revision 1.2  1995/04/09  23:24:27  dz
 *      IBM C Set version
 *
 *      Revision 1.1  1995/03/11  18:31:14  dz
 *      Initial revision
 *
 *
 *         Rev 1.1   09 Apr 1993 15:27:52   dz
 *      Spool management moved to UU2.LIB
 *      
 *         Rev 1.0   04 Oct 1992 19:44:14   dz
 *      Initial revision.
 *      
 *
\*/

#include	<app.h>
#include	<style.h>
#include	<fido_msg.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<ctype.h>
#include	<errno.h>
#include	<time.h>

#include	"setup.h"
#include	"log.h"
#include	<elist.h>
#include	<spool.h>
#include	<strng.h>



/****************************************************************************
                               Data types
****************************************************************************/

struct fido_info
    {
    fido_user       origin;
    char            origin_text[80];
    char            msgid[80];
    char            reply[80];
    char            area[80];
    string          RFC_Message_Id;
    string          RFC_References;
    string          RFC_From;
    string          RFC_Path;
    string          RFC_Distribution;
    string          REPLYADDR;
    };




/****************************************************************************
                                Functions
****************************************************************************/

class SnewsApp : public U1App
	{
	bool	process( void );


public:
	int	Run();




	void    scan( void );                   // Scan netmail for packets

	bool    unpack( String pkt_name );
	bool	do_unpack( FILE *fp );

	bool    send( FILE *pkt_file );
	bool	do_send( news_out_spool_file & spf, FILE *fp );
	bool	copy_letter( FILE *out, FILE *fp );

        bool    read_origin( fido_info &info, FILE *fp );

        bool    readl( char *buf, int len, FILE *fp );
        bool    read_form( char *buf, FILE *fp );           // 80 bytes/line

        bool    get_from( uucp_addr &ufm, const fido_user &ffm, bool &registered );
        bool	make_from( FILE *out, const fido_user &ffm, echo_def &def, bool &registered, fido_info &fi );
        bool	make_ids ( FILE *out, fido_info &fi );
        time_t	f2u_date( char *fido_date );

	};

@


1.4
log
@Using class App
@
text
@d8 3
d14 2
a15 2
 * Revision 1.2  1995/04/09  23:24:27  dz
 * IBM C Set version
d17 2
a18 2
 * Revision 1.1  1995/03/11  18:31:14  dz
 * Initial revision
a85 1
//	bool    unpack( char *pkt_name );
a92 2
//	void    error_letter( void );           // Send letter to master

a95 1
//      bool    reads( char *buf, int len, FILE *fp );
@


1.3
log
@extended with RFC-* and REPLYADDR...
@
text
@d8 3
d27 1
d38 3
d70 22
d93 1
a93 10
void        scan( void );                               // Scan netmail for packets
bool        unpack( char *pkt_name );
bool        send( FILE *pkt_file );

void        error_letter( void );                       // Send letter to master
bool        read_origin( fido_info &info, FILE *fp );

bool        readl( char *buf, int len, FILE *fp );
//bool        reads( char *buf, int len, FILE *fp );
bool        read_form( char *buf, FILE *fp );           // 80 bytes/line
d95 3
d99 4
d104 1
@


1.2
log
@IBM C Set version
@
text
@d8 3
d48 6
@


1.1
log
@Initial revision
@
text
@d7 3
a9 1
 *      $Log:   Q:/news/snews/vcs/snews.h_v  $
d11 1
d63 1
a63 1
bool        reads( char *buf, int len, FILE *fp );
@
