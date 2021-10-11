head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	96.05.21.00.21.04;	author dz;	state Exp;
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
date	95.03.06.22.36.33;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
log
@cosmetical changes
@
text
@/************************ UU2 News Batcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Feeds file parser
 *
 *      $Log: feeds.c $
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








@


1.3
log
@IBM C Set version
@
text
@d8 2
a9 2
 * Revision 1.2  1995/03/11  18:26:07  dz
 * snews=host mode
d11 2
a12 2
 * Revision 1.1  1995/03/06  22:36:33  dz
 * Initial revision
d14 4
a17 2
 *      
 *         Rev 1.5   07 Jun 1993 17:07:20   dz
d19 2
a20 2
 *      
 *         Rev 1.4   28 Nov 1992 22:08:04   dz
d22 2
a23 2
 *      
 *         Rev 1.3   23 Oct 1992 14:53:42   dz
d25 2
a26 2
 *      
 *         Rev 1.2   18 Jun 1992 11:04:04   dz
d28 2
a29 2
 *      
 *         Rev 1.1   25 Apr 1992 16:41:24   dz
d31 1
a31 1
 *      
d50 1
a50 1
	{
d55 1
a55 1
	{
d62 25
a86 27
bool feeds::
open( const char *fn )
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

void feeds::
close()
	{
	if( ff )
    	fclose( ff );
d93 2
a94 3
void feeds::
rewind()
	{
d102 2
a103 3
bool feeds::
next( feed & f )
	{
d108 3
a110 3
	f.codetab_name[0] = '\0';
	f.add_lines = No;
	f.snews[0] = 0;
d113 1
a113 1
    	{
d115 5
a119 5
	    if( fgets( buf, FEED_REC_LEN, ff ) == NULL )	// Read next line
	    	{
    	    if( ferror( ff ) )							// Error?
				error( EI_Full, "Can't read feeds file" );
	    	return Err;
d122 2
a123 2
		p = strpbrk( buf, "\r\n;" );					// Cut newlines
    	if( p )	*p = '\0';								// and comment part
d125 3
a127 3
	    bb = buf;										// Buffer pointer
    	while( isspace( *bb ) )							// Skip whitespaces
    		bb++;
d130 1
a130 1
        	continue;									// Go get next
d132 3
a134 3
		p = bb;											// Start of token
		while( *bb && !isspace( *bb ) )					// Go to next space
        	bb++;
d137 2
a138 2
        	{
			error( EI_None, "No host name found: '%s'", buf );
d142 3
a144 3
		if( (l = (int)(bb - p)) > MAXHOST )					// Long host name?
        	{
			error( EI_None, "Host name too long: %.*s", p, l );
d148 1
a148 1
		strncpy( f.dest, p, l );						// Get it
d151 7
a157 7
	if( NULL != (p = strchr(f.dest, '/')) )
		{
		*p = '\0';
		strncpy( f.aliases, p+1, MAXALIAS );
		}
	else
		strncpy( f.aliases, f.dest, MAXALIAS );
d159 2
a160 2
    	while( isspace( *bb ) )							// Skip whitespaces
    		bb++;
d164 1
a164 1
        	bb++;
d167 2
a168 2
        	{
			error( EI_None, "No feed mode found: '%s'", buf );
d178 1
a178 1
    		bb++;
d180 2
a181 2
		while( *bb == '-' || *bb == '/' )				// Comp type flag
        	{
d183 2
a184 2
			while( *bb && !isspace( *bb ) )				// Go to next space
    	    	bb++;
d186 2
a187 2
			if( bb-p > 1 )
				f.parse_comp( p+1, (int)(bb-p-1) );
d189 1
a189 1
				error( EI_None, "Empty flag: %s", buf );
d191 7
a197 7
	    	while( isspace( *bb ) )						// Skip whitespaces
    			bb++;
			}

		if( strlen( bb ) > MAXGRLIST )
        	{
			error( EI_None, "Feed list too long: %s", buf );
d210 1
a210 2
bool feed::
parse_mode( char *kw, int len )
d214 1
a214 1
    	{
d218 1
a218 1
        	{
d224 1
a224 1
	error( EI_None, "Unknown feed mode: %.*s", kw, len );
d228 28
a255 46
bool feed::
parse_comp( char *kw, int len )
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
d257 19
a275 2
		if( len == strlen( p ) && 0 == strnicmp( p, kw, len ) )
			{
@


1.2
log
@snews=host mode
@
text
@d8 3
d39 2
d63 10
a72 1
	ff = _fsopen( fn, "rt", SH_DENYWR );
d74 2
d77 2
d220 1
a220 1
        if( len == strlen( p ) && 0 == strncmpi( p, kw, len ) )
d278 1
a278 1
		if( len == strlen( p ) && 0 == strncmpi( p, kw, len ) )
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/batch/vcs/feeds.c_v  $
d94 1
d231 17
@
