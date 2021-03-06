head	1.2;
access;
symbols;
locks;
comment	@ * @;


1.2
date	95.04.13.11.06.28;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.00.54;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@debug
@
text
@/************************ FIDO interface library ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Fido .MSG file data structures
 *
 *      $Log: FMsg_Str.h $
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 *      
 *         Rev 1.3   09 Apr 1993 10:06:42   dz
 *      Extended flags support
 *      
 *         Rev 1.2   25 Feb 1993 18:40:16   dz
 *      OS2 port: int/uint -> ushort
 *      
 *         Rev 1.1   06 Nov 1992 19:20:14   dz
 *      Ver 1.12a save
 *      
 *         Rev 1.0   30 Mar 1992 21:37:52   dz
 *      Initial revision.
 *
 *
\*/

#ifndef	FMSG_STR_H
#define	FMSG_STR_H


#include	<style.h>


struct msgtime
    {
	unsigned  day   : 5;
	unsigned  month : 4;
	unsigned  year  : 7;
	unsigned  tsec  : 5;
	unsigned  min   : 6;
	unsigned  hour  : 5;
	};



typedef ushort	fmsg_hdr_attr;

const ushort	FF_Private					= 0x0001u;
const ushort	FF_Crash                   = 0x0002u;
const ushort	FF_Recd                    = 0x0004u;
const ushort	FF_Sent                    = 0x0008u;
const ushort	FF_FileAttached            = 0x0010u;
const ushort	FF_InTransit               = 0x0020u;
const ushort	FF_Orphan                  = 0x0040u;
const ushort	FF_KillSent                = 0x0080u;
const ushort	FF_Local                   = 0x0100u;
const ushort	FF_HoldForPickup           = 0x0200u;
const ushort	FF_unused                  = 0x0400u;
const ushort	FF_FileRequest             = 0x0800u;
const ushort	FF_ReturnReceiptRequest    = 0x1000u;
const ushort	FF_IsReturnReceipt         = 0x2000u;
const ushort	FF_AuditRequest            = 0x4000u;
const ushort	FF_FileUpdateReq           = 0x8000u;


#pragma pack(1)

struct fido_msg_hdr
	{
	char			from[36];			// UserName, null terminated
	char			to[36];				// UserName, null terminated
	char			subject[72];		// see FileList below
	char			date[20];			// message body was last edited
	ushort			timesRead;			//
	ushort			destNode;			// of message
	ushort			origNode;			// of message
	ushort			cost;				// in lowest unit of originator's currency
	ushort			origNet;			// of message
	ushort			destNet;			// of message
//	msgtime			date_written;		// When user wrote the msg (GMT)
//	msgtime			date_arrived;		// When msg arrived on-line (GMT)

	ushort			destZone;			// Next 8 bytes are have two meanings
	ushort			origZone;
	ushort			destPoint;
	ushort			origPoint;

	ushort			replyTo;			// msg to which this replies
	fmsg_hdr_attr	attr;
	ushort			nextReply;			// msg which replies to this

	// Message text follows

	};

#pragma pack()

//
//  DateTime format:
//		`01 Jan 86  02:34:56'	- FIDO
//		`Mon  1 Jan 86 02:34'	- SEAdog


#endif //	FMSG_STR_H
@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/fido/vcs/fmsg_str.h_v  $
d107 1
a107 1
#endif	FMSG_STR_H
@
