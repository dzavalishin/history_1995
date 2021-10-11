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
date	95.08.02.14.03.28;	author dz;	state Exp;
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
 *      Module  :       Header
 *
 *      $Log: GRemote.h $
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

@


1.2
log
@rexx subcom added
@
text
@d8 2
a9 2
 *  Revision 1.1  1995/07/01  21:19:55  dz
 *  Initial revision
d11 2
d26 2
a27 4
#ifndef FIDO_MSG_H
#include	<fido_msg.h>
#endif

a29 1

d35 2
a36 27
#include	"setup.h"
#include	"log.h"



/****************************************************************************
						Function prototypes
****************************************************************************/


//              PROCESS.C

bool			process( fido_msg * );


//				SELECT.C

bool			select( fido_msg * );

//				MAINLOOP.C

void			mainloop( void );

//				ADDRESS.C

bool			get_from( uucp_addr &uu_fm, const fido_user &ffm );
bool            postm_address( fido_user &u );
a37 1
//				SEND_ERR.C
d39 3
a41 1
void			error_letter( fido_msg *orig );
d43 5
a47 1
//				SEND_LET.C
d49 4
a52 1
//void			send_letter( fido_msg *orig );
d54 3
a56 1
// rexxgr.c
d58 1
a58 1
bool			Register_Gr_Rexx_Subcommand_Handler( void );
d60 1
a61 1
#endif	// GREMOTE@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/tools/remote/vcs/gremote.h_v  $
d71 4
@
