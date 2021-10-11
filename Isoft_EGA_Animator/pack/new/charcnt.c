#include	<stdio.h>
#include	<stdlib.h>
#include	<style.h>

#pragma	option	-G -O -Z -K

void
main( int ac, char **av ) {
	FILE	*fp, *out;
	int	freeq[256];
	int	c;
	long	i, all;

	if( ac < 3 ) {
		printf("Usage:\tCharcnt <input_file> <log_file>\n");
		exit(33);
		}

	fp = fopen( av[1], "rb" );
	if( fp == NULL ) {
		printf("Can't open input file %s\n", av[1]);
		exit(1);
		}

	out = fopen( av[2], "wt" );
	if( out == NULL ) {
		printf("Can't create log file %s\n", av[2]);
		exit(2);
		}

	for( i = 0; i < 256; i++ )
		freeq[i] = 0L;

	fprintf(out, "Symbols freequense in file %s\n\n", av[1]);

	all = 0L;

	while( (c = fgetc(fp)) != EOF )
		all++, freeq[c]++;


	for( i = 0; i < 64; i++ ) {
		for( c = 0; c < 4; c++ )
			fprintf(out, "%2X = %4d\t", i*4+c, freeq[(i*4)+c]);

		fprintf(out,"\n");
		}

	fprintf(out, "Symbols in file: %4ld\n", all);
	
	fclose( fp );
	fclose( out );
	exit(0);
	}
		