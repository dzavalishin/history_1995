head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	95.08.07.14.55.01;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.09.18.25.15;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.32.26;	author dz;	state Exp;
branches;
next	;


desc
@UU2 News Unbatcher
@


1.3
log
@Using class App
@
text
@/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1994 by Infinity Soft
 *
 *	Module 	:	FTN From address generator
 *
 *      $Log: FTNFrom.c $
 * Revision 1.2  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.2  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 *
 * Revision 1.1  1995/03/11  18:32:26  dz
 * Initial revision
 *
 *
 *
\*/


#include "unbatch.h"
#include <uucpaddr.h>


fido_user
UnbatchApp::FTN_From( const uuinfo &ui, bool &fsc35_mode )
	{
	uucp_addr	uu_from;
	fido_user	fu;
	const		dlen = 37;
	char		dummy[dlen];

	if( strlen( ui.replyto ))
		uu_from = ui.replyto;
	else
		uu_from = ui.from;

	if( uu_from.convert_to_fido( fu, dummy, dlen ) == Yes )
		{
		fsc35_mode = No;
		return fu;
		}

	fsc35_mode = Yes;

	if( strlen((const char *)uu_from) >= conf.from_field_size )
		sprintf( dummy, "%.35s", (const char *)conf.magic.c_str() );
	else
		sprintf( dummy, "%.35s", (const char *)uu_from );

	((fido_addr&)fu) = conf.echo_from_addr;

	fu.name( dummy );

	return fu;
	}


@


1.2
log
@IBM C Set version. Phew, it was not a piece of cake to get here...
@
text
@d8 6
d28 1
a28 1
FTN_From( const uuinfo &ui, bool &fsc35_mode )
@


1.1
log
@Initial revision
@
text
@d7 1
a7 1
 *      $Log:$
d9 2
d12 2
d48 1
@
