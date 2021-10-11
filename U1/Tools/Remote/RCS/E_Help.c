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
date	95.08.02.13.51.49;	author dz;	state Exp;
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
 *      $Log: E_Help.c $
 *      Revision 1.2  1995/08/02 13:51:49  dz
 *      tail available (command string)
 *
 *  Revision 1.1  1995/07/01  21:19:55  dz
 *  Initial revision
 *
 *      
 *         Rev 1.1   28 Nov 1992 23:40:24   dz
 *      minor typos fixed
 *      
 *         Rev 1.0   04 Feb 1992 00:21:24   dz
 *      Initial revision.
 *
 *
\*/

#include	"gremote.h"
#include	"../../version.h"

#include	"cmds.h"

static const char *bar =
		"\r\n\r\n"
		"              °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\r\n"
		"\r\n\r\n";

// #pragma argsused

bool
exec_help( const string &tail, FTN_Msg &reply, FTN_Msg &orig )
    {
    struct cmd_tab    *tp;
    string             buf;
    FILE	      *hf;
    
    
    if( helped_once )
        return Ok;
    else
        helped_once = Yes;
    
    
    reply.puts(
               "\r\n\r\n"
               "              °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\r\n"
               "              °                                            °\r\n"
               "              °    Gate Remote Control Unit Help screen    °\r\n"
               "              °                                            °\r\n"
               "              °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\r\n"
               "\r\n\r\n\r\n"
               "Remote control recognizes the following commands:\r\n"
               "\r\n"
              );
    for( tp = cmd; tp->cmd; tp++ ) 
        {
        if( (secure || (!(tp->privil ))) && strlen(tp->help) ) 
            {
            char b[200];
            sprintf( b, " %-10s - %s", tp->cmd, tp->help );
            reply.puts( b );
            }
        }
    
    reply.puts( bar );
    
    if( (hf = fopen( conf.help_file.c_str(), "rt" )) != NULL ) 
        {
        while( buf.load( hf ) == Ok )
            reply.puts( buf );
        
        fclose( hf );
        }
    
    reply.puts( bar );
    
    return Ok;
    }


// #pragma argsused

bool
exec_hello( const string &tail, FTN_Msg &reply, FTN_Msg &orig )
    {
    reply.puts( "\r\nHello 2U2!\r\n" );
    return Ok;
    }





@


1.2
log
@tail available (command string)
@
text
@d8 3
d37 48
a84 45
exec_help( const string &tail, fido_msg &reply, fido_msg &orig ) 
	{
	struct cmd_tab		*tp;
	char				buf[200];
	FILE				*hf;


	if( helped_once )		return Ok;
	else					helped_once = Yes;


	reply.puts(
					   "\r\n\r\n"
		"              °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\r\n"
		"              °                                            °\r\n"
		"              °    Gate Remote Control Unit Help screen    °\r\n"
		"              °                                            °\r\n"
		"              °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°\r\n"
		"\r\n\r\n\r\n"
		"Remote control recognizes the following commands:\r\n"
		"\r\n"
		);
	for( tp = cmd; tp->cmd; tp++ ) 
		{
		if( (secure || (!(tp->privil ))) && strlen(tp->help) ) 
			{
			sprintf( buf, " %-10s - %s", tp->cmd, tp->help );
			reply.puts( buf );
			}
		}

	reply.puts( bar );

	if( (hf = fopen( conf.help_file.c_str(), "rt" )) != NULL ) 
		{
		while( fgets( buf, 80, hf ) != NULL )
			reply.puts( buf );

		fclose( hf );
		}

	reply.puts( bar );

	return Ok;
	}
d90 5
a94 9
exec_hello( const string &tail, fido_msg &reply, fido_msg &orig ) 
	{

	reply.puts(
		"\r\nHello 2U2!\r\n"
		);

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
 *      $Log:   Q:/tools/remote/vcs/e_help.c_v  $
d22 1
a22 1
#include	"version.h"
d31 1
a31 1
#pragma argsused
d34 1
a34 1
exec_help( fido_msg &reply, fido_msg &orig ) 
d81 1
a81 1
#pragma argsused
d84 1
a84 1
exec_hello( fido_msg &reply, fido_msg &orig ) 
@
