/************************ UU2 News Batcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Feeds file parser
 *
 *      $Log: feeds.c $
 *      Revision 1.4  1996/05/21 00:21:04  dz
 *      cosmetical changes
 *
 *      Revision 1.3  1995/04/10  02:08:14  dz
 *      IBM C Set version
 *
 *      Revision 1.2  1995/03/11  18:26:07  dz
 *      snews=host mode
 *
 *      Revision 1.1  1995/03/06  22:36:33  dz
 *      Initial revision
 *
 *      Rev 1.5   07 Jun 1993 17:07:20   dz
 *      Minor bug fix
 *
 *      Rev 1.4   28 Nov 1992 22:08:04   dz
 *      -lines option added
 *
 *      Rev 1.3   23 Oct 1992 14:53:42   dz
 *      cosmetic...
 *
 *      Rev 1.2   18 Jun 1992 11:04:04   dz
 *      codetab flag parsing written
 *
 *      Rev 1.1   25 Apr 1992 16:41:24   dz
 *      Initial revision
 *
 *
 *
\*/

#include	"feeds.h"
#include	"feeds_kw.h"
#include	<log.h>

#include	<share.h>
#include	<io.h>
#include	<fcntl.h>
#include	<string.h>
#include	<ctype.h>


/**********************************************************************/

feeds::feeds()
    {
    ff = NULL;
    }

feeds::~feeds()
    {
    if( ff )	close();
    }

/**********************************************************************/


bool feeds::open( const char *fn )
    {
    int fd = _sopen( fn, O_RDONLY|O_TEXT, SH_DENYWR );

    if( fd < 0 )
        {
        ff = NULL;
        return Err;
        }

    ff = fdopen( fd, "r" );

    if( ff == NULL )
        {
        _close( fd );
        return Err;
        }

    return Ok;
    }

void feeds::close()
    {
    if( ff )
        fclose( ff );
    ff = NULL;
    };


/**********************************************************************/

void feeds::rewind()
    {
    ::rewind( ff );
    };

/**********************************************************************/



bool feeds::next( feed & f )
    {
    char	buf[FEED_REC_LEN];
    char	*p, *bb;
    int		l;

    f.codetab_name[0] = '\0';
    f.add_lines = No;
    f.snews[0] = 0;

    while( 1 )											// Find valid line
        {

        if( fgets( buf, FEED_REC_LEN, ff ) == NULL )	// Read next line
            {
            if( ferror( ff ) )							// Error?
                error( EI_Full, "Can't read feeds file" );
            return Err;
            }

        p = strpbrk( buf, "\r\n;" );					// Cut newlines
        if( p )	*p = '\0';								// and comment part

        bb = buf;										// Buffer pointer
        while( isspace( *bb ) )							// Skip whitespaces
            bb++;

        if( *bb == '\0' )								// Empty line?
            continue;									// Go get next

        p = bb;											// Start of token
        while( *bb && !isspace( *bb ) )					// Go to next space
            bb++;

        if( bb == p )
            {
            error( EI_None, "No host name found: '%s'", buf );
            continue;
            }

        if( (l = (int)(bb - p)) > MAXHOST )					// Long host name?
            {
            error( EI_None, "Host name too long: %.*s", p, l );
            continue;                                   // Skip it
            }

        strncpy( f.dest, p, l );						// Get it
        f.dest[l] = '\0';

        if( NULL != (p = strchr(f.dest, '/')) )
            {
            *p = '\0';
            strncpy( f.aliases, p+1, MAXALIAS );
            }
        else
            strncpy( f.aliases, f.dest, MAXALIAS );

        while( isspace( *bb ) )							// Skip whitespaces
            bb++;

        p = bb;                                         // Start of token
        while( *bb && !isspace( *bb ) )                 // Go to next space
            bb++;

        if( bb == p )
            {
            error( EI_None, "No feed mode found: '%s'", buf );
            continue;
            }

        if( f.parse_mode( p, (int)(bb-p) ) == Err )
            continue;

        f.comp = feed::NoComp;                          // No compression yet

        while( isspace( *bb ) )                         // Skip whitespaces
            bb++;

        while( *bb == '-' || *bb == '/' )				// Comp type flag
            {
            p = bb;
            while( *bb && !isspace( *bb ) )				// Go to next space
                bb++;

            if( bb-p > 1 )
                f.parse_comp( p+1, (int)(bb-p-1) );
            else
                error( EI_None, "Empty flag: %s", buf );

            while( isspace( *bb ) )						// Skip whitespaces
                bb++;
            }

        if( strlen( bb ) > MAXGRLIST )
            {
            error( EI_None, "Feed list too long: %s", buf );
            continue;
            }

        strcpy( f.groups, bb );

        return Ok;
        }
    }


/**********************************************************************/

bool feed::parse_mode( char *kw, int len )
    {

    for( int i = 0; i < modetab_s; i++ )
        {
        char	*p = modetab[i].fkeyw;

        if( len == strlen( p ) && 0 == strnicmp( p, kw, len ) )
            {
            mode = modetab[i].fmode;
            return Ok;
            }
        }

    error( EI_None, "Unknown feed mode: %.*s", kw, len );
    return Err;
    }

bool feed::parse_comp( char *kw, int len )
    {

    if( strncmp( kw, "codetab=", 8 ) == 0 && len > 8 )
        {
        int l = len - 8;

        if( l > MAXCODENAME )
            {
            error( EI_None, "Code table name too long");
            return Err;
            }

        strncpy( codetab_name, kw+8, l );
        codetab_name[l] = '\0';
        return Ok;
        }


    if( strncmp( kw, "snews=", 6 ) == 0 && len > 8 )
        {
        int l = len - 6;

        if( l > MAXHOST )
            {
            error( EI_None, "Snews host name too long");
            return Err;
            }

        strncpy( snews, kw+6, l );
        snews[l] = '\0';
        return Ok;
        }


    if( strncmp( kw, "lines", 5 ) == 0 && len == 5 )
        {
        add_lines = Yes;
        return Ok;
        }


    for( int i = 0; i < comptab_s; i++ )
        {
        char    *p = comptab[i].fkeyw;

        if( len == strlen( p ) && 0 == strnicmp( p, kw, len ) )
            {
            comp = comptab[i].fcomp;
            return Ok;
            }
        }

    error( EI_None, "Unknown feed mode: %.*s", len, kw );
    return Err;
    }








