head	1.7;
access;
symbols;
locks;
comment	@ * @;


1.7
date	96.03.09.11.28.29;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	95.11.06.02.20.45;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	95.11.06.01.42.16;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.08.08.13.22.34;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.02.13.14.18;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.12.21.20.45;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.24.01;	author dz;	state Exp;
branches;
next	;


desc
@@


1.7
log
@Bug fix: process all the messages, not just first one
@
text
@/************************ FIDO to UUCP convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :    Main conversion loop
 *
 *      $Log: MainLoop.c $
 *      Revision 1.6  1995/11/06 02:20:45  dz
 *      Using ffolder class
 *
 *      Revision 1.5  1995/11/06 01:42:16  dz
 *      Using FTN_Msg! Seems to be working.
 *
 *      Revision 1.4  1995/08/08 13:22:34  dz
 *      App class used
 *
 *      Revision 1.3  1995/08/02 13:14:18  dz
 *      'did_something_this_round' kludge around errors in finds
 *
 *      Revision 1.2  1995/04/12  21:20:45  dz
 *      IBM C Set version seems to be working...
 *
 *      Revision 1.1  1995/03/11  18:24:01  dz
 *      Initial revision
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
 *
 *
\*/

#include    "fido2uu.h"
#include    <stdlib.h>
#include    <errno.h>
#include    <ffolder.h>

//#define INCL_DOSFILEMGR
//#define INCL_DOSERRORS
//#include    <os2.h>



void Fido2uuApp::
mainloop( void ) 
    {
    bool        processed = No;
    ffolder     folder( conf.netmail_dir );
    
    while( 1 )
        {
        FTN_Msg fm;
        
        switch( folder.next( fm ) )
            {
            case Err:
                error( EI_Full, "Can't get next message in %s", (const char *)conf.netmail_dir.c_str() );
            case No:
                break;
                
            case Yes:
                // Subject to convert ?
                if( !select( fm ) )
                    continue;
                
                fmsg_attr    at;
                
//                debug("Processing '%s\\msg.%d'", (const char *)conf.netmail_dir.c_str(), msg_no );
                
                track_reset();                        // Prepare to log tracking
                
                fm.get_attr( at );
                
                bool    c_report = No, c_err = No;
                
                convert( fm, c_report, c_err );        // Convert letter
                
                if( c_report || c_err )
                    error_letter( fm );            // Send letter back
                
                if( !c_err )                        // Not an error, delete original
                    {
                    if( at & (FF_ReturnReceiptRequest|FF_AuditRequest))    // Confirmation Request
                        acknovledge_letter( fm );    // Send confirmation
                    
                    processed = Yes;
                    }
                
                
                at |= FF_Sent;                        // Mark letter as sent
                if( !(at&FF_Local) )                   // Foreighn letter?
                    at |= FF_KillSent;                // Force deletion
                fm.set_attr( at );
                
                fm.detach();                        // Forget it.
                
                continue;
            } // switch
        break;
        } // while
    
    if( processed && conf.kick_uucp_flag.length() != 0 )
        {
        FILE    *flag;
        
        if( ( flag = fopen( conf.kick_uucp_flag.c_str(), "w" ) ) == NULL )
            error( EI_Full, "Can't create flag file '%s'", 
                  (const char *)conf.kick_uucp_flag.c_str() );
        else
            fclose( flag );
        }
    
    }







#if 0

#define ATTRS2FIND    0x27



void
Fido2uuApp::do_one( int msg_no, bool &processed )
    {
    FTN_Msg    fm;
    
    if( fm.attach( conf.netmail_dir.c_str(), msg_no ) == Err )
        {
        error( EI_None, "Can't open message '%d.msg'", msg_no );
        return;
        }
    
    if( !select( fm ) )                        // Subject to convert ?
        return;
    
    fmsg_attr    at;
    
    debug("Processing '%s\\msg.%d'", (const char *)conf.netmail_dir.c_str(), msg_no );
    
    track_reset();                        // Prepare to log tracking
    
    fm.get_attr( at );
    
    bool    c_report = No, c_err = No;
    
    convert( fm, c_report, c_err );        // Convert letter
    
    if( c_report || c_err )
        error_letter( fm );            // Send letter back
    
    if( !c_err )                        // Not an error, delete original
        {
        if( at & (FF_ReturnReceiptRequest|FF_AuditRequest))    // Confirmation Request
            acknovledge_letter( fm );    // Send confirmation
        
        processed = Yes;
        }
    
    
    at |= FF_Sent;                        // Mark letter as sent
    if( !(at&FF_Local) )                   // Foreighn letter?
        at |= FF_KillSent;                // Force deletion
    fm.set_attr( at );
    
    fm.detach();                        // Forget it.
    }



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



void
Fido2uuApp::mainloop( void )
    {
    HDIR        h;
    ULONG        count;
    APIRET        rc;
    bool        did_something_this_round;
    char        mask[_MAX_PATH];
    bool        processed = No;
    
    sprintf( mask, "%s/*.msg", (const char *)conf.netmail_dir.c_str() );
    
    do
        {
        FILEFINDBUF3    ff = { 0 };
        count = 1;
        h = HDIR_SYSTEM;
        rc = DosFindFirst( mask, &h, ATTRS2FIND, &ff, sizeof(ff), &count, FIL_STANDARD );
        
        
        //    dirp = opendir( (char *)(const char *)conf.netmail_dir.c_str() );
        //    if( dirp == NULL )
        //        fatal( EC_IO_Error, EI_Full, "Can't open directory (%s)", (const char *)conf.netmail_dir.c_str() );
        
        if( rc != NO_ERROR )
            {
            if( rc == ERROR_NO_MORE_FILES )
                {
                log( "#", "Nothing to do - no messages found" );
                return;
                }
            
            fatal( EC_OS_Error, EI_Full, "FindFirst rc = %d", rc );
            }
        
        
        did_something_this_round = No;
        
        //    while( ( de = readdir( dirp )) != NULL ) 
        do
            {
            int            mno;
            
            if( msg_file( ff.achName ) )
                {
                mno = atoi( ff.achName );
                do_one( mno, did_something_this_round );
                }
            
            if( did_something_this_round )
                processed = Yes;
            
            } while( 
                    count = 1, 
                    (rc = DosFindNext( h, &ff, sizeof(ff), &count )) 
                    == NO_ERROR );
        
        //    closedir( dirp );
        
        // dunno why ERROR_INVALID_HANDLE happens, but it does :(
        if( (rc != ERROR_NO_MORE_FILES) && (rc != ERROR_INVALID_HANDLE) )
            fatal( EC_OS_Error, EI_Full, "FindNext rc = %d", rc );
        
        if( (rc = DosFindClose( h )) != NO_ERROR )
            error( EI_Full, "FindClose rc = %d", rc );
        
        
        } while( did_something_this_round );
    
    
    if( processed && strlen( (const char*) conf.kick_uucp_flag.c_str()) != 0 )
        {
        FILE    *flag;
        
        if( ( flag = fopen( conf.kick_uucp_flag.c_str(), "w" ) ) == NULL )
            error( EI_Full, "Can't create flag file '%s'", 
                  (const char *)conf.kick_uucp_flag.c_str() );
        else
            fclose( flag );
        }
    
    }



#endif

@


1.6
log
@Using ffolder class
@
text
@d8 3
d125 1
@


1.5
log
@Using FTN_Msg! Seems to be working.
@
text
@d8 3
a59 1
//#include    <dirent.h>
d61 5
d67 79
a145 3
#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#include    <os2.h>
d150 1
d301 2
@


1.4
log
@App class used
@
text
@d5 1
a5 1
 *      Module  :	Main conversion loop
d8 3
d55 4
a58 4
#include	"fido2uu.h"
#include	<stdlib.h>
//#include	<dirent.h>
#include	<errno.h>
d62 1
a62 1
#include	<os2.h>
d64 1
a64 1
#define ATTRS2FIND	0x27
d69 43
a111 44
	{
	fido_msg	fm;
	
	if( fm.attach( conf.netmail_dir.c_str(), msg_no ) == Err )
		{
		error( EI_None, "Can't open message '%d.msg'", msg_no );
		return;
		}

	if( !select( fm ) )						// Subject to convert ?
		return;

	fmsg_attr	at;

	debug("Processing '%s\\msg.%d'", (const char *)conf.netmail_dir.c_str(), msg_no );

	track_reset();						// Prepare to log tracking

	fm.get_attr( at );

	bool	c_report = No, c_err = No;

	convert( fm, c_report, c_err );		// Convert letter

	if( c_report || c_err )
		error_letter( &fm );			// Send letter back

	if( !c_err )						// Not an error, delete original
		{
		if( at & (FF_ReturnReceiptRequest|FF_AuditRequest))	// Confirmation Request
			acknovledge_letter( &fm );	// Send confirmation

		processed = Yes;
		}


	at |= FF_Sent;						// Mark letter as sent
	if( !(at&FF_Local) )               	// Foreighn letter?
		at |= FF_KillSent;				// Force deletion
	fm.set_attr( at );

	fm.flush();
	fm.detach();						// Forget it.
	}
d117 13
a129 14
	{
	
	char	ext[_MAX_EXT], dummy[_MAX_PATH];
	_splitpath( (char *)fn, dummy, dummy, dummy, ext);


	if( stricmp( ext, "msg" ) == 0 )
		return Yes;

	if( stricmp( ext, ".msg" ) == 0 )
		return Yes;

	return No;
	}
d135 80
a214 80
	{
	HDIR		h;
	ULONG		count;
	APIRET		rc;
	bool		did_something_this_round;
	char		mask[_MAX_PATH];
	bool		processed = No;

	sprintf( mask, "%s/*.msg", (const char *)conf.netmail_dir.c_str() );

	do
		{
		FILEFINDBUF3	ff = { 0 };
		count = 1;
		h = HDIR_SYSTEM;
		rc = DosFindFirst( mask, &h, ATTRS2FIND, &ff, sizeof(ff), &count, FIL_STANDARD );


//	dirp = opendir( (char *)(const char *)conf.netmail_dir.c_str() );
//	if( dirp == NULL )
//		fatal( EC_IO_Error, EI_Full, "Can't open directory (%s)", (const char *)conf.netmail_dir.c_str() );

		if( rc != NO_ERROR )
			{
			if( rc == ERROR_NO_MORE_FILES )
				{
				log( "#", "Nothing to do - no messages found" );
				return;
				}

			fatal( EC_OS_Error, EI_Full, "FindFirst rc = %d", rc );
			}


		did_something_this_round = No;

//	while( ( de = readdir( dirp )) != NULL ) 
		do
			{
			int			mno;

			if( msg_file( ff.achName ) )
				{
				mno = atoi( ff.achName );
				do_one( mno, did_something_this_round );
				}

			if( did_something_this_round )
				processed = Yes;

			} while( 
				count = 1, 
				(rc = DosFindNext( h, &ff, sizeof(ff), &count )) 
					== NO_ERROR );

//	closedir( dirp );

// dunno why ERROR_INVALID_HANDLE happens, but it does :(
		if( (rc != ERROR_NO_MORE_FILES) && (rc != ERROR_INVALID_HANDLE) )
			fatal( EC_OS_Error, EI_Full, "FindNext rc = %d", rc );

		if( (rc = DosFindClose( h )) != NO_ERROR )
			error( EI_Full, "FindClose rc = %d", rc );


		} while( did_something_this_round );


	if( processed && strlen( (const char*) conf.kick_uucp_flag.c_str()) != 0 )
		{
		FILE	*flag;

		if( ( flag = fopen( conf.kick_uucp_flag.c_str(), "w" ) ) == NULL )
			error( EI_Full, "Can't create flag file '%s'", 
				(const char *)conf.kick_uucp_flag.c_str() );
		else
			fclose( flag );
		}

	}
@


1.3
log
@'did_something_this_round' kludge around errors in finds
@
text
@d8 2
a9 2
 * Revision 1.2  1995/04/12  21:20:45  dz
 * IBM C Set version seems to be working...
d11 5
a15 2
 * Revision 1.1  1995/03/11  18:24:01  dz
 * Initial revision
d64 2
a65 2
static void
do_one( int msg_no, bool &processed )
d133 1
a133 1
mainloop( void )
@


1.2
log
@IBM C Set version seems to be working...
@
text
@d8 3
a95 6

// Sorry... what a fuck next 4 lines mean?
		// Update statistics

//		fido_addr	from;
//		fm.get_from( from );
d132 2
a133 5
//	DIR *		dirp;
//	dirent *	de;
	HDIR		h = HDIR_SYSTEM;
	FILEFINDBUF3	ff = { 0 };
	ULONG		count = 1;
d135 1
a135 1

d137 1
d140 7
a146 2
	
	rc = DosFindFirst( mask, &h, ATTRS2FIND, &ff, sizeof(ff), &count, FIL_STANDARD );
d153 1
a153 3
	if( rc != NO_ERROR )
		{
		if( rc == ERROR_NO_MORE_FILES )
d155 7
a161 2
			log( "#", "Nothing to do - no messages found" );
			return;
a163 3
		fatal( EC_OS_Error, EI_Full, "FindFirst rc = %d", rc );
		}

d165 1
a165 1
	bool	processed = No;
d168 1
a168 5
	do
		{
		int			mno;

		if( msg_file( ff.achName ) )
d170 1
a170 3
			mno = atoi( ff.achName );
			do_one( mno, processed );
			}
d172 13
a184 4
		} while( 
			count = 1, 
			(rc = DosFindNext( h, &ff, sizeof(ff), &count )) 
				== NO_ERROR );
d188 6
a193 2
	if( rc != ERROR_NO_MORE_FILES )
		fatal( EC_OS_Error, EI_Full, "FindNext rc = %d", rc );
a194 2
	if( DosFindClose( h ) != NO_ERROR )
		error( EI_Full, "FindClose rc = %d", rc );
d196 1
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/fido2uu/vcs/mainloop.c_v  $
d48 1
a48 2
//#include	<dir.h>
#include	<dirent.h>
d51 5
d57 1
d117 2
a118 2
	char	ext[20];
	_splitpath( fn, NULL, NULL, NULL, ext);
d135 10
a144 2
	DIR *		dirp;
	dirent *	de;
d146 6
a151 3
	dirp = opendir( (char *)(const char *)conf.netmail_dir.c_str() );
	if( dirp == NULL )
		fatal( EC_IO_Error, EI_Full, "Can't open directory (%s)", (const char *)conf.netmail_dir.c_str() );
d153 12
d167 2
a168 1
	while( ( de = readdir( dirp )) != NULL ) 
d172 12
a183 2
		if( !msg_file( de->d_name ) )
			continue;
d185 2
a186 2
		mno = atoi( de->d_name );
		do_one( mno, processed );
d188 2
a189 1
		}
a191 1
	closedir( dirp );
a195 1
//		char	fn[200];
a196 2
//		sprintf( fn, "%s\\uumail.flg", (const char *)conf.flag_dir );
//		if( ( flag = fopen( fn, "w" ) ) == NULL )
@
