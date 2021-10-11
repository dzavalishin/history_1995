head	1.35;
access;
symbols;
locks;
comment	@ * @;


1.35
date	95.08.07.14.55.01;	author dz;	state Exp;
branches;
next	1.34;

1.34
date	95.08.07.14.10.40;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	95.08.02.13.45.51;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.04.12.13.52.54;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.04.09.18.25.15;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.09.10.45.11;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.32.26;	author dz;	state Exp;
branches;
next	;


desc
@UU2 News Unbatcher
@


1.35
log
@Using class App
@
text
@/************************ UUCP to FIDO gate ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Add letter to echo packet.
 *
 *      $Log: SendNews.C $
 *      Revision 1.34  1995/08/07 14:10:40  dz
 *      Cleanup
 *
 *      Revision 1.5  1995/08/02 13:45:51  dz
 *      LOTS of changes. Mostly new kludges and double-gateing
 *      support.
 *
 *      Revision 1.4  1995/04/12  13:52:54  dz
 *      fixed error in save_to_file - inability to create
 *      file due to absence of O_CREAT flag...
 *
 *      Revision 1.4  1995/04/12  13:52:54  dz
 *      fixed error in save_to_file - inability to create
 *      file due to absence of O_CREAT flag...
 *
 *      Revision 1.3  1995/04/09  18:25:15  dz
 *      IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 *      Revision 1.2  1995/04/09  10:45:11  dz
 *      rewriting for C Set
 *
 *      Revision 1.1  1995/03/11  18:32:26  dz
 *      Initial revision
 *
 *      
 *         Rev 1.27   07 Jun 1993 17:17:58   dz
 *      Splitter numbering system bug fixed
 *      Variable size of FIDO <from name> field.
 *      
 *         Rev 1.26   10 Mar 1993 14:08:02   dz
 *      Crossposting support written
 *      
 *         Rev 1.25   05 Mar 1993 03:11:02   dz
 *      Direct unbatching
 *      
 *         Rev 1.24.1.0   03 Mar 1993 20:32:16   dz
 *      Temp: trying to implement direct unbatching.
 *      
 *         Rev 1.24   16 Feb 1993 04:10:14   dz
 *      Selective headers elimination logic fixed - see
 *      comments in code.
 *      
 *         Rev 1.23   07 Jan 1993 16:33:54   dz
 *      Bug fix: now checking for end of headlines before
 *      selective headline elimination code. Otherwise it
 *      tends to 'selectively' eliminate all the message
 *      text :).
 *      
 *         Rev 1.22   28 Nov 1992 22:35:04   dz
 *      Saving to file implemented
 *
 *      Todo: when saving to file, cut off text header and send
 *            it to echo.
 *      
 *         Rev 1.21   26 Nov 1992 17:34:00   dz
 *      Using echo_def class, first edition
 *      
 *         Rev 1.20   11 Nov 1992 00:04:12   dz
 *      Conversion of UseNet path line to FSC44 path
 *      line written. Selective elimination of headlines
 *      implemented.
 *      
 *         Rev 1.19   23 Oct 1992 15:05:20   dz
 *      Retaining 'newsgroups:' in a case of group is unknown or
 *      message is a control. Moving messages with unknown group
 *      to UU2.BAD_GROUP, control messages to UU2.CONTROL.
 *
 *         Rev 1.18   11 Oct 1992 05:47:02   dz
 *      Configurable chunk size for splitter
 *      Support for 'Comment-To' headline
 *
 *         Rev 1.17   18 Sep 1992 03:13:26   dz
 *      subject length decreased by 2 bytes: Gecho gets crazy when
 *      receives packet with long subject.
 *
 *         Rev 1.16   14 Jul 1992 15:18:28   dz
 *      X-Long-Subject kludge removed, now just passing
 *      through Subject headline if it's contents are
 *      longer then FIDO subject field max size.
 *      
 *         Rev 1.15   06 Jul 1992 23:56:42   dz
 *      recoder class used
 *      path processing fixed to support complex
 *      values in conf.news_path
 *      
 *         Rev 1.14   29 Jun 1992 22:09:22   dz
 *      PTH: conf.free_domain instead of "fidonet.org"
 *      X-Long-Subject kludge
 *      
 *         Rev 1.13   29 Jun 1992 21:51:06   dz
 *      Multi-message packets
 *      
 *         Rev 1.11   24 May 1992 18:51:38   dz
 *      msgid generation: ( time << 6 ) + seq, to be sure that there will be
 *      unique msgids untill it will produce more than 64 messages a second.
 *      
 *         Rev 1.10   17 May 1992 11:08:14   dz
 *      Internal packetizer, kiae-specific unbatcher
 *
 *         Rev 1.9   17 Apr 1992 11:37:34   dz
 *      FSC35, reply-to support
 *
 *         Rev 1.8   07 Apr 1992 12:31:34   dz
 *      conditional decoding
 *
 *         Rev 1.6   03 Mar 1992 19:38:06   dz
 *      Dupeloop recognition using Path line written, but not tested.
 *
 *         Rev 1.5   03 Feb 1992 23:53:18   dz
 *      - Headlines killer updated
 *      - Subject cleaner bug fix (Leading spaces)
 *      - FSC-0044 (^APTH:) support added.
 *      - Self identification generalized - now uses conf. file
 *        parmeter 'GateId' to identify itself in Organiztion line.
 *
 *         Rev 1.4   11 Jan 1992 13:41:04   dz
 *      Stripping spaces after [NEWS], cutting off more headlines.
 *
 *         Rev 1.3   03 Dec 1991 13:15:24   dz
 *      Wiping out [NEWS] from subject
 *
 *         Rev 1.2   08 Nov 1991 23:14:20   dz
 *      Headlines stripping written
 *
 *         Rev 1.1   21 Oct 1991 05:19:44   dz
 *      A lot of bugs fixed.
 *      Date field reconstruction written.
 *      PID kludge added.
 *
 *         Rev 1.0   11 Oct 1991 05:38:52   dz
 *      Initial revision.
 *
 *
 *
\*/

#include    "unbatch.h"
#include    "fido_pkt.h"
#include    "stat.h"
#include    "..\..\version.h"
#include    "..\..\u1.h"
#include    <recode.h>
#include    <hl_match.h>

#include    <uu2io.h>
#include    <fcntl.h>
#include    <share.h>

#include    "vread.h"


static fmsg_attr	ZeroAttr;

static bool		write_pth( FILE *fp, const char *path );
static bool		put_path_seenby( FILE *fp );
static bool		writef( FILE *fp, char *fmt, ... );
static void		set_date( char *buf, time_t tmt );

static bool		empty_line( char *h );
static void		clear_subj( uchar *subj );
static void		invalidate( char *s );



bool
UnbatchApp::copy_data(
		VReadBase &in, FILE *fp, uuinfo &ui,
		echo_def &edef, bool retain_groupname,
		int part_no
		)
	{


	const			bufsize = 4000;
	char			buf[bufsize], *p;
	int				len,
					run_len;				// Bytes written
	long			t;						// Time
	fido_pmsg_hdr	mh;						// Packetized msg header
	char		mh_date[21];
	static int		seq = 0;				// Helps to create unique MSGID
	bool			fsc35_mode;
	fido_user		ftn_from = FTN_From( ui, fsc35_mode );

	/*************************************************************
			  Prepare and write packetized message header
	*************************************************************/

	mh.origNet	= conf.echo_from_addr.net();
	mh.origNode	= conf.echo_from_addr.node();

	mh.destNet	= conf.echo_to_addr.net();
	mh.destNode	= conf.echo_to_addr.node();

	mh.attr		= (short)ZeroAttr;
	mh.cost		= 0;

	set_date( (char *)mh_date, ui.gm_time );


	if( fwrite( &mh, 1, sizeof(mh), fp ) != sizeof(mh) )
		return Err;

	mh_date[19] = '\0';
	if( fwrite( &mh_date, 1, 20, fp ) != 20 )
		return Err;

	if( ui.ftn_to.length() > 0 )
		{
		int	l = ui.ftn_to.length();

		if( l > 35 )	l = 35;

		if( fwrite( ui.ftn_to.c_str(), 1, l, fp ) != l )	// To
			return Err;
		if( fwrite( "", 1, 1, fp ) != 1 )	// \0
			return Err;
		}
	else
		{
		int	l = strlen( ui.comment_to );
		if( l > 35 )
			{
			l = 35;
			ui.comment_to[35] = '\0';
			}

		if( l > 0 )
			{
			if( fwrite( ui.comment_to, 1, l+1, fp ) != l+1 )	// To
				return Err;
			}
		else
			{
			if( fwrite( "All", 1, 4, fp ) != 4 )				// To
				return Err;
			}
		}

// NB!
/*
	if( strlen( ui.replyto ))
		sprintf( buf, "%.35s", ui.replyto );                // From
	else
		sprintf( buf, "%.35s", ui.from );                   // From
	len = strlen( buf );

    if( len >= conf.from_field_size )                       // Too long?
		{
		strcpy( buf, conf.magic );          				// Put magic name
		len = strlen( buf );
		}
*/
	sprintf( buf, "%.35s", ftn_from.name() );                   // From
	len = strlen( buf );

	if( fwrite( buf, 1, len+1, fp ) != (len+1) )
		return Err;

	clear_subj( (uchar *)ui.subject );

	if( part_no )
		sprintf( buf, "(%d) %.60s", part_no, ui.subject );
	else
		sprintf( buf, "%.70s", ui.subject );			// Subject
	len = strlen( buf );
	if( fwrite( buf, 1, len+1, fp ) != (len+1) )
		return Err;


		/*********************************************
		   Insert kludges in the beginning of text
		*********************************************/

	if( writef( fp, "AREA:%s\r\n", (const char *)edef.area().c_str() ) == Err )
		return Err;

	time( &t );

	if( ui.ftn_msgid.length() )
		{
		if( writef( fp, "\x1MSGID: %s\r\n", ui.ftn_msgid.c_str() ) )
			return Err;
		}
	else
		if( writef( fp, "\x1MSGID: %s %lX\r\n",
	            (const char*)((String)(fido_addr)ftn_from).c_str(),
			(t<<6) + (seq++) ) == Err )
			return Err;

	if( ui.ftn_reply.length() )
		{
		if( writef( fp, "\x1REPLY: %s\r\n", ui.ftn_reply.c_str() ) )
			return Err;
		}


	if( writef( fp, "\x1PID: U1 %s\r\n", U1_VER_STR ) )
		return Err;

	if( writef( fp, "\x1RFC-Message-ID: %s\r\n", ui.message_id ) )
		return Err;

	if( write_pth( fp, ui.path ) == Err )
		return Err;

	if( fsc35_mode && strlen( ui.replyto ) )
		{
		if( writef( fp, "\x1REPLYADDR %s\r\n", ui.replyto ) )
			return Err;
		}
	else if( fsc35_mode && strlen( ui.from ) )
		{
		if( writef( fp, "\x1REPLYADDR %s\r\n", ui.from ) )
			return Err;
		}

	if( fsc35_mode && writef( fp, "\x1REPLYTO %s %s\r\n", (const char *)((String)conf.echo_from_addr).c_str(), (conf.magic).c_str() ) )
		return Err;

	bool		pass_subject = No;

	if( strlen( ui.subject ) > 60 )
		pass_subject = Yes;
	else
		pass_subject = No;

		/*********************************************
						Put real name in text
		*********************************************/

	if( strlen( ui.realname ) )
		{
		if( writef( fp, "X-RealName: %s\r\n", ui.realname ) )
			return Err;
		}

		/*********************************************
						Copy text
		*********************************************/

	// Header can be found in first (1) or single (0) part only.
	bool    header = (part_no < 2) ? Yes : No;

	run_len = 0;

	recoder		r( conf.code_uf.c_str() );
	if( !r.valid() )
		error( EI_None, "Codetable '%s' not found", (const char *)conf.code_uf.c_str() );

	while( in.gets( (uchar *)buf, bufsize ) != Err )
		{
		if( (p = strpbrk( buf, "\n\r" )) != NULL )		// CR or LF?
			*p = '\0';									// Strip!

		if( header && empty_line( buf ))				// Empty line marks
			header = No;								// end of header.

		// Ok, here we decide, whether to put headline into the
		// message text, or no. There's two exceptions from
		// basic rule: We must present "Newsgroups" line
		// in message text if message has unknown newsgroup
		// (retain_groupname flag) and "Subject" line if
		// subject too long for FIDO (pass_subject flag).
		//
		// The basic rule is: if headline matches kill list
		// (and exceptions above doesn't take place) remove
		// it in any case, else if it matches hide list - pass
		// it hidden under ^A, and if it matches pass list
		// pass it as is.
		//
		// Note that if subject is mentioned in both kill and
		// hide lists, short subjects will not be included in
		// message text, and long (see exception 2) subjects
		// will be hidden under ^A.

		if( header )
			{
			bool	pass = No;

			if( hl_match_list( buf, conf.news_pass_hl.c_str() ) )
				pass = Yes;

			if( hl_match_list( buf, conf.news_kill_hl.c_str() ) )
				pass = No;

			if( retain_groupname && strnicmp( buf, "newsgroups:", 11 ) == 0 )
				pass = Yes;

			if( pass_subject && strnicmp( buf, "subject:", 8 ) == 0 )
				pass = Yes;

			if( pass && hl_match_list( buf, conf.news_hide_hl.c_str() ) )
				{
				run_len++;
				if( fwrite( "\x1", 1, 1, fp ) != 1 )        // Add ^A
					return Err;
				pass = Yes;
				}

			if( !pass )
				continue;

			run_len += 4;
			if( fwrite( "RFC-", 1, 4, fp ) != 4 )        // Add RFC-
				return Err;

			}

		len = strlen( buf );
		run_len += (len+1);

		r.rs( (uchar *)buf );

		invalidate( buf );								// Kill tearline/origin

		if( fwrite( buf, 1, len, fp ) != len )				// Write line
			return Err;
		if( fwrite( "\r", 1, 1, fp ) != 1 )					// Add hard CR
			return Err;

		if( run_len >= conf.part_size )                      // Exceeds limit?
			break;										// Stop here.

		}

    const char *orig_text = (const char *)conf.origin.c_str();
    const char *orig_addr = (const char*)((String)(fido_addr&)ftn_from).c_str();

    if( strlen( orig_text ) == 0 )
        orig_text = ui.organization;

    // 14 - length of static characters " * Origin:  ()"
    int orig_txt_len = 79 - 14 - strlen( orig_addr );

    if( orig_txt_len < 0 )
        error( EI_None, "Crazy world: address (%s) too long to fit in Origin line!", orig_addr );
    else
        {
        if( writef( fp,	"\r\n---\r\n * Origin: %.*s (%s)\r\n",
            orig_txt_len,
			orig_text, orig_addr
	    ) == Err )
		    return Err;
        }

    if( put_path_seenby( fp ) == Err )
        return Err;


	if( fwrite( "", 1, 1, fp ) != 1 )					// Terminating zero byte
		return Err;

	return Ok;
	}

bool
put_path_seenby( FILE *fp )
    {

    if(
       conf.echo_from_addr.net() == conf.echo_to_addr.net() &&
       conf.echo_from_addr.node() == conf.echo_to_addr.node()
      )
        {
	    if( writef( fp, "SEEN-BY: %d/%d\r\n",
         conf.echo_from_addr.net(), conf.echo_from_addr.node()
         ) == Err )
		    return Err;
        }
    else
        {
        // Write sorted address pair
        int net1, net2, node1, node2;

        if( conf.echo_from_addr.net() > conf.echo_to_addr.net() )
            {
            net1 = conf.echo_to_addr.net();
            net2 = conf.echo_from_addr.net();
            node1 = conf.echo_to_addr.node();
            node2 = conf.echo_from_addr.node();
            }
        else if( conf.echo_to_addr.net() > conf.echo_from_addr.net() )
            {
            net2 = conf.echo_to_addr.net();
            net1 = conf.echo_from_addr.net();
            node2 = conf.echo_to_addr.node();
            node1 = conf.echo_from_addr.node();
            }
        else if( conf.echo_from_addr.node() > conf.echo_to_addr.node() )
            {
            net1 = conf.echo_to_addr.net();
            net2 = conf.echo_from_addr.net();
            node1 = conf.echo_to_addr.node();
            node2 = conf.echo_from_addr.node();
            }
        else
            {
            net2 = conf.echo_to_addr.net();
            net1 = conf.echo_from_addr.net();
            node2 = conf.echo_to_addr.node();
            node1 = conf.echo_from_addr.node();
            }

        if( writef( fp, "SEEN-BY: %d/%d %d/%d\r\n", net1, node1, net2, node2 ) == Err )
		    return Err;
        }

	if( writef( fp, "\x1PATH: %d/%d\r\n",
			conf.echo_from_addr.net(),		conf.echo_from_addr.node()
			) == Err )
		return Err;

    return Ok;
    }





bool
UnbatchApp::do_packetize_news( VReadBase &in, uuinfo &ui, echo_def &edef, bool retain_groupname )
	{
	FILE *fp;				// Echo packet file
	bool	split = No;							// Split mode
	int		part_no = 1;						// Slice number
	int		msg_type;

	in.seekg( 0 );							// Go to beginning of file

	if( (fp = get_packet_file()) < 0 )			// Find/create packet file
		return Err;								// Can't??

	fseek( fp, 0L, SEEK_CUR );					// ?? Over last delimiter

	// Add 512-byte safety margin
	if( ui.textsize > (conf.part_size-512) )    // Big letter?
		split = Yes;							// Well, let's split it

	while( !in.eof() )							// Have something on input?
		{
		msg_type = 2;
		if( fwrite( &msg_type, 1, 2, fp ) != 2 )	// Write delimiter
			return Err;

		if(	copy_data( in, fp, ui, edef, retain_groupname,
							split ? (part_no++) : 0 ) == Err )
			return Err;

		// If, for some reason, we was going to put all
		// the article into the single part, but it doesn't
		// worked out, continue from part 2 in multipart mode
		// anyway.

		if( split == No )
            {
            split = Yes;
            part_no = 2;
            }
		}

	add_stat_news();                            // Increase statistics counter

	return Ok;
	}


bool
UnbatchApp::packetize_news( VReadBase &in, uuinfo &ui, echo_def &edef,
	bool retain_groupname )
	{

	if( !edef.pass_to_fido() )
		{
		error( EI_None, "Group %s isn't allowed to be passed to FIDO", (const char *) edef.group().c_str() );
		return Ok; // We did our best with that article
		}

	fprintf( stderr, "Echo: %s\n", (const char *)edef.area().c_str() );
	log( "e", "Echo import: %s", (const char *)edef.area().c_str() );

	if( edef.in_file() )
		return save_to_file( in, ui, edef, retain_groupname );
	else
		return do_packetize_news( in, ui, edef, retain_groupname );
	}






/****************************************************************************
					Make PTH line from UseNet path
							(See FSC44)
****************************************************************************/

static bool
write_pth( FILE *fp, const char *path_a )
	{
	const	maxp = 500;
	char	path[maxp];
	char	*el;

	strncpy( path, path_a, maxp );
	path[maxp-1] = '\0';

	if( strlen( path_a ) >= maxp )
		{
		char	*p = strrchr( path, '!' );
		if( p ) *p = '\0';
		}

// Write prefix
	if( writef( fp, "\x1PTH: " ) )
		return Err;

// Convert UseNet path
	if( strlen( path ) )
		{
		do
			{
			el = strrchr( path, '!' );

			if( el == NULL )
				{
				el = path;
				if( writef( fp, "@@%s ", path ) )
					return Err;
				}
			else
				{
				if( writef( fp, "@@%s ", el + 1 ) )
					return Err;
				*el = '\0';
				}
			} while( el > path );
		}


// Write suffix
	if( writef( fp, "@@%s %s\r\n",
      (const char *)conf.def_domain.c_str(),
      (const char *)((String)conf.echo_from_addr).c_str()
	  ) )
		return Err;

	return Ok;
	}


/****************************************************************************
                           Misc. tools
****************************************************************************/


void
invalidate( char *s )
	{
	while( *s == ' ' || *s == '\t' )					// Skip WS
		s++;

	if( strncmp( s, "---", 3 ) == 0 && s[3] != '-' )	// Tearline?
		s[1] = '$';										// Invalidate
	if( strnicmp( s, "* origin", 8 ) == 0 )				// Origin?
		s[0] = '#';										// As well.
	}



bool
writef( FILE *fp, char *fmt, ... )
	{
	char buffer[600];
	va_list argptr;
	int cnt;

	va_start(argptr, fmt);
	cnt = vsprintf(buffer, fmt, argptr);
	va_end(argptr);

	return fwrite( buffer, 1, cnt, fp ) == cnt ? Ok : Err;
	}




static	char months[][4] =
	{
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
	};

void
set_date( char *buf, time_t tmt )
	{
	if( tmt <= 0 )
		time( &tmt );

	tm		*t = localtime( &tmt );

	if( t == NULL )
		fatal( EC_Got_Insane, EI_None, "localtime(%ld) returns NULL", tmt );

	if( t->tm_year > 99 )
		{
		warning( EI_None, "Got year no. %d, set to %d", t->tm_year, t->tm_year % 100 );
		t->tm_year %= 100;
		}

	sprintf( buf,	"%02d %3.3s %02d  %02d:%02d:%02d",
							t->tm_mday, months[t->tm_mon], t->tm_year,
							t->tm_hour, t->tm_min, t->tm_sec);
	}


bool
empty_line( char *h )
	{
	while( *h == ' ' || *h == '\t' )		h++;
	return *h ? No : Yes;
	}


void
clear_subj( uchar *subj )
	{
	char	*p, *d;

	if(
		((p = strstr((char *)subj, (char *)"[NEWS]")) != NULL) ||
		((p = strstr((char *)subj, (char *)"[News]")) != NULL)
		) {

		d = p;
		p += 6;
		while( *p == ' ' || *p == '\t' )
			p++;
		strcpy( d, p );
		}
	}




@


1.34
log
@Cleanup
@
text
@d8 1
a8 1
 *      Revision 1.43  1995/08/07 14:06:09  dz
a169 1
extern bool		save_to_file( VReadBase &in, uuinfo &, echo_def &edef, bool );
d172 2
a173 3

static bool
copy_data(
d528 2
a529 2
static bool
do_packetize_news( VReadBase &in, uuinfo &ui, echo_def &edef, bool retain_groupname )
d576 1
a576 1
packetize_news( VReadBase &in, uuinfo &ui, echo_def &edef,
@


1.5
log
@LOTS of changes. Mostly new kludges and double-gateing
support.
@
text
@d8 2
a9 7
 * Revision 1.4  1995/04/12  13:52:54  dz
 * fixed error in save_to_file - inability to create
 * file due to absence of O_CREAT flag...
 *
 * Revision 1.4  1995/04/12  13:52:54  dz
 * fixed error in save_to_file - inability to create
 * file due to absence of O_CREAT flag...
d11 3
a13 2
 * Revision 1.3  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
d15 3
a17 2
 * Revision 1.2  1995/04/09  10:45:11  dz
 * rewriting for C Set
d19 12
a30 2
 * Revision 1.1  1995/03/11  18:32:26  dz
 * Initial revision
a99 3
 *         Rev 1.12   18 Jun 1992 11:18:02   dz
 *      Searching for [News] as well as for [NEWS] in subject
 *
a199 2
//	mh.origNet	= ftn_from.net();
//	mh.origNode	= ftn_from.node();
a436 1
//    const char *orig_addr = (const char*)(String)conf.echo_from_addr;
a575 83
#if 0


static bool
save_to_file( VReadBase &in, uuinfo &, echo_def &edef, bool )
	{
	char	fn[100];
	FILE	*fp;

	recoder		r( conf.code_uf.c_str() );
	if( !r.valid() )
		error( EI_None, "Codetable '%s' not found", (const char *)conf.code_uf.c_str() );


	while( 1 )
		{
		sprintf( fn, "%s\\%lx", (const char *)edef.in_file_dir().c_str(), time(NULL) );

		int fd;

		errno = 0;

//		fd = _sopen( fn, O_WRONLY|O_TEXT, SH_DENYWR, 0666 );
		fd = _sopen( fn, O_WRONLY|O_TEXT|O_CREAT|O_EXCL, SH_DENYWR, 0666 );
		if( fd < 0 )
			{
			if( errno == EEXIST ) 
				continue;
			error( EI_Full, "Can't create (_sopen) %s", fn );
			return Err;
			}

		fp = fdopen( fd, "w" );
		if( fd != NULL )
			break;

		error( EI_Full, "Can't fdopen %s", fn );
		return Err;
		}

	log( "n", "Saving article to %s", fn );
	fprintf( stderr, "Saving article to %s\n", fn );

	in.seekg( 0 );

	const	bl = 8192;
	char	buf[bl];
	int		l;

	while( (l = in.read( buf, bl )) > 0 )
		{
		for( int i = l; i; i-- )
			buf[i] = r.rc( buf[i] );

		if( fwrite( buf, l, 1, fp ) != l )
			break;
		}

	if( in.ferr() )
		{
		error( EI_Full, "Error reading article file" );
		fclose( fp );
		return Err;
		}

	if( ferror( fp ) )
		{
		error( EI_Full, "Error writing output file" );
		fclose( fp );
		return Err;
		}

	if( fclose( fp ) )
		{
		error( EI_Full, "Error closing output file" );
		return Err;
		}

	add_stat_news();                            // Increase statistics counter
	return Ok;
	}

#endif
@


1.4
log
@fixed error in save_to_file - inability to create
file due to absence of O_CREAT flag...
@
text
@d8 8
d140 2
a141 2
#include	"unbatch.h"
#include	"fido_pkt.h"
d143 8
a150 8
#include	"..\..\version.h"
#include	"..\..\u1.h"
#include	<recode.h>
#include	<hl_match.h>

#include	<uu2io.h>
#include	<fcntl.h>
#include	<share.h>
d152 1
a152 1
#include	"vread.h"
d157 4
a160 3
static bool		write_pth( int fd, const char *path );
static bool		put_path_seenby( int fd );

a161 3

static bool		writef( int fd, char *fmt, ... );
static void		set_date( char *buf, time_t tmt );
d166 1
d172 1
a172 1
		VReadBase &in, int fd, uuinfo &ui,
d185 1
d205 1
a205 1
	set_date( (char *)mh.date, ui.gm_time );
d208 1
a208 1
	if( write( fd, &mh, sizeof(mh) ) != sizeof(mh) )
d211 16
d237 1
a237 1
			if( write( fd, ui.comment_to, l+1 ) != l+1 )	// To
d242 1
a242 1
			if( write( fd, "All", 4 ) != 4 )				// To
d264 1
a264 1
	if( write( fd, buf, len+1 ) != (len+1) )
d274 1
a274 1
	if( write( fd, buf, len+1 ) != (len+1) )
d282 1
a282 1
	if( writef( fd, "AREA:%s\r\n", (const char *)edef.area().c_str() ) == Err )
d287 8
a294 2
	if( writef( fd, "\x1MSGID: %s %lX\r\n",
            (const char*)((String)(fido_addr)ftn_from).c_str(),
d296 8
a303 1
		return Err;
d305 1
a305 1
	if( writef( fd, "\x1PID: U1 %s\r\n", U1_VER_STR ) )
d308 1
a308 1
	if( writef( fd, "\x1X-RFC-Message-ID: %s\r\n", ui.message_id ) )
d311 1
a311 1
	if( write_pth( fd, ui.path ) == Err )
d316 1
a316 1
		if( writef( fd, "\x1REPLYADDR %s\r\n", ui.replyto ) )
d321 1
a321 1
		if( writef( fd, "\x1REPLYADDR %s\r\n", ui.from ) )
d325 1
a325 1
	if( fsc35_mode && writef( fd, "\x1REPLYTO %s %s\r\n", (const char *)((String)conf.echo_from_addr).c_str(), (conf.magic).c_str() ) )
d341 1
a341 1
		if( writef( fd, "X-RealName: %s\r\n", ui.realname ) )
d388 6
d400 1
a400 1
			if( pass || !hl_match_list( buf, conf.news_kill_hl.c_str() ) )
d402 4
a405 12

				if( hl_match_list( buf, conf.news_pass_hl.c_str() ) )
					pass = Yes;

				if( hl_match_list( buf, conf.news_hide_hl.c_str() ) )
					{
					run_len++;
					if( write( fd, "\x1", 1 ) != 1 )        // Add ^A
						return Err;
					pass = Yes;
					}

d410 5
d424 1
a424 1
		if( write( fd, buf, len ) != len )				// Write line
d426 1
a426 1
		if( write( fd, "\r", 1 ) != 1 )					// Add hard CR
d448 1
a448 1
        if( writef( fd,	"\r\n---\r\n * Origin: %.*s (%s)\r\n",
d455 1
a455 1
    if( put_path_seenby( fd ) == Err )
d459 1
a459 1
	if( write( fd, "", 1 ) != 1 )					// Terminating zero byte
d466 1
a466 1
put_path_seenby( int fd )
d474 1
a474 1
	    if( writef( fd, "SEEN-BY: %d/%d\r\n",
d513 1
a513 1
        if( writef( fd, "SEEN-BY: %d/%d %d/%d\r\n", net1, node1, net2, node2 ) == Err )
d517 1
a517 1
	if( writef( fd, "\x1PATH: %d/%d\r\n",
d532 1
a532 1
	int		fd;									// Echo packet file
d539 1
a539 1
	if( (fd = get_packet_file()) < 0 )			// Find/create packet file
d542 1
a542 1
	lseek( fd, 0L, SEEK_CUR );					// ?? Over last delimiter
d551 1
a551 1
		if( write( fd, &msg_type, 2 ) != 2 )	// Write delimiter
d554 1
a554 1
		if(	copy_data( in, fd, ui, edef, retain_groupname,
d575 2
d620 1
a620 1
	const	bl = 16384;
d629 1
a629 1
		if( fwrite( buf, 1, l, fp ) != l )
d657 1
a657 1

d690 1
a690 1
write_pth( int fd, const char *path_a )
d706 1
a706 1
	if( writef( fd, "\x1PTH: " ) )
d719 1
a719 1
				if( writef( fd, "@@%s ", path ) )
d724 1
a724 1
				if( writef( fd, "@@%s ", el + 1 ) )
d733 1
a733 1
	if( writef( fd, "@@%s %s\r\n",
d763 1
a763 1
writef( int fd, char *fmt, ... )
d773 1
a773 1
	return write( fd, buffer, cnt ) == cnt ? Ok : Err;
d788 1
a788 1
	if( tmt < 0 )
d792 3
@


1.3
log
@IBM C Set version. Phew, it was not a piece of cake to get here...
@
text
@d8 3
d552 15
a566 2
		if( ((fd = _sopen( fn, O_WRONLY|O_TEXT, SH_DENYWR )) >= 0) 
		 && ((fp = fdopen( fd, "w" )) != NULL) )
d569 1
a569 4
		if( errno == EEXIST )
			continue;

		error( EI_Full, "Can't create %s", fn );
d578 1
a578 1
	const	bl = 512;
d625 1
a625 1
		return Err;
d628 1
a628 1
	printf("Echo: %s\n", (const char *)edef.area().c_str() );
@


1.2
log
@rewriting for C Set
@
text
@d8 3
a136 1
//#include	<dir.h>
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/unbatch/vcs/sendnews.c_v  $
d134 1
a134 1
#include	<dir.h>
d192 1
a192 1
	set_date( mh.date, ui.gm_time );
d238 1
a238 1
	clear_subj( ui.subject );
d316 1
a316 1
	while( in.gets( buf, bufsize ) != Err )
d346 1
a346 1
			if( retain_groupname && strncmpi( buf, "newsgroups:", 11 ) == 0 )
d349 1
a349 1
			if( pass_subject && strncmpi( buf, "subject:", 8 ) == 0 )
d375 1
a375 1
		r.rs( buf );
d545 4
a548 1
		if( (fp = _fsopen( fn, "wt", SH_DENYWR )) != NULL )
d699 1
a699 1
	if( strncmpi( s, "* origin", 8 ) == 0 )				// Origin?
d759 1
a759 1
	uchar	*p, *d;
d762 2
a763 2
		((p = strstr((char *)subj, "[NEWS]")) != NULL) ||
		((p = strstr((char *)subj, "[News]")) != NULL)
@
