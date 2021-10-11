#include	".\font.h"
#include	<stdio.h>
#include	".\gw.h"
#include	<style.h>
#include	<string.h>
#include	<stdlib.h>


void
main( void ) {

	if( gwinit( ) == Err )
		exit( 1 );

	push_bar("                 Bar number one");

	press_any_key();

	push_bar("Bar number two");

	press_any_key();

	pop_bar();

	press_any_key();

	pop_bar();

	press_any_key();



	gwreset( );
	}
