/************************ UU2 FILE2UU ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Process one TIC
 *
 *      $Log:   Q:/file/file2uu/vcs/process.c_v  $
 *      
 *         Rev 1.0   29 Dec 1992 01:30:42   dz
 *      Initial revision.
 *      
 *
\*/

#include    "file2uu.h"
#include	<spool.h>
#include	<elist.h>
#include	<stat.h>

#include	<share.h>
#include	<bag.h>



static bool		do_send( FILE *fp, echo_def &def, Bag &spool_files, tic_file &tic );

bool
process_tic( const char *tic_name )
	{

	/*************************************************************
						   Load TIC file
	*************************************************************/

	tic_file	tic;

	if( tic.load_from( tic_name ) == Err )
		{
		error( EI_None, "Can't load TIC (%s) - skipping", tic_name );
		return Err;
		}

	/*************************************************************
				 Find corresponding echo definition
	*************************************************************/

	echo_def    def;

	if( def.find_by_area( tic.area() ) == Err )
		return Err;

	/*************************************************************
						   Find data file
	*************************************************************/


	char	fullname[100];
	sprintf( fullname, "%s\\%s",
		(const char *)conf.fileout_dir, tic.name() );

	FILE		*f = _fsopen( fullname, "rb", SH_DENYWR );
	if( f == NULL )
		{
		error( EI_Full, "Can't open %s for %s", fullname, tic_name );
		return Err;
		}

	/*************************************************************
						Perform conversion
	*************************************************************/

	Bag		spool_files;

	spool_files.ownsElements( 1 );

	if( do_send( f, def, spool_files, tic ) == Err )
		{
		fclose( f );
		return Err;
		}
	else
		{
		add_stat_news();	// Increase counter of sent articles (for log '%' record)

		ContainerIterator &i = spool_files.initIterator();
		for(; (int)i; i++ )
			{
			news_out_spool_file &sf = (news_out_spool_file &)i.current();
			sf.post( def.group() );
			}

		fclose( f );
		chmod( fullname, 0666 );
		if( unlink( fullname ) )
			error( EI_Full, "Can't delete %s", fullname );

		}

	return Ok;
	}


/****************************************************************************
						Ok, multipart sender
****************************************************************************/


static bool
do_send( FILE *fp, echo_def &def, Bag &spool_files, tic_file &tic )
	{
	int		n_parts;

	fflush( fp );
	long	size = filelength( fileno( fp ) );

	n_parts = (int) (size / part_bytes);
	if( size % part_bytes )
		n_parts++;

    fprintf( stderr, "File %s will be sent in %d part(s)\n", tic.name(), n_parts );
	log( "f", "File %s will be sent in %d part(s)", tic.name(), n_parts );

	for( int part = 1; part <= n_parts; part++ )
		{
		fprintf( stderr, "\tSending part %d...\n", part );
		log( "f", "Sending part %d...", part );

		news_out_spool_file * sf = new news_out_spool_file;
		if( ((Object *)sf) == Object::ZERO )
			{
			error( EI_None, "Out of memory!!" );
			return Err;
			}

		if( sf->create() == Err )
			return Err;

		spool_files.add( (Object &)*sf );

		if( encode_part
				(
				part, n_parts, part_bytes,
				def, tic, sf->fp(), fp
				) == Err )
			return Err;

		if( sf->close() == Err )
			return Err;

		}

	return Ok;
	}
















