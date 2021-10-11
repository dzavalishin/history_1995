head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	95.08.02.13.48.12;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.04.09.18.25.15;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.09.10.45.11;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.32.26;	author dz;	state Exp;
branches;
next	;


desc
@UU2 News Unbatcher
@


1.4
log
@new items
@
text
@/************************ UUCP to FIDO convertor ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	UUCP header parser
 *
 *      $Log: uuparse.h $
 * Revision 1.3  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.3  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.2  1995/04/09  10:45:11  dz
 * rewriting for C Set
 *
 * Revision 1.1  1995/03/11  18:32:26  dz
 * Initial revision
 *
 *      
 *         Rev 1.1   10 Mar 1993 15:44:58   dz
 *      Direct unbatching
 *      
 *         Rev 1.0.1.0   03 Mar 1993 20:39:38   dz
 *      Temp: trying to implement direct unbatching.
 *      
 *         Rev 1.0   17 May 1992 11:06:52   dz
 *      Internal packetizer, kiae-specific unbatcher
 *
 *
\*/

#ifndef	UUPARSE_H
#define	UUPARSE_H

#include	<time.h>
#include	<stdio.h>
#include	<style.h>
#include	<strng.h>


// The longest path I seen was 241 character

#define		HLSIZE		256
#define     PATHHLSIZE  400
#define		ETSIZE		40



struct uuinfo 
	{
	string		ftn_msgid;
	string		ftn_reply;
	string		ftn_to;

	char		from[HLSIZE];
	char		realname[HLSIZE];
	char		replyto[HLSIZE];
	char		subject[HLSIZE];
	char		date[HLSIZE];
	char		newsgroups[HLSIZE];
	char		organization[HLSIZE];
	char		message_id[HLSIZE];
	char		path[PATHHLSIZE];
	char		control[HLSIZE];

	char		x_batch[21];				// Used to unbatch KIAE-type news compression
	char        comment_to[HLSIZE];         // Used to form FIDO To field
                                            // in echo messages

	time_t		gm_time;					// Parsed "date:"

//	char		echo_tag[ETSIZE];

	long		textsize;
	bool		uuencoded;
//	bool		retain_groupname;
	};


/****************************************************************************
							Functions
****************************************************************************/

class VReadBase;

bool		uuparse( VReadBase & in, uuinfo *uu_info );


#endif //	UUPARSE_H
@


1.3
log
@IBM C Set version. Phew, it was not a piece of cake to get here...
@
text
@d8 6
d39 1
d50 6
a55 1
struct uuinfo {
@


1.2
log
@rewriting for C Set
@
text
@d8 3
a32 2

//#include	"vread.h"
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/unbatch/vcs/uuparse.h_v  $
d77 1
a77 1
#endif	UUPARSE_H
@
