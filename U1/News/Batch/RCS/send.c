head	1.6;
access;
symbols;
locks;
comment	@ * @;


1.6
date	96.05.21.00.21.04;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.05.20.23.58.45;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.08.07.05.15.02;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.02.13.31.38;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.03.11.18.26.07;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.06.22.43.45;	author dz;	state Exp;
branches;
next	;


desc
@@


1.6
log
@cosmetical changes
@
text
@/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Send article/batch using given method
 *
 *      $Log: send.c $
 *      Revision 1.5  1996/05/20 23:58:45  dz
 *      current
 *
 *      Revision 1.4  1995/08/07 05:15:02  dz
 *      Using App class
 *
 *      Revision 1.3  1995/08/02 13:31:38  dz
 *      grades done, but not tested well
 *      freopens replaced with dups partially. must be replaced everywhere!
 *
 *      Revision 1.2  1995/03/11  18:26:07  dz
 *      snews=host mode
 *
 *      Revision 1.2  1995/03/11  18:26:07  dz
 *      snews=host mode
 *
 *      Revision 1.1  1995/03/06  22:43:45  dz
 *      Initial revision
 *      
 *      Rev 1.5   28 Nov 1992 22:06:00   dz
 *      String to const char * conversions fixed
 *      
 *      Rev 1.4   23 Oct 1992 14:52:24   dz
 *      consts added, error()
 *      
 *      Rev 1.3   18 Sep 1992 03:06:22   dz
 *      special handling of Waffle uux
 *      
 *      Rev 1.2   18 Jun 1992 11:02:48   dz
 *      ViaRsh method added
 *      path searching kind of spawn used
 *      
 *      Rev 1.0   26 Apr 1992 14:18:56   dz
 *      Initial revision.
 *
 *
\*/

#include    "batch.h"
#include    "..\..\version.h"

#include	<process.h>
#include	<fcntl.h>
#include	<share.h>
#include	<io.h>

static bool		run_uusmail	( const char *to );
static bool		run_uux		( const char *to );
static bool		run_rsh		( const char *to );
static bool		run_filt	( const char *to );
static bool		run_uux_snews	( const char *to, const char *via );

bool BatchApp::send( feed::f_mode feed_mode, const char *dest, const char *snews, const char *fn )
    {
    bool	ex;
    
    log( "N", "Sending %s to %s, mode %d", fn, dest, (int)feed_mode );
    
    int in_fd = _dup( 0 );
    if( in_fd < 0 )
        {
        error( EI_Full, "Can't dup stdin" );
        return Err;
        }
    
    if( _close( 0 ) < 0 )
        {
        error( EI_Full, "Can't close stdin" );
        return Err;
        }
    
        {
        int rc;
        if( (rc = _sopen( fn, O_RDONLY, SH_DENYWR, 0 )) != 0 )
            {
            error( EI_Full, "Can't reopen stdin, rc = %d", rc );
            return Err;
            }
        }
    
    switch( feed_mode )
        {
        case feed::ViaMail:
            ex = run_uusmail( dest );
            break;
            
        case feed::ViaUux:
            if( strlen( snews ) )
                ex = run_uux_snews( dest, snews );
            else
                ex = run_uux( dest );
            break;
            
        case feed::ViaRsh:
            ex = run_rsh( dest );
            break;
            
        case feed::ViaFilt:
            ex = run_filt( dest );
            break;
            
        default:
            error( EI_None, "Unknown feed method %d for address %s", (int)feed_mode, dest );
            ex = Err;
            break;
        }
    
    if( _close( 0 ) < 0 )
        {
        error( EI_Full, "Can't close redirected stdin" );
        return Err;
        }
    
    int rc = _dup( in_fd );
    if( rc != 0 )
        {
        error( EI_Full, "Can't dup stdin back, rc = %d", rc );
        return Err;
        }
    
    if( ex != Err )	sent_something = Yes;
    
    return ex;
    }




static bool run_uusmail( const char *to )
    {
    int		ex;
    char	from[80];
    
    sprintf( from, "news@@%s", (const char *)conf.def_domain.c_str() );
    
    debug("spawning %s %s", (const char *)conf.rmail_exe_name.c_str(), to );
    ex = spawnlp( P_WAIT, (char *)(const char *)conf.rmail_exe_name.c_str(), (char *)(const char *)conf.rmail_exe_name.c_str(), to, NULL );
    
    if( ex != 0 && ex != conf.rmail_exitr )
        {
        error( EI_Full, "%s returned exit code %d", (const char *)conf.rmail_exe_name.c_str(), ex );
        return Err;
        }
    
    return Ok;
    }


static bool run_uux( const char *to )
    {
    int		ex;
    char	cmd[80];
    
    sprintf( cmd, "%s!rnews", to );
    
    if( conf.uucp == UUCP_Waffle )
        {
        debug("spawning %s -p %s", (const char *)conf.uux_exe_name.c_str(), cmd );
        ex = spawnlp( P_WAIT, (char *)(const char *)conf.uux_exe_name.c_str(), (char *)(const char *)conf.uux_exe_name.c_str(), "-p", cmd, NULL );
        }
    else if( conf.uucp == UUCP_11R )
        {
        const char *grade = conf.news_grade.length() ? conf.news_grade.c_str() : 0;
        
        if( grade )
            {
            debug("spawning %s -np -g%s %s", (const char *)conf.uux_exe_name.c_str(), conf.news_grade.c_str(), cmd );
            ex = spawnlp( P_WAIT, (char *)(const char *)conf.uux_exe_name.c_str(), (char *)(const char *)conf.uux_exe_name.c_str(), "-np", "-g", grade, cmd, NULL );
            }
        else
            {
            debug("spawning %s -np %s", (const char *)conf.uux_exe_name.c_str(), cmd );
            ex = spawnlp( P_WAIT, (char *)(const char *)conf.uux_exe_name.c_str(), (char *)(const char *)conf.uux_exe_name.c_str(), "-np", cmd, NULL );
            }
        }
    else
        {
        debug("spawning %s -np %s", (const char *)conf.uux_exe_name.c_str(), cmd );
        ex = spawnlp( P_WAIT, (char *)(const char *)conf.uux_exe_name.c_str(), (char *)(const char *)conf.uux_exe_name.c_str(), "-np", cmd, NULL );
        }

    
    if( ex != 0 )
        {
        error( EI_Full, "%s returned exit code %d", (const char *)conf.uux_exe_name.c_str(), ex );
        return Err;
        }
    
    return Ok;
    }




static bool run_uux_snews( const char *to, const char *via )
    {
    int		ex;
    char	cmd[80];
    
    sprintf( cmd, "%s!snews", via );
    
    if( conf.uucp == UUCP_Waffle )
        {
        debug("spawning %s -p %s %s", (const char *)conf.uux_exe_name.c_str(), cmd, to );
        ex = spawnlp( P_WAIT, (char *)(const char *)conf.uux_exe_name.c_str(), (char *)(const char *)conf.uux_exe_name.c_str(), "-p", cmd, to, NULL );
        }
    else if( conf.uucp == UUCP_11R )
        {
        const char *grade = conf.news_grade.length() ? conf.news_grade.c_str() : 0;
        
        if( grade )
            {
            debug("spawning %s -np -g%s %s %s", (const char *)conf.uux_exe_name.c_str(), conf.news_grade.c_str(), cmd, to );
            ex = spawnlp( P_WAIT, (char *)(const char *)conf.uux_exe_name.c_str(), (char *)(const char *)conf.uux_exe_name.c_str(), "-np", "-g", grade, cmd, to, NULL );
            }
        else
            {
            debug("spawning %s -np %s %s", (const char *)conf.uux_exe_name.c_str(), cmd, to );
            ex = spawnlp( P_WAIT, (char *)(const char *)conf.uux_exe_name.c_str(), (char *)(const char *)conf.uux_exe_name.c_str(), "-np", cmd, to, NULL );
            }
        }
    else
        {
        debug("spawning %s -np %s %s", (const char *)conf.uux_exe_name.c_str(), cmd, to );
        ex = spawnlp( P_WAIT, (char *)(const char *)conf.uux_exe_name.c_str(), (char *)(const char *)conf.uux_exe_name.c_str(), "-np", cmd, to, NULL );
        }
    
    if( ex != 0 )
        {
        error( EI_Full, "%s returned exit code %d", (const char *)conf.uux_exe_name.c_str(), ex );
        return Err;
        }
    
    return Ok;
    }



static bool run_rsh( const char *to )
    {
    int		ex;
    
    debug("spawning rsh %s -b rnews", to );
    
    ex = spawnlp( P_WAIT, "rsh", "rsh", to, "-b", "rnews", NULL );
    
    if( ex != 0 )
        {
        error( EI_Full, "rsh returned exit code %d", ex );
        return Err;
        }
    
    return Ok;
    }


static bool run_filt( const char *to )
    {
    int		ex;
    char	*tc = strdup( to );
    
    if( tc == NULL )
        {
        error( EI_None, "Out of memory in run_filt");
        return Err;
        }
    
    debug("spawning %s", tc );
    
    ex = spawnlp( P_WAIT, tc, tc, NULL );
    
    if( ex != 0 )
        {
        error( EI_Full, "%s returned exit code %d", tc, ex );
        return Err;
        }
    
    return Ok;
    }


@


1.5
log
@current
@
text
@d8 3
d18 2
a19 5
 * Revision 1.2  1995/03/11  18:26:07  dz
 * snews=host mode
 *
 * Revision 1.2  1995/03/11  18:26:07  dz
 * snews=host mode
d21 2
a22 2
 * Revision 1.1  1995/03/06  22:43:45  dz
 * Initial revision
d24 2
d27 1
a27 1
 *         Rev 1.5   28 Nov 1992 22:06:00   dz
d30 1
a30 1
 *         Rev 1.4   23 Oct 1992 14:52:24   dz
d33 1
a33 1
 *         Rev 1.3   18 Sep 1992 03:06:22   dz
d36 1
a36 1
 *         Rev 1.2   18 Jun 1992 11:02:48   dz
d40 1
a40 1
 *         Rev 1.0   26 Apr 1992 14:18:56   dz
@


1.4
log
@Using App class
@
text
@d8 3
d58 207
a264 231
bool
BatchApp::send( feed::f_mode feed_mode, const char *dest, const char *snews, const char *fn )
	{
	bool	ex;

	log( "N", "Sending %s to %s, mode %d", fn, dest, (int)feed_mode );

//	if( freopen( fn, "rt", stdin ) == NULL )

	int in_fd = _dup( 0 );
	if( in_fd < 0 )
		{
		error( EI_Full, "Can't dup stdin" );
		return Err;
		}

/*
	if( freopen( fn, "r", stdin ) == NULL )
		{
		error( EI_Full, "Can't reopen stdin" );
		return Err;
		}
*/

	if( _close( 0 ) < 0 )
		{
		error( EI_Full, "Can't close stdin" );
		return Err;
		}

	{
	int rc;
	if( (rc = _sopen( fn, O_RDONLY, SH_DENYWR, 0 )) != 0 )
		{
		error( EI_Full, "Can't reopen stdin, rc = %d", rc );
		return Err;
		}
	}

	switch( feed_mode )
		{
	case feed::ViaMail:
		ex = run_uusmail( dest );
		break;

	case feed::ViaUux:
		if( strlen( snews ) )
			ex = run_uux_snews( dest, snews );
		else
			ex = run_uux( dest );
		break;

	case feed::ViaRsh:
		ex = run_rsh( dest );
		break;

	case feed::ViaFilt:
		ex = run_filt( dest );
		break;

	default:
		error( EI_None, "Unknown feed method %d for address %s", (int)feed_mode, dest );
		ex = Err;
		break;
		}

//	freopen( "con", "rt", stdin );
//	freopen( "con", "r", stdin );


	if( _close( 0 ) < 0 )
		{
		error( EI_Full, "Can't close redirected stdin" );
		return Err;
		}

	int rc = _dup( in_fd );
	if( rc != 0 )
		{
		error( EI_Full, "Can't dup stdin back, rc = %d", rc );
		return Err;
		}

	if( ex != Err )	sent_something = Yes;

	return ex;
	}




static bool
run_uusmail( const char *to )
	{
	int		ex;
	char	from[80];

	sprintf( from, "news@@%s", (const char *)conf.def_domain.c_str() );

//	debug("spawning %s -f %s %s", conf.rmail_exe_name.c_str(), from, to );
//	ex = spawnl( P_WAIT, conf.rmail_exe_name, conf.rmail_exe_name, "-f", from, to, NULL );
	debug("spawning %s %s", (const char *)conf.rmail_exe_name.c_str(), to );
	ex = spawnlp( P_WAIT, (char *)(const char *)conf.rmail_exe_name.c_str(), (char *)(const char *)conf.rmail_exe_name.c_str(), to, NULL );

	if( ex != 0 && ex != conf.rmail_exitr )
		{
		error( EI_Full, "%s returned exit code %d", (const char *)conf.rmail_exe_name.c_str(), ex );
		return Err;
		}

	return Ok;
	}


static bool
run_uux( const char *to )
	{
	int		ex;
	char	cmd[80];

	sprintf( cmd, "%s!rnews", to );

	if( conf.uucp == UUCP_Waffle )
		{
		debug("spawning %s -p %s", (const char *)conf.uux_exe_name.c_str(), cmd );
		ex = spawnlp( P_WAIT, (char *)(const char *)conf.uux_exe_name.c_str(), (char *)(const char *)conf.uux_exe_name.c_str(), "-p", cmd, NULL );
		}
	else if( conf.uucp == UUCP_11R )
		{
		const char *grade = conf.news_grade.length() ? conf.news_grade.c_str() : 0;

		if( grade )
			{
			debug("spawning %s -np -g%s %s", (const char *)conf.uux_exe_name.c_str(), conf.news_grade.c_str(), cmd );
			ex = spawnlp( P_WAIT, (char *)(const char *)conf.uux_exe_name.c_str(), (char *)(const char *)conf.uux_exe_name.c_str(), "-np", "-g", grade, cmd, NULL );
			}
		else
			{
			debug("spawning %s -np %s", (const char *)conf.uux_exe_name.c_str(), cmd );
			ex = spawnlp( P_WAIT, (char *)(const char *)conf.uux_exe_name.c_str(), (char *)(const char *)conf.uux_exe_name.c_str(), "-np", cmd, NULL );
			}
		}
	else
		{
		debug("spawning %s -np %s", (const char *)conf.uux_exe_name.c_str(), cmd );
		ex = spawnlp( P_WAIT, (char *)(const char *)conf.uux_exe_name.c_str(), (char *)(const char *)conf.uux_exe_name.c_str(), "-np", cmd, NULL );
		}




	if( ex != 0 )
		{
		error( EI_Full, "%s returned exit code %d", (const char *)conf.uux_exe_name.c_str(), ex );
		return Err;
		}

	return Ok;
	}




static bool
run_uux_snews( const char *to, const char *via )
	{
	int		ex;
	char	cmd[80];

	sprintf( cmd, "%s!snews", via );

	if( conf.uucp == UUCP_Waffle )
		{
		debug("spawning %s -p %s %s", (const char *)conf.uux_exe_name.c_str(), cmd, to );
		ex = spawnlp( P_WAIT, (char *)(const char *)conf.uux_exe_name.c_str(), (char *)(const char *)conf.uux_exe_name.c_str(), "-p", cmd, to, NULL );
		}
	else if( conf.uucp == UUCP_11R )
		{
		const char *grade = conf.news_grade.length() ? conf.news_grade.c_str() : 0;

		if( grade )
			{
			debug("spawning %s -np -g%s %s %s", (const char *)conf.uux_exe_name.c_str(), conf.news_grade.c_str(), cmd, to );
			ex = spawnlp( P_WAIT, (char *)(const char *)conf.uux_exe_name.c_str(), (char *)(const char *)conf.uux_exe_name.c_str(), "-np", "-g", grade, cmd, to, NULL );
			}
		else
			{
			debug("spawning %s -np %s %s", (const char *)conf.uux_exe_name.c_str(), cmd, to );
			ex = spawnlp( P_WAIT, (char *)(const char *)conf.uux_exe_name.c_str(), (char *)(const char *)conf.uux_exe_name.c_str(), "-np", cmd, to, NULL );
			}
		}
	else
		{
		debug("spawning %s -np %s %s", (const char *)conf.uux_exe_name.c_str(), cmd, to );
		ex = spawnlp( P_WAIT, (char *)(const char *)conf.uux_exe_name.c_str(), (char *)(const char *)conf.uux_exe_name.c_str(), "-np", cmd, to, NULL );
		}

	if( ex != 0 )
		{
		error( EI_Full, "%s returned exit code %d", (const char *)conf.uux_exe_name.c_str(), ex );
		return Err;
		}

	return Ok;
	}



static bool
run_rsh( const char *to )
	{
	int		ex;

	debug("spawning rsh %s -b rnews", to );

	ex = spawnlp( P_WAIT, "rsh", "rsh", to, "-b", "rnews", NULL );

	if( ex != 0 )
		{
		error( EI_Full, "rsh returned exit code %d", ex );
		return Err;
		}

	return Ok;
	}


static bool
run_filt( const char *to )
	{
	int		ex;
d266 19
a284 19

	if( tc == NULL )
		{
		error( EI_None, "Out of memory in run_filt");
		return Err;
		}

	debug("spawning %s", tc );

	ex = spawnlp( P_WAIT, tc, tc, NULL );

	if( ex != 0 )
		{
		error( EI_Full, "%s returned exit code %d", tc, ex );
		return Err;
		}

	return Ok;
	}
@


1.3
log
@grades done, but not tested well
freopens replaced with dups partially. must be replaced everywhere!
@
text
@d7 5
a11 1
 *      $Log: send.C $
d56 1
a56 1
send( feed::f_mode feed_mode, const char *dest, const char *snews, const char *fn )
@


1.2
log
@snews=host mode
@
text
@d8 6
d41 3
d58 11
a68 1
	if( freopen( fn, "rt", stdin ) == NULL )
d73 16
d117 16
a132 1
	freopen( "con", "rt", stdin );
d178 15
d199 3
d226 15
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/batch/vcs/send.c_v  $
d40 1
d43 1
a43 1
send( feed::f_mode feed_mode, const char *dest, const char *fn )
d62 4
a65 1
		ex = run_uux( dest );
d142 32
@
