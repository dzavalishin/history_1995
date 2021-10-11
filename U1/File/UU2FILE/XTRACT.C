/************************ UU2 UU2FILE ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Extract file from group of encoded files
 *
 *      $Log:   Q:/news/snews/vcs/main.c_v  $
 *
 *
\*/

#include    "uu2file.h"
#include	"part.h"

FILE	*d_fp, *o_fp;			// Description & output files
char	d_fn[150], o_fn[150];	// And their names
bool	uu_decoder = Off;		// Can we use UUDECODE?

static bool
put_text( const uchar *s )
	{

	if( fputs( s, d_fp ) == EOF )
		{
		error( EI_Full, "Can't write to descripition file" );
		return Err;
		}

	return Ok;
	}

bool
put_text_delim( int part )
	{
	if( fprintf( d_fp, "   ------------- Part %d -------------\n", part ) == EOF )
		return Err;

	return Ok;
	}


bool
extract_one( FILE *in, bool first_part, String &out_name )
	{
	bool	in_code = No;		// We are in the text part of message now
	bool	in_header = Yes;	// We are in the header part of the message
	const	BS = 200;
	char	buf[BS];

	while( fgets( buf, BS, in ) != NULL )
		{
		if( buf[0] == '\r' || buf[0] == '\n' )
			{
			in_header = No;
			continue;
			}

		if( strncmp( buf, "BEGIN", 5 ) == 0 )
			{
			in_code = Yes;
			continue;
			}

		if( strncmp( buf, "END", 3 ) == 0 )
			{
			in_code = No;
			continue;
			}

		if( in_code == No )
			{
			if( (!first_part) && in_header )	// Don't need a copy of header
				continue;						// If not a firts part

			if( put_text( buf ) == Err )
				return Err;

			continue;
			}


		char	realname[101];
		if( sscanf( buf, "begin %*d %100s", realname ) == 1 )
			{
			if( !first_part )
				{
				error( EI_None, "Can't handle multi-file archive!!" );
				return Err;
				}
			out_name = (const signed char *)realname;
			uu_decoder = On;
			continue;
			}

		if( strncmp( buf, "end", 3 ) == 0 )
			{
			uu_decoder = Off;
			continue;
			}

		if( uu_decoder == On )
			{
			if( uudecode( buf, o_fp ) == Err )
				return Err;
			}
		else
			{
			error( EI_None, "Code line found in non-decoding state: '%s'", buf );
			return Err;
			}
		}

	return Ok;
	}



bool
go_extract( Bag &group, String &out_name )
	{
	String	id;
	int		nparts;

		{
		ContainerIterator	&i = group.initIterator();
		part		&p = (part &)i.current();
		id = p.id_v;
		nparts = p.pts;
		}


	for( int i = 1; i <= nparts; i++ )
		{
		part		*p = get_part( group, id, i );

		if( p == NULL )
			{
			error( EI_None, "Internal error - no part %d found in list!", i );
			return Err;
			}

		const char	*fn = (const char *)(p->fn_v);

		printf("\t\tExtracting part %d...\n", i );

		FILE *in = fopen( fn, "rt" );
		if( in == NULL )
			{
			error( EI_Full, "Can't open %s", fn );
			return Err;
			}

		bool	done;

		if( put_text_delim( i ) == Err )
			done = Err;
		else
			done = extract_one( in, i == 1 ? Yes : No, out_name );

		fclose( in );

		if( done == Err )
			return Err;

		}

	return Ok;
	}




static void
get_descr( String &descr, const Bag &group, const char *short_name )
	{
	String	id;

	ContainerIterator	&i = group.initIterator();
	part		&p = (part &)i.current();

	id = p.id_v;

	const char	*idp = (const char *)id;
	const char	*fnp = short_name;

	while( isspace( *idp ) )
		idp++;

	while( isspace( *fnp ) )
		fnp++;

	if( strstr( idp, fnp ) == idp )
		idp += strlen( fnp );

	while( isspace( *idp ) || (strchr( ",:-.", *idp ) != NULL) )
		idp++;

	descr = (signed char *)idp;
	}


static bool
save_tic( tic_file &tic )
	{
	if( tic.invalid() )
		{
		error( EI_None, "Internal problem - invalid contents of TIC object" );
		return Err;
		}

	char	tfn[100];

	for( int cnt = 10; cnt; cnt-- )
		{
		sprintf( tfn, "%s\\uu%.6d.tic",
			(const char *)conf.netfile_dir, time(NULL) );

		if( tic.save_to( tfn ) != Ok )
			continue;

		return Ok;
		}

	error( EI_None, "Can't save TIC: %s", tfn );
	return Err;
	}

bool
extract( Bag &group )
	{
	uu_decoder = Off;

	if( (d_fp = mk_temp_file( d_fn )) == NULL )
		return Err;

	if( ( o_fp = mk_temp_file( o_fn )) == NULL )
		{
		fclose( d_fp );
		unlink( d_fn );
		return Err;
		}

	bool	done;
	String	out_name;		// Name to give to output file

	done = go_extract( group, out_name );
	fclose( o_fp );

	if( done == Ok )
		{
		char	o_name[20], o_ext[5];
		char	o_drv[5], o_dir[80];
		char	o_file[100], short_name[30];

		_splitpath( (const char *)out_name, NULL, NULL, o_name, o_ext );
		_splitpath( o_fn, o_drv, o_dir, NULL, NULL );
		_makepath( o_file, o_drv, o_dir, o_name, o_ext );
		_makepath( short_name, NULL, NULL, o_name, o_ext );

		String		descr;

		get_descr( descr, group, short_name );

		// Rename o_fn to real file name
		chmod( o_file, 0666 );		// Make sure rename will not
		unlink( o_file );			// fail if output file exist
		debug( "Renaming %s to %s", o_fn, o_file );
		if( rename( o_fn, o_file ) != 0 )
			{
			error( EI_Full, "Can't rename %s to %s", o_fn, o_file );
			chmod( o_fn, 0666 );
			unlink( o_fn );
			done = Err;
			}
		else
			{
/*
			tic_file	tic;

			tic.from( conf.our_addr );
			tic.name( short_name );
tic.area( "UU2.FUF" );
			tic.desc( descr );

			if( save_tic( tic ) == Err )
				done = Err;	// Must unlink file here
			else
				{
				// Post d_fd to corresponding newsgroup
				}
*/
			}

		}
	else
		unlink( o_fn );

	fclose( d_fp );	unlink( d_fn );

	return done;
	}




















