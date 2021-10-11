head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	95.08.07.14.55.01;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.02.13.40.45;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.09.18.25.15;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.32.26;	author dz;	state Exp;
branches;
next	;


desc
@UU2 News Unbatcher
@


1.4
log
@Using class App
@
text
@/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Extract article from batch
 *
 *      $Log: get_art.c $
 *      Revision 1.3  1995/08/02 13:40:45  dz
 *      Xpost Guard written
 *
 * Revision 1.2  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.2  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.1  1995/03/11  18:32:26  dz
 * Initial revision
 *
 *
\*/

#include    "unbatch.h"
#include	<new_usr.h>
#include    "..\..\version.h"
#include    <fcntl.h>
#include	"vread.h"


static bool		scan_groups( VReadBase & in, uuinfo ui );
static bool		in_path( const char *path_a );



bool
UnbatchApp::get_art( VReadBase & in, long len )
	{
	VReadView	rw;
	long		beg = in.tellg();

	if( rw.open( &in, beg, len ) == Err )
		{
		error( EI_None, "Can't create virtual file" );
		return Err;
		}

	uuinfo		ui;								// uuparse stores result here

	if( uuparse( rw, &ui ) == Err )					// Parse headlines
		return Err;

	ui.textsize = rw.size();

	if( process_art( rw, ui ) == Err )
		return Err;

	in.seekg( beg+len );
	return Ok;
	}

bool
UnbatchApp::process_art( VReadBase & in, uuinfo ui )
	{
	if( strstr( ui.organization,
			conf.gate_id.c_str() ) )	// Has it our mark in hdr?
		{
		log( "#", "Found my marker in Organization - dupe killed");
		return Ok;                              // FIDO msg turned back.
		}                                       // Just ignore it.

	if( in_path(ui.path) )                      // This message returned back - don't pass it
		return Ok;

	if( scan_groups( in, ui ) == Err )
		return Err;


	if( conf.cnews_feed_back )
		if( art_to_spool( in, ui ) == Err )
			error( EI_None, "Error adding article to batch spool" );


	return Ok;
	}

	/*************************************************************
				  Check path line for our host name
	*************************************************************/



#define PATH_DELIM  "!@@,#$%&*()=+|;:"

// This must be rewritten to support multiple names
// Right now we check only for first element of conf.news_path

static bool
in_path( const char *path_a )
	{
	char    		*p;
	static char		test[40];
	static bool		have_test = No;
	const			maxp = 500;
	char			path[maxp];

	strncpy( path, path_a, maxp );
	path[maxp-1] = '\0';

	if( have_test == No )
		{
		have_test = Yes;
		strncpy( test, conf.news_path.c_str(), 40 );
		test[39] = '\0';
		if( ( p = strpbrk( test, PATH_DELIM ) ) != NULL )
			*p = '\0';
		}

	p = strtok( path, PATH_DELIM );
	while( p )
		{
//		debug("Path entry: %s", p );

		if( !strcmp( test, p ) )
			{
			log( "n#", "Found %s in path - dupe killed", p );
			return Yes;
			}

		p = strtok( NULL, PATH_DELIM );
		}

	debug("Our node name not found in path");
	return No;
	}


// NB!
// MSG_Hold must be suported too!

bool
UnbatchApp::send_unknown( VReadBase & in, uuinfo &ui, echo_def &edef,
	const char *gl, const char *group )
	{
	switch( conf.news_bad_group_msg )
		{
	default:
		error( EI_None, "Unreckognized bad group processing mode!" );
		// Fall through....
	case MSG_Separate:
		edef.set_passthrough( "UU2.BAD_GROUP" );
		error( EI_None, "Unknown groups: '%s', sending to UU2.BAD_GROUP", gl );
		// Do retain group name
		return packetize_news( in, ui, edef, Yes );

	case MSG_Kill:
		error( EI_None, "Unknown groups: '%s', throwing message out", gl );
		return Ok;

	case MSG_Pass:
		log( "n", "Passing group name: %s", group );
		edef.set_passthrough( group );
		// Don't retain group name
		return packetize_news( in, ui, edef, No );;
		}

	}


bool
UnbatchApp::scan_groups( VReadBase & in, uuinfo ui )
	{
	echo_def	edef;
	bool		retain_groupname = No;

	if( strlen( ui.control ) )
		{
		switch( conf.news_control_msg )
			{
		default:
			error( EI_None, "Unreckognized control message processing mode!" );
			// Fall through....
		case MSG_Separate:
			edef.set_passthrough( "UU2.CONTROL" );
			log( "n", "Control message, sending to UU2.CONTROL");
			// Do retain group name
			return packetize_news( in, ui, edef, Yes );

		case MSG_Kill:
			log( "n", "Control message, throwing out");
			return Ok;

		case MSG_Pass:
			log( "n", "Control message, passing");
			break;
			}
		}

	char	group[500], *p;
	char	*gbuf;

	group[0] = '\0';

	gbuf = strdup( ui.newsgroups );
	if( gbuf == NULL )
		fatal( EC_Out_Of_Memory, EI_None, "Out of memory" );

	char	*gl = gbuf;

	if( !strlen( gl ) )
		{
		free( gbuf );
		edef.set_passthrough( "UU2.BAD_GROUP" );
		error( EI_None, "No newsgroup name found, sending to UU2.BAD_GROUP" );
		retain_groupname = Yes;
		return packetize_news( in, ui, edef, retain_groupname );
		}

	bool	good_groups = No;
	bool	sent_once = No;
	bool	has_crosspost = No;

	while( *gl )
		{
		bool	breakout = No;

		while( *gl && (*gl == ' ' || *gl == '\t'))
			gl++;

		p = strchr( gl, ',' );
		if( p )
			{
			*p = '\0';
			strcpy( group, gl );
			gl = p+1;
			has_crosspost = Yes;
			}
		else
			{
			strcpy( group, gl );
			breakout = Yes;
			}

		p = strpbrk( group, "\t " );
		if( p )
			*p = '\0';

		if( edef.find_by_group( group ) == Ok )
			{
			debug("Area : %s <- %s", (const char *)edef.area().c_str(), (const char *)edef.group().c_str() );

			// Store newsgroup name/ user address into the
			// special file so SendRules program will find
			// it, check if it is the first letter to this
			// group from that user, and send him rules of
			// this group.

			rules_sender::add_address( group, ui.from );
			good_groups = Yes;

			if( edef.xpost_guard() && has_crosspost )
				{
				debug("Xpost Guard refused to send to %s", edef.area().c_str() );
				sent_once = Yes; // Make sure no complains will be made
				}
			else if( packetize_news( in, ui, edef, retain_groupname ) == Ok )
				{
				sent_once = Yes;
				if( conf.no_crosspost ) breakout = Yes;
				}

			}

		if( breakout )
			break;
		} /* while */

	if( good_groups )
		return sent_once ? Ok : Err;

	if( send_unknown( in, ui, edef, gbuf, group ) == Err )
		{
		free( gbuf );
		return Err;
		}
	else
		{
		free( gbuf );
		return Ok;
		}
	}


@


1.3
log
@Xpost Guard written
@
text
@d8 3
d36 1
a36 1
get_art( VReadBase & in, long len )
d62 1
a62 1
process_art( VReadBase & in, uuinfo ui )
d140 2
a141 2
static bool
send_unknown( VReadBase & in, uuinfo &ui, echo_def &edef,
d169 2
a170 2
static bool
scan_groups( VReadBase & in, uuinfo ui )
@


1.2
log
@IBM C Set version. Phew, it was not a piece of cake to get here...
@
text
@d8 6
d195 1
a195 1
	char	group[50], *p;
d217 1
d232 1
d255 8
a262 1
			if( packetize_news( in, ui, edef, retain_groupname ) == Ok )
a267 1
			good_groups = Yes;
@


1.1
log
@Initial revision
@
text
@d7 3
a9 28
 *      $Log:   Q:/news/unbatch/vcs/get_art.c_v  $
 *      
 *         Rev 1.7   10 Mar 1993 14:06:10   dz
 *      Crossposting to many usenet groups supported.
 *      
 *         Rev 1.6   05 Mar 1993 03:09:16   dz
 *      Cleanup
 *      
 *         Rev 1.5   05 Mar 1993 03:04:50   dz
 *      Direct unbatching
 *      
 *         Rev 1.4.1.0   03 Mar 1993 20:23:50   dz
 *      Temp: Trying to implement direct unbatching scheme.
 *      
 *         Rev 1.4   28 Nov 1992 22:31:14   dz
 *      consts
 *      
 *         Rev 1.3   23 Oct 1992 15:03:08   dz
 *      error()
 *      
 *         Rev 1.2   17 May 1992 11:05:46   dz
 *      Internal packetizer, kiae-specific unbatcher
 *      
 *         Rev 1.1   21 Feb 1992 12:07:44   dz
 *      Art. length: int -> long!
 *      
 *         Rev 1.0   21 Feb 1992 11:22:26   dz
 *      Initial revision.
@
