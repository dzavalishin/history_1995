/************************ UUCP to FIDO convertor ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	UUCP header parser
 *
 *      $Log: UUParse.h $
 *      Revision 1.4  1995/11/04 01:21:46  dz
 *      First RFC_Msg-based version, that compiles and does something
 *
 *      Revision 1.3  1995/11/02 14:25:20  dz
 *      Before switching to rfc_msg
 *
 *      Revision 1.2  1995/08/02 13:13:05  dz
 *      RRq support
 *
 *      Revision 1.1  1995/03/07  00:20:29  dz
 *      Initial revision
 *
 *      
 *         Rev 1.1   12 Apr 1993 12:08:52   dz
 *      stdin replaced with in - support for uupc 5.06
 *
 *         Rev 1.0   11 Oct 1991 05:40:04   dz
 *      Initial revision.
 *
 *
\*/

# if 0

#ifndef	UUPARSE_H
#define	UUPARSE_H

#include	<time.h>


#define		HLSIZE		400
#define		ETSIZE		40



struct uuinfo {
	char		from[HLSIZE];
	char		realname[HLSIZE];
	char		replyto[HLSIZE];
	char		subject[HLSIZE];
	char		date[HLSIZE];
	char		newsgroups[HLSIZE];
	char		organization[HLSIZE];
	char		path[HLSIZE];

//	char		x_batch[21];				// Used to unbatch KIAE-type news compression

	time_t      gm_time;                    // Parsed "date:"

	char		echo_tag[ETSIZE];

//	long		textsize;
	bool		uuencoded;

	bool		has_return_receipt_to;
	};


/****************************************************************************
							Functions
****************************************************************************/


bool		uuparse( const RFC_Msg &msg, uuinfo *uu_info );


#endif //	UUPARSE_H

#endif

