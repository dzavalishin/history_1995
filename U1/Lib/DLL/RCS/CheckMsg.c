head	1.2;
access;
symbols;
locks;
comment	@ * @;


1.2
date	95.04.13.11.06.28;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.00.54;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@debug
@
text
@/************************ FIDO MSG library test ***************************\
 *
 *      Copyright (C) 1993 by Infinity Soft
 *
 *      Module  :	Test
 *
 *      $Log: CheckMsg.c $
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 *
 *
\*/

#include	<time.h>

#include	<fido_msg.h>
#include	<setup.h>
#include	<log.h>


char program_name[] = "UU2 MSG Library test";

int
main( void )
	{
	fido_msg	fm;
	fmsg_attr	at;
	fido_addr	to;
	long		t;
//	char		buf[500];

	if( get_setup() == Err )
		fatal( EC_IO_Error, EI_None, "Can't get setup!" );

	time( &t );

	to.domain( "fidonet.org" );
	to.name( "UU2 Author" );
	to.zone = 2;
	to.net = 5020;
	to.node = 23;
	to.point = 77;

	if( fm.new_msg( conf.netmail_dir ) == Err )
		fatal( EC_UU2_Error, EI_None, "Can't create message file" );

	fm.get_attr( at );
	at.Private	= (uint)Yes;
	at.KillSent	= (uint)Yes;
	at.Local	= (uint)Yes;
	fm.set_attr( at );

	fm.set_from( conf.our_addr );
	fm.set_to( to );

	fm.set_date( t );

	fm.set_subj("UU2 MSG Library test");

	fm.rewind();

	fm.puts( ">>> Ok, ok, i have to put something into");
	fm.puts( ">>> the message, cause I just need to check");
	fm.puts( ">>> this stuff...");

	fm.flush();
	fm.detach();						// Send it.

	return 0;
	}



@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/fido2uu/vcs/send_err.c_v  $
@
