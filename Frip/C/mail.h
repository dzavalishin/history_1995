/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Mail processing
 *
 *      $Log: mail.h $
 *      Revision 1.7  1997/01/31 09:54:36  dz
 *      PackDirect support, fixes.
 *
 *      Revision 1.6  1997/01/21 08:53:51  dz
 *      Flags, msgid, via lines
 *
 *      Revision 1.5  1997/01/01 21:31:39  dz
 *      msg received time/date support
 *
 *      Revision 1.4  1997/01/01 16:24:36  dz
 *      treat those 8 bytes as msgtime, not point/zone addr.
 *
 *      Revision 1.3  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.2  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.1  1996/07/31 07:00:23  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#ifndef	MAIL_H
#define	MAIL_H

#include	<style.h>
#include	<strng.h>

#include	"rif.h"
#include	<datetime.h>

string new_msg_name( const string &dir );
void msg_add_via_lines( string name, const vector <path_elem> &path );
void msg_mark_sent( string in );
void msg_mark_received( string in );
void msg_create_new( string contents, const addr &to, string fromname, string toname, string subject, string reply_msgid );





#pragma pack(1)


/****************************************************************************
                                    Message
****************************************************************************/



typedef ushort	fmsg_hdr_attr;

#pragma pack(1)

struct fido_msg_hdr
    {
    char			from[36];      	// UserName, null terminated
    char			to[36];	        // UserName, null terminated
    char			subject[72];    // see FileList below
    char			date[20];      	// message body was last edited
    ushort			timesRead;     	//
    ushort			destNode;      	// of message
    ushort			origNode;      	// of message
    ushort			cost;	       	// in lowest unit of originator's currency
    ushort			origNet;       	// of message
    ushort			destNet;       	// of message

#if 1
    msgtime			date_written;   // When user wrote the msg (GMT)
    msgtime			date_arrived;   // When msg arrived on-line (GMT)
#else
    ushort			destZone;      	// Next 8 bytes are have two meanings
    ushort			origZone;
    ushort			destPoint;
    ushort			origPoint;
#endif
    ushort			replyTo;   	// msg to which this replies
    fmsg_hdr_attr               attr;
    ushort			nextReply;   	// msg which replies to this

    // Message text follows

    };

#pragma pack()

//
//  DateTime format:
//		`01 Jan 86  02:34:56'	- FIDO
//		`Mon  1 Jan 86 02:34'	- SEAdog




const ushort	FF_Private    	           = 0x0001u;
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




const ulong		FF_ArchiveSent		   = 0x00010000ul;
const ulong		FF_Direct                  = 0x00020000ul;
const ulong		FF_Immediate               = 0x00040000ul;
const ulong		FF_ZoneGate                = 0x00080000ul;
const ulong		FF_HubHostRoute            = 0x00100000ul;
const ulong		FF_Xmail                   = 0x00200000ul;
const ulong		FF_KillFileSent            = 0x00400000ul;
const ulong		FF_TruncateFileSent        = 0x00800000ul;
const ulong		FF_Lock                    = 0x01000000ul;
const ulong		FF_ConfirmReceiptRequest   = 0x02000000ul;
const ulong		FF_FAX_HiRes               = 0x04000000ul;
const ulong		FF_FAX_CoverLetter         = 0x08000000ul;
const ulong		FF_FAX_Signature           = 0x10000000ul;
const ulong		FF_FAX_LetterHead          = 0x20000000ul;

#define FLAGS_KLUDGE_BITS	                    \
    (                                               \
    FF_Private					|   \
    FF_Crash					|   \
    FF_Recd    					|   \
    FF_Sent    					|   \
    FF_FileAttached				|   \
    FF_KillSent					|   \
    FF_HoldForPickup			        |   \
    FF_FileRequest				|   \
    FF_ReturnReceiptRequest		        |   \
    FF_ArchiveSent				|   \
    FF_Direct					|   \
    FF_Immediate				|   \
    FF_ZoneGate					|   \
    FF_HubHostRoute				|   \
    FF_Xmail					|   \
    FF_KillFileSent				|   \
    FF_TruncateFileSent			        |   \
    FF_Lock					|   \
    FF_ConfirmReceiptRequest	                |   \
                                                    \
    FF_FAX_HiRes				|   \
    FF_FAX_CoverLetter			        |   \
    FF_FAX_Signature		                |   \
    FF_FAX_LetterHead             		    \
    )


ulong mail_scan_flags( const string &b );
void mail_print_flags( string &buf, ulong f );




#endif // MAIL_H
