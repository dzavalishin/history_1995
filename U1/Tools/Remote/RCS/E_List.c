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
 *      Module  :	Process letter
 *
 *      $Log: E_List.c $
 *      Revision 1.2  1995/08/02 13:52:37  dz
 *      tail available (command string)
 *
 *  Revision 1.1  1995/07/01  21:19:55  dz
 *  Initial revision
 *
 *      
 *         Rev 1.2   28 Nov 1992 23:40:52   dz
 *      formatting changed
 *      
 *         Rev 1.1   11 Nov 1992 02:52:10   dz
 *      New userlist access scheme
 *      
 *         Rev 1.0   04 Feb 1992 00:21:36   dz
 *      Initial revision.
 *
 *
\*/

#include	"gremote.h"
#include	"../../version.h"
#include	<userlist.h>

#include	"cmds.h"


static bool
put_user( const fido_user_def &ud, void *arg )
    {
    FTN_Msg	 &reply = *( (FTN_Msg*) arg );
    char			buf[200];
    
    sprintf( buf, "    %-40s %s@@%s (From %s)",
            (const char *)((String)(ud.fido_address())).c_str(),
            (const char *)ud.uu_name().c_str(),
            (const char *)ud.uu_to_domain().c_str(),
            (const char *)ud.uu_from_domain().c_str()
           );
    
    reply.puts( buf );
    return Ok;
    }

//#pragma argsused

bool
exec_users( const string &tail, FTN_Msg &reply, FTN_Msg &orig )
    {
    reply.puts( "\r\nList of registered gate users:\r\n" );
    return fuser.foreach( put_user, &reply );
    }


//#pragma argsused

bool
exec_echoes( const string &tail, FTN_Msg &reply, FTN_Msg &orig ) {
#if 0
char	buf[200];

reply.puts( "\r\nList of gated echoes/newsgroups:\r\n" );

for( int i = 0; i < MAXECHO; i++ ) 
    {
    
    if( strlen( echo[i].echo ) == 0 )
        continue;
    
    sprintf( buf, "    %-36.36s\t%s",
            echo[i].echo,
            echo[i].newsgroup
           );
    
    reply.puts( buf );
    
    }
#else
reply.puts( "\r\nSorry, but list of gated echoes/newsgroups is not available in this beta\r\n" );
#endif

return Ok;
}



@


1.2
log
@tail available (command string)
@
text
@d8 3
d36 7
a42 7
	{
	fido_msg		&reply = *( (fido_msg*) arg );
	char			buf[200];

	sprintf( buf, "    %-40s %s@@%s (From %s)",
			(const char *)((String)(ud.fido_address())).c_str(),
			(const char *)ud.uu_name().c_str(),
d45 5
a49 5
			);

	reply.puts( buf );
	return Ok;
	}
d54 5
a58 5
exec_users( const string &tail, fido_msg &reply, fido_msg &orig )
	{
	reply.puts( "\r\nList of registered gate users:\r\n" );
	return fuser.foreach( put_user, &reply );
	}
d64 1
a64 1
exec_echoes( const string &tail, fido_msg &reply, fido_msg &orig ) {
d66 1
a66 13
	char	buf[200];

	reply.puts( "\r\nList of gated echoes/newsgroups:\r\n" );

	for( int i = 0; i < MAXECHO; i++ ) {

		if( strlen( echo[i].echo ) == 0 )
			continue;

		sprintf( buf, "    %-36.36s\t%s",
			echo[i].echo,
			echo[i].newsgroup
			);
d68 1
a68 1
		reply.puts( buf );
d70 14
a83 1
		}
d85 1
a85 1
	reply.puts( "\r\nSorry, but list of gated echoes/newsgroups is not available in this beta\r\n" );
d88 2
a89 2
	return Ok;
	}
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/tools/remote/vcs/e_list.c_v  $
d25 1
a25 1
#include	"version.h"
d48 1
a48 1
#pragma argsused
d51 1
a51 1
exec_users( fido_msg &reply, fido_msg &orig )
d58 1
a58 1
#pragma argsused
d61 1
a61 1
exec_echoes( fido_msg &reply, fido_msg &orig ) {
@
