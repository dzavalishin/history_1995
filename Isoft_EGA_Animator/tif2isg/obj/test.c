#include	<stdio.h>

void	print_percents( int i, int all );

void
main( void ) {
	int	i;

	for( i = 0; i < 10656; i++ )
		print_percents(i, 10656);

	}

void
print_percents( int i, int all ) {
	static	int	old = -1;
	int	new;

	new = (long)i*100L/(long)all;
	if( old != new ) {
		old = new;
		printf("%d\n", new);
		}
	}
	