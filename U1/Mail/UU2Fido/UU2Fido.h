/************************ UUCP to FIDO convertor ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	Header
 *
 *      $Log: UU2Fido.h $
 *      Revision 1.8  1996/03/08 22:35:08  dz
 *      2.09g
 *
 *      Revision 1.7  1995/11/05 17:37:36  dz
 *      Using FTN_Msg
 *
 *      Revision 1.6  1995/11/05 13:50:34  dz
 *      Before fido_msg rewrite
 *
 *      Revision 1.5  1995/11/04 18:32:53  dz
 *      Now returning message to sender in a case of
 *      address conversion errors.
 *      Some debugging done.
 *
 *      Revision 1.4  1995/11/04 01:21:46  dz
 *      First RFC_Msg-based version, that compiles and does something
 *
 *      Revision 1.3  1995/11/02 14:25:20  dz
 *      Before switching to rfc_msg
 *
 *      Revision 1.2  1995/08/02 13:12:46  dz
 *      rexx support, etc...
 *
 *      Revision 1.1  1995/03/07  00:20:29  dz
 *      Initial revision
 *
 *      
 *         Rev 1.3   12 Apr 1993 12:08:54   dz
 *      stdin replaced with in - support for uupc 5.06
 *      
 *         Rev 1.2   13 Mar 1993 23:26:36   dz
 *      up to date
 *      
 *         Rev 1.1   29 Jun 1992 03:36:08   dz
 *      up to date
 *      
 *         Rev 1.0   11 Oct 1991 05:39:56   dz
 *      Initial revision.
 *
 *
\*/

// prevent unwanted inclusion
#define FIDO_MSG_H

#include	<app.h>
#include	<style.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<ctype.h>
#include	<errno.h>
#include	<time.h>
#include        <fcntl.h>

#include	"..\..\u1.h"
#include	"..\..\version.h"

#include	<setup.h>
#include	<log.h>
#include	<uucpaddr.h>
#include	<userlist.h>
#include	<ftn_msg.h>
#include	<datec.h>
#include	<recode.h>
#include        <stat.h>
#include	<hl_match.h>

//#include	<rfc_msg.h>
#include	"rfc_msg.h"


class Uu2FidoApp : public U1App
    {
public:
    int     Run();
    bool    must_use_recoder() { return use_recoder; }

            Uu2FidoApp();

protected:

    bool    use_recoder;                // Convert code tables
    bool    from_Unix;                  // Was started with -u flag
    bool    got_job;                    // Our msg is loaded
    
    RFC_Msg msg;


    //			ADDRESS.C
    bool    map_addr( RFC_Msg &msg, fido_user &fto, String &filedir, string to_a );
    bool    is_news( RFC_Msg &msg, const string &uu_dest );
    bool    is_batch( RFC_Msg &msg, const string &uu_dest );

    //			SEND.C
    bool    send( RFC_Msg &msg, const String &uu_dest );

    //			SENDSPOL.C
    bool    spool_news();
    
    //                  SendFile.c
    bool    copy_data( RFC_Msg &msg, FILE *out, bool codeback = Yes );
    bool    prepare_file( RFC_Msg &msg, char *arc_name, const char *filedir );
    bool    send_file( RFC_Msg &msg, FTN_Msg &fm, const string &received_for, const String &filedir );

    //                  SendLett.c
    bool    send_letter( RFC_Msg &msg, FTN_Msg &fm, const string &received_for );
    
    //			OPEN_PKT.C
    int	    get_packet_file( void );
    
    //			RUNSMAIL.C
    bool    run_uusmail( RFC_Msg &msg, const string &to );
    bool    reply_error( RFC_Msg &msg, const string &to );

    //			MAIN.C
    int     load_args();


    };
