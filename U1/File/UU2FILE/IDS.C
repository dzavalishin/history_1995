/************************ UU2 UU2FILE ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Read identifiers from file
 *
 *      $Log:   Q:/news/snews/vcs/main.c_v  $
 *
 *
\*/

#include    "uu2file.h"

#define	BUFS	400


static bool		do_ids			( FILE *in, uchar *subj );
static bool     is_first_from	( uchar *s );
static bool     is_new_hl		( uchar *s );
static bool     is_cont_hl		( uchar *s );




bool
get_ids( const uchar *name, String &ident, int &part_no, int &parts )
	{
	FILE	*in;
	bool	err = Ok;
	char	subj[ BUFS * 2 ];


	in = fopen( name, "rt" );
	if( in == NULL )
		{
		error( EI_Full, "Can't open %s", name );
		return Err;
		}

	if( do_ids( in, subj ) == Err )
		err = Err;

	if( ferror( in ) )
		{
		error( EI_Full, "Problems reading %s", name );
		err = Err;
		}

	fclose( in );

	if( err != Err )
		{
		char	s_id[101];
		if( sscanf( subj, "%*[^:]: %*[^:]:%100[^(](%*s%d/%d)", s_id, &part_no, &parts ) != 3 )
			err = Err;
		else
			{
			uchar *cp = s_id+strlen(s_id);

			while( cp > s_id && isspace( *(cp-1) ) )
				cp--;
			*cp = '\0';

			cp = s_id;
			while( isspace( *cp ) )
				cp++;

			if( *cp == '\0' )
				err = Err;

			ident = (signed char *)cp;
			}
		}

	return err;
	}


static bool
do_ids( FILE *in, uchar *subj )
	{
	char			buf[ BUFS ];
	char			hl[ BUFS * 2 ];

	bool			header	= Yes;
	bool			have_hl	= No;

	rewind( in );
	while( 1 )
		{
		char	*ee;

		if( fgets( buf, BUFS, in  ) == NULL )		// Next line
			{

			if( ferror( in ) )
				error( EI_Full, "Problems reading file" );

			if( have_hl )
				{
				if( strncmpi( hl, "subject:", 8 ) == 0 )
					{
					strcpy( subj, hl );
					return Ok;
					}

				hl[0] = '\0';
				have_hl = No;							// Forget it
				}

			break;
			}

		if( (ee = strpbrk( buf, "\r\n" )) != NULL )	// Kill any CRLFs
			*ee = '\0';

		if( is_first_from( buf ) )					// From[^:] ??
			{
			strcpy( hl, buf );						// Get beg. of headline
			have_hl = Yes;							// We have one
			continue;
			}

		if( header && is_cont_hl( buf ) )			// headline continuation?
			{
			char	*pp = buf;

			if( !have_hl )
				fatal( EC_Got_Insane, EI_None, "Continuation withno headline!!");

			while( *pp == ' ' || *pp == ' ' )		// Skip ws
				pp++;

			strcat( hl, " " );						// Delimiter
			strcat( hl, pp );						// Add continuation
			continue;
			}

		if( have_hl )								// Not a continuation
			{
			if( strncmpi( hl, "subject:", 8 ) == 0 )
				{
				strcpy( subj, hl );
				return Ok;
				}
			hl[0] = '\0';
			have_hl = No;							// Forget it
			}

		if( header && is_new_hl( buf ) )			// Oh, new headline
			{
			strcpy( hl, buf );						// Save it
			have_hl = Yes;							// Mark - we have one
			continue;
			}

		header = No;								// End of hdr reached

		break;
		}

	return Err;
	}


/****************************************************************************
						Headline detectors
****************************************************************************/

bool
is_first_from( uchar *s )					// Message leader?
	{
	if( strncmp( s, "From", 4 ) )			// `From' withno `:'
		return No;

	if( s[4] == ':' )
		return No;

	return Yes;
	}

bool
is_new_hl( uchar *s )						// Headline?
	{
	while( *s )
		{
		if( isalpha( *s ) || *s == '-' )
			{
			s++;
			continue;
			}

		if( *s == ':' )
			return Yes;

		return No;
		}

	return No;
	}


bool
is_cont_hl( uchar *s )						// Headline continuation ?
	{
	if( *s == ' ' || *s == '\t' )
			return Yes;

	return No;
	}








