/**--------------------   Graphics Windows   -------------------**\
 *                                                               
 *     Copyright (C) 1989 by DZ - Moscow, 110.6728		 
 *                                                               
 *     Module : Beta Chi-Soft font load function.                
 *        
 *	$Log:   C:/c/gw/vcs/betafont.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:19:52   dz
 *      Initial revision.
 *                                                       
 *     Revision 1.1     22.08.89                                 
 *       Initial revision.                                       
 *                                                               
 *                                                               
\********/

#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <alloc.h>
#include <stdlib.h>

#include "font.h"




#define	BETA_X_POS	658
#define BETA_Y_POS	660
#define	BETA_LL_POS     (512+154)


bool
load_beta_font( font *fnt, char *file ) {
	int	fd;
	unsigned char	cb;
	font	tf;

	if((fd = open( file, O_RDONLY|O_BINARY )) < 0 )
		return Err;


	lseek( fd, BETA_X_POS, 0 );
	if( read( fd, &cb, 1 ) != 1 ) {
		close( fd );
		return Err;
		}
	tf.hsize = cb;

	lseek( fd, BETA_Y_POS, 0 );
	if( read( fd, &cb, 1 ) != 1 ) {
		close( fd );
		return Err;
		}
	tf.vsize = cb;

	tf.bpl = ( tf.hsize % 8 ) ? tf.hsize/8 + 1 : tf.hsize/8;
	tf.bpl *= tf.vsize;

	tf.fontptr = malloc( tf.bpl*256 );
	if( tf.fontptr == NULL ) {
		close( fd );
		errno = ENOMEM;
		return Err;
		}

	lseek( fd, BETA_LL_POS, 0 );
	if( read( fd, tf.fontptr, tf.bpl*256 )
			!= tf.bpl*256 ) {
		close( fd );
		return Err;
		}

	close( fd );




	if( fnt->allc )
		free( fnt->fontptr );
	if( fnt->pallc )
		free( fnt->propptr );

	tf.pallc	= 0;
	tf.prop		= 0;


	*fnt = tf;

	return Ok;
	}


