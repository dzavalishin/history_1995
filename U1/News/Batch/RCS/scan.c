head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	96.05.21.00.21.04;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.08.07.05.15.02;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.04.10.02.08.14;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.03.11.18.26.07;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.06.22.44.52;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@cosmetical changes
@
text
@/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Scan directory for batches, unbatch & kill 'em
 *
 *  Todo: Lines: hl in batched news
 *
 *      $Log: scan.c $
 *      Revision 1.4  1995/08/07 05:15:02  dz
 *      Using App class
 *
 *      Revision 1.3  1995/04/10  02:08:14  dz
 *      IBM C Set version
 *
 *      Revision 1.2  1995/03/11  18:26:07  dz
 *      snews=host mode
 *
 *      Revision 1.1  1995/03/06  22:44:52  dz
 *      Initial revision
 *      
 *      Rev 1.8   07 Jun 1993 17:05:52   dz
 *      Light fixes
 *      
 *      Rev 1.7   28 Nov 1992 22:01:58   dz
 *      Lines:, first try
 *      
 *      Rev 1.6   15 Nov 1992 23:00:52   dz
 *      add_from_ uses library function now.
 *      
 *      Rev 1.5   23 Oct 1992 14:51:04   dz
 *      error()
 *      
 *      Rev 1.4   22 Oct 1992 14:00:46   dz
 *      Structure reorganization
 *      
 *      Rev 1.3   18 Sep 1992 03:05:08   dz
 *      Calling add_stat_news()
 *      
 *      Rev 1.2   18 Jun 1992 10:57:58   dz
 *      recoding
 *
 *      Rev 1.0   26 Apr 1992 14:18:10   dz
 *      Initial revision.
 *
 *
\*/

#include    "batch.h"
#include    "..\..\version.h"
#include	<datec.h>

#include	"feeds.h"
#include	"lists.h"

#include	"stat.h"
#include        "rfc.h"

#include	<fcntl.h>
#include	<share.h>

static void             kill( char *fn );
static void		add_from_( int fd );
static bool		add_lines( int fd_to, const char *fn_from );

static void		scan_feeds( feeds &f, FILE *fp );
static void		scan_arts_nc( const feed &fe, FILE *fp );
static void		scan_arts_c( const feed &fe, FILE *fp );

void BatchApp::scan()
    {
    FILE	*fp;
    lists	ld( conf.newsout_dir.c_str() );

    // Cycle through article lists, process and kill each one.

    for( ld.rewind(); ( fp = ld.next() ) != NULL; ld.kill() )
        {
        feeds	f;
        char	af[_MAX_PATH+1], ag[AL_MAX_GROUP+1];

        if( f.open( conf.feedlist_file.c_str() ) == Err )
            fatal( EC_Incorrect_Setup, EI_Full, "Can't open feeds file" );

        debug("new article list");

        scan_feeds( f, fp );

            {
            char    path[5000];    // Dummy. Such a bug one.
            printf( "Cleanup..." );
            for( alist	al1( fp ); al1.next( af, ag, path ) != Err; )
                kill( af );
            printf( "\b\b\b Done.\n" );
            }

        }
    }


//
// Cycle through feeds, send each article list to each feed
//
void BatchApp::scan_feeds( feeds &f, FILE *fp )
    {
    feed	fe;

    for( f.rewind(); f.next( fe ) == Ok; )
        {
        if( strlen( fe.snews ) )
            {
            printf( "Feed to %s via %s (snews)\n", fe.dest, fe.snews );
            log( "n", "Feed to %s via %s (snews)\n", fe.dest, fe.snews );
            }
        else
            {
            printf("Feed to %s\n", fe.dest );
            log( "n", "feed to %s", fe.dest );
            }

        if( fe.mode == feed::ViaMail &&				// Incorrect feed
           fe.comp != feed::NoComp &&				// mode & compression
           fe.comp != feed::Batch					// combination
          ) {
        error( EI_None, "ViaMail mechanism can't be used to send compressed batches" );
        fe.comp = feed::Batch;					// Correct it.
        }

        if( fe.comp == feed::NoComp )
            scan_arts_nc( fe, fp );
        else
            scan_arts_c( fe, fp );

        }
    }

//
//	Scan & send articles - non-compressed & non-batched feed
//
void BatchApp::scan_arts_nc( const feed &fe, FILE *fp )
    {
    char	af[_MAX_PATH+1], ag[AL_MAX_GROUP+1];
    char	path[5000];

    for( alist	al( fp ); al.next( af, ag, path ) != Err; )
        {
        if( !group_in_list( ag, fe.groups ) )
            continue;

        if( dest_in_path(  path, fe.aliases ) )
            continue;

        // log( "N", "feeding '%s' (%s)", af, ag );
        add_stat_news();                        // Update counter (to show '%' field in log)

        char 	cbatch_fn[_MAX_PATH+1];		// Recoded batch
        int		cbatch_fd;

        // Prepare temp file to recode batch into
        if( mk_temp_batch( cbatch_fn, &cbatch_fd ) != Ok )
            {
            error( EI_Full, "Can't create temp file (recode)");
            continue;						// Go to next art
            }

        log( "N", "cbatch temp %s", cbatch_fn );

        add_from_( cbatch_fd );
        if( fe.add_lines )
            add_lines( cbatch_fd, af );

        // Do recode article
        const char	*code = "None";
        if( strlen(fe.codetab_name) != 0 )	// Recoding needed
            code = fe.codetab_name;

        if( recode( af, cbatch_fd, code ) == Err )
            {
            error( EI_None, "Can't recode article");
            close( cbatch_fd );
            continue;					// Go to next article
            }

        close( cbatch_fd );
        send( fe.mode, fe.dest, fe.snews, cbatch_fn );
        kill( cbatch_fn );
        }
    }


//
//	Scan & send articles - compressed or batched feed
//
void BatchApp::scan_arts_c( const feed &fe, FILE *fp )
    {
    char	batch_fn[_MAX_PATH+1];
    int		batch_fd;
    bool	batch_is_empty = Yes;
    char	af[_MAX_PATH+1], ag[AL_MAX_GROUP+1];
    char	path[5000];

    if( mk_temp_batch( batch_fn, &batch_fd ) != Ok )
        fatal( EC_OS_Error, EI_Full, "Can't create temp file");

    if( fe.mode == feed::ViaMail )			// Make RFC-822 header
        {
        add_from_( batch_fd );								// From_
        // write( batch_fd, "X-Batcher: UU2\n\n", 16 );	// End of header
        write( batch_fd, "\n", 1 );	// End of header
        }

    log( "N", "batch temp %s", batch_fn );

    for( alist	al( fp ); al.next( af, ag, path ) != Err; )
        {
        if( !group_in_list( ag, fe.groups ) )
            continue;

        if( dest_in_path(  path, fe.aliases ) )
            continue;

        // log( "N", "feeding '%s' (%s)", af, ag );
        add_stat_news();                        // Update counter (to show '%' field in log)
        put_art( af, batch_fd, fe.codetab_name );
        batch_is_empty = No;
        }

    if( batch_is_empty )				// Was made, but empty
        {
        log( "#", "Nothing to send to %s", fe.dest );
        close( batch_fd );
        kill( batch_fn );
        return;
        }

    if( fe.comp == feed::Comp12 || fe.comp == feed::Comp16 )
        {
        char 	cbatch_fn[_MAX_PATH+1];
        int		cbatch_fd;

        // Prepare temp file to compress batch into
        if( mk_temp_batch( cbatch_fn, &cbatch_fd ) != Ok )
            {
            error( EI_Full, "Can't create temp file (cbatch)");
            close( batch_fd );
            kill( batch_fn );
            return;							// Go to next feed
            }

        log( "N", "cbatch temp %s", cbatch_fn );

        lseek( batch_fd, 0L, SEEK_SET );

        if( compress(						// Compress batch
                     fe.comp == feed::Comp12 ? Yes : No,
                     batch_fd, cbatch_fd ) == Err )
            {
            error( EI_Full, "Can't compress batch");
            close( batch_fd );
            close( cbatch_fd );
            kill( batch_fn );
            kill( cbatch_fn );
            return;						// Go to next feed
            }

        close( batch_fd );
        close( cbatch_fd );
        kill( batch_fn );
        send( fe.mode, fe.dest, fe.snews, cbatch_fn );
        kill( cbatch_fn );
        }
    else
        {
        close( batch_fd );

        if( fe.comp != feed::NoComp )
            send( fe.mode, fe.dest, fe.snews, batch_fn );

        kill( batch_fn );
        }
    }

/****************************************************************************
                                   Delete file
****************************************************************************/

static void kill( char *f )
    {
    chmod( f, 0666 );
    unlink( f );
    }

/****************************************************************************
                                Add leading From_
****************************************************************************/

static void add_from_( int fd )
    {
    char  buf[130];

    mk_from_( buf, "postmaster" );
    write( fd, buf, strlen( buf ) );
    }


static bool add_lines( int fd_to, const char *fn_from )
    {
    int fd = _sopen( fn_from, O_RDONLY, SH_DENYWR );

    FILE	*fp;

    if( fd < 0 )
        {
        error( EI_Full, "Lines counter: Can't _sopen %s", fn_from );
        return Err;
        }

    fp = fdopen( fd, "r" );

    if( fp == NULL )
        {
        error( EI_Full, "Lines counter: Can't fdopen %s", fn_from );
        _close( fd );
        return Err;
        }

    int		c;
    long	l = 0;

    while( (c = getc( fp )) != EOF )
        if( c == '\n' )
            l++;

    if( ferror( fp ) )
        error( EI_Full, "Can't read %s", fn_from );

    fclose( fp );

    char buf[40];
    sprintf( buf, "Lines: %lu\n", l );
    write( fd_to, buf, strlen( buf ) );

    return Ok;
    }



@


1.4
log
@Using App class
@
text
@d10 2
a11 2
 * Revision 1.3  1995/04/10  02:08:14  dz
 * IBM C Set version
d13 2
a14 2
 * Revision 1.2  1995/03/11  18:26:07  dz
 * snews=host mode
d16 2
a17 2
 * Revision 1.1  1995/03/06  22:44:52  dz
 * Initial revision
d19 2
d22 1
a22 1
 *         Rev 1.8   07 Jun 1993 17:05:52   dz
d25 1
a25 1
 *         Rev 1.7   28 Nov 1992 22:01:58   dz
d28 1
a28 1
 *         Rev 1.6   15 Nov 1992 23:00:52   dz
d31 1
a31 1
 *         Rev 1.5   23 Oct 1992 14:51:04   dz
d34 1
a34 1
 *         Rev 1.4   22 Oct 1992 14:00:46   dz
d37 1
a37 1
 *         Rev 1.3   18 Sep 1992 03:05:08   dz
d40 1
a40 1
 *         Rev 1.2   18 Jun 1992 10:57:58   dz
d43 1
a43 1
 *         Rev 1.0   26 Apr 1992 14:18:10   dz
d57 1
a57 1
#include    "rfc.h"
d62 1
a62 1
static void     kill( char *fn );
d70 16
a85 27
void
BatchApp::scan()
	{
	FILE	*fp;
	lists	ld( conf.newsout_dir.c_str() );

	// Cycle through article lists, process and kill each one.

	for( ld.rewind(); ( fp = ld.next() ) != NULL; ld.kill() )
		{
		feeds	f;
		char	af[_MAX_PATH+1], ag[AL_MAX_GROUP+1];

		if( f.open( conf.feedlist_file.c_str() ) == Err )
			fatal( EC_Incorrect_Setup, EI_Full, "Can't open feeds file" );

		debug("new article list");

		scan_feeds( f, fp );

		{
                char    path[5000];    // Dummy. Such a bug one.
		printf( "Cleanup..." );
		for( alist	al1( fp ); al1.next( af, ag, path ) != Err; )
			kill( af );
		printf( "\b\b\b Done.\n" );
		}
d87 12
a98 2
		}
	}
d104 3
a106 32
void
BatchApp::scan_feeds( feeds &f, FILE *fp )
	{
	feed	fe;

	for( f.rewind(); f.next( fe ) == Ok; )
		{
		if( strlen( fe.snews ) )
			{
			printf( "Feed to %s via %s (snews)\n", fe.dest, fe.snews );
			log( "n", "Feed to %s via %s (snews)\n", fe.dest, fe.snews );
			}
		else
			{
			printf("Feed to %s\n", fe.dest );
			log( "n", "feed to %s", fe.dest );
			}

		if( fe.mode == feed::ViaMail &&				// Incorrect feed
			fe.comp != feed::NoComp &&				// mode & compression
			fe.comp != feed::Batch					// combination
			) {
			error( EI_None, "ViaMail mechanism can't be used to send compressed batches" );
			fe.comp = feed::Batch;					// Correct it.
			}

		if( fe.comp == feed::NoComp )
			scan_arts_nc( fe, fp );
		else
			{
			scan_arts_c( fe, fp );
			}
d108 28
a135 2
		}
	}
d140 49
a188 53
void
BatchApp::scan_arts_nc( const feed &fe, FILE *fp )
	{
//	char	batch_fn[_MAX_PATH+1];
//	int		batch_fd;

	char	af[_MAX_PATH+1], ag[AL_MAX_GROUP+1];
	char	path[5000];

	for( alist	al( fp ); al.next( af, ag, path ) != Err; )
		{
		if( !group_in_list( ag, fe.groups ) )
			continue;

		if( dest_in_path(  path, fe.aliases ) )
			continue;

//		log( "N", "feeding '%s' (%s)", af, ag );
		add_stat_news();                        // Update counter (to show '%' field in log)

		char 	cbatch_fn[_MAX_PATH+1];		// Recoded batch
		int		cbatch_fd;

		// Prepare temp file to recode batch into
		if( mk_temp_batch( cbatch_fn, &cbatch_fd ) != Ok )
			{
			error( EI_Full, "Can't create temp file (recode)");
			continue;						// Go to next art
			}

		log( "N", "cbatch temp %s", cbatch_fn );

		add_from_( cbatch_fd );
		if( fe.add_lines )
			add_lines( cbatch_fd, af );

		// Do recode article
		const char	*code = "None";
		if( strlen(fe.codetab_name) != 0 )	// Recoding needed
			code = fe.codetab_name;

		if( recode(	af, cbatch_fd, code ) == Err )
			{
			error( EI_None, "Can't recode article");
			close( cbatch_fd );
			continue;					// Go to next article
			}

		close( cbatch_fd );
		send( fe.mode, fe.dest, fe.snews, cbatch_fn );
		kill( cbatch_fn );
		}
	}
d194 88
a281 89
void
BatchApp::scan_arts_c( const feed &fe, FILE *fp )
	{
	char	batch_fn[_MAX_PATH+1];
	int		batch_fd;
	bool	batch_is_empty = Yes;
	char	af[_MAX_PATH+1], ag[AL_MAX_GROUP+1];
	char	path[5000];

	if( mk_temp_batch( batch_fn, &batch_fd ) != Ok )
		fatal( EC_OS_Error, EI_Full, "Can't create temp file");

	if( fe.mode == feed::ViaMail )			// Make RFC-822 header
		{
		add_from_( batch_fd );								// From_
//		write( batch_fd, "X-Batcher: UU2\n\n", 16 );	// End of header
		write( batch_fd, "\n", 1 );	// End of header
		}

	log( "N", "batch temp %s", batch_fn );

	for( alist	al( fp ); al.next( af, ag, path ) != Err; )
		{
		if( !group_in_list( ag, fe.groups ) )
			continue;

		if( dest_in_path(  path, fe.aliases ) )
			continue;

//		log( "N", "feeding '%s' (%s)", af, ag );
		add_stat_news();                        // Update counter (to show '%' field in log)
		put_art( af, batch_fd, fe.codetab_name );
		batch_is_empty = No;
		}

	if( batch_is_empty )				// Was made, but empty
		{
		log( "#", "Nothing to send to %s", fe.dest );
		close( batch_fd );
		kill( batch_fn );
		return;
		}

	if( fe.comp == feed::Comp12 || fe.comp == feed::Comp16 )
		{
		char 	cbatch_fn[_MAX_PATH+1];
		int		cbatch_fd;

		// Prepare temp file to compress batch into
		if( mk_temp_batch( cbatch_fn, &cbatch_fd ) != Ok )
			{
			error( EI_Full, "Can't create temp file (cbatch)");
			close( batch_fd );
			kill( batch_fn );
			return;							// Go to next feed
			}

		log( "N", "cbatch temp %s", cbatch_fn );

		lseek( batch_fd, 0L, SEEK_SET );

		if( compress(						// Compress batch
				fe.comp == feed::Comp12 ? Yes : No,
				batch_fd, cbatch_fd ) == Err )
			{
			error( EI_Full, "Can't compress batch");
			close( batch_fd );
			close( cbatch_fd );
			kill( batch_fn );
			kill( cbatch_fn );
			return;						// Go to next feed
			}

		close( batch_fd );
		close( cbatch_fd );
		kill( batch_fn );
		send( fe.mode, fe.dest, fe.snews, cbatch_fn );
		kill( cbatch_fn );
		}
	else
		{
		close( batch_fd );

		if( fe.comp != feed::NoComp )
			send( fe.mode, fe.dest, fe.snews, batch_fn );

		kill( batch_fn );
		}
	}
d284 1
a284 1
						Delete file
d287 5
a291 7
static void
kill( char *f )
	{
	chmod( f, 0666 );
	unlink( f );
//    debug("%s deleted", f );
	}
d294 1
a294 1
						Add leading From_
d297 1
a297 2
static void
add_from_( int fd )
d306 41
a346 37
static bool
add_lines( int fd_to, const char *fn_from )
	{
	int fd = _sopen( fn_from, O_RDONLY, SH_DENYWR );

	FILE	*fp;

	if( fd < 0 )
		{
		error( EI_Full, "Lines counter: Can't _sopen %s", fn_from );
		return Err;
		}

	fp = fdopen( fd, "r" );

	if( fp == NULL )
		{
		error( EI_Full, "Lines counter: Can't fdopen %s", fn_from );
		_close( fd );
		return Err;
		}

	int		c;
	long	l = 0;

	while( (c = getc( fp )) != EOF )
		if( c == '\n' )
			l++;

	if( ferror( fp ) )
		error( EI_Full, "Can't read %s", fn_from );

	fclose( fp );

	char buf[40];
	sprintf( buf, "Lines: %lu\n", l );
	write( fd_to, buf, strlen( buf ) );
a347 2
	return Ok;
	}
@


1.3
log
@IBM C Set version
@
text
@d10 3
d69 1
a69 1
scan()
d104 1
a104 1
scan_feeds( feeds &f, FILE *fp )
d143 1
a143 1
scan_arts_nc( const feed &fe, FILE *fp )
d201 1
a201 1
scan_arts_c( const feed &fe, FILE *fp )
d308 2
a309 2
	{
	char	buf[130];
d313 1
a313 1
	}
@


1.2
log
@snews=host mode
@
text
@d10 3
d54 1
a54 1
#include	<dir.h>
d76 1
a76 1
		char	af[MAXPATH+1], ag[AL_MAX_GROUP+1];
d87 1
d90 1
d142 1
a142 1
//	char	batch_fn[MAXPATH+1];
d145 1
a145 1
	char	af[MAXPATH+1], ag[AL_MAX_GROUP+1];
d159 1
a159 1
		char 	cbatch_fn[MAXPATH+1];		// Recoded batch
d200 1
a200 1
	char	batch_fn[MAXPATH+1];
d203 1
a203 1
	char	af[MAXPATH+1], ag[AL_MAX_GROUP+1];
d242 1
a242 1
		char 	cbatch_fn[MAXPATH+1];
d316 11
a326 1
	FILE	*fp = _fsopen( fn_from, "r", SH_DENYWR );
d330 2
a331 1
		error( EI_Full, "Lines counter: Can't open %s", fn_from );
@


1.1
log
@Initial revision
@
text
@d9 4
a12 1
 *      $Log:   Q:/news/batch/vcs/scan.c_v  $
d102 5
a106 2
		if( strlen( fe.snews )
			printf("Feed to %s via %s (snews)\n", fe.snews, fe.dest );
d108 1
d110 2
a111 1
		log( "n", "feed to %s", fe.dest );
d183 1
a183 1
		send( fe.mode, fe.dest, cbatch_fn );
d268 1
a268 1
		send( fe.mode, fe.dest, cbatch_fn );
d276 1
a276 1
			send( fe.mode, fe.dest, batch_fn );
@
