/************************ FIDO to UUCP convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :    Main conversion loop
 *
 *      $Log: MainLoop.c $
 *      Revision 1.7  1996/03/09 11:28:29  dz
 *      Bug fix: process all the messages, not just first one
 *
 *      Revision 1.6  1995/11/06 02:20:45  dz
 *      Using ffolder class
 *
 *      Revision 1.5  1995/11/06 01:42:16  dz
 *      Using FTN_Msg! Seems to be working.
 *
 *      Revision 1.4  1995/08/08 13:22:34  dz
 *      App class used
 *
 *      Revision 1.3  1995/08/02 13:14:18  dz
 *      'did_something_this_round' kludge around errors in finds
 *
 *      Revision 1.2  1995/04/12  21:20:45  dz
 *      IBM C Set version seems to be working...
 *
 *      Revision 1.1  1995/03/11  18:24:01  dz
 *      Initial revision
 *
 *      
 *         Rev 1.8   07 Jun 1993 16:37:32   dz
 *      Rearranged a lot
 *      
 *         Rev 1.7   28 Nov 1992 23:08:04   dz
 *      String to const char *, style fixes
 *      
 *         Rev 1.6   23 Oct 1992 14:33:14   dz
 *      reorganization
 *      
 *         Rev 1.5   29 Jun 1992 15:38:10   dz
 *      Don't delete messages with 'Local' flag but withno
 *      'KillSent' one
 *      
 *         Rev 1.4   18 Jun 1992 10:32:02   dz
 *      Cosmetic changes
 *      
 *         Rev 1.3   04 Feb 1992 00:08:32   dz
 *      Count number of files sent. Incorrectly implemented -
 *      will only count one file-attach of all listed in the
 *      same message.
 *      
 *         Rev 1.2   11 Oct 1991 05:24:36   dz
 *      Error letter generation, return receipt mode,
 *      statistics update.
 *
 *         Rev 1.1   29 Aug 1991 06:40:34   dz
 *      MSG handling rewritten
 *
 *         Rev 1.0   27 Aug 1991 02:47:18   dz
 *      Initial revision.
 *
 *
\*/

#include    "fido2uu.h"
#include    <stdlib.h>
#include    <errno.h>
#include    <ffolder.h>
#include    <rexx.h>




void Fido2uuApp::
mainloop( void ) 
    {
    bool        processed = No;
    ffolder     folder( conf.netmail_dir );
    
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
                
                fmsg_attr    at;
                
//                debug("Processing '%s\\msg.%d'", (const char *)conf.netmail_dir.c_str(), msg_no );
                
                track_reset();                        // Prepare to log tracking
                
                fm.get_attr( at );
                
                bool    c_report = No, c_err = No;
                
                convert( fm, c_report, c_err );        // Convert letter
                
                if( c_report || c_err )
                    error_letter( fm );            // Send letter back
                
                if( !c_err )                        // Not an error, delete original
                    {
                    if( at & (FF_ReturnReceiptRequest|FF_AuditRequest))    // Confirmation Request
                        acknovledge_letter( fm );    // Send confirmation
                    
                    processed = Yes;
                    }
                
                
                at |= FF_Sent;                        // Mark letter as sent
                if( !(at&FF_Local) )                   // Foreighn letter?
                    at |= FF_KillSent;                // Force deletion
                fm.set_attr( at );
                
                fm.detach();                        // Forget it.
                
                continue;
            } // switch
        break;
        } // while
    
    if( processed && conf.kick_uucp_flag.length() != 0 )
        {
        FILE    *flag;
        
        if( ( flag = fopen( conf.kick_uucp_flag.c_str(), "w" ) ) == NULL )
            error( EI_Full, "Can't create flag file '%s'", 
                  (const char *)conf.kick_uucp_flag.c_str() );
        else
            fclose( flag );
        }
    
    if( processed && conf.rexx_kick_uucp.length())
        {
        string dummy;
        CallRexx( conf.rexx_kick_uucp.c_str(), dummy, NULL, NULL );
        }

    }






