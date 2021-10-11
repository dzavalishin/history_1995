/************************ FIDO PKT file format ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:		File format structure
 *
 *      $Log: Fido_Pkt.h $
 * Revision 1.3  1995/05/07  23:03:10  dz
 * date is excluded from this structure 'cause it is a variable size
 * field, as it looks now.
 *
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 *      
 *         Rev 1.4   09 Apr 1993 10:05:34   dz
 *      Extended flags support
 *      
 *         Rev 1.3   25 Feb 1993 18:38:06   dz
 *      OS2 port - uint/int -> ushort
 *      
 *         Rev 1.2   06 Nov 1992 19:20:12   dz
 *      Ver 1.12a save
 *      
 *         Rev 1.1   04 Feb 1992 22:15:06   dz
 *      Type 2+ fields added
 *      
 *         Rev 1.0   11 Oct 1991 05:11:16   dz
 *      Initial revision.
 *
 *
\*/

#ifndef __cplusplus
#error You Need C++ to compile this
#endif


#ifndef	FIDO_PKT_H
#define	FIDO_PKT_H

#ifndef STYLE_H
#	include	<style.h>
#endif

/*#ifndef FIDO_MSG_H
#	include	"fido_msg.h"
#endif */

#include	<stdio.h>


#define		MSG_FN_LEN		150

#pragma pack(1)


struct pkt_date
    {
	ushort		year;
	ushort		month;
	ushort		day;
	ushort		hour;
	ushort		minute;
	ushort		second;
	};


struct fido_pkt_hdr
    {
	ushort		origNode;			// of packet
	ushort		destNode;			// of packet

	pkt_date	date;

	ushort		baud;				// Minimal for recvr/xmitter

	ushort		pkt_type;			// This = 02

	ushort		origNet;			// of packet
	ushort		destNet;			// of packet

	uchar		lo_product_code;
	uchar		hi_version_no;

	uchar		password[8];		// Password - set to 0s

	ushort		origZone;			// Zone of origin
	ushort		destZone;			// Zone of destination

//	char		fill[20];			// Zero Fill - was not used in
									// Stone age packets

	char		fill[2];			// 2+ fields follow

	ushort		CapValid;			// Capabilities word validation

	uchar		hi_product_code;
	uchar		lo_version_no;


	ushort		CapWord;			// Capabilities word

	ushort		_origZone;			// Zone of origin
	ushort		_destZone;			// Zone of destination

	ushort		origPoint;			// Point of origin
	ushort		destPoint;			// Point of destination

	ulong		ProdData;			// Product-specific data


	// --------------------------------------------------
	// Packed messages, terminated by two bytes of zeroes

	};




struct fido_pmsg_hdr
    {
    ushort          origNode;           // of msg
    ushort          destNode;           // of msg
    ushort          origNet;            // of msg
    ushort          destNet;            // of msg

    fmsg_hdr_attr   attr;               // Attributes
    ushort          cost;


// date is excluded from this structure 'cause it is a variable size 
// field, as it looks now.
//    uchar           date[20];           // Date/time of msg

	// 36 bytes max, null terminated to user name
	// 36 bytes max, null terminated from user name
	// 72 bytes max, null terminated subject
	// null terminated text

	};


#pragma pack()

#endif	// FIDO_PKT_H
