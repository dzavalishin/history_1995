/************************ Gate remote control ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :	Main loop
 *
 *      $Log: MainLoop.c $
 *      Revision 1.4  1995/11/06 14:28:09  dz
 *      Current
 *
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



