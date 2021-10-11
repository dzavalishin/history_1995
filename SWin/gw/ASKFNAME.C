/**********************   Graphics Windows   *************************\
 *                                                               
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728		 
 *                                                               
 *	Module : Functions for asking Input/output file name.     
 *                                                            
 *	$Log:   C:/c/gw/vcs/askfname.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:19:54   dz
 *      Initial revision.
 *
 *	   Revision 1.1     16 Dec 89                                
 *	Initial revision.                                       
 *                                                               
 *                                                               
\******/


#include "local.h"
#include "gw.h"
#include "font.h"

#include <string.h>
#include <io.h>
#include <stdio.h>

/*#include <>			/*		*/
/*#include <>			/*		*/
/*#include <>			/*		*/


static bool
askname(
	int x, int y, int len,
	char *mask, char *out,
	char *head, char *help,
	bool f_exist, bool f_dir, bool f_nonexist ) {


	char	dhelp[50];

	sprintf( dhelp, "%.45sdir", help );

	while( 1 ) {
		while( 1 ) {
			if( !gwedits( x, y, len, mask, head, help ) )
				return No;

			if( !strpbrk( mask, "*?" )) {
				strcpy( out, mask );
				break;
				}
			if( f_dir ) {
				if( dirmenu( mask, out, dhelp ) )
					break;
				}
			else {
				errmsg("Недопустимый символ в имени");
				continue;
				}
			}
		if( f_exist && access( out, 0 ) ) {	/* Must exist	*/
			errmsg("Файл `%s' не найден", out );
			continue;
			}

		if( f_nonexist && !access( out, 0 )) {	/* Must not exist */
			errmsg("Файл `%s' существует", out );
			continue;
			}

		return Yes;
		}
	return No;		/*	To keep TC happy	*/
	}




/**Doc************************************************************************
 *	Function: infname(), outfname(), newfname().	Header: gw.h
 *
 *	Parameters:
 *		int	x		X Position of the window
 *		int	y		Y position of the window
 *		int	len		Maximal length of the string
 *		char *	mask		File name buffer.
 *		char *	out		Resulting file name.
 *		char *	head		Edits header (question).
 *		char *	help		Help item name
 *
 *	Return value on error:          False
 *
 *	Normal return value:		True
 *
 *	Description:
 *		This function is used to ask a name of some input
 *		file. It asks name from user, lets him pick name
 *		from a directory list, checks file existance. This
 *		function also keeps user-entered name in 'mask'
 *		buffer, and puts it into the 'out' buffer. Thiese
 *		buffers will differ only when user entered a 'wild-card'.
 *		In this case 'mask' will contain a wildcarded name,
 *		but 'out' will contain picked from the directory list one.
 *
 *		Differencies:
 *			infname  - file must exist.
 *			outfname - file may exist, may not.
 *			newfname - file mustn't exist, no directory list.
 *
**/


bool
infname( int x, int y, int len, char *mask, char *out, char *head, char *help ) {
	return askname( x, y, len, mask, out, head, help, True /*f_exist*/, True /*f_dir*/, False /*f_nonexist*/ );
	}



bool
outfname( int x, int y, int len, char *mask, char *out, char *head, char *help ) {
	return askname( x, y, len, mask, out, head, help, False /*f_exist*/, True /*f_dir*/, False /*f_nonexist*/ );
	}


bool
newfname( int x, int y, int len, char *mask, char *out, char *head, char *help ) {
	return askname( x, y, len, mask, out, head, help, False /*f_exist*/, False /*f_dir*/, True /*f_nonexist*/ );
	}

































