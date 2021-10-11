head	1.4;
access;
symbols;
locks;
comment	@ * @;


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


1.4
log
@current.
@
text
@/************************ UU2 Conf. file parser ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Setup - parser
 *
 *      $Log: Set_Tab.C $
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
 *         Rev 1.5   07 Jun 1993 17:52:14   dz
 *      Rearranged (again!)
 *      
 *         Rev 1.4   15 Nov 1992 00:04:10   dz
 *      Announce keyword added, bool type: Yes/No supported
 *      
 *         Rev 1.3   06 Nov 1992 01:52:46   dz
 *      enumerated values added, table reorganized
 *      
 *         Rev 1.2   23 Oct 1992 15:30:02   dz
 *      error()
 *      
 *         Rev 1.1   16 Oct 1992 03:39:54   dz
 *      'required keyword' flag
 *      uucp type
 *      file.in/file.out directories
 *      
 *         Rev 1.0   22 Mar 1992 19:20:38   dz
 *      Initial revision.
 *
 *
\*/


#include	<string.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<ctype.h>

#include	"setup.h"
#include	"ftn.h"
#include	"log.h"


enum bftype							// Buffer type
	{
	BNone,							// No parameter buf.
	BString,						// String *
	BBool,							// bool
	BInt,							// int
    BEnum                           // list of keywords
	};

enum kw_req
	{
	NotRequired,
	Required
	};


struct sec_tab
    {
    char    *kw;                    // Keyword itself
    int     val;                    // Associated value
    };

struct sec_kw
    {
    int     *val;                   // Value pointer
    sec_tab *tab;                   // Value/Keyword table
    };

struct skeyw
	{
	char	*name;					// Conf. file keyword
	void	*value;					// Pointer to buffer (where to store)
	int		vlen;					// Buffer size (including trailing zero)
	bftype	type;					// Buffer type
	bool	(*func)( char * );		// Function to call
	kw_req	required;				// When setup loader founds keyword it
									// resets this flag. If after loading all
									// the setup some flag is still set it
									// cries loudly.
	};

//bool		set_get_addr	( char * );
//bool		set_get_newsaddr( char * );

bool		set_get_ftn		( char * );
bool		set_get_echoaddr( char * );

bool		set_get_freedom	( char * );
bool		set_open_log	( char * );
bool		set_load_ul		( char * );
bool		set_load_el		( char * );
bool		set_uucp_type	( char * );


sec_tab YN_kw[] =
    {
        { "Yes",    Yes   },
        { "No",     No    },
        { NULL, 0 }
    };

sec_tab KPH_kw[] =
    {
        { "Separate",   (int)MSG_Separate   },
        { "Kill",       (int)MSG_Kill       },
        { "Pass",       (int)MSG_Pass       },
        { "Hold",       (int)MSG_Hold       },
        { NULL, 0 }
    };

sec_kw  control_kph  = { &conf.news_control_msg,   KPH_kw };
sec_kw  badgroup_kph = { &conf.news_bad_group_msg, KPH_kw };


sec_tab LOG_kw[] =
    {
        { "Default",    (int)LOG_Default   },
        { "Def",        (int)LOG_Default   },
        { "Bink",       (int)LOG_Bink      },
        { "Binkley",    (int)LOG_Bink      },
        { "FrontDoor",  (int)LOG_Frodo     },
        { "FroDo",      (int)LOG_Frodo     },
        { "FD",         (int)LOG_Frodo     },
        { NULL, 0 }
    };

sec_kw  logstyle_kph = { &conf.log_style, LOG_kw };


static skeyw	sklist[] =
	{
	{ "Ftn",				NULL,				0,		BNone,		set_get_ftn,		  Required    },
	{ "Magic",				&conf.magic,		0,		BString,	NULL,				  Required    },

// REXX

	{ "Rexx.U2F.Address",			&conf.rexx_u2f_addr,	0,		BString,	NULL,				  NotRequired    },
	{ "Rexx.U2F.Pack",			&conf.rexx_u2f_pack,	0,		BString,	NULL,				  NotRequired    },

	{ "Rexx.F2U.Address",			&conf.rexx_f2u_addr,	0,		BString,	NULL,				  NotRequired    },
	{ "Rexx.Gremote.Cmd",			&conf.rexx_gremote_cmd,	0,		BString,	NULL,				  NotRequired    },


	{ "EchoAddress",	    NULL,				0,		BNone,		set_get_echoaddr,     NotRequired },

	{ "FREE",			    NULL,				0,		BNone,		set_get_freedom,      NotRequired },
	{ "ToAnyAddr",		    &conf.to_any_addr,	0,		BBool,		NULL,                 NotRequired },

	{ "PassUnknownBack",    &conf.pass_unknown_back, 0, BBool,      NULL,                 NotRequired },
	{ "PassUnknownAlong",   &conf.pass_unknown_along, 0, BBool,      NULL,                 NotRequired },
	{ "SendErrorNote",      &conf.send_error_notification, 0, BBool,      NULL,                 NotRequired },

	{ "Announce",			&conf.announce_module, 0, 	BBool,      NULL,                 NotRequired },

	{ "NetMail",		    &conf.netmail_dir,	0,		BString,   NULL,                 Required    },
	{ "NetFile",		    &conf.netfile_dir,	0,		BString,   NULL,                 Required    },
	{ "InFiles",		    &conf.infile_dirs,	0,		BString,   NULL,                 NotRequired },
// ? FlagDir unused now?
	{ "FlagDir",		    &conf.flag_dir,		0,		BString,   NULL,                 NotRequired },
	{ "StatDir",		    &conf.stat_dir,		0,		BString,   NULL,                 NotRequired },
	{ "InNewsSpool",	    &conf.newsin_dir,	0,		BString,   NULL,                 NotRequired },
	{ "OutNewsSpool",	    &conf.newsout_dir,	0,		BString,   NULL,                 NotRequired },
	{ "InFileSpool",	    &conf.filein_dir,	0,		BString,   NULL,                 NotRequired },
	{ "OutFileSpool",	    &conf.fileout_dir,	0,		BString,   NULL,                 NotRequired },
	{ "StoreFiles",		    &conf.store_dir,	0,		BString,   NULL,                 NotRequired },

	{ "KickUUCPFlag",	    &conf.kick_uucp_flag, 0,		BString,   NULL,                 NotRequired },

	{ "NewsPath",		    &conf.news_path,	0,		BString,   NULL,                 NotRequired },

	{ "Log",          	    &conf.log_file,		0,		BString,   set_open_log,         NotRequired },
	{ "LogLevel",      	    &conf.log_level,	0,		BString,   NULL,                 NotRequired },
    { "LogStyle",           &logstyle_kph,      0,      BEnum,     NULL,                 NotRequired },    //  Default|Binkley

	{ "HelpFile",		    &conf.help_file,		0,		BString,   NULL,                 NotRequired },
	{ "AttHelpFile",	    &conf.attach_help_file,		0,		BString,   NULL,                 NotRequired },
	{ "UserList",		    &conf.userlist_file,	0,		BString,   NULL,                 Required    },
	{ "EchoList",		    &conf.echolist_file,	0,		BString,   NULL,                 NotRequired },
	{ "FeedList",		    &conf.feedlist_file,	0,		BString,   NULL,                 NotRequired },
	{ "RuleList",		    &conf.rulelist_file,	0,		BString,   NULL,                 NotRequired },

	{ "New-Users-List",	    &conf.newusr_file,	0,		BString,   NULL,                 NotRequired },

	{ "PASSWORD",		    &conf.password,		0,		BString,   NULL,                 NotRequired },	// NB!! Must not be written to log!

	{ "Domain",			    &conf.def_domain,	0,		BString,   NULL,                 Required    },
//	{ "FreeDomain",		    &conf.free_domain,	0,		BString,   NULL,                 NotRequired },
	{ "PostMaster",		    &conf.postmaster,	0,		BString,   NULL,                 Required    },
	{ "NewsMaster",		    &conf.newsmaster,	0,		BString,   NULL,                 NotRequired },

	{ "Origin",			    &conf.origin,		0,		BString,   NULL,                 NotRequired },
	{ "Organization",	    &conf.organization,	0,		BString,   NULL,                 NotRequired },

	{ "RMailEXE",		    &conf.rmail_exe_name,0,		BString,   NULL,                 NotRequired },
	{ "RMailExitL",		    &conf.rmail_exitl,	0,		BInt,       NULL,                 NotRequired },
	{ "RMailExitR",		    &conf.rmail_exitr,	0,		BInt,       NULL,                 NotRequired },
	{ "UUXEXE",			    &conf.uux_exe_name,	0,		BString,   NULL,                 NotRequired },

	{ "Code_FU",		    &conf.code_fu,		0,		BString,   NULL,                 NotRequired },
	{ "Code_UF",		    &conf.code_uf,		0,		BString,   NULL,                 NotRequired },
	{ "Code_UF_Flag",	    &conf.code_fu_flag, 0,		BBool,      NULL,                 NotRequired },

//	{ "IGNORENAME",		    NULL,				0,		BNone,      add_ignname,          NotRequired },
	{ "ZipLimit",		    &conf.zip_limit,	0,		BInt,       NULL,                 NotRequired },
	{ "PktSize",		    &conf.pkt_size,		0,		BInt,       NULL,                 NotRequired },
    { "FidoFromFieldSize",  &conf.from_field_size, 0,   BInt,       NULL,                 NotRequired },
	{ "EchoMsgSize",        &conf.part_size,    0,      BInt,       NULL,                 NotRequired },

	{ "UUCP",               NULL,               0,      BNone,      set_uucp_type,        NotRequired },
        { "PercentInFrom",      &conf.at_to_percent_in_from_, 0, BBool,      NULL,                 NotRequired },

	{ "GateID",			    &conf.gate_id,		0,		BString,   NULL,                 NotRequired },
	{ "TZ",				    &conf.tz,			0,		BString,   NULL,                 Required    },
	{ "AttachStem",		&conf.attach_stem,   0,         BString,   NULL,                 NotRequired },

	{ "MailPassHL",         &conf.mail_pass_hl,  0,		BString,   NULL,                 NotRequired },	//	headlines
	{ "MailHideHL",         &conf.mail_hide_hl,  0,		BString,   NULL,                 NotRequired },	//	headlines
	{ "MailKillHL",         &conf.mail_kill_hl,  0,		BString,   NULL,                 NotRequired },	//	headlines

	{ "NewsPassHL",         &conf.news_pass_hl,  0,		BString,   NULL,                 NotRequired },	//	headlines
	{ "NewsHideHL",         &conf.news_hide_hl,  0,		BString,   NULL,                 NotRequired },	//	headlines
	{ "NewsKillHL",         &conf.news_kill_hl,  0,		BString,   NULL,                 NotRequired },	//	headlines

	{ "NewsControlMSG",     &control_kph,       0,      BEnum,      NULL,                 NotRequired },	//	Kill|Pass|Separate
	{ "NewsBadGroupMSG",    &badgroup_kph,      0,      BEnum,      NULL,                 NotRequired },	//	Kill|Pass|Hold|Separate

	{ "CnewsFeed",          &conf.cnews_feed_back, 0,   BBool,      NULL,			NotRequired },
        { "NoCrossPost",        &conf.no_crosspost,    0,   BBool,      NULL,                   NotRequired },

	{ "TruncateSentAttaches",          &conf.truncate_sent_attaches, 0,   BBool,      NULL,			NotRequired },
	{ "NoFlagsKludge",	&conf.no_flags_kludge,	0,  BBool,      NULL,                   NotRequired },

	{ "NewsGrade",          &conf.news_grade,  0,		BString,   NULL,                 NotRequired },

//
//  Compartibility options
//

    { "OldStatFile",        &conf.old_stats,    0,      BBool,      NULL,                 NotRequired },

	{ NULL,					NULL,				0,		BNone,		NULL, 				  NotRequired }
	};

const	nsklist	= sizeof( sklist )/sizeof( struct skeyw );

static void
strip_trailsp( char *p )
	{
	char	*lastch = p;

	for( ; *p; p++ )
		{
		if( *p != ' ' && *p != '\t' && *p != '\r' && *p != '\n' )
			lastch = p;					// Last nonspace
		}

	*(lastch+1) = '\0';
	}

static void     get_enum( sec_kw *value, char *param );

bool
parse_ctl_str( char *def ) {
	char    *p;
	char	*keyw, *param;

	while( *def == ' ' || *def == '\t' )
		def++;

	strip_trailsp( def );								// Trailing spaces

	p = def;
	while( *p > ' ' )
		{							// Uppercase keyword
		*p = toupper( *p );
		p++;
		}

	keyw = def;

	while( *def && *def != ' ' && *def != '\t' )		// Skip keyword
		def++;

	if( *def != '\0' )
		{
		*def = '\0';
		def++;
		}

	while( *def == ' ' || *def == '\t' )				// Skip spaces
		def++;

	param = def;


	for( int i = 0; i < nsklist; i++ )
		{
//		int		kwl = strlen( sklist[i].name );
		skeyw	*kwp;

		if( strcmpi( keyw, sklist[i].name ) )
			continue;

// The next two lines beheave strangely with global common
// subexpressions elimination.

		if( stricmp( sklist[i].name, "PASSWORD" ) != 0 )		// Not a password
			debug("Parm %s=<%s>", sklist[i].name, param );

//		printf( "%s=<%s>\n", sklist[i].name, param );

		kwp = &(sklist[i]);

		kwp->required = NotRequired;						// Got it

		switch( kwp->type )
			{
		case BNone:
			break;

		case BBool:
			if( stricmp( param, "no" ) == 0 )
				{
				*((bool *)kwp->value) = No;
				break;
				}
			else
				{
				if( *param && stricmp( param, "yes" ) )
					error( EI_None, "Setup: Please use '%s Yes' or '%s No'", keyw, keyw );

				*((bool *)kwp->value) = Yes;
				}
			break;

		case BInt:
			*((int *)kwp->value) = atoi( param );
			break;

		case BString:
			*((String*)kwp->value) = String( (const char *) param );
			break;

		case BEnum:
			get_enum( (sec_kw *)kwp->value, param );
			break;

		default:
			error( EI_None, "Internal problem - unknown keyword type" );
            break;
			}

		if( kwp->func )
			(kwp->func)( param );

		return Ok;
		}


	error( EI_None, "Bad parameter in .ctl file: %s %s", keyw, param );
	return Err;
	}

void
check_required_keywords() {
	bool	die = No;

	for( int i = 0; i < nsklist; i++ )
		{
		if( sklist[i].required == Required )
			{
			error( EI_None, "Required parameter '%s' not set in configuration file", sklist[i].name );
			die = Yes;
			}
		}
	if( die )
		fatal( EC_Incorrect_Setup, EI_None, "Setup is not complete!");
	}

static void
get_enum( sec_kw *value, char *param )
    {
    sec_tab *tab = value->tab;
    int     &val = *(value->val);

    while( isspace( *param ) )
        param++;

    for(; tab->kw; tab++ )
        {
        if( strcmpi( tab->kw, param ) )
            continue;

        val = tab->val;
        return;
        }

    error( EI_None, "Undefined keyword: '%s'", param );
    val = 0;
    }


/***************************************************************************
							Helper functions
***************************************************************************/

/*
bool	set_get_addr( char *def )	{	return conf.our_addr.aparse( def );	}
bool	set_get_newsaddr(char *def)	{	return conf.news_addr.aparse(def );	}
*/

bool
set_get_ftn( char *def )
	{
	return ftn::add_def( def );
	}

bool
set_get_echoaddr( char *def )
	{
	const char *d = def;
	return (conf.echo_from_addr.aparse( d )	||
			conf.echo_to_addr.aparse( d )) ? Err : Ok;
	}


bool
set_get_freedom( char *def )
	{
	extern void		get_free_domains( uchar *param );

	conf.free_mode = Yes;
	get_free_domains( (uchar*)def );
	return Ok;
	}


bool
set_open_log( char * )
	{
	if( strlen( conf.log_file.c_str() ) )
		open_log( conf.log_file.c_str() );
	return Ok;
	}


bool
set_uucp_type( char *t )
	{
	if( !stricmp( t, "ACHE30" ) )
		{
		conf.uucp = UUCP_Ache30;
		return Ok;
		}

	if( !stricmp( t, "UUPC11R" ) )
		{
		conf.uucp = UUCP_11R;
		return Ok;
		}

	if( !stricmp( t, "WAFFLE" ) )
		{
		conf.uucp = UUCP_Waffle;
		return Ok;
		}

	error( EI_None, "Unknown UUCP package type: %s", t );
	return Err;
	}
@


1.3
log
@debug
@
text
@d8 3
d146 9
a154 2
//	{ "Address",		    NULL,				0,		BNone,		set_get_addr,         Required    },
//	{ "NewsAddress",	    NULL,				0,		BNone,		set_get_newsaddr,     NotRequired },
d161 3
d242 3
@


1.2
log
@checkpoint
@
text
@d8 3
d428 1
a428 1
	get_free_domains( def );
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/lib/uu2/vcs/set_tab.c_v  $
d203 1
d205 2
@
