/************************ FIDO to UUCP convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :	Send reception acknovledgement back
 *
 *      $Log: Send_Ack.c $
 *      Revision 1.5  1995/11/06 02:20:45  dz
 *      Cleanup
 *
 *      Revision 1.4  1995/11/06 01:42:16  dz
 *      Using FTN_Msg! Seems to be working.
 *
 *      Revision 1.3  1995/08/08 13:22:34  dz
 *      App class used
 *
 *      Revision 1.2  1995/04/12  21:20:45  dz
 *      IBM C Set version seems to be working...
 *
 *      Revision 1.1  1995/03/11  18:24:01  dz
 *      Initial revision
 *
 *      
 *         Rev 1.7   07 Jun 1993 16:40:52   dz
 *      Message flags system redone
 *      
 *         Rev 1.6   28 Nov 1992 23:11:08   dz
 *      Style fixes
 *      
 *         Rev 1.5   23 Oct 1992 14:39:12   dz
 *      error()
 *      
 *         Rev 1.4   18 Sep 1992 02:47:34   dz
 *      exit codes changed
 *      
 *         Rev 1.3   29 Jun 1992 15:48:20   dz
 *      Text corrected.
 *      
 *         Rev 1.2   18 Jun 1992 10:37:00   dz
 *      Includes rearranged
 *      
 *         Rev 1.1   04 Feb 1992 00:15:40   dz
 *      fido_msg::set_date( time_t ) used
 *      
 *         Rev 1.0   11 Oct 1991 05:26:00   dz
 *      Initial revision.
 *
 *
\*/


#include	"fido2uu.h"
#include	<time.h>


void
Fido2uuApp::acknovledge_letter( FTN_Msg &orig )
    {
    FTN_Msg         fm;
    fido_user	to;
    long		t;
    
    time( &t );
    
    if( fm.new_msg( conf.netmail_dir.c_str() ) == Err )
        fatal( EC_UU2_Error, EI_None, "Can't create message file" );
    
    orig.get_from( to );
    
    fm.set_to( to );
    fm.set_from( fido_user( ftn::match_aka( to ), "SysOp" ) );
    fm.set_date( t );
    fm.add_attr( FF_Private|FF_KillSent|FF_Local|FF_IsReturnReceipt );
    fm.set_subj("UU2 Reception Ack");
    
    fm.rewind();
    
    fm.puts(
            ">>> This letter confirms reception of your message\r\n"
            ">>> to UUCP by UU2 gate.  Log track follows:\r\n"
           );
    
    fm.puts( "\r\n");
    
    put_track( fm );					// Fill it with track buffer
    
    fm.puts( "\r\n");
    
    fm.detach();						// Send it.
    
    }



