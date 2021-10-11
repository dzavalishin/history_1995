/************************ FIDO to UUCP convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :	Main conversion loop
 *
 *      $Log: FFolder.c $
 *      Revision 1.3  1995/11/05 18:50:12  dz
 *      RFC_Msg used
 *
 *      Revision 1.2  1995/08/02 13:53:36  dz
 *      Working version
 *
 *  Revision 1.1  1995/07/01  21:49:36  dz
 *  Initial revision
 *
 *
 *
\*/

#include	"ffolder.h"
#include	<stdlib.h>
#include	<errno.h>

#define ATTRS2FIND	0x27

static bool
msg_file( const char *fn )
        {

        char    ext[_MAX_EXT], dummy[_MAX_PATH];
        _splitpath( (char *)fn, dummy, dummy, dummy, ext);


        if( stricmp( ext, "msg" ) == 0 )
                return Yes;

        if( stricmp( ext, ".msg" ) == 0 )
                return Yes;

        return No;
        }


ffolder::
ffolder( const string &dir ) : dir_v( dir )
	{
	restart_flag = Yes;
	opened_flag = No;
	}

ffolder::~ffolder()
	{
	close();
	}

void            
ffolder::restart()
	{
	close();
	}

bool		
ffolder::do_restart( string &name )
	{
        ULONG           count = 1;
        APIRET          rc;
	FILEFINDBUF3	ff = { 0 };

	char	mask[400];
	sprintf( mask, "%s/*.msg", dir_v.c_str() );

	h = HDIR_CREATE; // HDIR_SYSTEM

	rc = DosFindFirst( mask, &h, ATTRS2FIND, &ff, sizeof(ff), &count, FIL_STANDARD );


	if( rc == NO_ERROR )
		{
		name = ff.achName;
		restart_flag = No;
		opened_flag;
		return Yes;
		}

	if( rc == ERROR_NO_MORE_FILES )
		{
		log( "#", "Nothing to do - no messages found" );
		close();
		return No;
		}

	error( EI_Full, "FindFirst rc = %d", rc );
	return Err;
	}


bool		
ffolder::do_continue( string &name )
	{
        ULONG           count = 1;
        APIRET          rc;
	FILEFINDBUF3	ff = { 0 };

	rc = DosFindNext( h, &ff, sizeof(ff), &count );

	if( rc == NO_ERROR )
		{
		name = ff.achName;
		return Yes;
		}

	// Sometimes handle goes nuts :(
	if( rc == ERROR_INVALID_HANDLE )
		{
		log( "#", "findnext gave ERROR_INVALID_HANDLE, restarting..." );
		close();
		return do_restart( name );
		}

	if( rc == ERROR_NO_MORE_FILES )
		return No;

	error( EI_Full, "FindNext rc = %d", rc );
	return Err;
	}


bool
ffolder::next( FTN_Msg &fm )
	{
	string	s;
	bool	rc;

	do	{
		rc = restart_flag ? do_restart( s ) : do_continue( s );

		if( rc == No || rc == Err )
			return rc;

		} while( !msg_file( s.c_str() ) );

	if( fm.attach( dir_v.c_str(), atoi(s.c_str()) ) == Err )
	        {
	        error( EI_None, "Can't open message '%s'", s.c_str() );
	        return Err;
	        }

        return Yes;
	}

bool		
ffolder::close()
	{
        APIRET          rc;

	if( !opened_flag ) return Ok;

	restart_flag = Yes;

	if( (rc = DosFindClose( h )) != NO_ERROR )
		{
        	error( EI_Full, "FindClose rc = %d", rc );
		return Err;
		}

	return Ok;
	}



