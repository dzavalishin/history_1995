/************************ Gate remote control ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :	Process letter
 *
 *      $Log: E_Help.c $
 *      Revision 1.3  1995/11/06 14:28:09  dz
 *      Current
 *
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





