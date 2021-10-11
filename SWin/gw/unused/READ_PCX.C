/**-------------------   Graphics Windows   -------------------**\
 *
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728
 *
 *	Module : Paint Brush file decoder
 *
 *	$Log:   C:/c/gw/vcs/read_pcx.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:19:40   dz
 *      Initial revision.
 *
 *
 *
\******/

#include	<graphics.h>
#include	<io.h>
#include	<fcntl.h>
#include	<stdio.h>
#include	<conio.h>
#include	<stdlib.h>
#include	<process.h>
#include	<alloc.h>
#include	<style.h>



void
decode_line( char *to, char **from, int hb, int plane ) {
	int	hp;	/* hor point	*/
	int	tim;
	uchar	val;

	val = 0x01u << plane;

	for( hp = 0; hp < hb * 8; (*from)++ ) {
		if( **from < 0xC0 )
			tim = 1;
		else
			tim = ((int)*((*from)++))-0xC0;

		hp += tim * 8;
		while( tim-- ) {
			uint	mask, cnt;

			mask=0x80;
			for( cnt=0; cnt < 8; cnt++ ) {
				*to++ |= (mask & **from) ? val : 0;
				mask>>=1;
				}
			}
		}
	}


void
decode_image( uint hbsize, uint hsize, uint vsize, uchar *pp ) {
	uchar	hline[640];	/* 640 points max. horiz.	*/
	int	vp, plane, i;


	for( vp = 0; vp < vsize; vp++ ) {

		for( i = 0; i < hsize; i++ )
			hline[i] = 0;

		for( plane = 0; plane < 4; plane++ ) {
			decode_line( hline, &pp, hbsize, plane );

			}
		for( i = 0; i < hsize; i++ )
			putpixel( i, vp, hline[i] );
		}
	}








int  errorcode,graphdriver,graphmode;


void	initialization( void );

void
main( int argc, char **argv ) {
	unsigned char 	*buff, *dat_pnt;
	int		handle;
	uint		fsize;
	char		*string;

	int  		error,
			cnt,
			xwidth,
			ywidth,
			width;	/* width - bytes per line	*/

	if( argc != 2 ) {
		printf("grunpack <file.pcx>\n");
		exit(1);
		}

	string = argv[1];


	handle=open(string,O_RDONLY | O_BINARY);
	if(handle == NULL) {
		printf("\nFatal-Unable to open file:%s\n",string);
		exit(1);
		}

	if( filelength( handle ) > 0xFFFF ) {
		printf("File too big\n");
		exit(1);
		}

	fsize		= (uint) filelength( handle );
	dat_pnt = buff	= malloc( fsize );

	if( buff == NULL ) {
		printf("\nFatal-Unable to allocate file:%s in memory\n",string);
		exit(1);
		}

	if( read( handle, dat_pnt, fsize ) != fsize ) {
		printf("\nFatal-Unable to read file:%s\n",string);
		exit(1);
		}

	error=Yes;

	if((*(dat_pnt++)) != 0x0A)
		;
	else if((*dat_pnt) !=5 && (*dat_pnt) !=3)
		;
	else if((*(++dat_pnt)) != 1)
		;
	else if((*(++dat_pnt)) != 1)
		;
	else error=No;

	if(error) {
		printf("\nFatal-Illegal format\n");
		exit(1);
		}

	initialization();

	cnt=3;
	while(cnt < 8) {
		dat_pnt++;
		cnt++;
		}

	xwidth  = *(dat_pnt++);
	xwidth += (int)*(dat_pnt++)<<8;		/* +1 ?	*/
	xwidth += 1;

	ywidth  = *(dat_pnt++);
	ywidth += (int)*(dat_pnt++)<<8;		/* +1 ?	*/
	ywidth += 1;


	cnt+=4;
	while(cnt < 66) {
		dat_pnt++;
		cnt++;
		}

	width=*(dat_pnt++);
	cnt++;


	while(cnt < 0x80) {
		dat_pnt++;
		cnt++;
		}

#ifndef OLD_CODE
	decode_image( width, xwidth, ywidth, dat_pnt );
#else

	for( row = 0; row < ywidth ; row++ ) {
		for( byte=0, x=0; byte < width; dat_pnt++ ) {
			if( *dat_pnt < 0xC0 )
				tim = 1;
			else
				tim = ((int)*(dat_pnt++))-0xC0;

			byte += tim;
			while(tim > 0) {
				uint	mask, cnt;

				mask=0x80;
				for(cnt=0;cnt < 8;cnt++) {
/*				if(!(mask & *dat_pnt))	*/
					putpixel( x, row, mask & *dat_pnt ? YELLOW : BLACK );
					x++;
					mask>>=1;
					}
				tim--;
				}
			}
		}

#endif

/*	setvisualpage(1);*/

	while(!getch());

	closegraph();
	printf("xwidth=%4u ywidth=%u width=%d\n",xwidth,ywidth,width);
	printf("Used part of file: %5d bytes\n", dat_pnt - buff );
	printf("File size:         %5d bytes\n", fsize );
	exit(0);
	}

void
initialization( void ){
	extern int errorcode,graphdriver,graphmode;
	graphdriver=DETECT;
	graphmode=EGAHI;

	initgraph(&graphdriver,&graphmode,"d:\\comp\\tc\\bgi");

	errorcode=graphresult();
	if(errorcode != grOk) {
		printf("\nFatal-BGI Error:%s\n",grapherrormsg(errorcode));
		exit(1);
		}
	}

