/************************ UU2 FILE2UU ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Scan given spool for complete packages
 *
 *      $Log:   Q:/file/uu2file/vcs/scan.c_v  $
 *
 *         Rev 1.1   25 Jan 1993 23:58:38   dz
 *      Seems to be working
 *      
 *         Rev 1.0   19 Jan 1993 23:14:06   dz
 *      Initial revision.
 *
 *
\*/

#include    "uu2file.h"
#include	"part.h"

#include	<dirent.h>
#include	<bag.h>
#include	<alloc.h>



static void		check_parts( Bag &parts );



bool
scan_spool( const uchar *dir )
	{
	char	dname[200];

	strncpy( dname, dir, 200 );

	DIR		*d =	opendir( dname );

	if( d == NULL )
		{
		error(EI_Full, "Can't open directory %s for scanning", dir );
		return Err;
		}

	dirent 		*ent;

	Bag     all_parts;
	all_parts.ownsElements( 1 );

	while( (ent = readdir( d )) != NULL )
		{
		log( "#", "Dirent: %s", ent->d_name );

		char	name[105];
		sprintf( name, "%.80s\\%.20s", dir, ent->d_name );

		struct stat		statbuf;

		if( stat( name, &statbuf ) != 0 )
			{
			error( EI_Full, "Can't stat(\"%s\")", name );
			continue;
			}

		if( statbuf.st_mode & S_IFDIR )
			continue;

		log( "f", "Parsing %s", name );
//		fprintf( stderr, "Parsing %s\n", name );

		String	ident;
		int		part_no, parts;

		if( get_ids( name, ident, part_no, parts ) != Err )
			{
			printf( "Got '%s', %d of %d\n",
				(const char *)ident, part_no, parts );

			part	*p = new part( (signed char *)name, ident, part_no, parts, statbuf );
			if( p == Object::ZERO )
				error( EI_None, "Out of memory" );
			else
				all_parts.add( *(Object *)p );
			}
		else
			error( EI_None, "Can't parse %s", name );
		}

	closedir( d );

	check_parts( all_parts );

	return Ok;
	}





static bool
kill_them( Bag &parts )
	{
	ContainerIterator	&i = parts.initIterator();
	bool done = Ok;

	for( ; i; i++ )
		{
		const char *fn = (const char *)(((part &)i.current()).fn_v);
		chmod( fn, 0666 );
		if( unlink( fn ) != 0 )
			{
			error( EI_Full, "\7\7\7CAN'T DELETE FILES - POSIBLE DUPES IN FILEECHO!\7\7\7" );
			done = Err;
			}
		}

	return done;
	}


static void
check_parts( Bag &parts )
	{
	ContainerIterator	&i = parts.initIterator();

	for( ; i; i++ )
		{
		part &p = (part &)i.current();
		Bag		group;
		bool	all = Yes;

		time_t	last_mtime = 0L;

		// Make sure we have all the parts

		for( int j = 1; j <= p.pts; j++ )
			{
			part *ord;

			if( (ord = get_part( parts, p.id_v, j )) != NULL )
				{
				group.add( *(Object *)ord );
				parts.detach( *(Object *)ord );
				i.restart();				// We may pull off the element iterator stands at
				if( last_mtime < ord->st_v.st_mtime ) last_mtime = ord->st_v.st_mtime;
				}
			else
				all = No;

			}

		if( all )
			{
			printf("All the parts (%d total) arrived, processing\n\t'%s'...\n",
			p.pts, (const char *)p.id_v );
			if( extract( group ) == Ok )
				kill_them( group );
			}
		else
			{
			error( EI_None, "Incomplete package: %s", (const char *)p.id_v );
			fprintf( stderr, "Last part arrived %s", ctime( &last_mtime ) );
			}

		}

	}



