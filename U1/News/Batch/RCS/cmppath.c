head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	96.05.21.00.21.04;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.08.07.05.15.02;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.26.07;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@cosmetical changes
@
text
@/************************ UU2 News Batcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Compare article path to host names list
 *
 *      $Log: cmppath.c $
 *      Revision 1.2  1995/08/07 05:15:02  dz
 *      Using App class
 *
 *      Revision 1.1  1995/03/11  18:26:07  dz
 *      Initial revision
 *
 *      Revision 1.1  1995/03/11  18:26:07  dz
 *      Initial revision
 *
 *
 *
\*/

#include    "batch.h"
#include    <log.h>
#include    <ctype.h>

const maxaliases = 10;
const maxalen = 50;

bool BatchApp::dest_in_path( const char *a_path, const char *a_aliases )
    {

    if( a_path == NULL || a_aliases == NULL )
        return No;

    char	*path = strdup( a_path );
    if( path == NULL )
        fatal( EC_Out_Of_Memory, EI_None, "No memory" );

    char    *aliases = strdup( a_aliases );
    if( aliases == NULL )
        fatal( EC_Out_Of_Memory, EI_None, "No memory" );

    char alias[maxaliases][maxalen+1];
    int nalias = 0;

    char *a = strtok( aliases, "," );
    if( a == NULL )					// No host names, done
        {
        free( path );
        free( aliases );
        return No;
        }

    do 
        {
        strncpy( alias[nalias], a, maxalen );
        alias[nalias][maxalen] = '\0';
        nalias++;

        a = strtok( NULL, "," );
        } while( a );

    a = strtok( path, "!@@#$%^&*()-+/<>,|" );

    if( a == NULL )					// No path, done
        {
        free( path );
        free( aliases );
        return No;
        }

    do 
        {
        for( int i = 0; i <nalias; i++ )
            if( 0 == strcmp( alias[i], a ) )
                {
                free( path );
                free( aliases );
                return Yes;		// Got it.
                }

        a = strtok( NULL, "!@@#$%^&*()-+/<>,|" );
        } while( a );

    free( path );
    free( aliases );
    return No;
    }




@


1.2
log
@Using App class
@
text
@d8 8
a15 2
 * Revision 1.1  1995/03/11  18:26:07  dz
 * Initial revision
a16 2
 * Revision 1.1  1995/03/11  18:26:07  dz
 * Initial revision
a17 1
 *      
d28 2
a29 38
bool
BatchApp::dest_in_path( const char *a_path, const char *a_aliases )
	{

	if( a_path == NULL || a_aliases == NULL )
		return No;

	char	*path = strdup( a_path );
	if( path == NULL )
#ifdef TEST
		exit(33);
#else
		fatal( EC_Out_Of_Memory, EI_None, "No memory" );
#endif

        char    *aliases = strdup( a_aliases );
        if( aliases == NULL )
#ifdef TEST
                exit(33);
#else
                fatal( EC_Out_Of_Memory, EI_None, "No memory" );
#endif

	char alias[maxaliases][maxalen+1];
	int nalias = 0;

	char *a = strtok( aliases, "," );
	if( a == NULL )					// No host names, done
		{
		free( path );
		free( aliases );
		return No;
		}

	do {
		strncpy( alias[nalias], a, maxalen );
		alias[nalias][maxalen] = '\0';
		nalias++;
d31 2
a32 2
		a = strtok( NULL, "," );
		} while( a );
d34 18
a51 1
	a = strtok( path, "!@@#$%^&*()-+/<>,|" );
d53 5
a57 6
	if( a == NULL )					// No path, done
                {
                free( path );
                free( aliases );
		return No;
		}
d59 2
a60 8
	do {
		for( int i = 0; i <nalias; i++ )
			if( 0 == strcmp( alias[i], a ) )
		                {
		                free( path );
		                free( aliases );
				return Yes;		// Got it.
				}
d62 1
a62 2
		a = strtok( NULL, "!@@#$%^&*()-+/<>,|" );
                } while( a );
d64 2
d68 2
a69 2
	return No;
	}
d71 9
d81 2
d84 4
a88 16
#ifdef TEST

void
main( void )
	{
	char path[500];
	char aliases[200];

	printf( "Path: " );
	gets( path );

	while( 1 ) 
		{
		printf("Aliases: " );
		if( NULL == gets( aliases ) )
			return;
a89 4
		if( dest_in_path( path, aliases ) == Yes )
			printf(" --- YES\n");
		else
                        printf(" --- NO\n");
a90 2
		}
	}
a91 1
#endif
@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:    $
d26 1
a26 1
dest_in_path( const char *a_path, const char *a_aliases )
@
