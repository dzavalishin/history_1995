// Fake sendmail, used to test FIDO2UU.
// Copies stdin to sendmail.cpy

#include <stdio.h>
#include <process.h>

void
main( int ac, char **av )
{
    FILE    *f;
    int c;
    char   name[50];

    for( ; *av; av++ )
        printf("Arg: '%s'\n", *av );

    sprintf( name, "rmail%d.cpy", getpid() % 999 );

    f = fopen( name, "w" );
    if( f == NULL ) {
        printf( "Can't create file\n" );
        exit(33);
    }

    while( (c = getchar()) != EOF )
        putc( c, f );

    fclose( f );
    exit(48);		// 48 = remote delivery (UUPC)
}

