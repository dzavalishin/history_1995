head	1.3;
access;
symbols;
locks;
comment	@ *  @;


1.3
date	95.11.06.14.28.09;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.08.02.14.00.10;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.07.01.21.19.55;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@Current
@
text
@/************************ Gate remote control ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :	Separator
 *
 *      $Log: Select.c $
 *      Revision 1.2  1995/08/02 14:00:10  dz
 *      cosmetical
 *
 *  Revision 1.1  1995/07/01  21:19:55  dz
 *  Initial revision
 *
 *      
 *         Rev 1.3   07 Jun 1993 17:24:08   dz
 *      Message flags system changed
 *      
 *         Rev 1.2   28 Nov 1992 23:38:14   dz
 *      up to date
 *
 *         Rev 1.1   04 Feb 1992 00:18:48   dz
 *      Headers rearranged
 *
 *         Rev 1.0   29 Oct 1991 10:37:36   dz
 *      Initial revision.
 *
 *
\*/


#include	"gremote.h"
#include	<ctype.h>



bool GRemoteApp::
select( FTN_Msg &fm )
	{
	fido_user	to;

	if( (fm.attr()) & (FF_Sent|FF_Lock) )
		return No;

	fm.get_to( to );

	if( !conf.to_any_addr )					// Take dest. address in account
		{
		ftn_def def;

		if( Ok != ftn::find( def, to ) )
			return No;
		}

	if( stricmp( to.name(), "gate" ) == 0 )
		{
		log( "a", "-----> to '%s', processing",	to.name() );
		return Yes;
		}

	return No;
	}














@


1.2
log
@cosmetical
@
text
@d8 3
d36 2
a37 2
bool
select( fido_msg *fm )
d41 1
a41 1
	if( (fm->attr()) & (FF_Sent|FF_Lock) )
d44 1
a44 1
	fm->get_to( to );
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/tools/remote/vcs/select.c_v  $
d35 1
a35 1
    {
d45 1
a45 1
        ftn_def def;
d47 3
a49 3
        if( Ok != ftn::find( def, to ) )
            return No;
        }
d52 1
a52 1
        {
d54 2
a55 2
        return Yes;
        }
@
