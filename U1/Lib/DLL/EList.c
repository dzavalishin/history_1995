/************************ UU2 EchoList ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	EchoList management
 *
 *      $Log: EList.c $
 *      Revision 1.5  1995/11/05 13:52:48  dz
 *      current.
 *
 * Revision 1.4  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.3  1995/04/06  23:33:30  dz
 * checkpoint
 *
 * Revision 1.2  1995/04/06  19:46:16  dz
 * Echolist cache size limit removed, so it is not
 * a cache anymore - we just loading all the descriptions
 * at once and use 16Kb buffer when loading - such a strategy
 * speeds things up to twice.
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *
\*/

#include	<io.h>
#include	<fcntl.h>
#include	<stdio.h>
#include    <errno.h>
#include	<stdlib.h>
#include	<share.h>
#include	<ctype.h>
#include	<style.h>

#include	"setup.h"
#include	"log.h"

#include	"elist.h"
#include	"elist_c.h"
#include	"DLLInits.h"


static FILE		*elf = NULL;				// Echo-List file

//static long		c_hits = 0L;				// Times we found it in cache
//static long		c_misses = 0L;				// Times we found it in file
//static long		c_preloads = 0L;			// Times we pre-loaded a record
//static long		c_errors = 0L;				// Times we not found it
//static long		c_pass = 0L;				// Times we passed through



static bool
open_elf( void )
	{
	if( elf != NULL )
		return Ok;

	debug("Elf: Opening file");

	int fd = _sopen( conf.echolist_file.c_str(), O_RDONLY|O_TEXT, SH_DENYWR, 0666 );
	if( fd < 0 )
		{
		error( EI_Full, "Elf: Can't _sopen echo list file '%s'", (const char *)conf.echolist_file.c_str() );
		return Err;
		}

	elf = fdopen( fd, "r" );
	if( elf == NULL )
		{
		error( EI_Full, "Elf: Can't fdopen echo list file '%s'", (const char *)conf.echolist_file.c_str() );
		_close( fd );
		return Err;
		}

	setvbuf( elf, NULL, _IOFBF, 16384 );

	return Ok;
	}

extern "SYSTEM" void
close_elf( void )
	{
	if( elf == NULL )
		return;

    debug("Elf: Closing file");

	if( fclose( elf ) != 0 )
		error( EI_Full, "Elf: Can't close echo list file '%s'", (const char *)conf.echolist_file.c_str() );

#if 0
	if( c_hits != 0 || c_misses != 0 )
		{
		if( c_misses == 0 && c_hits != 0 )
			error( EI_None, "Cache strangeness: %ld hits, 0 misses!", c_hits );
		else
			log( "%", "ELcache Hit %ld, Miss %ld, NoGrp %ld, Preload %ld, HitRatio %d%%",
				c_hits, c_misses, c_errors, c_preloads,
				(c_hits*100)/(c_hits+c_misses)
				);
		}
#endif

	}

//#pra gma exit close_elf 255


/**
 *			File interface
**/

static bool
yesno( const char *v )
	{
	if( *v == '\0' || !strcmpi( v, "Yes" ) )
		return Yes;

	if( !strcmpi( v, "No" ) )
		return No;

	error( EI_None, "Boolean area parameter value is not 'Yes' or 'No'");
	return Yes;
	}

bool echo_def::
parse_param( char *def )
	{
	char	*val = strchr( def, '=' );

	if( val == NULL )
		{
		error( EI_None, "No '=' in area parameter: %s", def );
		return Err;
		}
	else
		*val++ = '\0';

	if( strcmpi( def, "InFile" ) == 0 )
		{
		in_file_dir_v = (const char *)val;
#if 0
		if( in_file_dir_v.is_empty() )
			{
			error( EI_None, "Out of memory in edef" );
			return Err;
			}
#endif
		in_file_v = Yes;
		return Ok;
		}

	if( strcmpi( def, "Rules" ) == 0 )
		{
		rules_file_v =  (const char *) val;
		send_rules_v = Yes;
		return Ok;
		}

	if( strcmpi( def, "FidoRules" ) == 0 )
		{
		fido_rules_v =  yesno( (const char *) val);
		return Ok;
		}

	if( strcmpi( def, "FEcho" ) == 0 )
		{
		file_echo_v =  yesno( (const char *) val);
		return Ok;
		}

	if( strcmpi( def, "ModF" ) == 0 )
		{
		fido_premoderator_v =  (const char *) val;
		return Ok;
		}

	if( strcmpi( def, "ModU" ) == 0 )
		{
		uucp_premoderator_v =  (const char *) val;
		return Ok;
		}

	if( strcmpi( def, "Approved" ) == 0 )
		{
		approved_v =  (const char *) val;
		return Ok;
		}

    if( strcmpi( def, "Distrib" ) == 0 )
		{
        distrib_v =  (const char *) val;
		return Ok;
		}

    if( strcmpi( def, "FHold" ) == 0 )
		{
		file_hold_dir_v =  (const char *) val;
		return Ok;
		}

	if( strcmpi( def, "Dir" ) == 0 )
		{
		if( !strcmpi( val, "none" ) )
			{
			pass_to_fido_v = No;
			pass_to_uucp_v = No;
			}
		else if( !strcmpi( val, "both" ) )
			{
			pass_to_fido_v = Yes;
			pass_to_uucp_v = Yes;
			}
		else if( !strcmpi( val, "fu" ) )
			{
			pass_to_fido_v = No;
			pass_to_uucp_v = Yes;
			}
		else if( !strcmpi( val, "uf" ) )
			{
			pass_to_fido_v = Yes;
			pass_to_uucp_v = No;
			}
		else
			{
			error( EI_None, "Unknown DIR mode" );
			pass_to_fido_v = Yes;
			pass_to_uucp_v = Yes;
			}

		return Ok;
		}

	if( strcmpi( def, "RegOnly" ) == 0 )
		{
		pass_fu_reg_only_v =  yesno( (const char *) val);
		return Ok;
		}

	if( strcmpi( def, "XPostGuard" ) == 0 )
		{
		xpost_guard_v =  yesno( (const char *) val);
		return Ok;
		}

	error( EI_None, "Unknown area parameter %s", def );
	return Err;
	}


bool echo_def::
parse( const char *def )
	{
	char		*p;
	const		sav_l = 200;
	char		sav[sav_l+1];

	while( isspace( *def ) )
		def++;

	if( *def == '\0' )
		{
		error( EI_None, "Bad echo definition: %s", def );
		return Err;
		}

	strncpy( sav, def, sav_l );
	sav[sav_l] = '\0';
	for( p = sav; *p; p++ )
		if( *p <= ' ' )
			{
			*p = '\0';
			break;
			}

	strlwr( sav );
	newsgroup_v = (const char *)sav;

	while( *def && *def > ' ' )
		def++;

	while( isspace( *def ) )
		def++;

	if( *def == 0 )
		{
		error( EI_None, "Bad echo definition: %s", def );
		return Err;
		}

	strncpy( sav, def, sav_l );
	sav[sav_l] = '\0';
	for( p = sav; *p; p++ )
		if( *p <= ' ' )
			{
			*p = '\0';
			break;
			}

	strlwr( sav );
	echo_v		= (const char *)sav;

#if 0
	if( newsgroup_v == NULL || echo_v == NULL )
		{
		error( EI_None, "Out of memory in echo_def::parse" );
		return Err;
		}
#endif

	while( *def && *def > ' ' )
		def++;

	while( isspace( *def ) )
		def++;

	if( *def == '\0' )
		return Ok;

	if( *def && *def != '{' )
		{
		error( EI_None, "Bad echo definition (no '{' before par.): %s", def );
		return Err;
		}

	def++;											// Skip '{'
	while( *def && *def != '}' )
		{
		const char	*e;

		while( isspace( *def ) )
			def++;

		e = def;

		while( *def && !isspace( *def ) && *def != '}' )
			def++;

		if( def-e > sav_l )
			{
			error( EI_None, "Area parameter too long: %.*s", e, def-e );
			return Err;
			}

		strncpy( sav, e, sav_l );
		sav[(int)(def-e)] = '\0';
		if( parse_param( sav ) == Err )
			return Err;

		while( isspace( *def ) )
			def++;
		}

	return Ok;
	}


static el_cache *
read_record()
	{
	char	buf[300];

	if( elf == NULL )
		if( open_elf() != Ok )
			fatal( EC_IO_Error, EI_None, "Can't open echo list file!" );

	while( fgets( buf, 300, elf ) != NULL )
		{
		char	*p;
		bool	good = No;

		if( (p = strpbrk( buf, ";\r\n" )) != NULL )
			*p = '\0';

		for( p = buf; *p; p++ )
			{
			if( *p > ' ' )
				{
				good = Yes;
				break;
				}
			}

		if( !good )
			continue;

		el_cache *c = new el_cache;
		if( c == NULL )
			{
			error( EI_None, "Out of memory in elc");
			return NULL;
			}

		if( c->parse( p ) == Err )
			{
			delete c;
			return NULL;
			}

		return c;
		}

	return NULL;
	}

/**
 *			User interface
**/


void
load_all( void )
	{
	el_cache	*cp;

	if( elf != NULL )
		return;

	if( open_elf() == Err )
		fatal( EC_IO_Error, EI_None, "Can't open echo list file!" );

	fseek( elf, 0L, SEEK_SET );					// Rewind file

	while( 1 )
		{
		cp = read_record();
		if( cp == NULL )
			return;

		cp->attach();
		}

	}



bool echo_def::
find_by_group( const char *group )
	{
	el_cache	*cp;

	if( elf == NULL )
		load_all();

	cp = el_cache::get_by_group( group );
	if( cp != NULL )							// Found in cache! Hit! :)
		{
		*this = *cp;
		return Ok;
		}


	return Err;
	}


bool echo_def::
find_by_area( const char *area )
	{
	el_cache	*cp;

	if( elf == NULL )
		load_all();

	cp = el_cache::get_by_area( area );
	if( cp != NULL )							// Found in cache! Hit! :)
		{
		*this = *cp;
		return Ok;
		}

	return Err;
	}



	/*************************************************************
						   C'tor/D'tor
	*************************************************************/

echo_def::
echo_def( void )
	{
	in_file_v    =
	send_rules_v =
	fido_rules_v =
	xpost_guard_v =
	file_echo_v	 = No;

	pass_to_fido_v =
	pass_to_uucp_v = Yes;

    pass_fu_reg_only_v = No;
    }

echo_def::
~echo_def( void )
	{
	clear();
	}

	/*************************************************************
						   Assignment
	*************************************************************/

echo_def& echo_def::
operator = ( echo_def &r )
	{
	clear();
	newsgroup_v 		= r.newsgroup_v;
	echo_v				= r.echo_v;
	in_file_v			= r.in_file_v;
	in_file_dir_v		= r.in_file_dir_v;
	rules_file_v		= r.rules_file_v;
	send_rules_v 		= r.send_rules_v;
	fido_rules_v 		= r.fido_rules_v;

	file_echo_v 		= r.file_echo_v;
	file_hold_dir_v 	= r.file_hold_dir_v;

	fido_premoderator_v	= r.fido_premoderator_v;
	uucp_premoderator_v	= r.uucp_premoderator_v;

	approved_v 			= r.approved_v;
    distrib_v           = r.distrib_v;

	pass_to_fido_v 		= r.pass_to_fido_v;
	pass_to_uucp_v 		= r.pass_to_uucp_v;
	xpost_guard_v		= r.xpost_guard_v;



    pass_fu_reg_only_v  = r.pass_fu_reg_only_v;

	return *this;
	}

void echo_def::
clear( void )
	{
	}

	/*************************************************************
                       Passthrough support
	*************************************************************/



void echo_def::
set_passthrough( const char *aog )
	{
//	c_pass++;

	newsgroup_v = (const char *)aog;
	echo_v = (const char *)aog;
	}




