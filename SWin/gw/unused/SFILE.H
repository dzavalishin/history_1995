/**-------------------   Graphics Windows   -------------------**\
 *
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728
 *
 *	Module : SuperFile header.
 *
 *	$Log:   C:/c/gw/vcs/sfile.h_v  $
 * 
 *    Rev 1.0   29 Apr 1990 16:20:46   dz
 * Initial revision.
 *
 *
 *
\******/

#include <errno.h>

#ifndef STYLE_H
#	include <style.h>
#endif


/* Internal errors */


#define		NO_FREE_DESCR	-10	/* 	*/
#define		NO_FREE_SPACE	-11	/*	*/
#define		NO_NAME		-12	/*	*/
#define		NAME_OUT	-13	/* Name copyed in out */


/**************************************************************************/


#define         MAX_NUM_DESCR	1000
#define		MAX_SF_OPEN	5

#define		SUPERBLOCK	11	/*	*/
#define		DESCRIPTION	12	/*	*/
#define		FREEBLOCK	13	/*	*/
#define		CATALOG		14	/*	*/

#define		ALL		-15	/*	*/


/*************************************************************************/


/* Return's functions superfile */


#define		EXIST		1	/* something exist */
#define		NOEXIST		2	/* something not exist */
#define		NO_ADRESS	-3	/* No adress */

#define		F_DESCR		0	/* Free description */
#define		NF_DESCR	1	/* Allok description */

#define		IS_DIR		1	/* Is a directory	*/
#define		NOT_DIR		0	/* Not a directory	*/
#define		NEW_DSCR        -1	/* For function sfwrite */


#define         REZERVSIZE	16
#define         DIREXTENSION	5

/************************************************************************/


/* Structure for superblock */



struct	Superblock {
	char	spec[21];
	int	num_descr;
	long	adress_end_of_data;
	long	adress_one_free_block;
	long    adress_last_free_block;
	int	exist_free_block;
	char	label[21];
	char	dummy[ 128 - 42 - sizeof(int)*2 - sizeof(long)*3 ];
	};


/************************************************************************/


/* Structure for description */



struct	Descr {
	unsigned	status : 1;	/* Free or no free 	*/
	unsigned	dir : 1;	/* Directory		*/
	int		num_dname;	/* number of the names directory */
	char		flags;          /* User's flags		*/
	long		adress_data;	/* begin adress		*/
	long		data_size;   	/* free size of the relation	*/
	long		real_space;     /* space in block		*/
	};

/***********************************************************************/


/* Structure for free block */



struct	Freeblock {
	long	size;                   /* size of the block	*/
	long	adress_next_free_block; /* adress nrxt free block */
	};

/***********************************************************************/


/* Structure for directores */


struct	Bnames {
	char  	bname[14];
	int	numdscr;
	};

/***********************************************************************/

/* SuperFile signature	*/

extern char	Sf_sign[];


/***********************************************************************/


int     mksf( char *sfname, int numdescr, int numndir );     /* Make superfile */
int     sfopen( char *sfname );                 /* Open superfile */
int     sfclose( int key, int nsfd   );         /* Close superfile , if key - ALL will be closed all opend superfiles */

/***************************************************************************/

int     sfalloc( int nsfd, long size );        /* Allocation block superfile */
int     allocbsf( int nsfd, long size );        /* Allocation block superfile */

int	sfrealloc( int nsfd, int ndscr, long badr, long size );
int	reallocbsf( int nsfd, int ndscr, long badr, long size );

/***************************************************************************/

int     sffree( int nsfd, int ndscr );         /*      */
int     freebsf( int nsfd, int ndscr );         /*      */

int     sfwrite( int nsfd, int ndscr, uint size, long adress_in_data, void *out);
int     sfread( int nsfd, int ndscr, uint size, long adress_in_data, void *in );

long	sfret_sizeblk( int nsfd, int ndscr );

/***************************************************************************/
/*		Function for directory
*/

int     sfmkdir( int nsfd, int num_bname );     /*      */

int  	sfread_name_dir( int nsfd, int ndscr, int numdsc, char *out ); 	/*	*/
int  	read_name_dir( int nsfd, int ndscr, int numdsc, char *out ); 	/*	*/

int	sfwrite_name_dir( int nsfd, int ndscr, int numdsc, char *in );	/*	*/
int	write_name_dir( int nsfd, int ndscr, int numdsc, char *in );	/*	*/

int	sfdel_name_dir( int nsfd, int ndscr, int numdsc, char *out );	/*	*/
int	del_name_dir( int nsfd, int ndscr, int numdsc, char *out );	/*	*/
int	sfreallocdir( int nsfd, int ndscr, int numdname );
bool	sfflush( int nsfd );
int	sfload( int nsfd, int ndscr, char *fname, long baddrin, long baddrout, long size );

int	sfsetspace( int nsfd, int ndscr, long new_space );


/***************************************************************************/




