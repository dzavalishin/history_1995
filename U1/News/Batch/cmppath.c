/************************ UU2 News Batcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Compare article path to host names list
 *
 *      $Log: cmppath.c $
 *      Revision 1.3  1996/05/21 00:21:04  dz
 *      cosmetical changes
 *
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

    a = strtok( path, "!@#$%^&*()-+/<>,|" );

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

        a = strtok( NULL, "!@#$%^&*()-+/<>,|" );
        } while( a );

    free( path );
    free( aliases );
    return No;
    }




