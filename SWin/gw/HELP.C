/**-------------------   Graphics Windows   -------------------**\
 *
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728
 *
 *	Module :
 *
 *	$Log:   C:/c/gw/vcs/help.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:19:10   dz
 *      Initial revision.
 *
 *
 *
\******/

#include <style.h>
#include <graphics.h>
#include "sfile.h"
#include <string.h>
#include <stdio.h>

#include <dos.h>
#include <dir.h>

#include "futil.h"

#include "font.h"

#include "gw.h"
#include "local.h"
#include "mouse.h"






static cstyle  hstyle = {
	&_8x16t_font, YELLOW,
	DARKGRAY, 3, 5,		/*      Shadow parameters       */
	1,			/*      Bold factor             */
	0,			/*      Italics skew            */
	0, 0,
	0, 0,
	DARKGRAY,		/*      Back color              */
	{ 0 },
	/* Shadow */1, /*Bold*/0, /*Italics*/0, 0, 0,
	0			/*      copy put                */
	};


static void
set_ref_colors( void ) {
	hstyle.b_color = MAGENTA;
	hstyle.b_fill = Yes;
	}

static void
set_txt_colors( void ) {
	hstyle.b_color = DARKGRAY;
	hstyle.b_fill = Yes;
	}


static void
putline( char *buf ) {
	char	*e;
	char	*o;

	char	ref_text[100];
	char	ref_item[100];

	e = buf;
	do {
		buf = e;
		e = strchr( buf, '\\' );
		if( e )		*e++ = '\0';

		gwsputs( &hstyle, buf );

		if( e ) {
			switch( *e ) {
				case 'r':	/*	Reference	*/
				case 'R':
					e++;				/* Skip 'r'		*/
					o = ref_text;			/* Get text		*/
					while( *e != '.' && *e )
						*o++ = *e++;
					*o = '\0';
					if( *e == '\0' )
						return;

					e++;				/* Skip '.'		*/
					o = ref_item;			/* Get reference	*/
					while( *e != '.' && *e )
						*o++ = *e++;
					*o = '\0';
					if( *e )	e++;		/* Skip '.'		*/

					set_ref_colors();		/* Set reference colors	*/
					gwsputs( &hstyle, ref_text );	/* Display ref. text	*/
					set_txt_colors();		/* Reset old colors	*/
					break;
				}
			}

		} while( e );

	}


/************************************************************
 *
 * NB!  This function uses GWindows' "plane buffer" (28K)
 *	to store it's help data.
**/

#define	HBUFS	100


static void
disp_help_page( int sfd, int bd ) {
	char	buf[HBUFS+2];
	char	*point;
	char	*out;
	int	len;

		/*	Read all the text			*/
	len = sfread( sfd, bd, PLANEBUF_SIZE, 0L, planebuf );
	if( len <= 0 ) {
		ioerr_msg();
		return;
		}
	planebuf[len] = '\0';

	point = planebuf;
	moveto( 5, 5+12 );

	do {
		/*	Get one line into the buf		*/
		out = buf;
		len = HBUFS;
		while( *point >= ' ' && --len )
			*out++ = *point++;

		*out = '\0';
		putline( buf );
		gwsnl( &hstyle, 5 );

		while( *point == '\r' || *point == '\n' )
			point++;

		} while( *point >= ' ' );
	}









#if 0
static void
disp_help_page( int sfd, int bd ) {
	char	buf[HBUFS+2];
	uint	pos;
	uint	len	= 0;
	uint	free	= HBUFS;

	char	*e;

	moveto( 5, 5 );

	pos = 0;
	while( True ) {
		if( free >= 0 ) {				/* Free place in the buffer ?	*/
			strncpy( buf, buf+free, HBUFS-free );	/* Move rest to the left	*/
			len = sfread( sfd, bd, free, pos, buf+(HBUFS-free) );	/* Read more	*/
			if( len < 0 )				/* Error ?			*/
				return;

			pos += len;				/* Move file data pointer	*/

			len += (HBUFS-free);			/* Rest+what we read		*/
			buf[len] = '\0';			/* Cut data			*/

			if( len <= 0 )				/* Buffer is empty ?		*/
				return;

			}
		/*	We have len bytes in the buffer		*/

		e = strpbrk( buf, "\r\n\x1A" );
		if( e ) 	*e = '\0';

		putline( buf );
		if( e ) {
			gwsnl( &hstyle, 5 );
			e++;
			while( *e && *e < ' ' )
				e++;

			free = HBUFS - strlen(e);

			}
		else
			free = HBUFS;

		}
	}
#endif


/**Doc************************************************************************
 *	Function: help()      		Header: gw.h
 *
 *	Parameters:
 *		char *		topic	- Help item name.
 *
 *	Return value:			None.
 *
 *	Description:
 *		Opens help window and displays a page with
 *		a given name. This function searches help
 *		text in the superfile with the name of current
 *		program and extension '.shf'. Superfile must
 *		be created with 'mkhelp' program.
 *
**/


void
help( char *topic ) {
	int	sfd, bd;
	char	helpname[80];
	char	drive[5], dir[80], name[10], ext[5];
	static gwin	helpwin = {
		100, 100, 440, 150
		};

	fnsplit( _argv[0], drive, dir, name, ext );
	fnmerge( helpname, drive, dir, name, ".shf" );

	if( (sfd = sfopen( search_file( helpname, "JHELP" ) )) == Err ) {
		errmsg("Не могу откыть файл '%s'", helpname );
		return;
		}

	if( (bd = read_name_dir( sfd, 0, -1, topic )) == Err ) {
		errmsg("Информация не найдена (%s)", topic );
		sfclose( 0, sfd );
		return;
		}

	if( gwregister( &helpwin ) || gwattach( &helpwin ) ) {
		outofmem_msg();
		sfclose( 0, sfd );
		return;
		}


	gwborder3d( 3, LIGHTGRAY, 5, DARKGRAY, DARKGRAY );

	disp_help_page( sfd, bd );

	press_any_key();
	sfclose( 0, sfd );

	gwpermdetach();
	gwdestroy( &helpwin );
	}
