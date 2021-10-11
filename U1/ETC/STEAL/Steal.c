

#include	<stdio.h>
#include	<process.h>
#include	<time.h>


int
main( int ac, char **av ) {
	int		nn = 0;
	FILE	*fp;
	char	buf[ 300 ];

	while( ac-- )
		printf("Arg %d: '%s'\n", nn++, *av++ );


	sprintf( buf, "stolen.%3.3d", time(NULL) % 999 );
	fp = fopen(buf, "a" );
	if( fp == NULL ) {
		printf("Can't create out file\n");
		exit(1);
		}

	while( fgets( buf, 300, stdin ) != NULL )
		fputs( buf, fp );

	if( fclose( fp ) == EOF ) {
		printf("Can't close out file\n");
		exit(1);
		}

	return 0;
	}

