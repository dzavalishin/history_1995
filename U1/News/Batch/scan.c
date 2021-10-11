/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Scan directory for batches, unbatch & kill 'em
 *
 *  Todo: Lines: hl in batched news
 *
 *      $Log: scan.c $
 *      Revision 1.5  1996/05/21 00:21:04  dz
 *      cosmetical changes
 *
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



