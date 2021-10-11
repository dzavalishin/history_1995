/****************************** UU2 Snews *********************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	.PKT to news article conversion
 *
 *      $Log: Send.c $
 *      Revision 1.5  1995/08/07 13:57:10  dz
 *      Cleanup
 *
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
        
        if( verbose )
            fprintf( stderr, "Area: %s --> %s\n", (const char *)letter_info.area, (const char *)group.c_str() );
        else
            fprintf( stderr, "." );
        fflush(stderr);
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
	fprintf( out, "Sender: u1@%s\n", (const char *)conf.def_domain.c_str() );
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



