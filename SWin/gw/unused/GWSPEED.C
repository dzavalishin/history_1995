#include	<time.h>
#include	"gw.h"
#include	<stdio.h>
#include	<stdlib.h>

void
main( void ) {
	long	starttime, endtime;
	int	i = 0;
	gwin	*win;

	if( gwinit( ) == Err ) {
		printf( "Error initializing graphics windows\n" );
		exit( 1 );
		}

	if ( (win = gwcreate( 0,0, 639, 349 ) ) == NULL ) {
		printf( "Error creating window\n" );
		exit ( 1 );
		}

	time( &starttime );

	for( i = 0; i < 20; i++ ) {
		gwattach( win );
		gwclear( BLUE );
		gwdetach( );
		}

	time( &endtime );

	gwdestroy( win );
	gwreset( );
	printf( "Elapsed time for 20 steps = %ld seconds.\n", endtime - starttime );
	printf( "Elapsed time for 1  step  = %ld seconds.\n", ( long ) ( endtime - starttime ) / 20 );
	exit( 0 );
	}
