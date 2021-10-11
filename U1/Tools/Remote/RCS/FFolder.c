head	1.2;
access;
symbols;
locks;
comment	@ *  @;


1.2
date	95.08.02.13.53.36;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.07.01.21.49.36;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@Working version
@
text
@/************************ FIDO to UUCP convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :	Main conversion loop
 *
 *      $Log: FFolder.c $
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
ffolder::next( fido_msg &fm )
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



@


1.1
log
@Initial revision
@
text
@d7 3
a9 3
 *      $Log: MainLoop.c $
 * Revision 1.2  1995/04/12  21:20:45  dz
 * IBM C Set version seems to be working...
a10 34
 * Revision 1.1  1995/03/11  18:24:01  dz
 * Initial revision
 *
 *      
 *         Rev 1.8   07 Jun 1993 16:37:32   dz
 *      Rearranged a lot
 *      
 *         Rev 1.7   28 Nov 1992 23:08:04   dz
 *      String to const char *, style fixes
 *      
 *         Rev 1.6   23 Oct 1992 14:33:14   dz
 *      reorganization
 *      
 *         Rev 1.5   29 Jun 1992 15:38:10   dz
 *      Don't delete messages with 'Local' flag but withno
 *      'KillSent' one
 *      
 *         Rev 1.4   18 Jun 1992 10:32:02   dz
 *      Cosmetic changes
 *      
 *         Rev 1.3   04 Feb 1992 00:08:32   dz
 *      Count number of files sent. Incorrectly implemented -
 *      will only count one file-attach of all listed in the
 *      same message.
 *      
 *         Rev 1.2   11 Oct 1991 05:24:36   dz
 *      Error letter generation, return receipt mode,
 *      statistics update.
 *
 *         Rev 1.1   29 Aug 1991 06:40:34   dz
 *      MSG handling rewritten
 *
 *         Rev 1.0   27 Aug 1991 02:47:18   dz
 *      Initial revision.
d15 1
a15 1
#include	"fido2uu.h"
a16 1
//#include	<dirent.h>
a18 4
#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#include	<os2.h>

d21 3
d25 2
a26 10
static void
do_one( int msg_no, bool &processed )
	{
	fido_msg	fm;
	
	if( fm.attach( conf.netmail_dir.c_str(), msg_no ) == Err )
		{
		error( EI_None, "Can't open message '%d.msg'", msg_no );
		return;
		}
a27 2
	if( !select( fm ) )						// Subject to convert ?
		return;
d29 2
a30 1
	fmsg_attr	at;
d32 2
a33 1
	debug("Processing '%s\\msg.%d'", (const char *)conf.netmail_dir.c_str(), msg_no );
d35 2
a36 1
	track_reset();						// Prepare to log tracking
a37 1
	fm.get_attr( at );
d39 6
a44 1
	bool	c_report = No, c_err = No;
d46 4
a49 1
	convert( fm, c_report, c_err );		// Convert letter
d51 5
a55 2
	if( c_report || c_err )
		error_letter( &fm );			// Send letter back
d57 6
a62 4
	if( !c_err )						// Not an error, delete original
		{
		if( at & (FF_ReturnReceiptRequest|FF_AuditRequest))	// Confirmation Request
			acknovledge_letter( &fm );	// Send confirmation
d64 2
a65 2
		processed = Yes;
		}
d67 1
d69 1
a69 4
	at |= FF_Sent;						// Mark letter as sent
	if( !(at&FF_Local) )               	// Foreighn letter?
		at |= FF_KillSent;				// Force deletion
	fm.set_attr( at );
a70 3
	fm.flush();
	fm.detach();						// Forget it.
	}
d72 5
a76 11


static bool
msg_file( const char *fn )
	{
	
	char	ext[_MAX_EXT], dummy[_MAX_PATH];
	_splitpath( (char *)fn, dummy, dummy, dummy, ext);


	if( stricmp( ext, "msg" ) == 0 )
d78 1
d80 6
a85 2
	if( stricmp( ext, ".msg" ) == 0 )
		return Yes;
d87 2
a88 1
	return No;
d92 2
a93 3

void
mainloop( void )
d95 3
a97 6
	HDIR		h;
	ULONG		count;
	APIRET		rc;
	bool		did_something_this_round;
	char		mask[_MAX_PATH];
	bool		processed = No;
d99 1
a99 1
	sprintf( mask, "%s/*.msg", (const char *)conf.netmail_dir.c_str() );
d101 1
a101 1
	do
d103 3
a105 4
		FILEFINDBUF3	ff = { 0 };
		count = 1;
		h = HDIR_SYSTEM;
		rc = DosFindFirst( mask, &h, ATTRS2FIND, &ff, sizeof(ff), &count, FIL_STANDARD );
d107 7
d115 2
a116 3
//	dirp = opendir( (char *)(const char *)conf.netmail_dir.c_str() );
//	if( dirp == NULL )
//		fatal( EC_IO_Error, EI_Full, "Can't open directory (%s)", (const char *)conf.netmail_dir.c_str() );
d118 3
a120 7
		if( rc != NO_ERROR )
			{
			if( rc == ERROR_NO_MORE_FILES )
				{
				log( "#", "Nothing to do - no messages found" );
				return;
				}
a121 2
			fatal( EC_OS_Error, EI_Full, "FindFirst rc = %d", rc );
			}
d123 5
d129 2
a130 1
		did_something_this_round = No;
d132 2
a133 4
//	while( ( de = readdir( dirp )) != NULL ) 
		do
			{
			int			mno;
d135 1
a135 5
			if( msg_file( ff.achName ) )
				{
				mno = atoi( ff.achName );
				do_one( mno, did_something_this_round );
				}
d137 5
a141 2
			if( did_something_this_round )
				processed = Yes;
d143 2
a144 4
			} while( 
				count = 1, 
				(rc = DosFindNext( h, &ff, sizeof(ff), &count )) 
					== NO_ERROR );
d146 4
a149 8
//	closedir( dirp );

// dunno why ERROR_INVALID_HANDLE happens, but it does :(
		if( (rc != ERROR_NO_MORE_FILES) && (rc != ERROR_INVALID_HANDLE) )
			fatal( EC_OS_Error, EI_Full, "FindNext rc = %d", rc );

		if( (rc = DosFindClose( h )) != NO_ERROR )
			error( EI_Full, "FindClose rc = %d", rc );
d151 1
d153 1
a153 1
		} while( did_something_this_round );
d155 1
a155 2

	if( processed && strlen( (const char*) conf.kick_uucp_flag.c_str()) != 0 )
d157 2
a158 7
		FILE	*flag;

		if( ( flag = fopen( conf.kick_uucp_flag.c_str(), "w" ) ) == NULL )
			error( EI_Full, "Can't create flag file '%s'", 
				(const char *)conf.kick_uucp_flag.c_str() );
		else
			fclose( flag );
d161 1
@
