/************************ Gate remote control ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :	Separator
 *
 *      $Log: Select.c $
 *      Revision 1.3  1995/11/06 14:28:09  dz
 *      Current
 *
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














