/************************ SPACK utility ***************************
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:	SPACK main module.
 *
 *      $Log$
 *
 *
*/

#include	<stdio.h>
#include	<stdlib.h>
#include	<style.h>
#include	<pack.h>
#include	<conio.h>


#define		INPUT_BUFFER	(10*1024)	// 10 Kbytes for input buffer
#define		OUTPUT_BUFFER	(10*1024)	// 10 Kbytes for output buffer

#define		PERCENT		( ((long) curr * 100L / (long) insize) )



FILE	*input = NULL, *output = NULL;
long	insize, outsize;
byte	*inbuf = NULL, *outbuf = NULL;	


static	void	usage( void );


void		error( char *msg, int exit_code = 1 );
void		Make_symbol( byte b );


void
error( char *msg, int exit_code ) {

	cprintf("SPACK Error:\t%s\n\r", msg);

	if( input )
		fclose( input );

	if( output )
		fclose( output );

	if( inbuf )
		delete	inbuf;

	if( outbuf )
		delete	outbuf;

	exit( exit_code );
	}

void
usage( void ) {

	cprintf("Usage:  SPACK <input_file> <output_file>\r\n");
	exit(33);
	}

void
main( int ac, char **av ) {
	long	curr;
	int	i, old_percent = 100;

	cprintf("SPACK Utility 1.0      Copyright (C) 1990 by Infinity Soft\r\n");

	if( ac < 3 )
		usage();

/****************************************************************************
			Open files	
****************************************************************************/

	input = fopen( av[1], "rb" );
	if( input == NULL )
		error("Can't open input file.");

	output = fopen( av[2], "wb" );
	if( output == NULL )
		error("Can't create output file.");

/****************************************************************************
			Prepare buffers
****************************************************************************/

	inbuf = new byte[INPUT_BUFFER];
	if( inbuf != NULL )
		setvbuf( input, inbuf, _IOFBF, INPUT_BUFFER );

	outbuf = new byte[OUTPUT_BUFFER];
	if( outbuf != NULL )
		setvbuf( output, outbuf, _IOFBF, OUTPUT_BUFFER );

/****************************************************************************
			Create pack stream			
****************************************************************************/

	Pack	str( Make_symbol );

	fseek( input, 0L, SEEK_END );

	insize	= ftell( input );
	outsize = curr = 0L;

	fseek( input, 0L, SEEK_SET );

	while( (i = fgetc( input )) != EOF ) {
		str.Do( (byte) i );
		curr++;

		if( old_percent != PERCENT ) {
			cprintf("Packing %s   %3d%%\r", av[1], PERCENT );
			old_percent = PERCENT;
			}
		}


	fclose( input );
	fclose( output );

/****************************************************************************
			Delete buffers
****************************************************************************/

	if( inbuf )
		delete	inbuf;

	if( outbuf )
		delete	outbuf;

	cprintf("\r");
	clreol();
	cprintf("%s done.\r\n", av[1]);

	}

void
Make_symbol( byte b ) {

	if( fwrite( &b, 1, 1, output ) != 1 )
		error("Can't write to output file.", 99);
	}