#include	<stdio.h>
#include	"uucpaddr.h"
#include	<userlist.h>
#include	<setup.h>

char	program_name[] = "UUCP library check module " __DATE__ " " __TIME__;


bool
dump( uucp_addr::el_type t, const char *el, int len, void * )
	{
	char *n;
	switch( t )
		{
	case uucp_addr::UU_name:	n = "name";		break;
	case uucp_addr::UU_via:		n = " via";		break;
	case uucp_addr::UU_domain:	n = " dom";		break;
	case uucp_addr::UU_bang:	n = "bang";		break;
		}
	printf("%s: %.*s\n", n, len, el );
	return Yes;
	}

int
main( void )
	{

	char		s[200];

	get_setup();

	printf("\n\n\nGo: ");
	while( gets( s ) != NULL )
		{
		uucp_addr	a( s );
		fido_addr	f;

//		a.traverse( dump, NULL );

		if( a.convert_to_fido( f, fuser ) == Err )
			printf("Can't convert\n");
		else
			{
			printf( "Ok: %s, %d:%d/%d.%d@%s\n",
				f.name(),
				f.zone,	f.net,	f.node,	f.point,
				f.domain()
				);
			}

        printf("\nGo: ");
		}

	return 0;
	}