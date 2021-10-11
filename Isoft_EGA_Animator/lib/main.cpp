/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module 	:
 *
 *      $Log:   D:/animate/vcs/main.cpv  $
 *      
 *         Rev 1.1   09 Jan 1991 06:40:14   gn
 *      Using `hero'
 *      
 *         Rev 1.0   09 Jan 1991 00:32:02   dz
 *      Initial revision.
 *
 *
\*/

#include	<style.h>
#include	<alloc.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<bios.h>

#include	"animate.h"
#include	"ega.h"
#include	"scene.h"


#define	BACK_FILE	"backing.isg"


void gr_on( void ) {	ega::start_graphics();	}
void gr_off( void ) {	ega::stop_graphics();	}


#pragma startup gr_on	100
#pragma exit	gr_off	100


inline void
check_key( void ) {
	if( bioskey(1) && bioskey(0) ) {
		exit(1);
		}
	}


#define		FX	96
#define		FY	20

uint	_stklen		= 32*1024;


int
main( void ) {
	image	bg, b[4], flag[2];
	int	nb, dir, flagstate;
//	long	allcore = farcoreleft();
	scene	s;

	hero	bird1( 1 ), bird2( 3 ), back( 0 ), flagh( 2 );


	for( int i = 0; i < 4; i++ ) {
		char	name[20];

		sprintf( name, "b%d.isg", i );
		if( b[i].load( name ) == Err )
			return 22;
		}

	if( 
		flag[0].load( "flag0.isg" ) ||
		flag[1].load( "flag1.isg" ) 
		) return 22;


#define FULL 1

#if FULL
	if( bg.load( BACK_FILE ) == Err )
		return 22;
#endif


	
	back.set_image( &bg );
	back.set_visibility_status( Yes );

	flagh.set_image( &flag[0] );
	flagh.set_position( FX, FY );
	flagh.set_visibility_status( Yes );

	bird1.set_image( &b[0] );
	bird1.set_visibility_status( Yes );
	bird1.set_position( 0, -10 );

	bird2.set_image( &b[0] );
	bird2.set_visibility_status( Yes );
	bird2.set_position( 0, -10 );

	//
	//	Create scene
	//

	s.add( &back );
	s.add( &flagh );
	s.add( &bird1 );
	s.add( &bird2 );

	s.draw();			// Draw scene

	nb 		= 0;
	flagstate 	= 0;

	for( i = 0; i < 100; i += 2 ) {
		int	yy = i/2 - 10;

		check_key();

		flagh.set_image( &flag[flagstate] );
		flagstate = !flagstate;

		bird1.set_image( &b[nb] );
		bird1.set_position( i, yy );

		bird2.set_image( &b[nb] );
		bird2.set_position( 220-i, yy );

		if( nb > 2 )
			dir = -1;

		if( nb < 1 )
			dir = 1;

		nb += dir;

		s.draw();
		}
/*
	bird1.set_priority( 1 );
	bird2.set_priority( 1 );
	flagh.set_priority( 2 );
*/
	nb = 0;
	dir = 1;

	for( i = 100; i < 200; i += 2 ) {
		int	yy = 50 - (i-100)/2 - 10;

		check_key();

		flagh.set_image( &flag[flagstate] );
		flagstate = !flagstate;

		bird1.set_image( &b[nb] );
		bird1.set_position( i, yy );

		bird2.set_image( &b[nb] );
		bird2.set_position( 220-i, yy );

		if( nb > 2 )
			dir = -1;

		if( nb < 1 )
			dir = 1;

		nb += dir;

		s.draw();
		}

	}


