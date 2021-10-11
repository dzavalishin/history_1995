head	1.1;
access;
symbols;
locks;
comment	@ * @;


1.1
date	94.08.05.01.12.42;	author root;	state Exp;
branches;
next	;


desc
@main loop
@


1.1
log
@Initial revision
@
text
@/************************ Mali mover ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :	Main conversion loop
 *
 *      $Log:   Q:/fido2uu/vcs/mainloop.c_v  $
 *      
 *
 *
\*/

#include	"Mover.h"

#include	<stdlib.h>
//#include	<dir.h>
#include	<dirent.h>
#include	<errno.h>


static void
do_one( int msg_no, bool &processed )
	{

#if 0
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

// Sorry... what a fuck next 4 lines mean?
		// Update statistics

//		fido_addr	from;
//		fm.get_from( from );
		}


	at |= FF_Sent;						// Mark letter as sent
	if( !(at&FF_Local) )               	// Foreighn letter?
		at |= FF_KillSent;				// Force deletion
	fm.set_attr( at );

	fm.flush();
	fm.detach();						// Forget it.

#endif
	}



static bool
msg_file( const char *fn )
	{
	
	char	ext[20];
	_splitpath( fn, NULL, NULL, NULL, ext);


	if( stricmp( ext, "msg" ) == 0 )
		return Yes;

	if( stricmp( ext, ".msg" ) == 0 )
		return Yes;

	return No;
	}



void
mainloop( void )
	{
	DIR *		dirp;
	dirent *	de;
	
	dirp = opendir( (char *)(const char *)conf.netmail_dir.c_str() );
	if( dirp == NULL )
		fatal( EC_IO_Error, EI_Full, "Can't open directory (%s)", (const char *)conf.netmail_dir.c_str() );

	bool	processed = No;

	while( ( de = readdir( dirp )) != NULL ) 
		{
		int			mno;

		if( !msg_file( de->d_name ) )
			continue;

		mno = atoi( de->d_name );
		do_one( mno, processed );

		}


	closedir( dirp );


#if 0
	if( processed && strlen( (const char*) conf.kick_uucp_flag.c_str()) != 0 )
		{
		FILE	*flag;
//		char	fn[200];

//		sprintf( fn, "%s\\uumail.flg", (const char *)conf.flag_dir );
//		if( ( flag = fopen( fn, "w" ) ) == NULL )
		if( ( flag = fopen( conf.kick_uucp_flag.c_str(), "w" ) ) == NULL )
			error( EI_Full, "Can't create flag file '%s'", 
				(const char *)conf.kick_uucp_flag.c_str() );
		else
			fclose( flag );
		}

#endif


	}



@
