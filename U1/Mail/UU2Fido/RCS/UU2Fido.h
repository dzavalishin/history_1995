head	1.8;
access;
symbols;
locks;
comment	@ * @;


1.8
date	96.03.08.22.35.08;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	95.11.05.17.37.36;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	95.11.05.13.50.34;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	95.11.04.18.32.53;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.11.04.01.21.46;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.11.02.14.25.20;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.08.02.13.12.46;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.07.00.20.29;	author dz;	state Exp;
branches;
next	;


desc
@@


1.8
log
@2.09g
@
text
@/************************ UUCP to FIDO convertor ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	Header
 *
 *      $Log: UU2Fido.h $
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
@


1.7
log
@Using FTN_Msg
@
text
@d8 3
d105 1
a105 1
    bool    copy_data( RFC_Msg &msg, FILE *out );
@


1.6
log
@Before fido_msg rewrite
@
text
@d8 3
d44 3
d63 1
a63 1
#include	<fido_msg.h>
d104 1
a104 1
    bool    send_file( RFC_Msg &msg, fido_msg *fm, const string &received_for, const String &filedir );
d107 1
a107 1
    bool    send_letter( RFC_Msg &msg, fido_msg &fm, const string &received_for );
@


1.5
log
@Now returning message to sender in a case of
address conversion errors.
Some debugging done.
@
text
@d8 5
a49 4
//#include	<dbllist.h>

//#include	"uuparse.h"

a82 3
//    FILE	*	in;				// Re-fdopene'ed stdin


d85 1
a85 5

    //bool  remap_addr( fido_user &fido_to, const char *to );
    bool    remap_addr( fido_user &fto, String &filedir, const char *to_a );
//    bool    is_news( uuinfo &ui, char *uu_dest );
//    bool    is_batch( uuinfo &ui, char *uu_dest );
a89 1

a92 1

a95 1
    
a100 1
    
a103 1

d105 1
a105 1

a106 1

a110 1

a111 8


//    void	    rewind_to_text();

//    bool 		CallRexx( const char *prog, string &out, const char *a1, const char *a2 );
//    bool		Register_Rexx_Subcommand_Handler( void );
//    bool		Register_Rexx_Function_Handlers( void );

@


1.4
log
@First RFC_Msg-based version, that compiles and does something
@
text
@d8 3
d47 1
a47 1
#include	"uuparse.h"
d120 1
@


1.3
log
@Before switching to rfc_msg
@
text
@d8 3
d59 4
d66 2
a67 2
    int                 Run();
    bool                must_use_recoder() { return use_recoder; }
d69 1
a69 1
                        Uu2FidoApp();
d73 5
a77 2
    bool         	use_recoder;			// Convert code tables
    bool		from_Unix;			// Was started with -u flag
a81 2
    bool		send_letter( fido_msg *fm, char *received_for );
    bool		send_file( fido_msg *fm, char *received_for, const uuinfo &ui, const String &filedir );
d85 6
a90 4
    //bool		remap_addr( fido_user &fido_to, const char *to );
    bool		remap_addr( fido_user &fto, String &filedir, const char *to_a );
    bool		is_news( uuinfo &ui, char *uu_dest );
    bool		is_batch( uuinfo &ui, char *uu_dest );
d94 1
a94 1
    bool		send( char *uu_dest );
d98 12
a109 2
    bool		spool_news();

d112 1
a112 1
    int			get_packet_file( void );
d116 1
a116 1
    bool		run_uusmail( char *from, char *to );
d120 1
a120 1
      int               load_args();
@


1.2
log
@rexx support, etc...
@
text
@d8 5
a12 2
 * Revision 1.1  1995/03/07  00:20:29  dz
 * Initial revision
d30 1
d37 1
a37 1
#include    <fcntl.h>
d53 1
a53 1
#include    "stat.h"
d56 5
d62 1
a62 16
/****************************************************************************
						Global variables
****************************************************************************/

extern bool         	use_recoder;			// Convert code tables
extern bool				from_Unix;				// Was started with -u flag

extern FILE			*	in;						// Re-fdopene'ed stdin

/****************************************************************************
							Functions
****************************************************************************/


bool		send_letter( fido_msg *fm, char *received_for );
bool		send_file( fido_msg *fm, char *received_for, const uuinfo &ui, const String &filedir );
d64 1
d66 2
d69 1
a69 1
//			ADDRESS.C
d72 2
a73 4
//bool		remap_addr( fido_user &fido_to, const char *to );
bool		remap_addr( fido_user &fto, String &filedir, const char *to_a );
bool		is_news( uuinfo &ui, char *uu_dest );
bool		is_batch( uuinfo &ui, char *uu_dest );
d75 1
d77 4
a80 1
//			SEND.C
d82 1
d84 1
a84 1
bool		send( char *uu_dest );
d86 1
d88 1
a88 1
//			SENDSPOL.C
d90 1
d92 1
a92 1
bool		spool_news();
d94 1
d96 1
a96 1
//			OPEN_PKT.C
d98 1
a98 1
int			get_packet_file( void );
d100 1
a101 1
//			RUNSMAIL.C
d103 1
a103 1
bool		run_uusmail( char *from, char *to );
d105 3
a107 1
//			MAIN.C
a108 1
void	    rewind_to_text();
d111 1
a111 3
bool 		CallRexx( const char *prog, string &out, const char *a1, const char *a2 );
bool		Register_Rexx_Subcommand_Handler( void );
bool		Register_Rexx_Function_Handlers( void );
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/uu2fido/vcs/uu2fido.h_v  $
d68 1
a68 1
bool		send_file( fido_msg *fm, char *received_for, const uuinfo &ui );
d75 2
a76 1
bool		remap_addr( fido_user &fido_to, const char *to );
d106 4
@
