/************************ TiffView ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:	Main module.
 *
 *      $Log:$
 *      
 *
\*/

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<graphics.h>
#include	<dir.h>
#include	<alloc.h>

#include	"Tif2Isg.h"

void	do_view( FILE *input );


void
main( int ac, char **av ) {
	FILE	*input;
	int	gdriver = DETECT, gmode;
	byte	*Disk_buffer;

	printf("TIFFVIEW - Tiff Graphics files viewer.\n");
	printf("Copyright (C) 1991 by Infinity Soft.  Version 1.0.\n");

	if( ac < 2 ) {
		printf("\nUsage:\tTiffView <Input_file.TIF>\n");
		exit(33);
		}

	if( (input = fopen( av[1], "rb" )) == NULL ) {
		printf("Can't open input file %s\n", av[1]);
		exit(1);
		}


	initgraph( &gdriver, &gmode, "E:\\TC\\BGI" );

	Disk_buffer = new byte[10*1024];
	if( Disk_buffer != NULL )
		setvbuf( input, Disk_buffer, _IOFBF, 10*1024 );

	do_view( input );

	getchar();
	closegraph();

	fclose( input );

	if( Disk_buffer != NULL )
		delete	Disk_buffer;

	printf("All done, good bye!\n");
	exit(0);
	}


void
do_view( FILE *input ) {
	tiff_gf		t;
	PHeader		p;
	int		q, j, k;
	char		*i[4];
	byte		color;


	if( do_load_tiff_image( input, t ) == Err ) {
		printf("Can't load picture!\n");
		exit(1);
		}


	p.xsize = (uint) t.width;
	p.ysize = (uint) t.height;
	p.bpp   = 0x01;

/****************************************************************************
			Go work
****************************************************************************/

	i[0] = t.img_ptr[0];
	i[1] = t.img_ptr[1];
	i[2] = t.img_ptr[2];
	i[3] = t.img_ptr[3];

	for( q = 0; q < p.ysize; q++ ) {
		for( j = 0; j < p.xsize; j+= 8 ) {
			uint	mask = 0x80;

			for( int s = 0; s < 8; s++, mask >>= 1 ) {
				uchar	f = 0x01;

				color 	= 0;

				for( int n = 0; n < 4; n++, f <<= 1 ) {
					if( ( *(i[n]) & mask) )
						color |= f;

					}

				putpixel( j+s, q, color );
				}

			i[0]++;
			i[1]++;
			i[2]++;
			i[3]++;
			}
		}

	}
