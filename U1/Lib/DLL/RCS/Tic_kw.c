head	1.2;
access;
symbols;
locks;
comment	@ * @;


1.2
date	95.04.13.11.06.28;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.00.54;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@debug
@
text
@/************************ UU2 FILE2UU ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	.TIC file keywords parser
 *
 *      $Log: Tic_kw.c $
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   29 Dec 1992 01:32:28   dz
 *      Initial revision.
 *      
 *
\*/

#include    "style.h"
#include    "tic_kw.h"
#include    <string.h>
#include    <log.h>

bool	tic_kw::in_use = No;


struct kw_el
	{
	char				*keyw;		// Keyword text
	tic_kw::tic_token	token;		// Internal value
	bool				required;	// Is it required?
	bool				multy;		// More than one per file permitted
	int					count;		// How many items we got
	};

kw_el	kw_tab[] =
	{
	{	"app",		tic_kw::app,		No,		Yes,	0	},
	{	"area",		tic_kw::area,		Yes,	No,		0	},
	{	"crc",		tic_kw::crc,		No,		No,		0	},
	{	"created",	tic_kw::created,	No,		No,		0	},
	{	"date",		tic_kw::date,		No,		No,		0	},
	{	"desc",		tic_kw::desc,		No,		Yes,	0	},
	{	"file",		tic_kw::file,		Yes,	No,		0	},
	{	"from",		tic_kw::from,		No,		No,		0	},
	{	"origin",	tic_kw::origin,		No,		No,		0	},
	{	"path",		tic_kw::path,		No,		Yes,	0	},
	{	"pw",		tic_kw::pw,			No,		No,		0	},
	{	"release",	tic_kw::release,	No,		No,		0	},
	{	"replaces",	tic_kw::replaces,	No,		Yes,	0	},
	{	"seenby",	tic_kw::seenby,		No,		Yes,	0	},
	{	"size",		tic_kw::size,		No,		No,		0	},
	};

int	n_kw_tab = sizeof(kw_tab) / sizeof(kw_el);


tic_kw::tic_kw()
	{
	if( in_use )
		{
		error( EI_None, "TIC parser reuse error" );
		valid = No;
		}
	else
		{
		in_use = Yes;
		valid = Yes;

		log( "#", "Starting up TIC file parser");

		for( int i = 0; i < n_kw_tab; i++ )
			kw_tab[i].count = 0;
		}
	}


bool
tic_kw::finish( void )
	{
	bool	bad = Ok;

    log( "#", "Finishing TIC file parser operation");

	if( valid )
		{
		for( int i = 0; i < n_kw_tab; i++ )
			{
			kw_el	&e = kw_tab[i];

			if( e.count == 0 && e.required )
				{
				error( EI_None, "Required item '%s' not found", e.keyw );
				bad = Err;
				}

			if( e.count > 1 && !e.multy )
				{
				error( EI_None, "More than one '%s' in TIC file", e.keyw );
				bad = Err;
				}
			}
		}

	return bad;
	}



tic_kw::~tic_kw()
	{
	if( valid )
		{
		valid = No;
		in_use = No;
		}
	}


/*************************************************************************
								Parser itself
*************************************************************************/




tic_kw::tic_token tic_kw::
parse( char *&keyw )
	{
	if( !valid )
		return no_token;

	for( int i = 0; i < n_kw_tab; i++ )
		{
		kw_el	&e = kw_tab[i];
		int		l = strlen( e.keyw );

		if( strncmpi( keyw, e.keyw, l ) )			// Keyw. doesn't match
			continue;

		if( keyw[l] != ':' && keyw[l] != ' ' && keyw[l] != '\t' )
			continue;

		keyw += l;
		if( *keyw == ':' )
			keyw++;

		while( *keyw == ' ' || *keyw == '\t' )
			keyw++;

		e.count++;
		if( e.count > 1 && !e.multy )
			error( EI_None, "Extra '%s %s' in TIC file", e.keyw, keyw );

		return e.token;
		}

	return no_token;
	}


@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/file/file2uu/vcs/tic_kw.c_v  $
@
