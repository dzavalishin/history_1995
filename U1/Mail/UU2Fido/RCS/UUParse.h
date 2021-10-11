head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	95.11.04.01.21.46;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.11.02.14.25.20;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.08.02.13.13.05;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.07.00.20.29;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
log
@First RFC_Msg-based version, that compiles and does something
@
text
@/************************ UUCP to FIDO convertor ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	UUCP header parser
 *
 *      $Log: UUParse.h $
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

@


1.3
log
@Before switching to rfc_msg
@
text
@d8 3
d27 2
d50 1
a50 1
	char		x_batch[21];				// Used to unbatch KIAE-type news compression
d56 1
a56 1
	long		textsize;
d68 1
a68 1
bool		uuparse( uuinfo *uu_info );
d72 3
@


1.2
log
@RRq support
@
text
@d8 5
a12 2
 * Revision 1.1  1995/03/07  00:20:29  dz
 * Initial revision
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/uu2fido/vcs/uuparse.h_v  $
d50 2
d63 1
a63 1
#endif	UUPARSE_H
@
