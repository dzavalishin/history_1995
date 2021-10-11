head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	95.08.07.13.57.10;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.08.07.13.28.26;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.02.13.38.18;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.09.23.24.27;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.31.14;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@Cleanup
@
text
@/****************************** UU2 Snews *********************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	.PKT to news article conversion
 *
 *      $Log: Send.c $
 *      Revision 1.4  1995/08/07 13:28:26  dz
 *      Using class App
 *
 *      Revision 1.3  1995/08/02 13:38:18  dz
 *      using a lot of info from original message, if available,
 *      to recreate message better if double gateing occured.
 *
 *      Revision 1.2  1995/04/09  23:24:27  dz
 *      IBM C Set version
 *
 *      Revision 1.1  1995/03/11  18:31:14  dz
 *      Initial revision
 *
 *      
 *         Rev 1.10   07 Jun 1993 17:12:46   dz
 *      Rearranged, MSGID generation fixes.
 *      
 *         Rev 1.9   28 Nov 1992 22:25:14   dz
 *      using new echo/group list interface, new spool interface,
 *      supporting new echolist options, style fixes.
 *      
 *         Rev 1.8   23 Oct 1992 14:58:02   dz
 *      error()
 *      
 *         Rev 1.7   18 Sep 1992 03:10:14   dz
 *      statistics
 *
 *         Rev 1.6   19 Jul 1992 23:25:02   dz
 *      New area2group mechanism used.
 *
 *         Rev 1.5   18 Jun 1992 11:10:36   dz
 *      Formatting & date conversion
 *      
 *         Rev 1.4   07 Apr 1992 20:46:08   dz
 *      Seems to be working version, but withno formatting.
 *      
 *         Rev 1.3   07 Apr 1992 19:11:04   dz
 *      echo2uu converted to snews
 *      
 *         Rev 1.2   04 Feb 1992 00:02:42   dz
 *      Added Message-Id, References, Path, turn-back check logic
 *      magic word now taken from conf-file, Fido-type From: address
 *      corrected.
 *      
 *         Rev 1.1   11 Oct 1991 05:19:32   dz
 *      Backfeed identification done using Organization line
 *
 *         Rev 1.0   06 Oct 1991 00:20:30   dz
 *      Initial revision.
 *
 *
\*/

#include	"snews.h"
#include	"..\..\version.h"
#include	"..\..\u1.h"
#include	<fido_pkt.h>
#include	<uucpaddr.h>
#include	<userlist.h>
#include	<new_usr.h>
#include	<datec.h>
#include	<elist.h>
#include	<spool.h>

#include	"stat.h"

#define		BUFS	500






bool
SnewsApp::copy_letter( FILE *out, FILE *fp )
	{
	char			buf[ BUFS ];
	bool			got_area = No, endtext = No;


	while( 1 )
		{
		bool	r;

		if( ( r = read_form( buf, fp  )) != Yes )	// Next line
			{
			if( (r == Err) || ferror( fp ) )
				{
				error( EI_Full, "Problems reading packet" );
				return Err;
				}

			break;
			}

		if( buf[0] == '\x1' )						// Kludge
			{
			if( (!got_area) && strnicmp( buf+1, "area:", 5 ) == 0 )
				got_area = Yes;

			continue;
			}

		if( (!got_area) && strnicmp( buf, "area:", 5 ) == 0 )
			{
			got_area = Yes;
			continue;
			}

		if( !strnicmp( buf, "SEEN-BY:", 8)) endtext = Yes;

		if( got_area && !endtext )
			{
			fputs( buf, out );						// So it's text follows
			fputs( "\n", out );
			}
		}

	return Ok;
	}




bool
SnewsApp::do_send( news_out_spool_file & spf, FILE *fp )
	{
	FILE			*out = spf.fp();

	fido_info		letter_info;					// Original letter info
	fido_pmsg_hdr	ph;

	char			ph_date[21];
	char			to_name[40];
	char			from_name[40];
	char			subject[80];
	String          group;
	time_t			t;

	bool			err			= No;
	bool			abort_it	= No;

	/*************************************************************
					Read packed message header
	*************************************************************/

	if( fread( &ph, sizeof( ph ), 1, fp ) != 1 )
		{
		error( EI_Full, "Can't read packed message header!" );
		return Err;
		}

	if( readl( ph_date, 21, fp ) == Err )
		{
		error( EI_Full, "Can't read packed message date!" );
		return Err;
		}

	if( readl( to_name, 37, fp ) == Err )
		{
		error( EI_Full, "Can't read packed message 'To:' field!" );
		return Err;
		}

	if( readl( from_name, 37, fp ) == Err )
		{
		error( EI_Full, "Can't read packed message 'From:' field!" );
		return Err;
		}

	if( readl( subject, 79, fp ) == Err )
		{
		error( EI_Full, "Can't read packed message subject!" );
		return Err;
		}


	/*************************************************************
						  Copy contents
	*************************************************************/

	read_origin( letter_info, fp );
	letter_info.origin.name( from_name );

	echo_def    def;

	if( def.find_by_area( letter_info.area ) == Err )
		{
		if( conf.news_bad_group_msg != MSG_Pass )
			{
			group = (const char *) "unknown";
			abort_it = Yes;
			error( EI_None, "Message to unknown echo area %s skipped", letter_info.area );
			}
		group = (const char *)letter_info.area;
		}
	else
		{
		group = def.group();
		fprintf( out, "Newsgroups: %s\n", (const char *)group.c_str() );
		}

	fprintf( stderr, "Area: %s --> %s\n", (const char *)letter_info.area, (const char *)group.c_str() );
	log( "E", "Area %s --> %s", (const char *)letter_info.area, (const char *)group.c_str() );

	bool	registered;		// Reg'd user?

	if( make_from( out, letter_info.origin, def, registered, letter_info ) == Err )
		return Err;

	if( !def.pass_to_uucp() )
		{
		error( EI_None, "Group %s: FIDO->UUCP transfer disabled", letter_info.area );
		abort_it = Yes;
		}

	if( def.pass_fu_reg_only() && (!registered) )
		{
		error( EI_None, "Group %s: transfer disabled: User is not registered", letter_info.area );
		abort_it = Yes;
		}

	t = f2u_date( (char *)ph_date );

	if( strlen( subject ) == 0 )
		strcpy( subject, "<None>" );

	make_ids( out, letter_info );
	fprintf( out, "Subject: %s\n", (const char *)subject );
	fprintf( out, "Sender: u1@@%s\n", (const char *)conf.def_domain.c_str() );
	fprintf( out, "Date: %s\n", timeline( t, (const uchar *)conf.tz.c_str() ) );
	fprintf( out, "X-Gate: U1 %s\n", U1_VER_STR );

	if( strlen((const char *)conf.organization.c_str()) )
		fprintf( out, "Organization: %s %s\n", (const char *)conf.organization.c_str(), (const char *)conf.gate_id.c_str() );
	else
		fprintf( out, "Organization: %s %s\n", (const char *)letter_info.origin_text, (const char *)conf.gate_id.c_str() );


	if( stricmp( to_name, "all" ))
		fprintf( out, "X-FTN-To: %s\n", to_name );

	if( strlen(letter_info.msgid ))
		fprintf( out, "X-FTN-MsgId: %s\n", letter_info.msgid );

        if( strlen( letter_info.reply ))
		fprintf( out, "X-FTN-Reply: %s\n", letter_info.reply );

	if( strlen( def.approved().c_str() ) )
		fprintf( out, "Approved: %s\n", (const char *)def.approved().c_str() );

	if( strlen( def.distrib().c_str() ) )
		fprintf( out, "Distribution: %s\n", (const char *)def.distrib().c_str() );

	// End of RFC header

	fprintf( out, "\n" );

	if( copy_letter( out, fp ) == Err )				// Send as a letter
		err = Yes;

	fflush( out );									// To be sure size is correct

	if( (!err) && (!abort_it) )
		log( "$", "\"Echo\", \"%s\", \"%s - %s\", \"from fido\", %ld",
			(const char *)((String)letter_info.origin).c_str(),	letter_info.area,
			(const char *)group.c_str(),	filelength( fileno( out ))
		);

	if( abort_it )						// We can't send this stuff
		{
		spf.abort();					// So kill it
		return Ok;						// But let us process next letter
		}

	if( err )
		return Err;
	else
		{
		if( spf.close() == Err )
			return Err;

// NB!
//
// It is useless now to add path info to
// message list, since we do not convert
// FIDO path to RFC one. It will be better 
// to do that later though.

		if( spf.post( (const char *)group.c_str() ) == Err )
			return Err;
		}

	return Ok;
	}


bool
SnewsApp::send( FILE *fp )
	{
	news_out_spool_file		spf;

	if( spf.create() == Err )
		return Err;

	if( do_send( spf, fp ) == Err )
		return Err;

	add_stat_news();	// Increase couter of sent articles (for log '%' record)

	return Ok;
	}



@


1.4
log
@Using class App
@
text
@d8 3
d15 2
a16 2
 * Revision 1.2  1995/04/09  23:24:27  dz
 * IBM C Set version
d18 2
a19 2
 * Revision 1.1  1995/03/11  18:31:14  dz
 * Initial revision
a76 8
/*
static bool     get_from( uucp_addr &ufm, const fido_user &ffm, bool &registered );
static bool		make_from( FILE *out, const fido_user &ffm, echo_def &def, bool &registered, fido_info &fi );
static bool		make_ids ( FILE *out, fido_info &fi );


static time_t	f2u_date( char *fido_date );
*/
@


1.3
log
@using a lot of info from original message, if available,
to recreate message better if double gateing occured.
@
text
@d8 4
d74 1
a74 1

d81 1
d86 2
a87 3

static bool
copy_letter( FILE *out, FILE *fp )
d137 2
a138 2
static bool
do_send( news_out_spool_file & spf, FILE *fp )
d311 1
a311 1
send( FILE *fp )
a325 287


/****************************************************************************
				  From name & newsgroup name conversion
****************************************************************************/


static bool
make_from( FILE *out, const fido_user &ffm, echo_def &def, bool &registered, fido_info &fi )
	{
	uucp_addr	ufm;
	const char	*realname = (const char *)ffm.name();

	if( get_from( ufm, ffm, registered ) == Err )
		return Err;

        if( fi.RFC_From.length() )
                ufm=fi.RFC_From.c_str();
	else if( fi.REPLYADDR.length() )
		{
		const char *p1 = strchr( fi.REPLYADDR.c_str(), '@@' );
		const char *p2 = strchr( fi.REPLYADDR.c_str(), '.' );

		// we have both '@@' & '.', and '.' is to the right from '@@'
		if( p1 != 0 && p2 != 0 && p2 > p1 )
	                ufm=(const char *)fi.REPLYADDR.c_str();
		}


	fprintf( out, "From: %s <%s>\n", realname, (const char *)ufm );
	printf("From: %s <%s>\n", realname, (const char *)ufm );

	if( def.send_rules() && def.fido_rules() && rules_sender::active() )
		rules_sender::add_address( def.group().c_str(), ufm );

	return Ok;
	}

/****************************************************************************
						From: conversion
****************************************************************************/


static bool
get_from( uucp_addr &ufm, const fido_user &ffm, bool &registered )
	{
	fido_user_def	ud;

	if( fuser.get( ud, ffm ) != Err )
		{
		char	buf[200];
		sprintf( buf, "%s@@%s", (const char *)ud.uu_name().c_str(), (const char *)ud.uu_from_domain().c_str() );
		ufm = buf;
		debug("get_from: %s", (const char *)ufm );
		registered = Yes;
		return Ok;
		}

	registered = No;

	ftn_def   def;

    if( Ok != ftn::match( def, ffm ) )
        {
        error( EI_None, "Can't find corresponding FTN for '%s', using default uucp domain", (const char*)((String)ffm).c_str() );

        if( ffm.convert_to_uucp( ufm, conf.def_domain.c_str() ) == Err )
                {
                error( EI_None, "Can't convert FIDO address (%s) to uucp form",
                        (const char *)((String)ffm).c_str() );

                ufm = "/dev/null";

                return Ok;
                }

        return Ok;
        }

	if( ffm.convert_to_uucp( ufm, def.uucp_d.c_str() ) == Err )
		{
		error( EI_None, "Can't convert FIDO address (%s) to uucp form",
			(const char *)((String)ffm).c_str() );
		return Err;
		}

	debug( "get_from: %s", (const char *)ufm );
	return Ok;
	}




/****************************************************************************
						Message-Id/References
****************************************************************************/

static bool
test_ftn_addr( uucp_addr &out, const char *in )
	{
	int	d;

	if( 
		3 != sscanf( in, "%d:%d/%d", &d, &d, &d ) &&
		2 != sscanf( in, "%d/%d", &d, &d )
	)
		return No;

	fido_addr	ffm;

	if( ffm.aparse( in ) == Err )
		return Err;

	ftn_def   def;

	if( Ok != ftn::match( def, ffm ) )
		{
		error( EI_None, "Can't find corresponding FTN for '%s', using default uucp domain", (const char*)((String)ffm).c_str() );

		if( ffm.convert_to_uucp( out, conf.def_domain.c_str() ) == Err )
			{
			error( EI_None, "Can't convert FIDO address (%s) to uucp form", (const char *)((String)ffm).c_str() );
			return Err;
			}

		return Yes;
		}

	if( ffm.convert_to_uucp( out, def.uucp_d.c_str() ) == Err )
		{
		error( EI_None, "Can't convert FIDO address (%s) to uucp form",
			(const char *)((String)ffm).c_str() );
		return Err;
		}

	debug( "MSGID addr: %s", (const char *)out );
	return Yes;
	}


static bool
make_one_id( FILE *out, char *name, char *src )
	{
	uucp_addr	id_u_addr;
	char		id_s_addr[100], id_s_stamp[100];

	if( 2 != sscanf( src, "%s %s", id_s_addr, id_s_stamp ))
		{
		error( EI_None, "Can't parse ID for %s: %s", name, src );
		return Err;
		}

	if( test_ftn_addr( id_u_addr, id_s_addr ) != Yes )
		fprintf( out, "%s: <%s@@%s>\n", name, id_s_stamp, id_s_addr );
	else
		fprintf( out, "%s: <%s@@%s>\n", name, id_s_stamp, (const char *)id_u_addr );

	return Ok;
	}



static bool
make_ids( FILE *out, fido_info &fi )
	{
	static int	inc = 0;

	if( strlen( fi.RFC_Message_Id.c_str() ) )
		{
		debug( "reusing mesage-id %s from kludge", (const char *)fi.RFC_Message_Id.c_str() );
		fprintf( out, "Message-Id: %s\n", (const char *)fi.RFC_Message_Id.c_str() );
		}
	else if( strlen( fi.msgid ) )
		{
		if( make_one_id( out, "Message-Id", fi.msgid ) != Ok )
			fprintf( out, "Message-Id: <U1.%ld.%d@@%s>\n", time(NULL), inc++, (const char *)conf.def_domain.c_str() );
		}
	else
		error( EI_None, "No MSGID in article (Origin = %s)", (const char *)((String)fi.origin).c_str() );

	if( strlen( fi.RFC_References.c_str() ) )
		{
		fprintf( out, "References: %s\n", (const char *)fi.RFC_References.c_str() );
		}
	else if( strlen( fi.reply ) )				// Hет REPLY:? Hу и не надо...
		make_one_id( out, "References", fi.reply );

// BUG! Contents of fidonet path and pth must be added here!
	if( strlen( fi.RFC_Path.c_str() ) )
		{
		fprintf( out, "Path: %s!%s\n", (const char *)conf.news_path.c_str(), (const char *)fi.RFC_Path.c_str() );
		}
	else
		fprintf( out, "Path: %s!echogate\n", (const char *)conf.news_path.c_str() );


	if( strlen( fi.RFC_Distribution.c_str() ) )
		fprintf( out, "Distribution: %s\n", (const char *)fi.RFC_Distribution.c_str() );


	return Ok;
	}


/****************************************************************************
					 Date conversion: FIDO->UseNet
****************************************************************************/

// Month parser taken from getindate.c

#define     PACK_TWO_CHARS(c1, c2)   (((c1)<<8)|(c2))

static int
getmon( char *m )
	{
	strupr( m );

	switch (PACK_TWO_CHARS( m[0], m[1])) {

	case PACK_TWO_CHARS('J', 'A'):				return 0;
	case PACK_TWO_CHARS('F', 'E'):      		return 1;
	case PACK_TWO_CHARS('M', 'A'):   /* March, May */
		return ( m[2] == 'r' || m[2] == 'R' ? 2 : 4);
	case PACK_TWO_CHARS('A', 'P'):				return 3;
	case PACK_TWO_CHARS('J', 'U'):
		return ( m[2] == 'l' || m[2] == 'L') ? 6 : 5;
	case PACK_TWO_CHARS('A', 'U'):				return 7;
	case PACK_TWO_CHARS('S', 'E'):				return 8;
	case PACK_TWO_CHARS('O', 'C'):				return 9;
	case PACK_TWO_CHARS('N', 'O'):				return 10;
	case PACK_TWO_CHARS('D', 'E'):				return 11;

	default:
		return -1;      /* bad month name */
		}
	}

static time_t
f2u_date( char *fido_date )
	{
	char    mon[4], wday[4];
	int     hour, min, sec, day, year;

	struct tm	bdt;


	if( 6 == sscanf( fido_date, "%d %3s %d %d:%d:%d",
					&day, mon, &year, &hour, &min, &sec ))
		{
		wday[0] = '\0';
		}
	else
		{
		if( 6 != sscanf( fido_date, "%3s %d %3s %d %d:%d",
					wday, &day, mon, &year, &hour, &min ))
			{
			error( EI_None, "Can't parse date: %s", fido_date );
			return time(NULL);
			}
		else
			sec = 0;
		}

	bdt.tm_sec = sec;
	bdt.tm_min = min;
	bdt.tm_hour = hour;
	bdt.tm_mday = day;
	bdt.tm_mon = getmon( mon ); // 0-11
	bdt.tm_year = year;
	bdt.tm_isdst = 0;

	if( bdt.tm_mon  < 0 )
		{
		error( EI_None, "Invalid month: %s", fido_date );
		return time(NULL);
		}

	time_t	t = mktime( &bdt );

	if( t < 0 )
		{
		error( EI_None, "Can't convert fido date: %s", fido_date );
		time( &t );
		}

	return t;
	}
@


1.2
log
@IBM C Set version
@
text
@d8 3
d72 1
a72 1
static bool		make_from( FILE *out, const fido_user &ffm, echo_def &def, bool &registered );
d141 1
d161 6
d201 1
d216 1
a216 1
	if( make_from( out, letter_info.origin, def, registered ) == Err )
d231 1
a231 1
	t = f2u_date( (char *)ph.date );
d330 1
a330 1
make_from( FILE *out, const fido_user &ffm, echo_def &def, bool &registered )
d333 1
d338 15
a352 3
	fprintf( out, "From: %s <%s>\n", (const char *)ffm.name(), (const char *)ufm );
//	fprintf( out, "From: %s <%s>\n", ffm.name(), (const char *)ufm );
	printf("From: %s <%s>\n", (const char *)ffm.name(), (const char *)ufm );
d489 6
a494 1
	if( strlen( fi.msgid ) )
d502 5
a506 1
	if( strlen( fi.reply ) )				// Hет REPLY:? Hу и не надо...
d509 12
a520 1
	fprintf( out, "Path: %s!echogate\n", (const char *)conf.news_path.c_str() );
d599 9
a607 1
	return mktime( &bdt );
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/snews/vcs/send.c_v  $
d103 1
a103 1
			if( (!got_area) && strncmpi( buf+1, "area:", 5 ) == 0 )
d109 1
a109 1
		if( (!got_area) && strncmpi( buf, "area:", 5 ) == 0 )
d115 1
a115 1
		if( !strncmpi( buf, "SEEN-BY:", 8)) endtext = Yes;
d220 1
a220 1
	t = f2u_date( ph.date );
d228 1
a228 1
	fprintf( out, "Date: %s\n", timeline( t, (const char *)conf.tz.c_str() ) );
d358 1
a358 1
    ftn_def   def;
@
