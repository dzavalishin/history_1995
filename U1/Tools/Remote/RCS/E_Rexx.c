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
date	95.08.02.13.52.55;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.07.02.11.10.40;	author dz;	state Exp;
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
 *      $Log: E_Rexx.c $
 *      Revision 1.2  1995/08/02 13:52:55  dz
 *      Working version
 *
 *  Revision 1.1  1995/07/02  11:10:40  dz
 *  Initial revision
 *
 *
 *
\*/

#include	"gremote.h"
#include	"..\..\version.h"

#include	<time.h>

#include	"cmds.h"

#define     INCL_RXSHV
//#define     INCL_RXSUBCOM
#include    <rexxsaa.h>

#include    <rexx.h>


static FTN_Msg *out_msg, *in_msg;


bool
exec_rexx( const string &verb, const string &tail, FTN_Msg &reply, FTN_Msg &orig )
	{
	string	Rexx_Out;

	if( conf.rexx_gremote_cmd.length() == 0 )
		return No;

	out_msg = &reply;
	in_msg = &orig;

	if( Ok == CallRexx( conf.rexx_gremote_cmd.c_str(), Rexx_Out, verb.c_str(), tail.c_str() ) )
		{
		out_msg = 0;
		in_msg = 0;
		return Yes;
		}

	out_msg = 0;
	in_msg = 0;

	return Err;
	}




int
sc_GetLetterLine( string s )
	{
	SHVBLOCK block;                  // block to pass to REXX

	log( "x#", "Rexx GetLetterLine %s", s.c_str() );

	if( in_msg == 0 )
		{
		error( EI_None, "Rexx tried to execute subcommand 'GetLetterLine' out of letter context" );
		return 1;
		}

	string buf;

	if( in_msg->gets( buf ) == Err )
		return 0;

	block.shvnext = (PSHVBLOCK) NULL;
        
        char copy[1024];
        s.ucase();
        strncpy( copy, s, 1024 );
	block.shvname.strptr = copy;
	block.shvname.strlength = s.length();

	MAKERXSTRING(block.shvvalue, (const char *)buf, buf.length());
	block.shvnamelen = block.shvname.strlength;
	block.shvvaluelen = block.shvvalue.strlength;
	block.shvcode = RXSHV_SET;

	return RexxVariablePool(&block) & 0xFFFFFFFE;
	}


int
sc_PutLetterLine( string &s )
	{
	log( "x#", "Rexx PutLetterLine %s", s.c_str() );

	if( out_msg == 0 )
		{
		error( EI_None, "Rexx tried to execute subcommand 'PutLetterLine' out of letter context" );
		return 1;
		}

	out_msg->puts( s.c_str() );

	return 0;
	}


#if 0

static const char tempfn[] = "broad.$$$";

static const char *bar =
		"\r\n\r\n"
		"                     Broadcast message from GateKeeper!\r\n"
		"              같같같같같같같같같같같같같같같같같같같같같같같\r\n"
		"\r\n\r\n";

struct p2
	{
	FILE		*f;
	fido_msg	*m;
	};



static bool
send_broad( fido_user &fa, FILE *tf )
    {
    FTN_Msg      fm;
    long	     t;
    string       buf;
    
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
    
    
    fm.flush();
    fm.detach();						// Send it.
    
    return Ok;
    }

static bool
do_user( const fido_user_def &ud, void *arg )
	{
	p2				&p = *( (p2*) arg );
	char			buf[200];

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
do_broad( fido_msg &reply, FILE *tf )
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
exec_broadcast( FTN_Msg &reply, FTN_Msg &orig )
    {
    string    buf;
    FILE	 *tf;
    
    tf = fopen( tempfn, "wt+" );
    if( tf == NULL )
        {
        reply.puts( "Can't create temp file - aborted" );
        return Ok;
        }
    
    
    fputs( bar, tf );
    
    while( orig.gets( buf ) != Err )
        {
        char	*p = buf;
        
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


#endif


@


1.2
log
@Working version
@
text
@d8 3
d32 1
a32 1
static fido_msg *out_msg, *in_msg;
d36 1
a36 1
exec_rexx( const string &verb, const string &tail, fido_msg &reply, fido_msg &orig )
d63 1
a63 1
sc_GetLetterLine( string &s )
d75 1
a75 1
	char buf[10240];
d81 5
a85 5

	char temp[1024];
	strncpy( temp, s.c_str(), 1024 );
	strupr( temp );
	block.shvname.strptr = temp;
d88 1
a88 1
	MAKERXSTRING(block.shvvalue, buf, strlen(buf));
d134 16
a149 16
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

d151 1
a151 1

d154 19
a172 19

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
d246 39
a284 39
exec_broadcast( fido_msg &reply, fido_msg &orig )
	{
	char				buf[300];
	FILE				*tf;

	tf = fopen( tempfn, "wt+" );
	if( tf == NULL )
		{
		reply.puts( "Can't create temp file - aborted" );
		return Ok;
		}


	fputs( bar, tf );

	while( orig.gets( buf ) != Err )
		{
		char	*p = buf;

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


1.1
log
@Initial revision
@
text
@d7 2
a8 2
 *      $Log: E_Broad.c $
 *  Revision 1.1  1995/07/01  21:19:55  dz
a10 12
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
a16 1
#include	<userlist.h>
d22 91
d284 1
a284 1

@
