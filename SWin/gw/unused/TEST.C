#include	".\font.h"
#include	<stdio.h>
#include	".\gw.h"
#include	<style.h>
#include	<string.h>
#include	<stdlib.h>

	int	sfd;
	char	fname[31]="8x8";

	cstyle	*mystyle;
	font	*myfont = &_8x8_font;


void
main( void ) {

	char	name[41] = "test.txt", string[81];
	FILE	*fp;


	if( gwinit( ) == Err )
		exit( 1 );

	sfd = sfopen( "jfonts.sfs" );

	if ( !gwedits( 50, 50, 40, name, "Enter file name", "" ) )
		exit( 0 );

	if( ( fp = fopen( name, "r" ) ) == NULL ) {
		errmsg("Cant open file !");
		exit( 1 );
		}
	mystyle = &def_style;

	while( True ) {

		string[0] = '\0';
		if( fgets( string, 81, fp ) == NULL )
			break;


		if( string[0] == '.' && string[1] == 'f' &&
		    string[2] == '_' && string[3] == '_' ) {

			strncpy( fname, string+4, strlen(string+4) - 1 );

			if( sf_load_font( sfd, myfont, fname ) == Err ) {
				errmsg( "error loading font %s", fname );
				exit( 1 );
				}

			mystyle->s_font = myfont;
			continue;
			}

		if( string[0] == '.'  && string[1] == 'b' ) {
			mystyle->b_flag = mystyle->b_flag == Yes ? No : Yes;
			mystyle->b_width = mystyle->b_width != 0 ? 2 : 0;
			continue;
			}


		gp_sprintf( mystyle, "%s", string);

		}
	sfclose( sfd );
	gwreset( );
	}
