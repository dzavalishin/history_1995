:xmp.

// Fake rmail, analyzes From_, calls sendmail

#include <stdio.h>
#include <string.h>
#include <process.h>

#define ALOT  16384

char cmd[ALOT];
char from_[ALOT];
int have_from_ = 0, have_host = 0, pos;
char dummy[100], path[ALOT], host[100], full_path[ALOT];
int c;

int
main( int ac, char **av )
    {
    FILE *pipe;

    fgets( from_, 4096, stdin );

    if( strncmpi( from_, "From", 4) == 0 &amp. from_[4] != '&colon.' )
        {
        char *p;
        have_from_ = 1;

        sscanf( from_, "%s %s%n", dummy, path, &amp.pos );

        p = from_+pos;

        p = strstr( p, "remote from");

        if( p == 0 )
            strcpy( host, "" );
        else
            {
            sscanf( p+11, "%s", host );
            have_host = 1;
            }

        }

    if( have_host )
        sprintf( full_path, "%s!%s", host, path );
    else if( have_from_ )
        sprintf( full_path, "%s", path );
    else
        sprintf( full_path, "localhost!uucp" );

    sprintf( cmd, "sendmail -f %s", full_path );

    while( --ac )
        {
        strcat( cmd, " " );
        strcat( cmd, *++av );
        }

    fprintf( stderr, "cmd&colon. %s\n", cmd );

    if((pipe = _popen(cmd, "wb")) == NULL)
        { 
        fprintf( stderr, "Unable to create pipe\n" );
        return 1;
        } 

    if( !have_from_ )
        fputs( from_, pipe );


    while( (c = getchar()) != EOF )
        putc( c, pipe );

    if( ferror( pipe ) )
        {
        _pclose( pipe );
        return 2;
        }

    return _pclose( pipe );
    }
 

:exmp.
