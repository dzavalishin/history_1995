head	1.2;
access;
symbols;
locks;
comment	@ *      @;


1.2
date	95.11.06.02.33.43;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.05.17.45.44;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@Fixes & cleanups caused by FTN_Msg introduction
@
text
@/************************ FIDO MSG file class ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:		Implementation
 *
 *      $Log: FTN_Msg.c $
 *      Revision 1.1  1995/11/05 17:45:44  dz
 *      Initial revision
 *
 *      Revision 1.1  1995/11/05 17:37:36  dz
 *      Initial revision
 *
 *      Revision 1.2  1995/04/13  11:06:28  dz
 *      debug
 *
 *      Revision 1.1  1995/03/11  18:00:54  dz
 *      Initial revision
 *
 *      Revision 1.1  1995/03/11  18:00:54  dz
 *      Initial revision
 *      
 *         Rev 1.6   09 Apr 1993 10:07:42   dz
 *      Extended flags support
 *      
 *         Rev 1.5   26 Nov 1992 19:42:18   dz
 *      Reorganization
 *      
 *         Rev 1.4   06 Nov 1992 19:19:50   dz
 *      Ver 1.12a save
 *      
 *         Rev 1.3   23 Oct 1992 15:17:08   dz
 *      Style correction
 *      
 *         Rev 1.2   16 Oct 1992 03:38:00   dz
 *      FSC35 fix - initialization
 *      
 *         Rev 1.1   12 Sep 1991 01:33:54   dz
 *      Zone/Point fields zeroing
 *      
 *         Rev 1.0   28 Aug 1991 00:36:56   dz
 *      Initial revision.
 *
 *
\*/

#include	<style.h>
#include	<uu2io.h>
#include	<string.h>
#include	<time.h>

#include	"ftn_msg.h"

//#define	BLEN	300


/****************************************************************************
						C'tor / D'tor
****************************************************************************/


FTN_Msg::FTN_Msg( void ) : cursor( text )
	{
	clear_hdr();
	error 	= Yes;
//	hflush	= No;
	rewrite	= No;
	}



FTN_Msg::~FTN_Msg()  				// D'tor
	{
	if( error )		return;
	detach();
	}


/**
 *
 *		Clear header
 *
**/

void
FTN_Msg::clear_hdr( void )
	{
//	if( !error )	flush();

        via_addr = "";
        
        hdr.from[0]		=
	hdr.to[0]		=
	hdr.date[0]		=
	hdr.subject[0]	= '\0';

	hdr.timesRead	=
	hdr.destNode	=
	hdr.origNode	=
	hdr.cost		=
	hdr.origNet		=
	hdr.destNet		=
	hdr.destZone	=
	hdr.origZone	=
	hdr.destPoint	=
	hdr.origPoint	=
	hdr.replyTo		=
	hdr.nextReply	= 0;

	*((uint *)(&hdr.attr))	= 0;

	kl_reply	= "";
	kl_pid	        = "";
	fsc35_reply_addr = "";
	fsc35_reply_to.net(0);

	flags = 0ul;

	}

@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: FTN.c $
d67 1
a67 1
//	rewrite	= No;
@
