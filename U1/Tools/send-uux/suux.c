// uux preprocessor, analyzes From_, builds From_...remote from, calls uux

#include <stdio.h>
#include <string.h>
#include <process.h>
#include <time.h>
#include <dos.h>




#define ALOT  16384

char cmd[ALOT];
char from_[ALOT];
int have_from_ = 0, have_host = 0, pos;
char dummy[100], midpart[100], path[ALOT], host[100], full_path[ALOT];
int c;


void usage( void );
void make_time( char *buf );

int
main( int ac, char **av )
    {
    FILE *pipe;
    char *sys_and_cmd = av[2];
    char *thishost = av[1];

    if( ac < 4 )
        {
        usage();
        exit(1);
        }


    av += 2;
    ac -= 2;

    fgets( from_, 4096, stdin );

    if( strncmpi( from_, "From", 4) == 0 & from_[4] != ':' )
        {
        char *p, *p1;
        have_from_ = 1;

        sscanf( from_, "%s %s%n", dummy, path, &pos );

        p1 = p = from_+pos;

        p = strstr( p, "remote from");

        if( p == 0 )
            strcpy( host, "" );
        else
            {
            sscanf( p+11, "%s", host );
            have_host = 1;
            *p = '\0';
            }

	strcpy( midpart, p1 );

        }
    else
        make_time( midpart );

    {
    char *pp;
    pp = strpbrk( midpart, "\r\n" );
    if( pp ) *pp = 0;
    }

    while( *midpart == ' ' /* || *midpart == '\r' || *midpart == '\n' */ )
        strcpy( midpart, midpart+1 );

    if( strlen( midpart ) == 0 )
        make_time( midpart );

    if( have_host )
        sprintf( full_path, "%s!%s", host, path );
    else if( have_from_ )
        sprintf( full_path, "%s", path );
    else
        sprintf( full_path, "localhost!uucp" );


    sprintf( cmd, "n:\\net\\uupc\\bin\\uux.exe -n -p -r %s ", sys_and_cmd );
    while( --ac )
        {
        strcat( cmd, " " );
        strcat( cmd, *++av );
        }

    fprintf( stderr, "cmd: %s\n", cmd );

    if((pipe = _popen(cmd, "wb")) == NULL)
        { 
        fprintf( stderr, "Unable to create pipe\n" );
        return 1;
        } 

    fprintf( pipe, "From %s %s remote from %s\r\n", full_path, midpart, thishost );
 
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
 

void
usage( void ) 
    {
    printf("Usage: suux our-host-name system!command arg1.....argN\n");
    }


 
 
void
make_time( char *buf )
    { 
    struct tm *time_now;
    time_t secs_now;

    tzset();
    time(&secs_now);
    time_now = localtime(&secs_now);

    strftime( buf, 80, "%a %b %d %X %Z %Y", time_now);
 
    } 
 

