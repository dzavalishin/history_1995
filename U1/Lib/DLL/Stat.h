/************************ UUCP to FIDO convertor ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Statistics module header
 *
 *      $Log: Stat.h $
 *      Revision 1.3  1995/11/05 18:50:12  dz
 *      RFC_Msg used
 *
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *         Rev 1.2   07 Jun 1993 18:00:22   dz
 *      update
 *      
 *         Rev 1.1   04 Feb 1992 22:17:00   dz
 *      first_used field added
 *      
 *         Rev 1.0   12 Sep 1991 01:29:58   dz
 *      Initial revision.
 *
 *
\*/


#ifndef	STAT_H
#define	STAT_H


#ifndef	FIDO_MSG_H
#include "ftn_msg.h"
#endif

#pragma pack(1)


struct StatCount {
		long		Letters;				// How many letters passed
		long		Bytes;					// How many bytes passed
		long		Files;					// ... files

		long		fill[40];
		};

struct FidoUserEntry {
	fido_addr	Address;					// Address of user

	StatCount	ToFido;						// Passed in FIDO direction
	StatCount	ToUu;						// To uucp....

	long		last_used;					// Last time gate was used
	long		first_used;					// When first letter was accounted

	char		fill[(200-4)];
	};


void _Export	add_stat_fido(
            const fido_addr &who,
			const unsigned char *uucp_addr,
			bool in_ussr,
			bool file,
			bool to_fido,
			long bytes
			);

void _Export    add_stat_news();

extern long _Export	stat_mail_messages_processed;	// Updated in stat.c, add_stat_fido()
extern long _Export     stat_news_messages_processed;   // Updated in stat.c, add_stat_fido()

#pragma pack()

#endif

