#include "version.h"
#include "stdio.h"
#include "string.h"

int
main( int ac, char **av )
    {
    printf("VER=%s\n", U1_VER_STR_B );

#if 0
    if( ac > 1 /* && av[1][1] == 'd' */ )
#endif
	{
	char s[20] = U1_VER_STR_B;
	char *p;

	while( (p = strchr( s, '.' )) != NULL )
		strcpy( p, p+1 );

        printf("DLLVER=%s\n", s );
        printf("VER_=%s\n", s );
	}

    return 0;
    }

