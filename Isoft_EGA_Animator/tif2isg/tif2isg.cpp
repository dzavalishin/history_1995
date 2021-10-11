/************************ Tiff2Is ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:	Main module.
 *
 *      $Log:   D:/animate/tif2isg/vcs/tif2isg.cpv  $
 *      
 *         Rev 1.1   24 Dec 1990 02:49:08   gn
 *      Squeezing added, The METHOD of Byte Repeater is 0x01
 *      
 *         Rev 1.0   18 Dec 1990 22:32:52   gn
 *      Initial revision.
 *
 *
\*/

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<graphics.h>
#include	<dir.h>
#include	<alloc.h>

#include	"tif2isg.h"
#include	"Pack.h"


#define DRAW		0			// Draw picture
#define	PERCENT_COUNT	0			// Show packing's percent

static	char	*color_names[] = {
	"BLACK",	"BLUE",
	"GREEN",	"CYAN", 
	"RED",  	"MAGENTA",
	"BROWN",	"LIGHTGRAY",

	"DARKGRAY",	"LIGHTBLUE",
	"LIGHTGREEN",	"LIGHTCYAN",
	"LIGHTRED",	"LIGHTMAGENTA",
	"YELLOW",	"WHITE",
	"NONE",		"- Use this if you haven't invisible color"
	};

#if PERCENT_COUNT
static	long	inp_size = 0L;
#endif

void	do_convert( FILE *input, FILE *output, int color );


void
main( int ac, char **av ) {
	int	inv_color;
	FILE	*input;
	FILE	*output;
	char	outp_name[81];
	char	drive[MAXDRIVE], dir[MAXDIR], name[MAXFILE], ext[MAXEXT];

	byte	*Disk_buffer;

#if DRAW
	int	gdriver = DETECT, gmode;
#endif

	printf("TIF2ISG - Graphics file converter from Tiff to Infinity Soft Graphics format.\n");
	printf("Copyright (C) 1990 by Infinity Soft.  Version 1.02.\n");

	if( ac < 3 ) {
		printf("\nUsage:\ttif2is <Invisible_color_name> <Input_file.tif> [<Output_file.isg>]\n");
		printf("Valid color names:\n");
		for( int cn = 0; cn < 18; cn += 2 ) 
			printf("\t\t%12s\t%12s\n", color_names[cn], 
						   color_names[cn+1] );
		exit(33);
		}

	if( (input = fopen( av[2], "rb" )) == NULL ) {
		printf("Can't open input file %s\n", av[1]);
		exit(1);
		}


	if( ac == 4 )
		strcpy( outp_name, av[3] );
	else {
		fnsplit( av[2], drive, dir, name, ext );
		strcpy( ext, ".isg" );
		fnmerge( outp_name, drive, dir, name, ext );
		}


	if( (output = fopen( outp_name, "wb" )) == NULL ) {
		printf("Can't create output file %s\n", av[2]);
		exit(1);
		}

	inv_color = -1;
	for( int cnt = 0; cnt < 17; cnt++ )
		if( !strcmp( color_names[cnt], strupr( av[1] ) ) ) {
			inv_color = cnt;
			break;
			}

	if( inv_color == -1 ) {
		printf("Invalid color name %s !\n", av[1]);
		fclose( input );
		fclose( output );
		exit(22);
		}

#if DRAW
	initgraph( &gdriver, &gmode, "E:\\TC\\BGI" );
#endif

	Disk_buffer = new byte[10*1024];
	if( Disk_buffer != NULL )
		setvbuf( output, Disk_buffer, _IOFBF, 10*1024 );

#if 0
#if PERCENT_COUNT
	fseek( input, 0L, SEEK_END );
	inp_size = ftell( input );
	fseek( input, 0L, SEEK_SET );
#endif
#endif

	do_convert( input, output, inv_color );

#if DRAW
	getchar();
	closegraph();
#endif

	fclose( input );
	fclose( output );

	if( Disk_buffer != NULL )
		delete	Disk_buffer;

	printf("All done, good bye!\n");
	exit(0);
	}


long	pos;

static void
write_global_header( FILE *fp ) {
	FHeader	h;

	strcpy( h.magic, "ISGF" );
	h.nblocks = 2;
	h.first   = sizeof( FHeader ) + 2;
	h.who	  = PRODUCER;
	h.method  = 0x01;			// Byte repeater

	fseek( fp, 0L, SEEK_SET );
	if( fwrite( &h, sizeof( FHeader ), 1, fp ) != 1 ) {
		printf("Can't write global header!\n");
		exit(2);
		}

	pos = h.first;

	}

static void
write_pict_header( FILE *fp, PHeader *h ) {
	_Block	b;

	fseek( fp, pos, SEEK_SET );

	b.btype = (byte) Header;
	b.bsize = sizeof( PHeader );
	b.bnext = ftell( fp ) + b.bsize + sizeof( _Block );

	if( fwrite( &b, sizeof( _Block ), 1, fp ) != 1 ) {
		printf("Can't write block header!\n");
		exit(2);
		}

	if( fwrite( h, sizeof( PHeader ), 1, fp ) != 1 ) {
		printf("Can't write picture header!\n");
		exit(2);
		}

	pos = b.bnext;

	}

	/*************************************************************
				Output packed data
	*************************************************************/

static	FILE	*fp_out;

static	void	
Make_sym( byte b ) {
#if PERCENT_COUNT
	static	int	old_percent = -1;
	static	long	current = 0L;
		int	new_percent;
#endif

	if( fwrite( &b, sizeof( byte ), 1, fp_out ) != 1 ) {
		printf("Can't write planes data!\n");
		exit(2);
		}

#if PERCENT_COUNT	
	new_percent = (int)((long)++current*100L/(long)inp_size);
	if( new_percent != old_percent ) {
		if( old_percent < 0 )
			printf("Packing, ");

		old_percent = new_percent;
		printf("%3d%% done...\b\b\b\b\b\b\b\b\b\b\b\b", old_percent);
		}
#endif
	}	


static void
//	write_all_planes( FILE *fp, long size, Summpl *data ) {
write_all_planes( FILE *fp, long size, Summpl huge *data ) {
	_Block		b;
	long		i;
	Pack		p( Make_sym );
	byte	huge	*d = (byte huge *) data;




	fseek( fp, pos, SEEK_SET );

	b.btype = (byte) All_planes;
	b.bsize = sizeof( Summpl ) * size;
	b.bnext = ftell( fp ) + b.bsize + sizeof( _Block );


	if( fwrite( &b, sizeof( _Block ), 1, fp ) != 1 ) {
		printf("Can't write block header!\n");
		exit(2);
		}

	fp_out = fp;


	/*************************************************************
				Pack data
	*************************************************************/


	for( i = 0L; i < size; i++ ) 
		p.Do( *d++ );


	}

void
do_convert( FILE *input, FILE *output, int color ) {
	tiff_gf		t;
	PHeader		p;
	int		q, j, k;
	Summpl		huge	*data;
	Summpl		huge	*curr;
	char		*i[4];
	int		old_percent = -1, new_percent;


	if( do_load_tiff_image( input, t ) == Err )
		exit(1);

	write_global_header( output );

	p.xsize = (uint) t.width;
	p.ysize = (uint) t.height;
	p.bpp   = 0x01;

	printf("Picture size %d x %d\n", p.ysize, p.xsize );

	write_pict_header( output, &p );

	data = (Summpl huge *) farmalloc( (ulong) (t.height * t.width * 1) );
	if( data == NULL ) {
		printf("Out of memory!\n");
		exit(3);
		}

/****************************************************************************
			Go work
****************************************************************************/

#if 	PERCENT_COUNT
	inp_size = (ulong) (t.height*t.width*sizeof(byte));
#endif

	printf("Converting picture, please wait...");

	curr = data;
	i[0] = t.img_ptr[0];
	i[1] = t.img_ptr[1];
	i[2] = t.img_ptr[2];
	i[3] = t.img_ptr[3];

	for( q = 0; q < p.ysize; q++ ) {
		for( j = 0; j < p.xsize; j+= 8 ) {
			uint	mask = 0x80;

			for( int s = 0; s < 8; s++, mask >>= 1 ) {
				uchar	f = 0x01;

				curr->color 	= 0;
				curr->unused    = 0;
				curr->invisible = No;

				for( int n = 0; n < 4; n++, f <<= 1 ) {
					if( ( *(i[n]) & mask) )
						curr->color |= f;

					}

				if( curr->color == color )
					curr->invisible = Yes;
#if DRAW
				else
					putpixel( j+s, q, curr->color );
#endif
				curr++;

				}

#if 0 // PERCENT_COUNT	
			new_percent = (int)((long)curr*100L/(long)inp_size);
			if( new_percent != old_percent ) {
				old_percent = new_percent;
				printf("%3d%% done...\b\b\b\b\b\b\b\b\b\b\b\b", old_percent);
				}
#endif
			

			i[0]++;
			i[1]++;
			i[2]++;
			i[3]++;
			}
		}

	printf("\n");

	write_all_planes( output, ((long)p.xsize) * ((long)p.ysize), data );
	farfree( data );
	}
