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
date	95.08.02.13.52.37;	author dz;	state Exp;
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
 *      Module  :	Users notification
 *
 *      $Log: E_Notify.c $
 *      Revision 1.2  1995/08/02 13:52:37  dz
 *      tail available (command string)
 *
 *  Revision 1.1  1995/07/01  21:19:55  dz
 *  Initial revision
 *
 *      
 *         Rev 1.3   07 Jun 1993 17:26:30   dz
 *      New message flags system
 *      
 *         Rev 1.2   28 Nov 1992 23:41:14   dz
 *      Output formatting fixed
 *      
 *         Rev 1.1   11 Nov 1992 02:37:42   dz
 *      New userlist management used
 *      
 *         Rev 1.0   04 Feb 1992 00:21:48   dz
 *      Initial revision.
 *
 *
\*/

#include	"gremote.h"
#include	"../../version.h"
#include	<userlist.h>

#include	"cmds.h"

#include	<time.h>


static bool
send_notify( const fido_user &fa, const char *ua, const char *ud )
    {
    FTN_Msg    fm;
    long       t;
    char       buf[300];

    time( &t );

    if( fm.new_msg( conf.netmail_dir.c_str() ) == Err )
            fatal( EC_UU2_Error, EI_Full, "Can't create message file" );

    fm.set_to( fa );
//  fm.set_from( conf.our_addr );

    fido_user    postm;
    if( postm_address( postm ) == Err )
        fatal( EC_Incorrect_Setup, EI_None, "Can't invent postmaster address!" );

    fm.set_from( postm );
    fm.set_date( t );
    fm.add_attr( FF_Private|FF_KillSent|FF_Local );
    fm.set_subj("UU2 Notification Message");

    fm.rewind();

    sprintf( buf,
            " Hi!\r\n\r\n"
            " I just want to remind you, that you are registered user"
            " of FIDO/UUCP gate (%s), and your uucp address is"
            " %s@@%s\r\n\r\n",
            (const char *)((String)ftn::match_aka( fa )).c_str(),
            ua, ud
            );

    fm.puts( buf );
    fm.puts(
            "To learn more about our gate, please, send letter to user "
            "'Gate' at my address, with the word 'Help' in the first "
            "line of text\r\n\r\n");

    fm.puts( "\r\n                                      Gate Master.");

    fm.detach();						// Send it.

    return Ok;
    }

//#pragma argsused

static bool
do_user( const fido_user_def &ud, void *arg )
	{
	FTN_Msg      &reply = *( (FTN_Msg*) arg );
	char          buf[400];

	sprintf( buf, "    %-40s %s@@%s",
			(const char *)((String)(ud.fido_address())).c_str(),
			(const char *)ud.uu_name().c_str(), (const char *)ud.uu_to_domain().c_str()
			);
	fprintf( stderr, "%s\n", buf );
	reply.puts( buf );

	send_notify( ud.fido_address(), ud.uu_name().c_str(), ud.uu_to_domain().c_str() );
	return Ok;
	}

//#pragma argsused

bool exec_notify( const string &tail, FTN_Msg &reply, FTN_Msg &orig )
    {
    reply.puts( "\r\nSending notification to:\r\n" );
    fprintf( stderr, "Notifying users:\n" );
    return fuser.foreach( do_user, &reply );
    }


@


1.2
log
@tail available (command string)
@
text
@d8 3
d41 4
a44 4
	{
	fido_msg	fm;
	long		t;
	char		buf[300];
d46 1
a46 1
	time( &t );
d48 2
a49 2
	if( fm.new_msg( conf.netmail_dir.c_str() ) == Err )
		fatal( EC_UU2_Error, EI_Full, "Can't create message file" );
d51 2
a52 2
	fm.set_to( fa );
//	fm.set_from( conf.our_addr );
d58 21
a78 21
	fm.set_from( postm );
	fm.set_date( t );
	fm.add_attr( FF_Private|FF_KillSent|FF_Local );
	fm.set_subj("UU2 Notification Message");

	fm.rewind();

	sprintf( buf,
		" Hi!\r\n\r\n"
		" I just want to remind you, that you are registered user"
		" of FIDO/UUCP gate (%s), and your uucp address is"
		" %s@@%s\r\n\r\n",
		(const char *)((String)ftn::match_aka( fa )).c_str(),
		ua, ud
		);

	fm.puts( buf );
	fm.puts(
		"To learn more about our gate, please, send letter to user "
		"'Gate' at my address, with the word 'Help' in the first "
		"line of text\r\n\r\n");
d80 1
a80 1
	fm.puts( "\r\n                                      Gate Master.");
d82 1
a82 2
	fm.flush();
	fm.detach();						// Send it.
d84 2
a85 2
	return Ok;
	}
d92 2
a93 2
	fido_msg		&reply = *( (fido_msg*) arg );
	char			buf[200];
d108 6
a113 6
bool
exec_notify( const string &tail, fido_msg &reply, fido_msg &orig ) {
	reply.puts( "\r\nSending notification to:\r\n" );
	fprintf( stderr, "Notifying users:\n" );
	return fuser.foreach( do_user, &reply );
	}
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/tools/remote/vcs/e_notify.c_v  $
d28 1
a28 1
#include	"version.h"
d33 2
d85 1
a85 1
#pragma argsused
d104 1
a104 1
#pragma argsused
d107 1
a107 1
exec_notify( fido_msg &reply, fido_msg &orig ) {
@
