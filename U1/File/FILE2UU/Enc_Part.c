/************************ UU2 FILE2UU ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Convert one part
 *
 *      $Log:   Q:/file/file2uu/vcs/enc_part.c_v  $
 *      
 *         Rev 1.0   29 Dec 1992 01:30:52   dz
 *      Initial revision.
 *
 *
\*/

#include    "file2uu.h"
#include	<datec.h>
#include	"version.h"
#include	<fidoaddr.h>
#include	<uucpaddr.h>


#define	KLUDGES		1


bool
encode_part
		(
		int 					part_no,		// 1-nnn
		int						n_parts,		// nnn
		long					p_bytes,		// bytes per part
		echo_def 			&	def,			// Echo definition
		tic_file			&	tic,			// Original file info
		FILE				*	fo,				// Put here
		FILE				*	fi				// From here
		)
	{

	time_t	tt = tic.date();
	if( tt <= 0 )
		{
		error( EI_None, "No date field in TIC file - not fatal :)");
		tt = time( NULL );
		}

	fido_user	fi_from;
	uucp_addr	uu_from;

    ((fido_addr&)fi_from) = tic.from();
	fi_from.name("Sysop");

    ftn_def   fdef;

    if( Ok != ftn::match( fdef, fi_from ) )
        {
        error( EI_None, "Can't find corresponding FTN for '%s'", (const char*)(String)fi_from );
        return Err;
        }

	if( fi_from.convert_to_uucp( uu_from, fdef.uucp_d ) == Err )
		{
		error( EI_None, "Can't convert FIDO addr (%s) to uucp form",
			(const char *)(String)fi_from );
		return Err;
		}

	fprintf( fo, "Newsgroups: %s\n", (const char *)def.group() );
	fprintf( fo, "Sender: postmaster@%s\n", (const char *)conf.def_domain );
	fprintf( fo, "Date: %s\n", timeline( tt, (const char *)conf.tz ) );
	fprintf( fo, "From: %s (%s)\n", (const char *)uu_from, fi_from.name() );

	fprintf( fo, "X-Gate: UU2 " UU2_VER_STR "\n" );
	if( tic.crc() )
		fprintf( fo, "X-TIC-CRC: %lu\n", tic.crc() );

	if( tic.source().valid() )
		{
		fido_user	fi_src;
		uucp_addr	uu_src;

        ((fido_addr&)fi_src) = tic.source();
		fi_src.name("Sysop");

        ftn_def   fdef;

        if( Ok != ftn::match( fdef, fi_from ) )
            {
            error( EI_None, "Can't find corresponding FTN for '%s'", (const char*)(String)fi_from );
            return Err;
            }

		if( fi_src.convert_to_uucp( uu_src, fdef.uucp_d ) == Err )
			error( EI_None, "Can't convert FIDO addr (%s) to uucp form",
				(const char *)(String)fi_src );
		else
			fprintf( fo, "X-TIC-Source: %s (%s)\n",
				(const char *)uu_src, fi_src.name() );
		}

#if KLUDGES
	fprintf( fo, "Path: %s!filegate\n", (const char *)conf.news_path );
#else
	make_path()...
#endif


	ContainerIterator &i = tic.desc().initIterator();
	bool				   first = Yes;

	for(; (int)i; i++ )
		{
		String &s = (String &)(i.current());

		if( first )
			{
            fprintf( fo, "Subject: %s (part %d/%d)\n",
                (const char *) s, part_no, n_parts );
			first = No;
			}

		fprintf( fo, "Summary: %s\n", (const char *) s );
		}

	// No description found, provide fake subject
	if( first )
		{
		error( EI_None, "!! No desc. found, no meaningfull subject made" );
		fprintf( fo, "Subject: <none> (part %d/%d)\n" );
		}


	if( strlen( def.approved() ) )
		fprintf( fo, "Approved: %s\n", (const char *)def.approved() );
	if( strlen( def.distrib() ) )
		fprintf( fo, "Distribution: %s\n", (const char *)def.distrib() );


	// Close header with empty line
	fprintf( fo, "\n" );

	/*************************************************************
						   End of RFC header
	*************************************************************/


	fprintf( fo, "%s part%02d/%02d\n", tic.name(), part_no, n_parts );
	fprintf( fo, "BEGIN--cut here--cut here\n" );

	if( part_no == 1 )
		fprintf( fo, "begin 644 %s\n",  tic.name() );

	if( uuencode( fo, fi, p_bytes ) == Err )
		return Err;

	if( part_no == n_parts )
		fprintf( fo, "end\n",  tic.name() );

	fprintf( fo, "END--cut here--cut here\n" );

	if( ferror( fo ) )
		{
		error( EI_Full, "Error writing output file" );
		return Err;
		}

	if( ferror( fi ) )
		{
		error( EI_Full, "Error reading input file" );
		return Err;
		}

	return Ok;
	}




