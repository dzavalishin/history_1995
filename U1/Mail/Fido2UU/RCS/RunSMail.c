head	1.7;
access;
symbols;
locks;
comment	@ * @;


1.7
date	96.05.15.05.56.33;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.03.27.10.46.53;	author dz;	state Exp;
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
date	95.04.12.21.20.45;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.10.08.31.11;	author dz;	state Exp;
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
@cwait added
@
text
@/************************ FIDO to UUCP convertor ***************************\
 *
 *      Copyright (C) 1991-1993 by Infinity Soft
 *
 *      Module  :	SendMail (or Rmail :) starter
 *
 *      $Log: RunSMail.c $
 *      Revision 1.6  1996/03/27 10:46:53  dz
 *      current
 *
 *      Revision 1.5  1995/11/06 01:42:16  dz
 *      Using FTN_Msg! Seems to be working.
 *
 *      Revision 1.4  1995/08/08 13:22:34  dz
 *      App class used
 *
 * Revision 1.3  1995/04/12  21:20:45  dz
 * IBM C Set version seems to be working...
 *
 * Revision 1.2  1995/04/10  08:31:11  dz
 * C Set
 *
 * Revision 1.1  1995/03/11  18:24:01  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:24:01  dz
 * Initial revision
 *
 *      
 *         Rev 1.6   07 Jun 1993 16:41:54   dz
 *      CC system rewritten
 *      
 *         Rev 1.5   28 Nov 1992 23:12:42   dz
 *      String to const char *, UUCP variable checking deleted
 *
 *         Rev 1.4   11 Nov 1992 05:56:42   dz
 *      Using uucp_addr
 *      
 *         Rev 1.3   23 Oct 1992 14:40:06   dz
 *      Checking UUPC/Waffle environment variable before running rmail
 *      
 *         Rev 1.2   18 Sep 1992 02:49:24   dz
 *      using dup2() instead of reopen
 *      
 *         Rev 1.1   18 Jun 1992 10:38:06   dz
 *      Support for UUPC/extended 1.11R written
 *      
 *         Rev 1.0   09 Feb 1992 23:23:10   dz
 *      Initial revision.
 *
 *
\*/


#include	"fido2uu.h"
#include	<process.h>

#include	"CC_Bag.h"

#define INCL_DOSQUEUES   /* Queue values */
#include <os2.h>



#if 0
static void
    run_one( bool &err, const uucp_addr &from, const char *to )
    {
    int		ex;
    
    if( conf.uucp_rmail_ffrom )
        {
        debug("spawning %s -f %s %s", (const char *)conf.rmail_exe_name.c_str(), (const char *) from, to );
        ex = spawnl( P_WAIT, (char *)(const char *)conf.rmail_exe_name.c_str(), (char *)(const char *)conf.rmail_exe_name.c_str(), "-f", (const char *) from, to, NULL );
        }
    else
        {
        debug("spawning %s %s", (const char *)conf.rmail_exe_name.c_str(), to );
        ex = spawnl( P_WAIT, (char *)(const char *)conf.rmail_exe_name.c_str(), (char *)(const char *)conf.rmail_exe_name.c_str(), to, NULL );
        }
    
    if( ex == -1 )
        {
        error( EI_Full, "Can't execute %s %s", (const char *)conf.rmail_exe_name.c_str(), to );
        err = Err;
        }
    
    if( ex == conf.rmail_exitl && ex != -1 )        // local delivery
        {
        error( EI_Full, "Local delivery (to %s) refused. Are you playing ping-pong?", to );
        err = Err;
        }
    
    if( ex != 0 && ex != conf.rmail_exitr )
        {
        error( EI_Full, "%s %s returned exit code %d", (const char *)conf.rmail_exe_name.c_str(), to, ex );
        err = Err;
        }
    
    }
#endif

static bool copy_data( const RFC_Msg &msg, FILE *out, bool codeback = Yes )
    {
    /*
    msg.set_saver_recoder( 0 );
        
    if( use_recoder && codeback )
        {
        recoder      r( conf.code_fu.c_str() );

        if( !r.valid() )
            error( EI_None, "Code table '%s' not found", (const char *)conf.code_uf.c_str() );
        else
            msg.set_saver_recoder( &r );
        }
    */
    return msg.save( out );
    }


static bool run_one( bool &err, const RFC_Msg &msg, const string &from, const string &to )
    {
    int		ex;
    
    debug("run_one( msg, %s, %s )", (const char *)from, (const char *)to);
    

    ulong    ReadHandle;    /* Pointer to the read handle      */
    ulong    WriteHandle;   /* Pointer to the write handle     */
    APIRET   rc;            /* Return code                     */
    const    PipeSize = 4096;

    rc = DosCreatePipe(&ReadHandle, &WriteHandle, PipeSize);
    if (rc != 0)
        {
        error( EI_Full, "DosCreatePipe error: return code = %ld", rc );
        err = Err;
        return Err;
        }
    
    int stdin_save = dup( 0 );
    
    DosClose( 0 );
    if( 0 != dup2( ReadHandle, 0 ) )
        {
        error( EI_Full, "Can't dup2" );
        err = Err;
        return Err;
        }
    
    DosClose( ReadHandle );
    
    DosSetFHState( WriteHandle, OPEN_FLAGS_NOINHERIT );
    DosSetFHState( stdin_save, OPEN_FLAGS_NOINHERIT );

    const char *argv[20];
    int         argc = 1;
    
    argv[0] = "rmail";
    
    if( conf.uucp == UUCP_Ache30 || conf.uucp_rmail_ffrom )
        {
        argv[argc++] = "-f";
        argv[argc++] = from;
        debug("-f %s", (const char *)from );
        }
    
    /*
    if( conf.uucp == UUCP_Ache30 && from_Unix )
        {
        argv[argc++] = "-u";
        debug("-u");
        }
    */
    
    argv[argc++] = to;
    
    argv[argc++] = 0;     // End of args
    
    bool ret = Ok;
    
    int pid;
    
    pid = spawnv(
                 P_NOWAIT,
                 (char *)(const char *)conf.rmail_exe_name,
                 // "./rmail.exe",
                 (char **)argv );
    if( pid < 0 )
        {
        error( EI_Full, "Can't execute %s", 
              (const char *)conf.rmail_exe_name.c_str() );
        
        ret = Err;
        }
    
    
    FILE *fp = fdopen( WriteHandle, "w" );
    if( fp == NULL )
        {
        error( EI_Full, "Can't fdopen our end of pipe" );
        DosClose( WriteHandle );
        ret = Err;
        }
    
    if( ret != Err )
        {
        // Stuff RFC_Msg into the open end of pipe.
        // Copy_data will handle recode
        if( Err == copy_data( msg, fp ) ) ret = Err;
        
        int retcode;
        _cwait( &retcode, pid, WAIT_GRANDCHILD );
        if( retcode)
            {
            error( EI_None, "Error running %s: 0x%X",
                  (const char *)conf.rmail_exe_name,
                  retcode
                 );
            ret = Err;
            }
        }
    
    if( fp != NULL )
        fclose( fp );
    

    if( 0 != dup2( stdin_save, 0 ) )
        {
        error( EI_Full, "Can't dup2( %d, 0 )", stdin_save );
        ret = Err;
        }
    
    close( stdin_save );

    
    if( ret == Err ) err = Err;
    return ret;
    }


bool Fido2uuApp::run_uusmail( const uucp_addr &from, const string &to, CC_Bag &cc_list, const RFC_Msg &msg )
    {
    bool	err = Ok;
    
    int 	old_d0 = dup( 0 );						// Save stdin
    //DosClose( 0 );
    DosSetFHState( old_d0, OPEN_FLAGS_NOINHERIT );
    
    run_one( err, msg, (string &)from, to );
    
        {
        CC_BagIterator i( cc_list );
        
        for( i.setToFirst(); i.isValid(); i.setToNext() )
            {
            String &cc = (String &)cc_list.elementAt(i);
            run_one( err, msg, (string &)from, cc );
            }
        }
    
    close(0);										// Close redirection
    dup2( old_d0, 0 );								// Restore stdin
    close( old_d0 );								// Close it's copy
    
    return err;
    }


@


1.6
log
@current
@
text
@d8 3
d183 8
a190 6
    if( spawnv( 
               P_NOWAIT,
               (char *)(const char *)conf.rmail_exe_name,
               // "./rmail.exe",
               (char **)argv )
       < 0 )
d212 11
@


1.5
log
@Using FTN_Msg! Seems to be working.
@
text
@d8 3
d57 3
d61 2
d64 189
a252 66
run_one( bool &err, const uucp_addr &from, const char *to )
	{
	int		ex;

	if( conf.uucp_rmail_ffrom )
		{
		debug("spawning %s -f %s %s", (const char *)conf.rmail_exe_name.c_str(), (const char *) from, to );
		ex = spawnl( P_WAIT, (char *)(const char *)conf.rmail_exe_name.c_str(), (char *)(const char *)conf.rmail_exe_name.c_str(), "-f", (const char *) from, to, NULL );
		}
	else
		{
		debug("spawning %s %s", (const char *)conf.rmail_exe_name.c_str(), to );
		ex = spawnl( P_WAIT, (char *)(const char *)conf.rmail_exe_name.c_str(), (char *)(const char *)conf.rmail_exe_name.c_str(), to, NULL );
		}

	if( ex == -1 )
		{
		error( EI_Full, "Can't execute %s %s", (const char *)conf.rmail_exe_name.c_str(), to );
		err = Err;
		}

	if( ex == conf.rmail_exitl && ex != -1 )        // local delivery
		{
		error( EI_Full, "Local delivery (to %s) refused. Are you playing ping-pong?", to );
		err = Err;
		}

	if( ex != 0 && ex != conf.rmail_exitr )
		{
		error( EI_Full, "%s %s returned exit code %d", (const char *)conf.rmail_exe_name.c_str(), to, ex );
		err = Err;
		}

	}


bool
Fido2uuApp::run_uusmail( const uucp_addr &from, const string &to, CC_Bag &cc_list, int fd )
	{
	int 	old_d0 = dup( 0 );						// Save stdin
	bool	err = Ok;

	close(0);										// Close it
	dup2( fd, 0 );									// Redirect

	lseek( fd, 0L, SEEK_SET );
	run_one( err, from, to );

		{
		CC_BagIterator i( cc_list );

		for( i.setToFirst(); i.isValid(); i.setToNext() )
			{
			String &cc = (String &)cc_list.elementAt(i);

			lseek( fd, 0L, SEEK_SET );
			run_one( err, from, (const char *)cc.c_str() );
			}
		}

	close(0);										// Close redirection
	dup2( old_d0, 0 );								// Restore stdin
	close( old_d0 );								// Close it's copy

	return err;
	}
@


1.4
log
@App class used
@
text
@d8 3
d93 1
a93 1
Fido2uuApp::run_uusmail( const uucp_addr &from, char *to, CC_Bag &cc_list, int fd )
@


1.3
log
@IBM C Set version seems to be working...
@
text
@d8 3
d90 1
a90 1
run_uusmail( const uucp_addr &from, char *to, CC_Bag &cc_list, int fd )
@


1.2
log
@C Set
@
text
@d8 3
d101 1
a101 1
		for( i.setToFirst(); i.isValid; i.setToNext() )
@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/fido2uu/vcs/runsmail.c_v  $
d98 1
a98 1
		for( ; (int)i; i++ )
d100 1
a100 1
			String &cc = (String &)i.Current();
@
