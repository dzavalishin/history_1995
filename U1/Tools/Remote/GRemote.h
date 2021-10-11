/************************ Gate remote control ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :       Header
 *
 *      $Log: GRemote.h $
 *      Revision 1.3  1995/11/06 14:28:09  dz
 *      Current
 *
 *      Revision 1.2  1995/08/02 14:03:28  dz
 *      rexx subcom added
 *
 *      Revision 1.1  1995/07/01  21:19:55  dz
 *      Initial revision (C Set)
 *      
 *         Rev 1.0   11 Nov 1992 01:52:10   dz
 *      Initial revision.
 *
 *
\*/


#ifndef	GREMOTE
#define	GREMOTE



#include	<app.h>
#include	<ftn_msg.h>
#include	<uucpaddr.h>

#include	<stdio.h>
#include	<style.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<setup.h>
#include	<log.h>


class GRemoteApp : public U1App
    {
    bool        Register_Gr_Rexx_Subcommand_Handler( void ); // REXXGR.C

public:
    int	        Run();
                GRemoteApp() {Register_Gr_Rexx_Subcommand_Handler();}
    
protected:

    bool        process( FTN_Msg & );                       // PROCESS.C
    bool        select( FTN_Msg & );                        // SELECT.C
    void        mainloop( void );                            // MAINLOOP.C
    void        error_letter( FTN_Msg &orig );              // SEND_ERR.C

                                                             // ADDRESS.C
    bool        get_from( uucp_addr &uu_fm, const fido_user &ffm );
    };

    bool        postm_address( fido_user &u );

#endif	// GREMOTE

