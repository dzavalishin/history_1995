/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module :  TIFF file header layout.
 *
 *	$Log:   D:/animate/tif2isg/vcs/tiff.h_v  $
 *      
 *         Rev 1.0   18 Dec 1990 22:33:34   gn
 *      Initial revision.
 *
 *
\*/

#ifndef	TIFF_H
#define	TIFF_H



#ifndef STYLE_H
#	include <style.h>
#endif


typedef struct {
	char	hh[2];		/*	First two bytes of file		*/
	short	nplane;		/*	Number of planes		*/

	ulong	width;		/*	Width of image ( pixels )	*/
	ulong	height;		/*	Height of picture ( pixels )	*/

	uint	compress;

	long	file_img[4];	/*	Position of image in file	*/
	char	*img_ptr[4];	/*	Address in memory		*/
	} tiff_gf;		/*	Tiff Graphical Fragment		*/


struct tiff_head {
	char	hh[2];
	int	ver;
	long	d1offset;
	};

struct tiff_dir {
	int	tag;
	int	type;
	long	length;
	long	offset;
	};


enum Tiff_label {
	Tiff_nplane	= 277,
	Tiff_width	= 256,
	Tiff_height	= 257,
	Tiff_image	= 273,
	Tiff_compress	= 259,
	};


/****************************************************************************
				Prototypes
****************************************************************************/


bool	do_load_tiff_image( FILE *fp, tiff_gf &info );


#endif	TIFF_H