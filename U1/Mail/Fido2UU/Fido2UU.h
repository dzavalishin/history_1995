/************************ UUCP to FIDO convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :       Header
 *
 *      $Log: Fido2UU.h $
 *      Revision 1.4  1996/03/27 10:46:53  dz
 *      current
 *
 *      Revision 1.3  1995/11/06 01:42:16  dz
 *      Using FTN_Msg! Seems to be working.
 *
 *      Revision 1.2  1995/08/08 13:22:34  dz
 *      App class used
 *
 *      Revision 1.1  1995/03/11  18:24:01  dz
 *      Initial revision
 *
 *      Revision 1.1  1995/03/11  18:24:01  dz
 *      Initial revision
 *
 *      
 *         Rev 1.1   11 Oct 1991 05:28:22   dz
 *      Up to date.
 *      
 *         Rev 1.0   27 Aug 1991 02:47:20   dz
 *      Initial revision.
 *
 *
\*/

#include    <app.h>
#include    <ftn_msg.h>
#include    <rfc_msg.h>
#include    <uucpaddr.h>

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>

#include    "setup.h"
#include    "log.h"
#include    "cc_bag.h"



class Fido2uuApp : public U1App
	{
	bool	process( void );


public:
	int	Run();

protected:
	bool		f_silent;		// Don't chat
	bool		f_keeporig;		// Keep original message


//              CONVERT.C
	void	convert( FTN_Msg &, bool &report, bool &err );

//				SELECT.C
	bool	select( FTN_Msg & );

//				MAINLOOP.C
	void	mainloop( void );
	void	do_one( int msg_no, bool &processed );

//				COPY.C
//	void	copy_data( FILE *fp, fido_msg *fm, char *uu_to );

//				ADDRESS.C
	bool	get_address( char *uu_to, FTN_Msg &fm );
	bool	get_from( bool *reg_user, uucp_addr &uu_from, const fido_user &ffm );
	bool	fidoaddr2uuaddr( uucp_addr &uu_fm, const char *f_fm );

//				SEND_ERR.C
	void	error_letter( FTN_Msg &orig );
        void   	put_track( FTN_Msg &fm );

//				SEND_ACK.C
	void	acknovledge_letter( FTN_Msg &orig );

//                              RUNSMAIL.C
	bool    run_uusmail( const uucp_addr &from, const string &to, CC_Bag &cc, const RFC_Msg &msg );

//                              GATECTL.C
	bool    is_gate_control( string s );

	};


