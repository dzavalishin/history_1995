head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	95.11.05.18.50.12;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.11.05.13.52.48;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.04.13.11.06.28;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.06.23.33.30;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.04.17;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@RFC_Msg used
@
text
@/************************ UU2 Conf. file parser ***************************\
 *
 *	Copyright (C) 1991,1992 by Infinity Soft
 *
 *	Module 	:	Setup
 *
 *      $Log: SetUp.c $
 *      Revision 1.4  1995/11/05 13:52:48  dz
 *      current.
 *
 * Revision 1.3  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.2  1995/04/06  23:33:30  dz
 * checkpoint
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *         Rev 1.15   07 Jun 1993 17:53:30   dz
 *      char * --> String
 *      
 *         Rev 1.14   15 Nov 1992 00:03:12   dz
 *      User list processing rewritten
 *      
 *         Rev 1.13   08 Nov 1992 22:15:58   dz
 *      def_file() bugfix, adding our FTN domain to user address
 *      if it has no one.
 *      
 *         Rev 1.12   06 Nov 1992 01:51:40   dz
 *      Defaults for hl pass/kill/hide
 *      
 *         Rev 1.11   23 Oct 1992 15:24:18   dz
 *      error()
 *      
 *         Rev 1.10   16 Oct 1992 03:34:56   dz
 *      Default dir/file names, dir. autocreation
 *      
 *         Rev 1.9   16 Oct 1992 01:52:44   dz
 *      LOT of changes, see diff
 *      
 *         Rev 1.8   23 Mar 1992 23:03:48   dz
 *      Default values moved to .h
 *      
 *         Rev 1.7   06 Mar 1992 18:02:38   dz
 *      Config file parser rewritten & moved out.
 *      parse_addr() -> fido_addr::parse()
 *      
 *         Rev 1.6   05 Mar 1992 21:13:58   dz
 *      PassUnknownBack
 *      
 *         Rev 1.5   03 Mar 1992 19:36:20   dz
 *      NB!!
 *      Rmail -> RmailExe
 *
 *         Rev 1.4   18 Feb 1992 14:12:58   dz
 *      InNewsSpool var
 *      NewsPath (thing to add to path: headline)
 *      rmail_exitr - bug in default fixed.
 *      
 *         Rev 1.3   04 Feb 1992 22:10:14   dz
 *      Userlist, rmail interface setup, etc...
 *
 *         Rev 1.2   11 Oct 1991 05:08:14   dz
 *      New conf. parameters aded
 *      Bug in echo table parser fixed
 *      
 *         Rev 1.1   12 Sep 1991 01:43:16   dz
 *      A lot of conf. file parameters added, echo-news list added
 *      
 *         Rev 1.0   28 Aug 1991 01:01:36   dz
 *      Initial revision.
 *
 *
\*/


#include	<string.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<direct.h>
#include	<ctype.h>

#include	"setup.h"
#include	"log.h"
#include	"defset.h"
#include	"userlist.h"
#include	"..\..\version.h"

#ifndef	FTN_MSG_H
#include	<ftn_msg.h>
#endif


bool		log_debug;
setup		conf;

static void     def_dir( const char *base, String &out, const char *def );
static void     def_file( const char *base, String &out, const char *def );

static void		load_userlist( const char *ulfname );


#ifdef __IBMCPP__
#	define	TEXT_MODE
#else
#	define	TEXT_MODE "t"
#error Need C Set!
#endif

const int		bd_len = 200;


FILE *
open_setup( char *base_dir )
	{
	char	*sdir;
	char	sfname[100];
	FILE	*fp;

	sdir						= getenv("U1BETA");	// For testing purposes
	if( sdir == NULL )	sdir	= getenv("U1");

	if( sdir )
		{
		char	&c = sdir[strlen(sdir)-1];				// Cut off trailing
		if( c == '\\' || c == '/' )						// slash or backslash
			c = '\0';

		strncpy( base_dir, sdir, bd_len );
		base_dir[bd_len-1] = '\0';
		}
	else
		{
		error( EI_None, "Environment variable 'U1' is not defined. Trying to proceed...");
		strcpy( base_dir, "." );
		sdir = ".";
		}

	sprintf( sfname, "%s\\SETUP.U1", sdir );

	if( (fp = fopen( sfname, "r" TEXT_MODE )) != NULL )
		{
		if( setvbuf( fp, NULL, _IOFBF, 16*1024 ) )
			warning( EI_Full, "Can't setvbuf for config to 16K?" );
		return fp;
		}

	return NULL;
	}


bool
get_setup( void )
	{
	const   buf_len = 1024;
	char	buf[buf_len];
	FILE	*fp;
	char	base_dir[bd_len+1];

	fp = open_setup( base_dir );
	if( fp == NULL )
		{
		printf
			(
			"Error: can't open control file SETUP.U1\n"
			"Either set U1=control_file_dir or cd control_file_dir\n"
			);

		exit(33);
		}





	conf.at_to_percent_in_from_ = Yes;
        conf.send_error_notification = Yes;
        conf.pass_unknown_along = Yes;



	while( fgets( buf, buf_len, fp ) != NULL )
		{
		char	*p;
		bool	good = No;

		if( (p = strpbrk( buf, ";" )) != NULL )
			*p = '\0';

		for( p = buf; *p; p++ )
			{
            if( *p > ' ' )    {  good = Yes;  break;    }
			}

		if( !good )     continue;

		strcpy( buf, p );

		if( !memicmp( buf, "debug", 5 ) )
			{
			log_debug = Yes;
			continue;
			}

		parse_ctl_str( buf );
		}

	fclose( fp );

	check_required_keywords();		// Did we got everything really important?

	if(
//		conf.our_addr.net()			== 0	||
//		strlen( conf.domain )		== 0	||
		strlen( conf.netmail_dir.c_str() )	== 0	||
		strlen( conf.postmaster.c_str() )	== 0
		)
		{
		fatal( EC_Incorrect_Setup, EI_None, "Setup is not correct: check address/domain/netmail/postmaster fields\n");
		}
/*
	{
	char	tempo[buf_len];
	strncpy( tempo, conf.domain, buf_len );
	tempo[buf_len-1] = '\0';
	strlwr( tempo );
	conf.domain = tempo;
	}
*/
//
//      Ok, fill in defaults
//


	if( strlen( conf.rmail_exe_name.c_str() ) == 0 )            // Default rmail name
		conf.rmail_exe_name = (const char *)DEF_RMAIL_EXE;

// BUG! Redo!
	// for uupc
	if( conf.rmail_exitl == 0 )     conf.rmail_exitl = DEF_REXITL;
	if( conf.rmail_exitr == 0 )     conf.rmail_exitr = DEF_REXITR;

//	if( strlen( conf.store_dir ) == 0 )                 // Where to put outbound files
//		strcpy( conf.store_dir, conf.netfile_dir );		// Use inbound

	if( strlen( conf.gate_id.c_str() ) == 0 )					// Gate Id
		{
		char	b[100];
		sprintf( b, "(Gid:%.72s)", (const char *)ftn::def().uucp_d.c_str() );
		conf.gate_id = (const char *)b;
		}

	if( strlen( conf.attach_stem.c_str() ) == 0 )
                conf.attach_stem = (const char *)DEF_ATTACH_STEM;


// BUG! Redo!
	if( strlen( conf.code_fu.c_str() ) == 0 )
		conf.code_fu = (const char *)DEF_CODE_FU;

	if( strlen( conf.code_uf.c_str() ) == 0 )
		conf.code_uf = (const char *)DEF_CODE_UF;

	if( strlen( conf.news_path.c_str() ) == 0 )
		conf.news_path = ftn::def().uucp_d;

	if( conf.zip_limit == 0 )
		conf.zip_limit = DEF_ZIP_LIMIT;

	if( conf.pkt_size == 0 )
		conf.pkt_size = DEF_PKT_SIZE;

    if( conf.from_field_size == 0 )
        conf.from_field_size = DEF_FIDO_FROM_FIELD_SIZE;

    if( conf.from_field_size > MAX_FIDO_FROM_FIELD_SIZE )
        {
        error( EI_None, "Message 'from' filed size can't be > %d", MAX_FIDO_FROM_FIELD_SIZE );
        conf.from_field_size = MAX_FIDO_FROM_FIELD_SIZE;
        }


    if( conf.part_size == 0 )
		conf.part_size = DEF_PART_SIZE;

	if( strlen( conf.log_level.c_str() ) == 0 )
		conf.log_level = (const char *)DEF_LOG_LEVEL;

//	if( strlen( conf.free_domain ) == 0 )
//		conf.free_domain = (const char *)DEF_FREE_DOMAIN;

// HeadLines

	if( strlen( conf.mail_pass_hl.c_str() ) == 0 )
		conf.mail_pass_hl = (const char *)DEF_MAIL_PASS_HL;

	if( strlen( conf.mail_hide_hl.c_str() ) == 0 )
		conf.mail_hide_hl = (const  char *)DEF_MAIL_HIDE_HL;

	if( strlen( conf.mail_kill_hl.c_str() ) == 0 )
		conf.mail_kill_hl = (const char *)DEF_MAIL_KILL_HL;

	if( strlen( conf.news_pass_hl.c_str() ) == 0 )
		conf.news_pass_hl = (const char *)DEF_NEWS_PASS_HL;

	if( strlen( conf.news_hide_hl.c_str() ) == 0 )
		conf.news_hide_hl = (const char *)DEF_NEWS_HIDE_HL;

	if( strlen( conf.news_kill_hl.c_str() ) == 0 )
		conf.news_kill_hl = (const char *)DEF_NEWS_KILL_HL;


// Check base dir
	if( strlen( base_dir ) > 64 )
		{
		char *defbase = "C:\\U1";
		error( EI_None, "Base dir name (%s) is too long, using default: %s",
			base_dir, defbase );
		strcpy( base_dir, defbase );
		}




// Set default directory names

	def_dir( base_dir, conf.flag_dir,     DEF_FLAG_DIR    );
	def_dir( base_dir, conf.stat_dir,     DEF_STAT_DIR    );
	def_dir( base_dir, conf.newsin_dir,   DEF_NEWSIN_DIR  );
	def_dir( base_dir, conf.newsout_dir,  DEF_NEWSOUT_DIR );
	def_dir( base_dir, conf.store_dir,    DEF_STORE_DIR   );
	def_dir( base_dir, conf.filein_dir,   DEF_FILEIN_DIR  );
	def_dir( base_dir, conf.fileout_dir,  DEF_FILEOUT_DIR );

// Set default file names

	def_file( base_dir, conf.userlist_file, DEF_USERLIST_FILE );  // Has no effect since this is required kw
	def_file( base_dir, conf.echolist_file, DEF_ECHOLIST_FILE );
	def_file( base_dir, conf.feedlist_file, DEF_FEEDLIST_FILE );
	def_file( base_dir, conf.rulelist_file, DEF_RULELIST_FILE );
	def_file( base_dir, conf.newusr_file,   DEF_NEWUSR_FILE );

// Well, what's with UUCP package type?

	switch( conf.uucp )
		{
	case UUCP_Unknown:								// Huh?
	default:
		conf.uucp = UUCP_Ache30;					// Widespread one (In Russia, of course!)
		// Fall to..

	case UUCP_Ache30:								// The first one :)
		conf.uucp_rmail_ffrom = Yes;				// rmail -f <from_addr>
		break;

	case UUCP_11R:
		conf.uucp_rmail_ffrom = No;                 // don't use -f flag
		break;

	case UUCP_Waffle:
		conf.uucp_rmail_ffrom = No;                 // don't use -f flag
		break;

		}

    fido_addr::def_zone  ( ftn::def().fido_a.zone() );
    fido_addr::def_domain( ftn::def().fido_a.domain() );

	load_userlist( conf.userlist_file.c_str() );

	return Ok;
	}

void
def_dir( const char *base, String &out, const char *def )
	{
	if( strlen( out.c_str() ) == 0 )
		{
		char	b[150];
		sprintf( b, "%.64s\\%.14s", base, def );	// 79 chars total
		out = (const char *)b;
//        debug("Default (%s) directory name used", out );
		}

	if( access( out.c_str(), 0 ) == 0 )						// Exist
		return;

	// Begin from the first subdirectory, creating each if needed
	char		d[80];
	const char	*p = out.c_str();

	while( (p = strchr( p+1, '\\' )) != NULL )
		{
		if( p == out )	continue;					// Leading slash, skeep
		if( p-(out.c_str()) == 2 && *(p-1) == ':' )			// Drive letter
			continue;

		strncpy( d, out.c_str(), (int)(p-(out.c_str())) );
		d[(int)(p-(out.c_str()))] = '\0';

		if( access( d, 0 ) == 0 )					// Fragment exist?
			continue;								// Ok...

		if( _mkdir( d ) == 0 )						// Created?
			continue;

// Merge dup. strings
//#pragma option -d

		error( EI_Full, "Can't create dir (%s)", d );
		return;
		}

	if( access( out.c_str(), 0 ) == 0 )						// Exist
		return;

	if( _mkdir( (char *)(out.c_str()) ) == 0 )							// Created?
		return;

	error( EI_Full, "Can't create dir (%s)", (const char *)out.c_str() );
	}


void
def_file( const char *base, String &out, const char *def )
	{
	if( strlen( out.c_str() ) == 0 )
		{
		char	b[150];
		sprintf( b, "%.64s\\%.14s", base, def );	// 79 chars total
		out = (const char *)b;
//      debug("Default (%s) file name used", out );
		}
	}


	/*************************************************************
						   Load users list
	*************************************************************/


void
load_userlist( const char *ulfname )
	{
	const   buf_len = 1024;
	char	buf[buf_len];
	FILE	*fp;

	if( (fp = fopen( ulfname, "r" TEXT_MODE )) == NULL )
		{
		error( EI_Full, "can't open userlist file %s", ulfname );
		return;
		}

	if( setvbuf( fp, NULL, _IOFBF, 32*1024 ) )
		warning( EI_Full, "Can't setvbuf for userlist to 32K?" );

	while( fgets( buf, buf_len, fp ) != NULL )
		{
		char	*p;

		if( (p = strpbrk( buf, ";" )) != NULL )
			*p = '\0';

		for( p = buf; *p; p++ )
			if( *p > ' ' )
				break;

		if( *p <= ' ' )
			continue;

		fuser.add( p );
		}

	fclose( fp );
	}



/***************************************************************************/

char message_about_who_wrote_all_ths_stuff[] =
	"\r\n\r\n"
    "This program was written by dz (Dmitry Zavalishin), Phantom of Infinity Soft Labs\r\n"
    "Configuration module $Id: SetUp.c 1.4 1995/11/05 13:52:48 dz Exp $, version " U1_VER_STR "\r\n"
	"\r\n\r\n";


















@


1.4
log
@current.
@
text
@d8 3
d91 2
a92 2
#ifndef	FIDO_MSG_H
#include	<fido_msg.h>
d487 1
a487 1
    "Configuration module $Id$, version " U1_VER_STR "\r\n"
@


1.3
log
@debug
@
text
@d8 3
d86 1
d93 1
a93 1
extern bool		log_debug;
d154 2
a155 1
	char	buf[200];
d176 2
d181 1
a181 2

	while( fgets( buf, 200, fp ) != NULL )
d222 3
a224 3
	char	tempo[200];
	strncpy( tempo, conf.domain, 200 );
	tempo[199] = '\0';
d444 2
a445 1
	char	buf[200];
d457 1
a457 1
	while( fgets( buf, 200, fp ) != NULL )
d484 1
@


1.2
log
@checkpoint
@
text
@d8 3
d76 1
d98 7
d136 1
a136 1
	if( (fp = fopen( sfname, "rt" )) != NULL )
d397 1
a397 1
		if( mkdir( d ) == 0 )						// Created?
d401 1
a401 1
#pragma option -d
d403 1
a403 1
		error( EI_Full, "Can't create dir (%s) - %s", d, sys_errlist[errno] );
d410 1
a410 1
	if( mkdir( out.c_str() ) == 0 )							// Created?
d413 1
a413 1
	error( EI_Full, "Can't create dir (%s) - %s", (const char *)out.c_str(), sys_errlist[errno] );
d441 1
a441 1
	if( (fp = fopen( ulfname, "rt" )) == NULL )
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/lib/uu2/vcs/setup.c_v  $
d154 8
@
