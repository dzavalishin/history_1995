/************************ UU2 Snews ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :	Scan for messages with echo packets attached
 *
 *      $Log: Scan.c $
 *      Revision 1.7  1995/11/06 13:01:46  dz
 *      Using FTN_Msg
 *
 *      Revision 1.6  1995/08/07 13:57:10  dz
 *      Cleanup
 *
 *      Revision 1.5  1995/08/07 13:52:20  dz
 *      Using ffolder class
 *
 *      Revision 1.4  1995/08/07 13:34:55  dz
 *      checkpoint
 *
 *      Revision 1.3  1995/08/07 13:28:26  dz
 *      Using class App
 *
 *      Revision 1.2  1995/04/09  23:24:27  dz
 *      IBM C Set version
 *
 *      Revision 1.1  1995/03/11  18:31:14  dz
 *      Initial revision
 *
 *      
 *         Rev 1.3   07 Jun 1993 17:10:42   dz
 *      Port to OS/2 fixes, new message flags system, other fixes.
 *      
 *         Rev 1.2   28 Nov 1992 22:22:40   dz
 *      style improvements, fixes...
 *      
 *         Rev 1.1   18 Jun 1992 11:08:58   dz
 *      update
 *      
 *         Rev 1.0   06 Oct 1991 00:17:56   dz
 *      Initial revision.
 *
 *
\*/

#include	"snews.h"
#include	"..\..\version.h"
#include	<ffolder.h>


static bool		select( FTN_Msg &fm );


void
SnewsApp::scan( void )
    {
    ffolder		folder( conf.netmail_dir );
    bool		err = No;
    bool		trunc_file = No;
    
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
                if( !select( fm ) )       // Subject to convert ?
                    continue;
                
                string     name;
                
                track_reset();						// Reset track buffer

                if( !(fm.attr()&FF_FileAttached) )
                    {
                    error( EI_None, "No file attached to message!");
                    err = Yes;
                    trunc_file = No;
                    }
                else
                    {
                    fm.get_subj( name );
                    log( "e", "Processing packet '%s'", (const char *)name );
                    trunc_file = Yes;
                    }
                
                if( !err )
                    unpack( name );
                
                if( trunc_file )
                    {
                    chmod( name, 0666 );
                    unlink( name );					// Don't truncate - kill
                    }
                
                fm.add_attr( FF_Sent );				// Mark letter as sent
                fm.add_attr( FF_KillSent );			// Force deletion
                
                fm.detach();						// Forget it.

                
            } // switch
        break;
        } // while

    }







bool
select( FTN_Msg &fm )
    {
    fido_user	to;
    
    fm.get_to( to );
    
    // echo_from_addr - address WE are sending echo packets from,
    // i.e our (echo-gate's) address
    
    if( ((fido_addr)to) == conf.echo_from_addr ) return Yes;
    
    return No;
    }


