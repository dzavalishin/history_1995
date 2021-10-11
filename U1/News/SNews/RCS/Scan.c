head	1.7;
access;
symbols;
locks;
comment	@ * @;


1.7
date	95.11.06.13.01.46;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	95.08.07.13.57.10;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	95.08.07.13.52.20;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.08.07.13.34.55;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.07.13.28.26;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.09.23.24.27;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.31.14;	author dz;	state Exp;
branches;
next	;


desc
@@


1.7
log
@Using FTN_Msg
@
text
@/************************ UU2 Snews ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :	Scan for messages with echo packets attached
 *
 *      $Log: Scan.c $
 *      Revision 1.6  1995/08/07 13:57:10  dz
 *      Cleanup
 *
 *      Revision 1.5  1995/08/07 13:52:20  dz
 *      Using ffolder class
 *
 *      Revision 1.4  1995/08/07 13:34:55  dz
 *      checkpoint
 *
 *      Revision 1.3  1995/08/07 13:28:26  dz
 *      Using class App
 *
 *      Revision 1.2  1995/04/09  23:24:27  dz
 *      IBM C Set version
 *
 *      Revision 1.1  1995/03/11  18:31:14  dz
 *      Initial revision
 *
 *      
 *         Rev 1.3   07 Jun 1993 17:10:42   dz
 *      Port to OS/2 fixes, new message flags system, other fixes.
 *      
 *         Rev 1.2   28 Nov 1992 22:22:40   dz
 *      style improvements, fixes...
 *      
 *         Rev 1.1   18 Jun 1992 11:08:58   dz
 *      update
 *      
 *         Rev 1.0   06 Oct 1991 00:17:56   dz
 *      Initial revision.
 *
 *
\*/

#include	"snews.h"
#include	"..\..\version.h"
#include	<ffolder.h>


static bool		select( FTN_Msg &fm );


void
SnewsApp::scan( void )
    {
    ffolder		folder( conf.netmail_dir );
    bool		err = No;
    bool		trunc_file = No;
    
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
                if( !select( fm ) )       // Subject to convert ?
                    continue;
                
                string     name;
                
                track_reset();						// Reset track buffer

                if( !(fm.attr()&FF_FileAttached) )
                    {
                    error( EI_None, "No file attached to message!");
                    err = Yes;
                    trunc_file = No;
                    }
                else
                    {
                    fm.get_subj( name );
                    log( "e", "Processing packet '%s'", (const char *)name );
                    trunc_file = Yes;
                    }
                
                if( !err )
                    unpack( name );
                
                if( trunc_file )
                    {
                    chmod( name, 0666 );
                    unlink( name );					// Don't truncate - kill
                    }
                
                fm.add_attr( FF_Sent );				// Mark letter as sent
                fm.add_attr( FF_KillSent );			// Force deletion
                
                fm.detach();						// Forget it.

                
            } // switch
        break;
        } // while

    }







bool
select( FTN_Msg &fm )
    {
    fido_user	to;
    
    fm.get_to( to );
    
    // echo_from_addr - address WE are sending echo packets from,
    // i.e our (echo-gate's) address
    
    if( ((fido_addr)to) == conf.echo_from_addr ) return Yes;
    
    return No;
    }


@


1.6
log
@Cleanup
@
text
@d8 3
d47 1
a47 1
static bool		select( fido_msg &fm );
d52 55
a106 46
	{
	ffolder		folder( conf.netmail_dir );
	bool		err = No;
	bool		trunc_file = No;

	while( 1 )
		{
		fido_msg	fm;

		switch( folder.next( fm ) )
			{
		case Err:
			error( EI_Full, "Can't get next message in %s", (const char *)conf.netmail_dir.c_str() );
		case No:
			break;

		case Yes:
			if( !select( fm ) )       // Subject to convert ?
				continue;

			char		name[200];

			track_reset();						// Reset track buffer


			if( !(fm.attr()&FF_FileAttached) )
				{
				error( EI_None, "No file attached to message!");
				err = Yes;
				trunc_file = No;
				}
			else
				{
				fm.get_subj( name );
				log( "e", "Processing packet '%s'", name );
				trunc_file = Yes;
				}

			if( !err )
				unpack( name );

			if( trunc_file )
				{
				chmod( name, 0666 );
				unlink( name );					// Don't truncate - kill
				}
d108 1
a108 14
			fm.add_attr( FF_Sent );				// Mark letter as sent
			fm.add_attr( FF_KillSent );			// Force deletion

			fm.flush();
			fm.detach();						// Forget it.



			} // switch
		break;
		} // while


	}
d117 6
a122 6
select( fido_msg &fm )
	{
	fido_user	to;

	fm.get_to( to );

d125 5
a129 5

	if( ((fido_addr)to) == conf.echo_from_addr ) return Yes;

	return No;
	}
@


1.5
log
@Using ffolder class
@
text
@d8 3
a42 1
//#include	<fcntl.h>
a89 1
#if 1 // KILLIT
a96 1
#endif // KILLIT
a103 16
#if 0
					// Subject to convert ?
			if( !select( &fm ) )
				continue;

			track_reset();               // Prepare to log tracking

			if( process( &fm ) == Err )  // Convert letter
				error_letter( &fm ); // Send letter back

			// Mark letter as sent & force deletion
			fm.add_attr( FF_Sent|FF_KillSent );

			fm.flush();
			fm.detach();						// Forget it.
#endif
a115 128
#if 0

#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#include	<os2.h>


#define KILLIT      1

//#define ATTRS2FIND  FA_RDONLY | FA_HIDDEN | FA_SYSTEM | FA_ARCH
#define ATTRS2FIND	0x27




void
SnewsApp::scan( void )
	{
//	ffblk		ff;
	char		mask[100];
	bool		err = No;
	bool		trunc_file = No;
	HDIR		h = HDIR_SYSTEM;
	FILEFINDBUF3	ff = { 0 };
	ULONG		count = 1;
	APIRET		rc;



	sprintf( mask, "%s\\*.msg", (const char *)conf.netmail_dir.c_str() );
	debug("Search mask: `%s'", mask );

/*
    if( findfirst( mask, &ff, ATTRS2FIND ) )
		{
		if( errno == ENOENT )
			{
			log( "#", "Nothing to do - no messages found");
			return;
			}

		fatal( EC_IO_Error, EI_Full, "Findfirst");
		}
*/

	rc = DosFindFirst( mask, &h, ATTRS2FIND, &ff, sizeof(ff), &count, FIL_STANDARD );
	if( rc != NO_ERROR )
		{
		if( rc == ERROR_NO_MORE_FILES )
			{
			log( "#", "Nothing to do - no messages found" );
			return;
			}

		fatal( EC_OS_Error, EI_Full, "FindFirst rc = %d", rc );
		}


	do {
		fido_msg	fm;
		int			mno;


		mno = atoi( ff.achName );


		if( fm.attach( conf.netmail_dir.c_str(), mno ) == Err )
			{
			error( EI_Full, "Can't open message '%s'", ff.achName );
			continue;
			}

		if( select( fm ) )						// Subject to convert ?
			{
			char		name[200];

			track_reset();						// Reset track buffer
//			fm.get_attr( at );

			if( !(fm.attr()&FF_FileAttached) )
				{
				error( EI_None, "No file attached to message!");
				err = Yes;
				trunc_file = No;
				}
			else
				{
				fm.get_subj( name );
				log( "e", "Processing packet '%s'", name );
				trunc_file = Yes;
				}

			if( !err )
				unpack( name );

#if KILLIT
			if( trunc_file )
				{
				chmod( name, 0666 );
				unlink( name );					// Don't truncate - kill
				}

			fm.add_attr( FF_Sent );				// Mark letter as sent
#endif // KILLIT
			fm.add_attr( FF_KillSent );			// Force deletion

			fm.flush();
			fm.detach();						// Forget it.

			} /* if select */
		} while( 
			count = 1, 
			(rc = DosFindNext( h, &ff, sizeof(ff), &count )) 
				== NO_ERROR );

//	if( errno != ENOENT )
//		fatal( EC_IO_Error, EI_Full, "Findnext");

	if( rc != ERROR_NO_MORE_FILES )
		fatal( EC_OS_Error, EI_Full, "FindNext rc = %d", rc );

	if( DosFindClose( h ) != NO_ERROR )
		error( EI_Full, "FindClose rc = %d", rc );


	}

#endif
@


1.4
log
@checkpoint
@
text
@d8 3
d38 95
a132 1
#include	<fcntl.h>
a144 1
static bool		select( fido_msg &fm );
d259 1
a259 1

@


1.3
log
@Using class App
@
text
@d8 2
a9 2
 * Revision 1.2  1995/04/09  23:24:27  dz
 * IBM C Set version
d11 5
a15 2
 * Revision 1.1  1995/03/11  18:31:14  dz
 * Initial revision
d174 1
@


1.2
log
@IBM C Set version
@
text
@d8 3
d49 1
a49 1
scan( void )
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/snews/vcs/scan.c_v  $
a28 2
#include	<dir.h>
#include    <dos.h>
d31 4
a36 1
#define ATTRS2FIND  FA_RDONLY | FA_HIDDEN | FA_SYSTEM | FA_ARCH
d38 2
d41 1
d48 1
a48 1
	ffblk		ff;
d52 5
d58 1
d62 1
d73 13
d93 1
a93 1
		mno = atoi( ff.ff_name );
d98 1
a98 1
			error( EI_Full, "Can't open message '%s'", ff.ff_name );
d133 1
a133 1
#endif KILLIT
d140 13
a152 1
		} while( findnext(&ff) == 0 );
a153 2
	if( errno != ENOENT )
		fatal( EC_IO_Error, EI_Full, "Findnext");
@
