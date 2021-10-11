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
date	95.03.11.18.04.17;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@debug
@
text
@/************************ UU2 FILE2UU ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Convert one part
 *
 *      $Log: Enc_Part.c $
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   07 Jun 1993 17:47:00   dz
 *      Initial revision.
 *
 *
\*/

#include    "file2uu.h"
#include	<datec.h>


bool
encode_part
		(
		int 					part_no,		// 1-nnn
		int						n_parts,		// nnn
		int						p_bytes,		// bytes per part
		echo_def 			&	def,			// Echo definition
		FILE				*	fo,				// Put here
		FILE				*	fi				// From here
		)
	{

	fprintf( fo, "Newsgroups: %s\n", (const char *)def.group() );
	fprintf( fo, "Sender: postmaster@@%s\n", (const char *)conf.domain );
	fprintf( fo, "X-Gate: U1 " U1_VER_STR "\n" );
	fprintf( fo, "Date: %s\n", timeline( tt, conf.tz ) );






























@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/uu2/vcs/enc_part.c_v  $
@
