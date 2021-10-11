/**********************   Graphics Windows   ************************\
 *                                                              
 *     Copyright (C) 1989 by DZ - Moscow, 110.6728		
 *                                                              
 *     Module : Windows startup/termination.                    
 *          
 *	$Log:   C:/c/gw/vcs/init.c_v  $
 *      
 *         Rev 1.1   29 Apr 1990 16:28:20   dz
 *      
 *         Rev 1.0   29 Apr 1990 16:19:46   dz
 *      Initial revision.
 *                                                    
 *	   Rev 1.2	28 Dec 89				
 *	Added function check_EGA_mem - checks		
 *	for a 256K EGA card.				
 *								
 *	   Rev 1.1    29 Oct 89				
 *	Initial revision.				
 *								
 *								
\**********/

#pragma warn -use
static char	GW_Copyright[] = "GraphicsWindows library Copyright (C) 1989, 1990 NovInTech";


#include "local.h"
#include "ega.h"

#include <stdlib.h>
#include <stdio.h>
#include <dos.h>


bool	__GW_ext_keyboard = No;


char far	*planebuf;

int	Xbase = 0,
	Ybase = 0,
	Xend  = 639,
	Yend  = 349;


gwin full_screen_window = {
	0, 0, 640, 350,
	0, { NULL, NULL, NULL, NULL },
	0, 0, 0, 0, Yes, Yes
	};


static int	at_ctrl_break( void );	/* CTRL-Break trap function	*/


int	Gresult = grOk;
bool	gw_inited = False;

int     Gdriver;
int	Gmode;


void
gwfatal( char *msg ) {
	gwreset();
	printf(
		"Graphics windows fatal error: %s.\n"
		"Please, report this error to NewInTech,\n"
		"Electronical Journal Technical Support Group.\n",
			msg );
	exit(23);
	}


int
gwresult( void ) {
	return Gresult = graphresult();
	}

static void
at_exit_cleanup( void ) {
	gwreset();
	}


static void
check_keyb_bios( void ) {
	union REGS	r;
	uchar		status;

	r.h.ah	= 0x02;		/* Standard keyboard shift status	*/
	int86( 0x16, &r, &r );	/* GET	*/

	status = r.h.al;	/* Save	*/


	r.h.ah	= 0x12;		/* Extended keyboard shift status	*/
	int86( 0x16, &r, &r );	/* GET	*/

	if( r.h.al == status )	/* Equal ??	*/
		__GW_ext_keyboard = Yes;	/* OK, it's extended bios */
	else
		__GW_ext_keyboard = No;		/* Standard.	*/
	}



static bool
check_EGA_mem( void ) {	/* Returns Ok for EGA 256K, Err for 64-128K	*/
	union REGS	r;

	r.h.ah = 0x12;			/* BIOS10, sub 12H		*/
	r.h.bl = 0x10;			/* Return EGA info		*/

	int86( 0x10, &r, &r );		/* Get EGA info			*/

	if( r.h.bh == 0x01 ) {		/* Monochrome mode ???		*/
		printf("Error: You must use colour monitor.\n");
		return Err;
		}

	if( r.h.bl != 0x03 ) {		/* Less than 256K on board.	*/
		printf("Error: Not a 256K EGA card.\n");
		return Err;
		}
	return Ok;
	}


/**Doc************************************************************************
 *	Function: bool	gwinit()      		Header: gw.h
 *
 *	Parameters:			None.
 *
 *	Return value on error:		Err.
 *
 *	Normal return value:		Ok.
 *
 *	Description:
 *		Initializes graphics windows package, turns
 *		screen into the EGA 640 X 350 16 colour graphics
 *		mode, resets visual and active pages to zero.
 *	Note:
 *		Package automatically terminates itself at exit
 *		time.
 *
**/


bool
gwinit( void ) {
/*	uint	dummy;	*/



#ifdef FAR_BGI
	registerfarbgidriver( EGAVGA_driver_far );
#else
#ifdef NEAR_BGI
	registerbgidriver( EGAVGA_driver );
#else
#	error You must specify either NEAR_BGI or FAR_BGI
#endif
#endif

	if( check_EGA_mem() == Err )	return Err;

/*	es_attach();		/*	Activate Ega.Sys interface	*/
/*	save_palette();		/*					*/

	Gdriver = DETECT;
	detectgraph( &Gdriver, &Gmode );
	if( gwresult() ) {
		printf("Error checking the graphics card.\n");
		return Err;
		}

	switch( Gdriver ) {

	case EGA:
		Gmode = EGAHI;		/* 640 X 350, 16 color, 2 pages	*/
		break;

	case VGA:
		Gmode = VGAMED;		/* 640 X 350, 16 color, 2 pages	*/
		break;

	case EGA64:
		Gresult = grInvalidMode;
		printf("Error: EGA 64K is not supported.");
		return Err;


	default:
		Gresult = grInvalidDriver;
		printf("Error: Not an EGA card.\n");
		return Err;

		}



	initgraph( &Gdriver, &Gmode, "." );
	if( gwresult() ) {
#if 0
		printf("Error during BGI graphics initialization: ");
		switch( Gresult ) {
			case grNoScanMem:
			case grNoLoadMem:
			case grNoFloodMem:
			case grNoFontMem:
				printf("Out of memory.\n");
				break;

			case grInvalidDriver:
				printf("Invalid driver.\n");
				break;

/*			case grInvalidDeviceNum:	*/
			case grIOerror:
			default:
				printf("Invalid device.\n");
				break;
			}

#endif
		return Err;
		}


	if( (planebuf = malloc(PLANEBUF_SIZE)) == NULL ) {
		closegraph();
		Gresult = grNoScanMem;	/* Out of memory.	*/
		printf("Out of memory in GraphicsWindows.\n");
		return Err;
		}

	gw_inited = True;
	gwattach( &full_screen_window );


	gwvpage(0);			/*	Reset page numbers	*/
	gwapage(0);

	atexit( at_exit_cleanup );
	ctrlbrk( at_ctrl_break );

	/*	O'K, All done.		*/

	return Ok;
	}

/**Doc************************************************************************
 *	Function: gwreset()      		Header: gw.h
 *
 *	Parameters:			None.
 *
 *	Return value on error:		Err.
 *
 *	Normal return value:		Ok.
 *
 *	Description:
 *		Resets graphics windows package, returns scrren
 *		into the text mode.
 *
 *
**/


bool
gwreset( void ) {
	union REGS	r;

	assert_init;

	gw_inited = False;
	free( planebuf );
	closegraph();


/*	rest_palette();		/*					*/

	r.h.ah = 0x01;
	r.h.ch = 6;
	r.h.cl = 7;
	int86( 0x10, &r, &r );

	/*
		Turn cursor on - It isn't
		turned on automatically on
		some computers
	*/

	return Ok;
	}

static int
at_ctrl_break( void ) {		/* CTRL-Break trap function	*/
#if 0
	static bool	reenter = No;

	if( reenter )		/* One more ^C, ighore it.	*/
		return 1;

	reenter = Yes;

	if( !gw_inited )	/* GW active ?			*/
		return 0;	/* Immediate exit.		*/

	if( confirm("Вы действительно хотите прервать программу") )
		return 0;

	reenter = No;
#endif
	return 1;
	}
