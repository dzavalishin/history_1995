/************************ UUCP to FIDO convertor ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Statistics module
 *
 *      $Log: Stat.c $
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *         Rev 1.7   07 Jun 1993 17:56:10   dz
 *      Now old statistics files support is optional
 *      
 *         Rev 1.6   15 Nov 1992 00:06:20   dz
 *      Style fixed :)
 *      
 *         Rev 1.5   23 Oct 1992 15:30:48   dz
 *      error()
 *      
 *         Rev 1.4   16 Oct 1992 03:40:52   dz
 *      $ and % records support
 *      
 *         Rev 1.3   05 Mar 1992 21:14:46   dz
 *      Using our zone number when unknown.
 *      
 *         Rev 1.2   04 Feb 1992 22:12:38   dz
 *      first_used time mark added
 *      
 *         Rev 1.1   11 Oct 1991 05:09:22   dz
 *      Resetting zone to 0
 *      
 *         Rev 1.0   12 Sep 1991 01:46:10   dz
 *      Initial revision.
 *
 *
\*/


#include	"log.h"
#include	<stdio.h>
#include	<share.h>
#include	<uu2io.h>
#include	<fcntl.h>
#include	<stdlib.h>
#include	<time.h>
#include	<errno.h>
#include	<string.h>

#include	"setup.h"
#include	"stat.h"
#include	"DLLInits.h"


static FILE		*sf_ussr = NULL;		// For FIDO users, in-USSR mail
static FILE		*sf_intl = NULL;		// For FIDO users, international mail




static bool
open_one( FILE **fpp, char *sname )
	{
	int				fd = -1;
	char			name[200];

    if( !conf.old_stats )
        return Err;

	sprintf( name, "%s\\%s", (const char *)conf.stat_dir.c_str(), sname );

	for( int i = 20; i; i-- )
		{
		fd	= _sopen( name, O_RDWR|O_BINARY|O_CREAT, SH_DENYWR, 0666 );
		if( fd >= 0 )
			break;

		// Supposed to check for sharing violation here...

		error( EI_Full, "Can't open or create statistics file '%s'", name );
		*fpp = NULL;
		return Err;
		}


	if( (*fpp = fdopen( fd, "ab+" )) == NULL )
		{
		close( fd );
		error( EI_Full, "Can't fdopen '%s'", name );
		return Err;
		}

	return Ok;
	}


void
open_stat( void )
	{
	static int set = 0;

	if( set == 0 )
		{
//		at exit( close_stat );
		set = 1;
		}

	close_stat();

	open_one( &sf_ussr, "FIDOUSSR.STT" );
	open_one( &sf_intl, "FIDOINTL.STT" );

	}

extern "SYSTEM" void
close_stat( void )
	{

    if( !conf.old_stats )
        return;

	if( sf_ussr )
		{
		fclose( sf_ussr );
		sf_ussr = NULL;
		}

	if( sf_intl )
		{
		fclose( sf_intl );
		sf_intl = NULL;
		}
	}

//#pra gma exit close_stat


/*--------------------------------------------------------------------------*/

static FidoUserEntry zero;


static bool
load_user( FILE *fp, FidoUserEntry *ue )
	{
	FidoUserEntry s;
	long		washere;

    if( !conf.old_stats )
        return Err;

	rewind( fp );
	do {
		washere = ftell( fp );
		if( washere < 0 )
			{
			error( EI_Full, "Can't ftell() in stat file");
			return Err;
			}

		if( fread( &s, sizeof( FidoUserEntry ), 1, fp ) != 1 )
			{
			ue->ToFido = zero.ToFido;
			ue->ToUu = zero.ToUu;
			time( &(ue->first_used) );
			for( int i = 0; i < sizeof(zero.fill); i++ )
				ue->fill[i] = 'ё';

			if( fseek( fp, 0L, SEEK_END ) < 0 )		// Prepare to write
				{
				error( EI_Full, "Stat: can't seek to end!");
				return Err;
				}
			return Ok;
			}

		if( ferror( fp ) )
			{
			error( EI_Full, "Can't read statistics file!");
			fseek( fp, 0L, SEEK_END );
			return Ok;								// Try to write to the end
			}

                if( s.Address.zone() == 0 )
                                s.Address.zone( ftn::def().fido_a.zone() );

                if( ue->Address.zone() == 0 )
                                ue->Address.zone( ftn::def().fido_a.zone() );

		} while( s.Address != ue->Address );

	*ue = s;
	if( fseek( fp, washere, SEEK_SET ) < 0 )
		{
		error( EI_Full, "stat: С дивана слезть не могу! (fseek(washere))");
		return Err;
		}

	return Ok;
	}



static void
save_user( FILE *fp, FidoUserEntry *ue )
	{
    time( &(ue->last_used) );

    if( !conf.old_stats )
        return;

        if( ue->Address.zone() == 0 )
        ue->Address.zone( ftn::def().fido_a.zone() );

	if( fwrite( ue, sizeof( FidoUserEntry ), 1, fp ) != 1 )
		error( EI_Full, "Stat: can't write down statistics record");
	}


/*--------------------------------------------------------------------------*/

long	stat_mail_messages_processed = 0l;
long    stat_news_messages_processed = 0l;



void
add_stat_fido(  const fido_addr &who, const unsigned char *uucp_addr,
				bool is_free,   bool file, bool to_fido,
				long bytes )
	{

	stat_mail_messages_processed++;

	log( "$", "\"Mail\", \"%s\", \"%s\", \"%s free\", \"%s fido\", %ld",
		(const char *)((String)who).c_str(),	uucp_addr,
		is_free ? "is" : "not",     to_fido ? "to" : "from",
        bytes
        );

    if( conf.old_stats )
        {

        FILE    *fp = is_free ? sf_ussr : sf_intl;

        if( fp == NULL )
            open_stat();

        fp = is_free ? sf_ussr : sf_intl;

        if( fp == NULL )
            {
            error( EI_None, "No statistics will be gathered");
            return;
            }


        FidoUserEntry       ue;

        ue.Address = who;
        if( load_user( fp, &ue ) == Err )
            return;

        StatCount   *sc = to_fido ? &(ue.ToFido) : &(ue.ToUu);

        sc->Letters++;
        sc->Bytes += bytes;
        if( file )
            sc->Files++;

        save_user( fp, &ue );
        }
	}



void
add_stat_news()
	{
    stat_news_messages_processed++;
	}


















