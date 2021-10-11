head	1.1;
access;
symbols;
locks;
comment	@ * @;


1.1
date	94.08.05.01.12.42;	author root;	state Exp;
branches;
next	;


desc
@main header
@


1.1
log
@Initial revision
@
text
@/************************ Mail Mover ***************************\
 *
 *      Copyright (C) 1991-1994 by Infinity Soft
 *
 *      Module  :       Header
 *
 *      $Log:   C:/net/gate/fido2uu/vcs/fido2uu.h_v  $
 *
 *
\*/

#include	<style.h>
#include	<fido_msg.h>
#include	<uucpaddr.h>

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"setup.h"
#include	"log.h"
//#include	"cc_bag.h"


/****************************************************************************
						Function prototypes
****************************************************************************/


//              CONVERT.C
//void	convert( fido_msg &, bool &report, bool &err );

//				SELECT.C
//bool	select( fido_msg & );

//				MAINLOOP.C
void	mainloop( void );

//				COPY.C
//void	copy_data( FILE *fp, fido_msg *fm, char *uu_to );

//				ADDRESS.C
//bool	get_address( char *uu_to, fido_msg *fm );
//bool	get_from( bool *reg_user, uucp_addr &uu_from, const fido_user &ffm );
//bool	fidoaddr2uuaddr( uucp_addr &uu_fm, const char *f_fm );

//				SEND_ERR.C
//void	error_letter( fido_msg *orig );

//				SEND_ACK.C
//void	acknovledge_letter( fido_msg *orig );

//              RUNSMAIL.C
//bool    run_uusmail( const uucp_addr &from, char *to, CC_Bag &cc, int fd );

//              gatectl.c
//bool    is_gate_control( uchar *s );


/****************************************************************************
						   Flags definition
****************************************************************************/



extern bool		f_silent;		// Don't chat
extern bool		f_keeporig;		// Keep original message



/****************************************************************************
						   Global variables
****************************************************************************/


@
