/************************ Animate ***************************\
 *
 *	Copyright (C) 1990 by Infinity Soft
 *
 *	Module :   Tiff grphics file support.
 *
 *	$Log:   D:/animate/tif2isg/vcs/tiff.cpv  $
 *      
 *         Rev 1.0   18 Dec 1990 22:33:10   gn
 *      Initial revision.
 *
\*/

#include	<style.h>
#include	<stdlib.h>
#include	<alloc.h>
#include	<stdio.h>
#include	<string.h>


#include	"tiff.h"




bool
decode_tiff( FILE *fp, tiff_gf &data ) {

	int	nitems;

	struct tiff_head	h;
	struct tiff_dir		d;

	fseek( fp, 0L, SEEK_SET );
	if( fread( &h, sizeof(h), 1, fp ) != 1 ) {
		printf("Error reading Tiff header\n", h.ver );
		return Err;
		}

	if( h.ver < 42 ) {
		printf("Unknown Tiff format version: %d\n", h.ver );
		return Err;
		}

	strncpy( data.hh, h.hh, 2 );

	fseek( fp, h.d1offset, SEEK_SET );

	if(fread( &nitems, sizeof(nitems), 1, fp ) != 1 ) {
		printf("Error reading Tiff file\n");
		return Err;
		}

	data.nplane = 0;

	while( nitems-- ) {	/*	Read all entries		*/
		if( fread( &d, sizeof(d), 1, fp ) != 1 ) {
			printf("Error reading Tiff file\n");
			return Err;
			}

		switch( d.tag ) {
		case Tiff_nplane:	data.nplane = (int) d.offset;	break;
		case Tiff_width:	data.width	= d.offset;	break;
		case Tiff_height:	data.height	= d.offset;	break;

		case Tiff_image:
			if( data.nplane == 1 ) {
				data.file_img[0] = d.offset;
				data.file_img[1] =
				data.file_img[2] =
				data.file_img[3] = 0L;
				break;
				}

			if( data.nplane == 4 ) {
				long save = ftell( fp );

				fseek( fp, d.offset, SEEK_SET );
				if( fread( data.file_img, sizeof(long) * 4, 1, fp ) != 1 ) {
					printf("Error reading Tiff file\n");
					return Err;
					}

				fseek( fp, save, SEEK_SET );

				break;
				}

			printf("Tiff file error: wrong number of planes ( %d )\n", data.nplane );
			return Err;	/*	Bad number of planes	*/

		case Tiff_compress:
			data.compress	= (uint)d.offset;	break;
			}

		}

	return Ok;
	}




bool
do_load_tiff_image( FILE *fp, tiff_gf &info ) {
	uint		size_to_load;
	uint		np;

	if( decode_tiff( fp, info ) == Err )
		return Err;


	if( info.compress != 0x1 ) {
		printf("Не могу прочесть картинку - сжатый формат");
		return Err;
		}

	size_to_load = (uint) (info.width % 8 ? (info.width/8)+1 : info.width/8);
	size_to_load *= (uint) info.height;


	for( np = 0; np < 4; np++ ) {

		if( (info.img_ptr[np] = new char[size_to_load] ) == NULL ) {
			printf("Недостаточно памяти для графики");
			return Err;
			}


		fseek( fp, info.file_img[np], SEEK_SET );
		if( fread( info.img_ptr[np], size_to_load, 1, fp ) <= 0 ) {
			printf("Ошибка чтения изображения\n");
			free( info.img_ptr[np] );
			return Err;
			}
		}

	return Ok;
	}







