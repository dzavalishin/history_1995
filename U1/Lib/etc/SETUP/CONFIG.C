/************************ UUCP to FIDO gate ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Implementation - other code
 *
 *      $Log: CONFIG.C $
 * Revision 1.1  1995/03/11  18:14:11  dz
 * Initial revision
 *
 *      
 *         Rev 1.1   14 May 1993 13:41:24   dz
 *
 *
\*/

#include	"config.h"
#include	<ctype.h>
#include	<strstream.h>
#include	<fstream.h>
#include	<assoc.h>
#include	<sys/stat.h>


/****************************************************************************
					config_list implementation
****************************************************************************/

bool
is_older( const char *fn1, const char *fn2 )
	{
	struct stat	sb1, sb2;

	if( stat( fn1, &sb1 ) || stat( fn2, &sb2 ) )
		return No;  // We'll better try to rebuild it!


	if( sb1.st_mtime > sb2.st_mtime )		// config is newer!
		return No;					// rebuild

	return Yes;						// use cache
	}

bool config_list::
setup( const char * config_file_name, const char * cache_file_name )
	{
	if( is_older( config_file_name, cache_file_name ) )
		{
		// Pick up configuration from cache (binary) file
		ifstream cache( cache_file_name );

		if( !cache )
			{
// error
			return Err;
			}

		DoubleListIterator& i = (DoubleListIterator&) items.initIterator();

		for( ; (int)i; i++ )
			{
			config_item &ci = (config_item&)i.current();
			if( ci.Pop( cache ) != Ok )
				{
// error
				return Err;
				}
			}

		}
	else
		{
		// Recompile configuration again
		ifstream config( config_file_name );
		ofstream cache( cache_file_name );

		if( !config )
			{
// error
			return Err;
			}

		if( !cache )
			{
// error
			return Err;
			}

		config_text		index( config );

		if( index.error() )
			{
// error
			return Err;
			}

		DoubleListIterator& i = (DoubleListIterator&) items.initIterator();

		for( ; (int)i; i++ )
			{
			config_item &ci = (config_item&)i.current();

			if( ci.LoadText( index ) != Ok )
				{
// error
				return Err;
				}

			if( ci.Push( cache ) != Ok )
				{
// error
				return Err;
				}
			}

		}

	return Ok;
	}









