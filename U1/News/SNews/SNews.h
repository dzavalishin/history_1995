/************************ UU2 Snews ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	Snews header
 *
 *      $Log: SNews.h $
 *      Revision 1.5  1995/08/07 13:57:10  dz
 *      Cleanup
 *
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
#include	<ftn_msg.h>
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
        
        bool    verbose;

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

