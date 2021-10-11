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
@/************************ FIDO MSG file class ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:		Debugging
 *
 *      $Log: Msg_Deb.c $
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 *      
 *         Rev 1.2   07 Jun 1993 17:36:20   dz
 *      io.h --> uu2io.h
 *      
 *         Rev 1.1   06 Nov 1992 19:19:44   dz
 *      Ver 1.12a save
 *      
 *         Rev 1.0   28 Aug 1991 01:00:18   dz
 *      Initial revision.
 *
 *
\*/

#include	<style.h>
#include	<uu2io.h>
#include	<string.h>
#include	<time.h>

#include	"fido_msg.h"

#define	BLEN	300


/****************************************************************************
								Debug
****************************************************************************/



void
fido_msg::print_hdr( void ) {
	if( error )		return;

	printf("From:         %-36.36s\n", hdr.from );
	printf("To:           %-36.36s\n", hdr.to );
	printf("Subject:      %-72.72s\n", hdr.subject );
	printf("Date:         %-20.20s\n", hdr.date );
	printf("Times read:   %d\n", hdr.timesRead );
	printf("Dest node:    %d\n", hdr.destNode );
	printf("Orig node:    %d\n", hdr.origNode );
	printf("cost:         %d\n", hdr.cost );
	printf("Dest net:     %d\n", hdr.destNet );
	printf("Orig net:     %d\n", hdr.origNet );

	printf("Attr:         %s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\n", 
		hdr.attr.Private				? "Private "		: "",
		hdr.attr.Crash					? "Crash "			: "",
		hdr.attr.Recd 					? "Recvd "			: "",
		hdr.attr.Sent 					? "Sent "			: "",
		hdr.attr.FileAttached			? "FileAtt'ed "		: "",
		hdr.attr.InTransit				? "InTransit "		: "",
		hdr.attr.Orphan					? "Orphan "			: "",
		hdr.attr.KillSent				? "KillSent "		: "",
		hdr.attr.Local					? "Local "			: "",
		hdr.attr.HoldForPickup			? "HoldForPickup "	: "",
		hdr.attr.unused					? "unused??? "		: "",
		hdr.attr.FileRequest			? "FileRQ "			: "",
		hdr.attr.ReturnReceiptRequest	? "RetREceiptRQ "	: "",
		hdr.attr.IsReturnReceipt		? "IsRetReceipt "	: "",
		hdr.attr.AuditRequest			? "AuditRQ "		: "",
		hdr.attr.FileUpdateReq			? "FielUpdRQ "		: ""

		);

	printf("--------------------------------------------------------------------------\n\n");



	}




void
fido_msg::print_msg( void ) {
	char	buf[BLEN];

	if( error )		return;

	rewind();

	printf("\n");
	while( fgets( buf, BLEN, fp ) != NULL )
		printf( "%s", buf );

	}


@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/fido/vcs/msg_deb.c_v  $
@
