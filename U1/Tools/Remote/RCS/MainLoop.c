head	1.4;
access;
symbols;
locks;
comment	@ *  @;


1.4
date	95.11.06.14.28.09;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.02.13.55.58;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.08.02.13.54.51;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.07.01.21.19.55;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
log
@Current
@
text
@/************************ Gate remote control ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :	Main loop
 *
 *      $Log: MainLoop.c $
 *      Revision 1.3  1995/08/02 13:55:58  dz
 *      cleaned up a bit
 *
 *      Revision 1.2  1995/08/02 13:54:51  dz
 *      rewritten with ffolder
 *      клево :)
 *
 *  Revision 1.1  1995/07/01  21:19:55  dz
 *  Initial revision
 *
 *
 *
\*/

#include	"gremote.h"
#include	"ffolder.h"



void GRemoteApp::
mainloop( void ) 
	{
	ffolder		folder( conf.netmail_dir );

	while( 1 )
		{
		FTN_Msg fm;

		switch( folder.next( fm ) )
			{
		case Err:
			error( EI_Full, "Can't get next message in %s", (const char *)conf.netmail_dir.c_str() );
		case No:
			break;

		case Yes:
					// Subject to convert ?
			if( !select( fm ) )
				continue;

			track_reset();               // Prepare to log tracking

			if( process( fm ) == Err )  // Convert letter
				error_letter( fm ); // Send letter back

			// Mark letter as sent & force deletion
			fm.add_attr( FF_Sent|FF_KillSent );

			fm.detach();						// Forget it.

			} // switch
		break;
		} // while


	}



@


1.3
log
@cleaned up a bit
@
text
@d8 3
d27 1
a27 1
void
d34 1
a34 1
		fido_msg	fm;
d45 1
a45 1
			if( !select( &fm ) )
d50 2
a51 2
			if( process( &fm ) == Err )  // Convert letter
				error_letter( &fm ); // Send letter back
a55 1
			fm.flush();
@


1.2
log
@rewritten with ffolder
клево :)
@
text
@d8 4
a21 4
//#include	<dir.h>
//#include	<dos.h>
//#include	<errno.h>

a26 8
//	char		mask[100];

//	sprintf( mask, "%s\\*.msg", (const char *)conf.netmail_dir.c_str() );
//	debug("Search mask: `%s'", mask );

//	string s( mask );

//	ffolder		folder( mask );
a44 2
//			debug("Processing '%s\\msg.%d'", (const char *)conf.netmail_dir.c_str(), mno );

a48 1

@


1.1
log
@Initial revision
@
text
@d7 4
a10 11
 *      $Log:   Q:/tools/remote/vcs/mainloop.c_v  $
 *      
 *         Rev 1.2   07 Jun 1993 17:22:38   dz
 *      OS/2 compartibility fixes
 *      Message flags system changed
 *      
 *         Rev 1.1   28 Nov 1992 23:37:42   dz
 *      String to const char *
 *      
 *         Rev 1.0   04 Feb 1992 00:18:28   dz
 *      Initial revision.
d16 1
d18 3
a20 4

#include	<dir.h>
#include	<dos.h>
#include	<errno.h>
a22 2
#define ATTRS2FIND  FA_RDONLY | FA_HIDDEN | FA_SYSTEM | FA_ARCH

d27 1
a27 5
	ffblk		ff;
	char		mask[100];

	sprintf( mask, "%s\\*.msg", (const char *)conf.netmail_dir.c_str() );
	debug("Search mask: `%s'", mask );
d29 2
a30 6
    if( findfirst( mask, &ff, ATTRS2FIND ) )
		{
		if( errno == ENOENT ) {
			log( "#", "Nothing to do - no messages found" );
			return;
			}
d32 1
a32 2
		fatal( EC_OS_Error, EI_Full, "Findfirst" );
		}
d34 2
d37 2
a38 1
	do {
a39 1
		int			mno;
d41 1
a41 3
		mno = atoi( ff.ff_name );

		if( fm.attach( conf.netmail_dir.c_str(), mno ) == Err )
d43 9
a51 3
			error( EI_Full, "Can't open message '%s'", ff.ff_name );
			continue;
			}
d53 1
a53 3
		if( select( &fm ) )						// Subject to convert ?
			{
			debug("Processing '%s\\msg.%d'", (const char *)conf.netmail_dir.c_str(), mno );
d55 1
a55 1
			track_reset();						// Prepare to log tracking
d57 2
a58 2
			if( process( &fm ) == Err )			// Convert letter
				error_letter( &fm );			// Send letter back
d67 3
a69 2
			} /* if select */
		} while( findnext(&ff) == 0 );
a70 2
	if( errno != ENOENT )
		fatal( EC_OS_Error, EI_Full, "Findfirst" );
@
