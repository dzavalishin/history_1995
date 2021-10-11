/************************ Gate remote control ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :	Process letter
 *
 *      $Log: Process.c $
 *      Revision 1.3  1995/11/06 14:28:09  dz
 *      Current
 *
 *      Revision 1.2  1995/08/02 13:56:54  dz
 *      command tail available
 *      stat commented out
 *      rexx support added
 *
 *  Revision 1.1  1995/07/01  21:19:55  dz
 *  Initial revision
 *
 *      
 *         Rev 1.3   07 Jun 1993 17:26:06   dz
 *      Now talks to screen
 *      
 *         Rev 1.2   28 Nov 1992 23:39:56   dz
 *      up to date
 *      
 *         Rev 1.1   04 Feb 1992 00:20:56   dz
 *      Password protection
 *      
 *         Rev 1.0   29 Oct 1991 15:31:20   dz
 *      Initial revision.
 *
 *
\*/

#include	"gremote.h"
#include	"../../version.h"
#include	<uucpaddr.h>

#include	"cmds.h"


// #include	<dir.h>
#include	<time.h>
#include	<errno.h>

static bool		parse( FTN_Msg &orig, FTN_Msg &reply );

bool GRemoteApp::
process( FTN_Msg &orig )
	{
	FTN_Msg        fm;
	fido_user      to, from;
	long	       t;
	char	       buf[200];
	uucp_addr      ua;

	time( &t );

	if( fm.new_msg( conf.netmail_dir.c_str() ) == Err )
		fatal( EC_UU2_Error, EI_Full, "Can't create message file" );

	orig.get_from( to );
	fm.set_to( to );

	from = fido_user( ftn::match_aka( to ), "Gate" );

	fm.set_from( from );
	fm.set_date( t );
	fm.add_attr( FF_KillSent | FF_Local | FF_IsReturnReceipt | FF_Private );
	fm.set_subj("UU2 Remote control reply");

	fm.rewind();

	fm.puts( ">>> Gate Remote Control Unit version " U1_VER_STR "\r\n"	);

	if( get_from( ua, to ) == Err )
		{
		sprintf( buf, ">>> Processing message from %s\r\n\r\n",
			(const char *)((String)to).c_str() );

		fm.puts( buf );
		fm.puts( ">>>\r\n>>> Sorry, but I don't know you!\r\n"	);
		fprintf( stderr, "Gremote: Message from %s refused\n",
						(const char *)((String)to).c_str() );
		}
	else
		{
		sprintf( buf, ">>> Processing message from %s <%s>\r\n\r\n",
			to.name(), (const char *)ua );
		fm.puts( buf );

		fprintf( stderr, "Gremote: Message from %s <%s> accepted\n",
					(const char *)((String)to).c_str(), (const char *)ua );

		parse( orig, fm );
		}

	fm.puts( "\r\n>>>\r\n>>> That's all, bye-bye!\r\n>>>\r\n" );

	fm.detach();						// Send it.

	return Ok;
	}



/****************************************************************************
                               Request parser
****************************************************************************/

bool			execute( const char *command, const char *tail, FTN_Msg &reply, FTN_Msg &orig );
static bool		is_hl( const char *s );
bool			helped_once;
bool			secure;


bool
parse( FTN_Msg &orig, FTN_Msg &reply )
	{
	string       line;
	const char  *p;
	bool         in_header = Yes;

	helped_once = No;
	secure = No;

	orig.get_subj( line );
	if( !strcmp( conf.password.c_str(), line ) )
		secure = Yes;

	orig.rewind();
	while( orig.gets( line ) != Err ) {				// Got a command

		p = line;

		if( in_header && is_hl( p ) )				// Skip RFC-822 headline
			{
			debug("Headline skipped: '%s'", p );
			continue;
			}

		while( *p == ' ' || *p == '\t' )			// Skip whitespaces
			p++;

		if( *p == '\0' )
			continue;
                
                    {
                    char    rep[500];
                    sprintf( rep, ">>> %s\r\n", p );			// Report to user
                    reply.puts( rep );
                    }

		in_header = No;								// End of RFC-822 header reached

		char *tail;
		tail = strpbrk( p, " \t" );	// Get first token
		if( tail )
			{
			*tail = '\0';	// 'close' string
			do { tail++; } while( isspace(*tail) );
			}
		else
			tail = "";

		log( "r", "Remote Command: %s (%s)", p, tail );
		fprintf( stderr, "Executing: %s (%s)\n", p, tail );

		if( (!strcmp( p, "---" )) || (!stricmp( p, "quit" )) )
			break;


		if( execute( p, tail, reply, orig ) == Err )		// Parse/execute cmd.
			return Err;
		}

	return Ok;
	}


/****************************************************************************
							Parse/execute one command
****************************************************************************/


struct cmd_tab	cmd[] = {

		"help",		exec_help,		No,		"Send this help text",
		"users",	exec_users,		Yes,	"Send list of registered gate users",
		"echoes",	exec_echoes,	Yes,	"Send list of gated echo/news groups (not implemented)",
		"notify",	exec_notify,	Yes,	"Send notification message to all gate users",
//		"stat",		exec_stat,		No,		"Send statistics file record",
		"hello",	exec_hello,		No,		"",
		"hello!",	exec_hello,		No,		"",
		"hi",		exec_hello,		No,		"",
		"hi!",		exec_hello,		No,		"",
		"broadcast",exec_broadcast,	Yes,	"Send rest of the letter to all gate users",

		NULL,		NULL,			No,		NULL
		};

bool
execute( const char *command, const char *tail, FTN_Msg &reply, FTN_Msg &orig )
	{
	struct cmd_tab		*tp;
	string ts( tail );

	for( tp = cmd; tp->cmd; tp++ )
		if( !stricmp( command, tp->cmd ) )
			{
			if( tp->privil && !secure )
				{
				reply.puts("Yeah-yeah. Come on...");
				return Ok;
				}
			return tp->func( tail, reply, orig );
			}

	switch( exec_rexx( command, tail, reply, orig ) )
		{
	case Err: return Err;
	case Yes: return Ok;
		}

	reply.puts( "    I don't understand this command, sorry :(\r\n" );
	exec_help( tail, reply, orig );
	return Err;
	}


static bool
is_hl( const char *s )
	{

	while( *s )
		{
		if( isalnum( *s ) || *s == '-' )
			{
			s++;
			continue;
			}

		if( *s == ':' )
			return Yes;

		break;
		}

	return No;
	}
