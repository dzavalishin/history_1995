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
date	95.08.02.13.51.13;	author dz;	state Exp;
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
 *      Module  :	Broadcast
 *
 *      $Log: E_Broad.c $
 *      Revision 1.2  1995/08/02 13:51:13  dz
 *      tail available (command string)
 *
 *  Revision 1.1  1995/07/01  21:19:55  dz
 *  Initial revision
 *
 *      
 *         Rev 1.3   07 Jun 1993 17:26:46   dz
 *      New message flags system
 *      
 *         Rev 1.2   28 Nov 1992 23:42:12   dz
 *      Output formatting fixed
 *      
 *         Rev 1.1   11 Nov 1992 02:51:48   dz
 *      New userlist access scheme
 *      
 *         Rev 1.0   04 Feb 1992 00:22:08   dz
 *      Initial revision.
 *
 *
\*/

#include	"gremote.h"
#include	"..\..\version.h"
#include	<userlist.h>

#include	<time.h>

#include	"cmds.h"

static const char tempfn[] = "broad.$$$";

static const char *bar =
		"\r\n\r\n"
		"                     Broadcast message from GateKeeper!\r\n"
		"              같같같같같같같같같같같같같같같같같같같같같같같\r\n"
		"\r\n\r\n";

struct p2
    {       
    FILE      *f;
    FTN_Msg   *m;
    };



static bool
send_broad( fido_user &fa, FILE *tf )
    {
    FTN_Msg    fm;
    long       t;
    string     buf;
    
    time( &t );
    
    if( fm.new_msg( conf.netmail_dir.c_str() ) == Err )
        {
        error( EI_Full, "Can't create message file" );
        return Err;
        }
    
    fm.set_to( fa );
    //	fm.set_from( conf.our_addr );
    
    fido_user    postm;
    
    if( postm_address( postm ) == Err )
        fatal( EC_Incorrect_Setup, EI_None, "Can't invent postmaster address" );
    
    fm.set_from( postm );
    fm.set_date( t );
    fm.add_attr( FF_Private|FF_KillSent|FF_Local );
    fm.set_subj("UU2 Broadcast Message");
    
    fm.rewind();
    
    rewind( tf );
    
    while( buf.load( tf ) == Ok )
        fm.puts( buf );

    fm.detach();						// Send it.
    
    return Ok;
    }

static bool
do_user( const fido_user_def &ud, void *arg )
	{
	p2       &p = *( (p2*) arg );
	char	 buf[200];

	sprintf( buf, "    %-40s %s@@%s",
			(const char *)((String)(ud.fido_address())).c_str(),
			(const char *)ud.uu_name().c_str(), (const char *)ud.uu_to_domain().c_str()
			);
	fprintf( stderr, "%s\n", buf );
	p.m->puts( buf );

	fido_user fu = ud.fido_address();
	if( send_broad( fu, p.f ) == Err )
		{
		strcpy( buf, "        Error sending message!" );
		p.m->puts( buf );
		fprintf( stderr, "%s\n", buf );
		}

	return Ok;
	}


bool
do_broad( FTN_Msg &reply, FILE *tf )
	{
	p2		p;

	p.f = tf;
	p.m = &reply;

	reply.puts( "\r\nSending broadcast msg to:\r\n" );
	fprintf( stderr, "Sending broadcast msg to:\n" );

	return fuser.foreach( do_user, &p );
/*
	for( int i = 0; i < MAXUSER; i++ )
		{
		fido_addr	&fa = user[i].fido_address;

		if( strlen( user[i].uu_name ) == 0 )
			continue;

		sprintf( buf, "    %-36.36s %d:%d/%d.%d%s%s %s@@%s",
			fa.name(),
			fa.zone,						fa.net,
			fa.node,						fa.point,
			fa.has_domain() ? "@@" : "",		fa.domain(),
			user[i].uu_name, 				user[i].uu_domain
			);

		reply.puts( buf );

		if( send_broad( fa, tf ) == Err )
			reply.puts("        Error sending message!");
		}

	return Ok;
*/
	}









bool
exec_broadcast( const string &tail, FTN_Msg &reply, FTN_Msg &orig )
	{
	string    buf;
	FILE     *tf;

	tf = fopen( tempfn, "wt+" );
	if( tf == NULL )
		{
		reply.puts( "Can't create temp file - aborted" );
		return Ok;
		}

	fputs( bar, tf );

	while( orig.gets( buf ) != Err )
		{
		const char  *p = buf;

		while( *p <= ' ' )
			p++;

		if( p[0] == '-' && p[1] == '-' && p[2] == '-' )
			break;

		fputs( buf, tf );
		fputs( "\n", tf );
		}

	while( orig.gets( buf ) != Err )		// Skip the rest of text
		;

	do_broad( reply, tf );
	fclose( tf );
	chmod( tempfn, 0666 );
	unlink( tempfn );

	return Ok;
	}





@


1.2
log
@tail available (command string)
@
text
@d8 3
d47 4
a50 4
	{
	FILE		*f;
	fido_msg	*m;
	};
d56 16
a71 16
	{
	fido_msg	fm;
	long		t;
	char		buf[300];

	time( &t );

	if( fm.new_msg( conf.netmail_dir.c_str() ) == Err )
		{
		error( EI_Full, "Can't create message file" );
		return Err;
		}

	fm.set_to( fa );
//	fm.set_from( conf.our_addr );

d73 1
a73 1

d76 17
a92 19

	fm.set_from( postm );
	fm.set_date( t );
	fm.add_attr( FF_Private|FF_KillSent|FF_Local );
	fm.set_subj("UU2 Broadcast Message");

	fm.rewind();

	rewind( tf );

	while( fgets( buf, 300, tf ) != NULL )
		fm.puts( buf );


	fm.flush();
	fm.detach();						// Send it.

	return Ok;
	}
d97 2
a98 2
	p2				&p = *( (p2*) arg );
	char			buf[200];
d120 1
a120 1
do_broad( fido_msg &reply, FILE *tf )
d166 1
a166 1
exec_broadcast( const string &tail, fido_msg &reply, fido_msg &orig )
d168 2
a169 2
	char				buf[300];
	FILE				*tf;
a177 1

d182 1
a182 1
		char	*p = buf;
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/tools/remote/vcs/e_broad.c_v  $
d28 1
a28 1
#include	"version.h"
d31 2
d106 2
a107 1
	if( send_broad( ud.fido_address(), p.f ) == Err )
d165 1
a165 1
exec_broadcast( fido_msg &reply, fido_msg &orig )
@
