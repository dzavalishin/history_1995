head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	96.03.27.10.46.53;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.11.06.01.42.16;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.08.08.13.22.34;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.24.01;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
log
@current
@
text
@/************************ UUCP to FIDO convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :       Header
 *
 *      $Log: Fido2UU.h $
 *      Revision 1.3  1995/11/06 01:42:16  dz
 *      Using FTN_Msg! Seems to be working.
 *
 *      Revision 1.2  1995/08/08 13:22:34  dz
 *      App class used
 *
 *      Revision 1.1  1995/03/11  18:24:01  dz
 *      Initial revision
 *
 *      Revision 1.1  1995/03/11  18:24:01  dz
 *      Initial revision
 *
 *      
 *         Rev 1.1   11 Oct 1991 05:28:22   dz
 *      Up to date.
 *      
 *         Rev 1.0   27 Aug 1991 02:47:20   dz
 *      Initial revision.
 *
 *
\*/

#include    <app.h>
#include    <ftn_msg.h>
#include    <rfc_msg.h>
#include    <uucpaddr.h>

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>

#include    "setup.h"
#include    "log.h"
#include    "cc_bag.h"



class Fido2uuApp : public U1App
	{
	bool	process( void );


public:
	int	Run();

protected:
	bool		f_silent;		// Don't chat
	bool		f_keeporig;		// Keep original message


//              CONVERT.C
	void	convert( FTN_Msg &, bool &report, bool &err );

//				SELECT.C
	bool	select( FTN_Msg & );

//				MAINLOOP.C
	void	mainloop( void );
	void	do_one( int msg_no, bool &processed );

//				COPY.C
//	void	copy_data( FILE *fp, fido_msg *fm, char *uu_to );

//				ADDRESS.C
	bool	get_address( char *uu_to, FTN_Msg &fm );
	bool	get_from( bool *reg_user, uucp_addr &uu_from, const fido_user &ffm );
	bool	fidoaddr2uuaddr( uucp_addr &uu_fm, const char *f_fm );

//				SEND_ERR.C
	void	error_letter( FTN_Msg &orig );
        void   	put_track( FTN_Msg &fm );

//				SEND_ACK.C
	void	acknovledge_letter( FTN_Msg &orig );

//                              RUNSMAIL.C
	bool    run_uusmail( const uucp_addr &from, const string &to, CC_Bag &cc, const RFC_Msg &msg );

//                              GATECTL.C
	bool    is_gate_control( string s );

	};


@


1.3
log
@Using FTN_Msg! Seems to be working.
@
text
@d8 3
d30 12
a41 11
#include	<app.h>
#include	<ftn_msg.h>
#include	<uucpaddr.h>

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"setup.h"
#include	"log.h"
#include	"cc_bag.h"
d84 1
a84 1
	bool    run_uusmail( const uucp_addr &from, const string &to, CC_Bag &cc, int fd );
@


1.2
log
@App class used
@
text
@d8 3
d28 1
a28 1
#include	<fido_msg.h>
a48 1

a49 3



a53 3



d55 1
a55 1
	void	convert( fido_msg &, bool &report, bool &err );
d58 1
a58 1
	bool	select( fido_msg & );
d68 1
a68 1
	bool	get_address( char *uu_to, fido_msg *fm );
d73 2
a74 1
	void	error_letter( fido_msg *orig );
d77 1
a77 8
	void	acknovledge_letter( fido_msg *orig );

//              RUNSMAIL.C
	bool    run_uusmail( const uucp_addr &from, char *to, CC_Bag &cc, int fd );

//              gatectl.c
	bool    is_gate_control( uchar *s );

d79 2
d82 2
@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   C:/net/gate/fido2uu/vcs/fido2uu.h_v  $
d24 1
a24 1
#include	<style.h>
d37 19
a55 3
/****************************************************************************
						Function prototypes
****************************************************************************/
d59 1
a59 1
void	convert( fido_msg &, bool &report, bool &err );
d62 1
a62 1
bool	select( fido_msg & );
d65 2
a66 1
void	mainloop( void );
d69 1
a69 1
void	copy_data( FILE *fp, fido_msg *fm, char *uu_to );
d72 3
a74 3
bool	get_address( char *uu_to, fido_msg *fm );
bool	get_from( bool *reg_user, uucp_addr &uu_from, const fido_user &ffm );
bool	fidoaddr2uuaddr( uucp_addr &uu_fm, const char *f_fm );
d77 1
a77 1
void	error_letter( fido_msg *orig );
d80 1
a80 1
void	acknovledge_letter( fido_msg *orig );
d83 1
a83 1
bool    run_uusmail( const uucp_addr &from, char *to, CC_Bag &cc, int fd );
d86 1
a86 8
bool    is_gate_control( uchar *s );


/****************************************************************************
						   Flags definition
****************************************************************************/


a87 2
extern bool		f_silent;		// Don't chat
extern bool		f_keeporig;		// Keep original message
d91 1
a91 3
/****************************************************************************
						   Global variables
****************************************************************************/
@
