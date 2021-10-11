head	1.2;
access;
symbols;
locks;
comment	@ * @;


1.2
date	95.08.07.13.57.10;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.08.07.13.28.26;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@Cleanup
@
text
@/****************************** UU2 Snews *********************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	.PKT to news article conversion: helpers
 *
 *      $Log: SendUtil.c $
 *      Revision 1.1  1995/08/07 13:28:26  dz
 *      Initial revision
 *
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





/****************************************************************************
				  From name & newsgroup name conversion
****************************************************************************/


bool
SnewsApp::make_from( FILE *out, const fido_user &ffm, echo_def &def, bool &registered, fido_info &fi )
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


bool
SnewsApp::get_from( uucp_addr &ufm, const fido_user &ffm, bool &registered )
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



bool
SnewsApp::make_ids( FILE *out, fido_info &fi )
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
	else if( strlen( fi.reply ) )				// H�� REPLY:? H� � �� ����...
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

time_t
SnewsApp::f2u_date( char *fido_date )
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


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: Send.c $
a28 12


/*
static bool     get_from( uucp_addr &ufm, const fido_user &ffm, bool &registered );
static bool		make_from( FILE *out, const fido_user &ffm, echo_def &def, bool &registered, fido_info &fi );
static bool		make_ids ( FILE *out, fido_info &fi );


static time_t	f2u_date( char *fido_date );
*/


@
