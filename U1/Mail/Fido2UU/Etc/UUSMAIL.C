/*
 * Fake sendmail, used to test FIDO2UU.
 * Copies stdin to sendmail.cpy
**/

#include <stdio.h>
#include <process.h>

void
main( int ac, char **av )
{
    FILE    *f;
    int c;

    for( ; *av; av++ )
        printf("Arg: '%s'\n", *av );


    f = fopen( "sendmail.cpy", "w" );
    if( f == NULL ) {
        printf( "Can't create file\n" );
        exit(33);
    }

    while( (c = getchar()) != EOF )
        putc( c, f );

    fclose( f );

    system("set >envir.cpy");

    exit(0);


}

