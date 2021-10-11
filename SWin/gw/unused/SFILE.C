/**-------------------   Graphics Windows   -------------------**\
 *
 *	Copyright (C) 1989 by DZ - Moscow, 110.6728
 *
 *	Module :	SuperFile
 *
 *	$Log:   C:/c/gw/vcs/sfile.c_v  $
 *      
 *         Rev 1.0   29 Apr 1990 16:20:32   dz
 *      Initial revision.
 *
 *
 *
\******/

#include <stdio.h>
#include <errno.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <alloc.h>
#include <conio.h>
#include "style.h"
#include "sfile.h"


#define	assert_sfd( nsfd )						\
	if( Sfd[nsfd] < 0 || nsfd >= MAX_SF_OPEN || nsfd < 0 ) {	\
		errno = EBADF;						\
		return Err;						\
		}

#define assert_dscr( ndscr )						\
	if( Dscr.status == F_DESCR || ndscr < 0 ) {			\
		errno = EINVAL;						\
		return Err;						\
		}



/***************************************************************************/
int	rubeg( int nsfd );/*	*/
int     lookfreedescr( int nsfd );              /*      */
long    lookfreespace( int nsfd, long size );   /*      */
long    serchfreespace( int nsfd, long size );  /*      */
int	freedscr( int nsfd, int ndscr );
long	allocspace( int nsfd );
int	sfcopyblock( int nsfd, long adr1, long adr2, long size );
int	freespace( int nsfd, long address_data, long data_size );
int	copyfilesfile( int fp, int nsfd, long addrin, long addrout, long size );

/***************************************************************************/

void    initdir( void );                        /*      */
int	delnamdir(int nsfd, int ndscr );
int	serch_eq_name_dir( int nsfd, long address, char *dname );

/***************************************************************************/

int     sfexist( char *sfname );                /* Control exist something */
void    initsdblk( struct Superblock *s, int numdescr );              /* Init blocks fo write's  */
void    initsfd( void );

int     writestruct( int sfd, int key );
int     readstruct ( int sfd, int key );
int     dseek( int nsfd, int ndescr );





struct  Superblock      Sblk[ MAX_SF_OPEN ];	/*	Superblocks	*/
int			Sb_r[ MAX_SF_OPEN ];	/*	Don't read	*/

struct  Descr           Dscr;
struct  Freeblock       Fblk;
struct  Bnames          Bnam;


#define     Sizesblk	sizeof( struct Superblock )
#define     Sizedscr	sizeof( struct Descr      )
#define     Sizefblk	sizeof( struct Freeblock  )
#define     Sizebnam	sizeof( struct Bnames     )

int     Sfd[ MAX_SF_OPEN ];
int     Sfopen = 0;


char    Sf_sign[] = "Superfile (C) by DYZ";






bool
wr_sblk( int nsfd ) {		/*	Write back superblock		*/
	if( lseek( Sfd[nsfd], 0l, SEEK_SET ) == -1L ) {
		errno = EFAULT;
		return Err;
		}

	if( write( Sfd[nsfd], &Sblk[nsfd], Sizesblk ) <= 0 ) {
		errno = EIO;
		return Err;
		}
	return Ok;
	}

bool
rd_sblk( int nsfd ) {
	if( Sb_r[nsfd] )
		return Ok;


	if( lseek( Sfd[nsfd], 0l, SEEK_SET )  == -1L ) {
		errno = EFAULT;
		return Err;
		}

	if( read( Sfd[nsfd], &Sblk[nsfd], Sizesblk ) <= 0 ) {
		errno = EIO;
		return Err;
		}
	return Ok;
	}




int
mksf( char *sfname, int numdescr, int numndir )
{
        int     sfd;
        int     i;

	struct	Superblock	s;

        if( numdescr > MAX_NUM_DESCR ) {
                errno = EMFILE;         /* Too many open files          */
                return Err;
                }

        if( sfexist( sfname ) == EXIST ) {
                errno = EINVAL;         /* File exist                   */
                return Err;
                }

        if( ( sfd = open( sfname, O_RDWR|O_CREAT|O_BINARY, 0666 ) ) == -1 ) {
                errno = EFAULT;
                return Err;
                }

	initsdblk( &s, numdescr );

	lseek( sfd, 0L, SEEK_SET );

	if( write( sfd, &s, sizeof(s)) != sizeof(s) ){
                close( sfd );
                unlink( sfname );
                errno = EFAULT;
                return Err;
                }

	for( i = 0; i < numdescr; i++){
		if( writestruct( sfd, DESCRIPTION ) == Err ){
                        close( sfd );
                        unlink( sfname );
                        errno = EFAULT;
                        return Err;
                        }
		}

	if( Sfopen == 0 ){
		Sfd[ 0 ] = sfd;
		if( sfmkdir( 0, numndir ) == Err )
			return Err;

		Sfd[ 0 ] = -1;
		close( sfd );
		return Ok;

		}
	else {
		for( i = 0; i < MAX_SF_OPEN; i++ ){

			if( Sfd[ i ] == -1 ){
				Sfd[ i ] = sfd;

				if( sfmkdir( i, numndir ) == Err )
					return Err;

				Sfd[ i ] = -1;
				close( sfd );
				return Ok;
				}
			}

		errno = ENMFILE;        /*  No more descriptors */
		return Err;

		}

	}

int
sfopen( char *sfname )
{
        int     i = 0;


        if( sfexist( sfname ) == NOEXIST ) {
                errno = ENOFILE;
                return Err;
                }

	if( Sfopen == 0 ){
                initsfd();
		Sfopen = 1;
		}


        for( i = 0; i < MAX_SF_OPEN; i++ ){
                if( Sfd[ i ] == -1 ){
                        int     sfd;

                        if( ( sfd = open( sfname, O_RDWR|O_BINARY ) ) == -1 ) {
                                /* Open sets errno */
                                return Err;
                                }
                        else{
				Sfd[ i ] = sfd;
				Sb_r[i] = No;	/* Re-read superblock [DZ] */
                                return i;
                                }
                        }
                }

        errno = ENMFILE;        /*  No more descriptors */
        return Err;

        }

int
sfclose( int key , int nsfd )
{
        int     i = 0;


        if( key == ALL ){

                for( i = 0; i < MAX_SF_OPEN; i++ ){

                        if( Sfd[i] != -1 ){
                                close( Sfd[i] );
				Sb_r[i] = No;	/* Re-read superblock [DZ] */
                                }
                        }

                Sfopen = 0;
                return Ok;

                }

        else{
                if( nsfd < 0 || nsfd > MAX_SF_OPEN ) {
                        errno = EBADF;
                        return Err;
                        }

                if( Sfd[ nsfd ] != -1 ) {
                        close( Sfd[ nsfd ] );
			Sb_r[nsfd] = No;	/* Re-read superblock [DZ] */
                        Sfd[ nsfd ] = -1;
                        }

                return Ok;

                }

        }

int
sfalloc( int nsfd, long size )
{
	return allocbsf(  nsfd,  size );
	}


int
sfrealloc( int nsfd, int ndscr, long badr, long size )
{
	return reallocbsf(  nsfd, ndscr, badr, size );
	}

int
sffree( int nsfd, int ndscr )
{
	return freebsf(  nsfd, ndscr );
	}


int
sfread( int nsfd, int ndscr, uint size, long adress_in_data, void *in )
{

	assert_sfd( nsfd );

        if( dseek( nsfd, ndscr ) == Err )
                return Err;

        if( readstruct( Sfd[ nsfd ], DESCRIPTION ) != Ok )
                return Err;

	assert_dscr( ndscr );

	if( Dscr.real_space < adress_in_data || adress_in_data < 0L ) {
                errno = EINVAL;
                return Err;
                }

	if( Dscr.real_space < size + adress_in_data )
		size = (int)( Dscr.real_space - adress_in_data) ;



        if(lseek( Sfd[ nsfd ], (long)( Dscr.adress_data + adress_in_data ), SEEK_SET ) == -1L ) {
                errno = EFAULT;
                return Err;
                }


	return read( Sfd[ nsfd ], in, size );

	}


int
sfwrite( int nsfd, int ndscr, uint size, long adress_in_data, void *out)
{


	assert_sfd( nsfd );


	if( dseek( nsfd, ndscr ) == Err )
                return Err;

        if( readstruct( Sfd[ nsfd ], DESCRIPTION ) != Ok )
                return Err;

	assert_dscr( ndscr );

/*	warnmsg( "before realloc bdsize = %ld , need = %ld ", Dscr.data_size, size + adress_in_data  );		*/
	if( Dscr.data_size < size + adress_in_data ){
		if( ( ndscr = reallocbsf( nsfd, ndscr, 0L, size + adress_in_data + REZERVSIZE ) ) == Err )
			return Err;

		if( dseek( nsfd, ndscr ) == Err )
			return Err;

		if( readstruct( Sfd[ nsfd ], DESCRIPTION ) != Ok )
			return Err;
		}

/*	warnmsg( "after realloc bdsize = %ld, ralspace = %ld", Dscr.data_size, Dscr.real_space );	*/

	if( size + adress_in_data > Dscr.real_space ){
		Dscr.real_space = size + adress_in_data;

/*	warnmsg( "after realloc bdsize = %ld, ralspace = %ld", Dscr.data_size, Dscr.real_space );	*/
		if( dseek( nsfd, ndscr ) == Err )
			return Err;

		if( writestruct( Sfd[ nsfd ], DESCRIPTION ) != Ok )
			return Err;
		}

        if(lseek( Sfd[ nsfd ], (long)( Dscr.adress_data + adress_in_data ), SEEK_SET ) == -1L ) {
                errno = EFAULT;
                return Err;
                }
	return write( Sfd[ nsfd ], out, size ) ;

	}


int
sfmkdir( int nsfd, int num_bname )
{
        int     ndscr;
        int     i;
        long    size;

	assert_sfd( nsfd );

        size = Sizebnam * num_bname;

        ndscr = allocbsf( nsfd, size );

        if( ndscr==Err )
                return Err;

        initdir();


        if( dseek( nsfd, ndscr ) == Err )
                return Err;

        if( readstruct( Sfd[ nsfd ], DESCRIPTION ) != Ok )
                return Err;


        if( lseek( Sfd[ nsfd ], (long)( Dscr.adress_data ), SEEK_SET ) == -1L ) {
                errno = EFAULT;
                return Err;
                }

        for( i = 0; i < num_bname; i++ ) {

                if( writestruct( Sfd[ nsfd ], CATALOG ) != Ok )
                        return Err;
                }

        Dscr.dir = IS_DIR;
	Dscr.num_dname = 0;
	Dscr.real_space = Sizebnam * num_bname;

        if( dseek( nsfd, ndscr ) == Err )
                return Err;

        if( writestruct( Sfd[ nsfd ], DESCRIPTION ) != Ok )
                return Err;



        return ndscr;

        }

int
sfreallocdir( int nsfd, int ndscr, int numdname ){

	long		addrd;
	int		i;
	int		ndir;

	assert_sfd( nsfd );

	if( numdname <= 0 ){
		errno = EINVAL;
		return Err;
		}

	if( dseek( nsfd, ndscr ) == Err )
                return Err;

        if( readstruct( Sfd[ nsfd ], DESCRIPTION ) != Ok )
                return Err;

	if( Dscr.dir == NOT_DIR || Dscr.status == F_DESCR ){
                errno = EINVAL;
                return Err;
                }

	if( Dscr.real_space / Sizebnam >= numdname ){
		if( Dscr.real_space / Sizebnam == numdname  )
			return ndscr;
		else{
			if( Dscr.num_dname > numdname )
				Dscr.num_dname = numdname;

			Dscr.real_space = numdname * Sizebnam;

			if( dseek( nsfd, ndscr ) == Err )
				return Err;

			if( writestruct( Sfd[ nsfd ], DESCRIPTION ) != Ok )
				return Err;

			return ndscr;
			}
		}

	addrd = Dscr.real_space ;
	ndir  = (int)( numdname - Dscr.real_space / Sizebnam );

	if( ( ndscr = reallocbsf( nsfd, ndscr, 0L, Sizebnam * numdname ) ) != ndscr )
		return Err;

	if( dseek( nsfd, ndscr ) == Err )
                return Err;

        if( readstruct( Sfd[ nsfd ], DESCRIPTION ) != Ok )
                return Err;


	if( lseek( Sfd[nsfd], Dscr.adress_data + addrd, SEEK_SET ) == -1L )
                return Err;

	initdir();

	for( i = 0; i < ndir; i++ ){
		if( writestruct( Sfd[ nsfd ], CATALOG ) != Ok )
			return Err;
		}

	Dscr.real_space = numdname * Sizebnam;

	if( dseek( nsfd, ndscr ) == Err )
		return Err;

	if( writestruct( Sfd[ nsfd ], DESCRIPTION ) != Ok )
		return Err;

	return ndscr;
	}

int
sfread_name_dir( int nsfd, int ndscr, int numdsc, char *out )
{
	return read_name_dir( nsfd, ndscr, numdsc, out );
	}

int
read_name_dir( int nsfd, int ndscr, int numdsc, char *out )
{
	int	i;

	assert_sfd( nsfd );

	if( dseek( nsfd, ndscr ) == Err )
                return Err;

        if( readstruct( Sfd[ nsfd ], DESCRIPTION ) != Ok )
                return Err;

	assert_dscr( ndscr );

	if( Dscr.dir == NOT_DIR ){
                errno = EINVAL;
                return Err;
                }

        if( lseek( Sfd[ nsfd ], (long)( Dscr.adress_data ), SEEK_SET ) == -1L ) {
                errno = EFAULT;
                return Err;
                }

	for( i = 0; i < (int)( Dscr.real_space / Sizebnam ); i++ ) {

		if( readstruct( Sfd[ nsfd ], CATALOG ) != Ok )
			return Err;

		if( Bnam.numdscr != NO_NAME ){

			if( numdsc == -1 ){
				if( strcmp( out , Bnam.bname ) == 0 )
					return Bnam.numdscr;
				}
			else {
				if( Bnam.numdscr == numdsc ){
					strcpy( out, Bnam.bname );
					return Ok;
					}
				}
			}
		}

	errno = ENOENT;
	return Err;

	}

int
sfwrite_name_dir( int nsfd, int ndscr, int numdsc, char *in )
{
	return write_name_dir( nsfd, ndscr, numdsc, in );
	}


int
write_name_dir( int nsfd, int ndscr, int numdsc, char *in )
{
	int	i;


	assert_sfd( nsfd );

	if( dseek( nsfd, ndscr ) == Err )
                return Err;

        if( readstruct( Sfd[ nsfd ], DESCRIPTION ) != Ok )
                return Err;

	assert_dscr( ndscr );

	if( Dscr.dir == NOT_DIR ){
                errno = EINVAL;
                return Err;
                }


	if( Dscr.num_dname >=(int)( Dscr.real_space / Sizebnam ) ){
		if( sfreallocdir( nsfd, ndscr, (int)( Dscr.real_space / Sizebnam ) + DIREXTENSION ) != ndscr )
			return Err;
			}

	if( serch_eq_name_dir( nsfd, Dscr.adress_data, in ) == Err )
		return Err;

        if( lseek( Sfd[ nsfd ], (long)( Dscr.adress_data ), SEEK_SET ) == -1L ) {
                errno = EFAULT;
                return Err;
                }

	for( i = 0; i < (int)( Dscr.real_space / Sizebnam ); i++ ) {

		if( readstruct( Sfd[ nsfd ], CATALOG ) != Ok )
			return Err;

		if( Bnam.numdscr == NO_NAME ){
			Bnam.numdscr = numdsc;
			strcpy( Bnam.bname, in );

			if( lseek( Sfd[ nsfd ], -(long)( Sizebnam ), SEEK_CUR ) == -1L ) {
				errno = EFAULT;
				return Err;
				}

			if( writestruct( Sfd[ nsfd ], CATALOG ) != Ok )
				return Err;

			Dscr.num_dname++;

			if( dseek( nsfd, ndscr ) == Err )
				return Err;

			if( writestruct( Sfd[ nsfd ], DESCRIPTION ) != Ok )
				return Err;

			return Ok;
			}
		}

	errno = ENMFILE;
	return Err;

	}

static	int
serch_eq_name_dir( int nsfd, long address, char *dname ){

	int	i = 0;

	if( lseek( Sfd[ nsfd ], address, SEEK_SET ) == -1L ) {
                errno = EFAULT;
                return Err;
                }

	for( i = 0; i <(int)( Dscr.real_space / Sizebnam ); i++ ) {

		if( readstruct( Sfd[ nsfd ], CATALOG ) != Ok )
			return Err;

		if( strcmp( Bnam.bname, dname ) == 0 )
			return Err;
		}

	return Ok;
	}



int
sfdel_name_dir( int nsfd, int ndscr, int numdsc, char *out )
{
	return del_name_dir( nsfd, ndscr, numdsc, out );
	}

int
del_name_dir( int nsfd, int ndscr, int numdsc, char *out )
{
	int	i;

	assert_sfd( nsfd );

	if( dseek( nsfd, ndscr ) == Err )
                return Err;

        if( readstruct( Sfd[ nsfd ], DESCRIPTION ) != Ok )
                return Err;

	assert_dscr( ndscr );

	if( Dscr.dir == NOT_DIR ){
                errno = EINVAL;
                return Err;
                }

        if( lseek( Sfd[ nsfd ], (long)( Dscr.adress_data ), SEEK_SET ) == -1L ) {
                errno = EFAULT;
                return Err;
                }

	for( i = 0; i < (int)(Dscr.real_space / Sizebnam); i++ ) {

		if( readstruct( Sfd[ nsfd ], CATALOG ) != Ok )
			return Err;

		if( Bnam.numdscr != NO_NAME ){

			if( numdsc == -1 ){

				if( strcmp( out , Bnam.bname ) == 0 ){
					return delnamdir( nsfd, ndscr );
					}
				}
			else {

				if( Bnam.numdscr == numdsc ){
					return delnamdir( nsfd, ndscr );
					}
				}
			}
		}
	errno = ENOENT;
	return Err;
	}

long
sfret_sizeblk( int nsfd, int ndscr ){

	assert_sfd( nsfd );

	if( dseek( nsfd, ndscr ) == Err )
                return Err;

        if( readstruct( Sfd[ nsfd ], DESCRIPTION ) != Ok )
                return Err;

	assert_dscr( ndscr );

	return Dscr.real_space ;
	}


static	int
delnamdir(int nsfd, int ndscr )
{

	Bnam.numdscr = NO_NAME;
	sprintf( Bnam.bname,"No name" );

	if( lseek( Sfd[ nsfd ], -(long)( Sizebnam ), SEEK_CUR ) == -1L ) {
		errno = EFAULT;
		return Err;
		}


	if( writestruct( Sfd[ nsfd ], CATALOG ) != Ok )
		return Err;


	Dscr.num_dname--;

	if( dseek( nsfd, ndscr ) == Err )
		return Err;

	if( writestruct( Sfd[ nsfd ], DESCRIPTION ) != Ok )
		return Err;

	return Ok;

	}



static  void
initdir( void )
{

        Bnam.numdscr = NO_NAME;
        sprintf( Bnam.bname,"No name" );

        }

static  int
dseek( int nsfd, int ndescr )
{

        if( lseek( Sfd[ nsfd ], (long)(Sizesblk + Sizedscr * ndescr ), SEEK_SET ) == -1l ) {
                errno = EFAULT;
                return Err;
                }

        return Ok;

        }


static	int
allocdscr( int nsfd )

{

        int     i;

	if( rd_sblk( nsfd ) != Ok )
                return Err;

	for( i = 0; i < Sblk[nsfd].num_descr; i++ ){

                if( dseek( nsfd, i ) == Err )
                        return Err;

                if( readstruct( Sfd[ nsfd ], DESCRIPTION ) == Err )
                        return Err;

		if( Dscr.status == F_DESCR ){
			Dscr.status = NF_DESCR;
			return i;
			}

                }

        errno = ENMFILE;
        return Err;
        }

static void
initsfd( void )
{
        int     i;

        for( i = 0; i < MAX_SF_OPEN; i++ )
                Sfd[i] = -1;

	}


static int
sfexist( char *sfname ){

        if( access(  sfname, 0 ))
                return NOEXIST;
        else
                return EXIST;
        }

static void
initsdblk( struct Superblock *s, int numdescr )
{
	sprintf( s->spec, Sf_sign );
	s->num_descr             = numdescr;
	s->adress_end_of_data    = Sizesblk + ( numdescr * Sizedscr );
	s->adress_one_free_block = NO_ADRESS;
	s->adress_last_free_block= NO_ADRESS;
	sprintf( s->label," No label " );
	s->exist_free_block = NOEXIST;
	s->dummy[0] = 0;

        Dscr.status                = F_DESCR;
        Dscr.dir                   = NOT_DIR;
        Dscr.adress_data           = NO_ADRESS;
        Dscr.data_size             = NULL;
        Dscr.real_space            = NULL;

        }




static int
writestruct( int sfd , int key ) {

	switch( key ){

                case DESCRIPTION:
                        if( write( sfd, &Dscr, Sizedscr ) <= 0) {
                                errno = EIO;
                                return Err;
                                }

                        break;

                case FREEBLOCK:
                        if( write( sfd, &Fblk, Sizefblk ) <= 0 ) {
                                errno = EIO;
                                return Err;
                                }

                        break;

                case CATALOG:
			if( write( sfd, &Bnam, Sizebnam ) <= 0 ) {
				errno = EIO;
                                return Err;
                                }

                        break;
                }

        return Ok;
        }

static	int
readstruct( int sfd, int key ){

	switch( key ){
                case DESCRIPTION:
                        if( read( sfd, &Dscr, Sizedscr ) <= 0 ) {
                                errno = EIO;
                                return Err;
                                }
                        break;

                case FREEBLOCK:
			if( read( sfd, &Fblk, Sizefblk ) != Sizefblk ) {
				errno = EIO;
                                return Err;
                                }
                        break;

                case CATALOG:
                        if( read( sfd, &Bnam, Sizebnam ) <= 0 ) {
                                errno = EIO;
                                return Err;
                                }
                        break;
                }

        return Ok;
        }

static	int
freedscr( int nsfd, int ndscr ){

	Dscr.status = F_DESCR;

	if( dseek( nsfd, ndscr ) == Err )
		return Err;

	if( writestruct( Sfd[ nsfd ], DESCRIPTION ) == Err )
		return Err;

	return Ok;

	}

static	long 	Data_size;

int
allocbsf( int nsfd, long size ){

	int	ndscr;
	long	address_for_data;


	assert_sfd( nsfd );

	Data_size = size;

	if( Data_size < Sizefblk )
		Data_size = Sizefblk;

	if( ( ndscr = allocdscr( nsfd ) ) == Err )
		return Err;


	if( ( address_for_data = allocspace( nsfd ) ) == Err ){


		if( freedscr( nsfd, ndscr ) == Err )
			return Err;

		return Err;
		}

	Dscr.adress_data = address_for_data;
	Dscr.data_size   = Data_size;
	Dscr.real_space	 = 0L;
	Dscr.dir	 = NOT_DIR;

	if( dseek( nsfd, ndscr ) == Err )
		return Err;

	if( writestruct( Sfd[ nsfd ], DESCRIPTION ) == Err )
		return Err;

	return ndscr;
	}


static	long
allocspace( int nsfd ){


	if( rd_sblk( nsfd ) != Ok )
                return Err;

	if( Sblk[nsfd].exist_free_block == NOEXIST ){
		Sblk[nsfd].adress_end_of_data += Data_size;

		if( wr_sblk( nsfd ) != Ok )
			return Err;

		return (Sblk[nsfd].adress_end_of_data - Data_size );
		}
	else{

		long	real_address;
		long	afto_address;
		long	next_address;
		int	key_one_free_block;


		real_address = Sblk[nsfd].adress_one_free_block;
		afto_address = NO_ADRESS;
		key_one_free_block = 0;


		while( real_address != NO_ADRESS ){
			long	may_size = 0;

			if( lseek( Sfd[nsfd], real_address ,SEEK_SET ) == -1l ) {
				errno = EFAULT;
				break;
				}

			if( readstruct( Sfd[ nsfd ], FREEBLOCK ) != Ok )
				break;

			may_size = Fblk.size;
			next_address = Fblk.adress_next_free_block;

			if( Fblk.size >= Data_size ){

				if( key_one_free_block == 0 ){

					if( (Sblk[nsfd].adress_one_free_block = next_address ) == NO_ADRESS ){

						Sblk[nsfd].adress_last_free_block = NO_ADRESS;
						Sblk[nsfd].exist_free_block = NOEXIST;

						if( wr_sblk( nsfd ) != Ok )
							return Err;

						Data_size = may_size;

						return real_address;

						}

					if( wr_sblk( nsfd ) != Ok )
						return Err;

					Data_size = may_size;

					return real_address;

					}

				if( lseek( Sfd[nsfd], afto_address ,SEEK_SET ) == -1l ) {
					errno = EFAULT;
					break;
					}

				if( readstruct( Sfd[ nsfd ], FREEBLOCK ) != Ok )
					break;

				Fblk.adress_next_free_block = next_address;

				if( lseek( Sfd[nsfd], afto_address ,SEEK_SET ) == -1l ) {
					errno = EFAULT;
					break;
					}

				if( writestruct( Sfd[ nsfd ], FREEBLOCK ) != Ok )
					break;

				if( next_address == NO_ADRESS )

					Sblk[nsfd].adress_last_free_block = afto_address;

				if( wr_sblk( nsfd ) != Ok )
					return Err;

				Data_size = may_size;

				return real_address;

				}

			afto_address = real_address;
			real_address = next_address;
			next_address = NO_ADRESS;
			key_one_free_block = 1;

			}

		Sblk[nsfd].adress_end_of_data += Data_size;

		if( wr_sblk( nsfd ) != Ok )
			return Err;

		return (Sblk[nsfd].adress_end_of_data - Data_size );
		}

	}

int
reallocbsf( int nsfd, int ndscr, long badr, long size ){
	uchar	begin_buf[Sizefblk];

	assert_sfd( nsfd );

	if( dseek( nsfd, ndscr ) == Err )
		return Err;

	if( readstruct( Sfd[ nsfd ], DESCRIPTION ) == Err )
		return Err;


	assert_dscr( ndscr );

/*	warnmsg( "Assert Ok, adress = %ld", Dscr.adress_data );		*/

	if( badr > Dscr.data_size || badr < 0L )
		return Err;

	if( size < Dscr.data_size && badr == 0L ){

		if( Dscr.real_space > size )
			Dscr.real_space = size;


		if( dseek( nsfd, ndscr ) == Err )
			return Err;

		if( writestruct( Sfd[ nsfd ], DESCRIPTION ) == Err )
			return Err;

		return ndscr;
		}

	if( size < Dscr.data_size && badr > 0L ){
		long	ssize;

		if( size < Dscr.real_space )
			ssize = size;
		else
			ssize = Dscr.real_space - badr ;


		if( sfcopyblock( nsfd, ( Dscr.adress_data + badr ), Dscr.adress_data, ssize ) == Err )
			return Err;

		Dscr.real_space = ssize;

		if( dseek( nsfd, ndscr ) == Err )
			return Err;

		if( writestruct( Sfd[ nsfd ], DESCRIPTION ) == Err )
			return Err;

		return ndscr;

		}

	if( size > Dscr.data_size ){
		int             size_begin_buf;
		int		bdscr;
		struct  Descr   Bdscr;

		Bdscr.status      = Dscr.status;
		Bdscr.dir         = Dscr.dir;
		Bdscr.num_dname   = Dscr.num_dname;
		Bdscr.flags       = Dscr.flags;
		Bdscr.adress_data = Dscr.adress_data;
		Bdscr.data_size   = Dscr.data_size;
		Bdscr.real_space  = Dscr.real_space;



		if( badr < Sizefblk && Dscr.real_space != 0 && badr < Dscr.real_space ){
/*	warnmsg(" read begin block " );			*/
			if( (size_begin_buf = sfread( nsfd, ndscr, Sizefblk - badr, badr, begin_buf )) == Err )
				return Err;
			}
		else{
			size_begin_buf = 0;
			}
/*	warnmsg(" freeblock " );		*/
		if( freebsf( nsfd, ndscr ) == Err )
			return Err;

/*	warnmsg(" freeblock Ok " );	*/
/*	warnmsg(" alloc  " );		*/
		if( ( bdscr = sfalloc( nsfd, size ) ) == Err )
			return Err;
/*	warnmsg(" alloc Ok  " );	*/

		if( dseek( nsfd, bdscr ) == Err )
			return Err;

		if( readstruct( Sfd[ nsfd ], DESCRIPTION ) == Err )
			return Err;

		if( size_begin_buf > 0 ){
			if( lseek( Sfd[nsfd], Dscr.adress_data, SEEK_SET ) ==-1L )
				return Err;

			if( write( Sfd[nsfd], begin_buf, size_begin_buf ) == -1 )
				return Err;

			}

/*		warnmsg( " Copy begin buf Ok, sizebuf = %d , badr = %ld, adress = %ld", size_begin_buf , badr, Bdscr.adress_data );		*/
		if( sfcopyblock( nsfd, ( Bdscr.adress_data + badr + size_begin_buf ), Dscr.adress_data + size_begin_buf ,  Bdscr.real_space - size_begin_buf ) == Err )
			return Err;

/*		warnmsg( " Copy data Ok " );		*/

		Bdscr.adress_data = Dscr.adress_data;
		Bdscr.data_size = Dscr.data_size;

		if( ndscr != bdscr ){
			if( freedscr( nsfd, bdscr ) == Err )
				return Err;

			}

		Dscr.status      = Bdscr.status;
		Dscr.dir         = Bdscr.dir;
		Dscr.num_dname   = Bdscr.num_dname;
		Dscr.flags       = Bdscr.flags;
		Dscr.adress_data = Bdscr.adress_data;
		Dscr.data_size   = Bdscr.data_size;
		Dscr.real_space  = Bdscr.real_space - badr;

		if( Dscr.real_space < 0L )
			Dscr.real_space = 0;

		if( dseek( nsfd, ndscr ) == Err )
			return Err;

		if( writestruct( Sfd[ nsfd ], DESCRIPTION ) == Err )
			return Err;

		return ndscr;

		}

	return ndscr;
	}



static	int
sfcopyblock( int nsfd, long adr1, long adr2, long size ){

	int	skey;
	uint	ssize;
	char	*bff;

	if( size <= 0L )
		return Ok;

/*	warnmsg( " copy block size - %ld ", size );	*/

	if( size > ( 31U * 1024U - 1 ) )
		ssize = ( 32U * 1024U - 1 );
	else
		ssize =(uint)size;


	skey = 1;
	while( (bff = malloc( ssize ) ) == NULL ){
		skey++;
		ssize = ssize / skey;

		if( ssize < 1 ){
			skey = -1;
			break;
			}
		}

/*	warnmsg(" Alloc mem Ok, size = %d", ssize );		*/

	if( skey > 0 ){
		long	summ = 0;
		uint	reads = 0;


		while( summ < size ){

			if( lseek( Sfd[nsfd], adr1, SEEK_SET ) == -1L )
				return Err;

			if( (reads = read( Sfd[nsfd], bff, ssize )) ==(uint)-1 )
				return Err;


			if( reads == 0 && summ < size )
				return Err;

			if( reads == 0 )
				return Ok;

			adr1 += reads;
			summ += reads;

			if( summ > size )
				reads -=(uint)( summ - size );


			if( lseek( Sfd[nsfd], adr2, SEEK_SET ) == -1L )
				return Err;

			if( write( Sfd[nsfd], bff, reads  ) != reads )
				return Err;

			adr2 += reads ;

			}

		free( bff );
		return Ok;

		}

	else{
		errno = EINVMEM;
		free( bff );
		return Err;
		}
	}



int
freebsf( int nsfd, int ndscr ){

	assert_sfd( nsfd );


	if( dseek( nsfd, ndscr ) == Err )
		return Err;

	if( readstruct( Sfd[ nsfd ], DESCRIPTION ) == Err )
		return Err;

	assert_dscr( ndscr );

	if( freespace( nsfd, Dscr.adress_data, Dscr.data_size ) == Err )
		return Err;

	if( freedscr( nsfd, ndscr ) == Err )
		return Err;

	if( rubeg( nsfd ) == Err )
		return Err;

	return Ok;

	}

static	int
freespace( int nsfd, long address_data, long data_size ){


	if( rd_sblk( nsfd ) != Ok )
                return Err;

	if( Sblk[nsfd].exist_free_block == NOEXIST ){

		if( Sblk[nsfd].adress_end_of_data == address_data + data_size ){
			Sblk[nsfd].adress_end_of_data -= data_size;

/*			if( chsize( Sfd[ nsfd ], Sblk[nsfd].adress_end_of_data ) == -1 )
				return Err;
*/
			if( wr_sblk( nsfd ) != Ok )
				return Err;
			else
				return Ok;
			}
		else {

			Sblk[nsfd].adress_one_free_block = address_data;
			Sblk[nsfd].adress_last_free_block = address_data;
			Sblk[nsfd].exist_free_block = EXIST;

			if( wr_sblk( nsfd ) != Ok )
				return Err;

			lseek( Sfd[ nsfd ], address_data, SEEK_SET );

			Fblk.size = data_size;
			Fblk.adress_next_free_block = NO_ADRESS;

			if( writestruct( Sfd[ nsfd ], FREEBLOCK ) != Ok )
				return Err;

			return Ok;
			}
		}
	else{

		if( Sblk[nsfd].adress_last_free_block < address_data ){

			lseek( Sfd[ nsfd ] , Sblk[nsfd].adress_last_free_block, SEEK_SET);

			if( readstruct( Sfd[ nsfd ], FREEBLOCK ) != Ok )
				return Err;

			Fblk.adress_next_free_block = address_data;

			lseek( Sfd[ nsfd ] , Sblk[nsfd].adress_last_free_block, SEEK_SET);

			if( writestruct( Sfd[ nsfd ], FREEBLOCK ) != Ok )
				return Err;

			Fblk.adress_next_free_block = NO_ADRESS;
			Fblk.size = data_size;

			lseek( Sfd[ nsfd ] , address_data, SEEK_SET);

			if( writestruct( Sfd[ nsfd ], FREEBLOCK ) != Ok )
				return Err;

			Sblk[nsfd].adress_last_free_block = address_data;

			if( wr_sblk( nsfd ) != Ok )
				return Err;

			return Ok;
			}

		if( Sblk[nsfd].adress_one_free_block > address_data ){

			Fblk.adress_next_free_block = Sblk[nsfd].adress_one_free_block;
			Fblk.size = data_size;

			lseek( Sfd[ nsfd ] , address_data, SEEK_SET);

			if( writestruct( Sfd[ nsfd ], FREEBLOCK ) != Ok )
				return Err;

			Sblk[nsfd].adress_one_free_block = address_data;

			if( wr_sblk( nsfd ) != Ok )
				return Err;

			return Ok;
			}

		if( address_data > Sblk[nsfd].adress_one_free_block && address_data < Sblk[nsfd].adress_last_free_block ){

			long	my_address = 0;
			long	next_address = 0;

			my_address = Sblk[nsfd].adress_one_free_block;

			while( 1 ){

				lseek( Sfd[ nsfd ], my_address, SEEK_SET );

				if( readstruct( Sfd[ nsfd ], FREEBLOCK ) != Ok )
					return Err;

				next_address = Fblk.adress_next_free_block;

				if( Fblk.adress_next_free_block > address_data ){


					Fblk.adress_next_free_block = address_data;

					lseek( Sfd[ nsfd ], my_address, SEEK_SET );
					if( writestruct( Sfd[ nsfd ], FREEBLOCK ) != Ok )
						return Err;

					Fblk.adress_next_free_block = next_address;
					Fblk.size = data_size;

					lseek( Sfd[ nsfd ], address_data, SEEK_SET );
					if( writestruct( Sfd[ nsfd ], FREEBLOCK ) != Ok )
						return Err;
					return Ok;
					}

				my_address = next_address;

				if( my_address > address_data )
					return Err;
				}
			}

		return Err;

		}
	}

static 	int
rubeg( int nsfd ){

	long			may_address;
	struct	Freeblock	bfblk;

	assert_sfd( nsfd );


	if( rd_sblk( nsfd ) != Ok )
                return Err;


	if( Sblk[nsfd].exist_free_block == NOEXIST )
		return Ok;

	if( Sblk[nsfd].adress_one_free_block == Sblk[nsfd].adress_last_free_block )
		return Ok;


	for( may_address = Sblk[nsfd].adress_one_free_block; may_address != NO_ADRESS;  ){

		if( lseek( Sfd[ nsfd ], may_address, SEEK_SET ) == -1L){
			return Err;
			}

		if( readstruct( Sfd[ nsfd ], FREEBLOCK ) != Ok )
			return Err;


		if( Fblk.adress_next_free_block == may_address + Fblk.size ){

			bfblk.adress_next_free_block = Fblk.adress_next_free_block;
			bfblk.size = Fblk.size;

			if( lseek( Sfd[ nsfd ], may_address + Fblk.size, SEEK_SET ) == -1L ){
				return Err;
				}

			if( readstruct( Sfd[ nsfd ], FREEBLOCK ) != Ok )
				return Err;


			Fblk.size += bfblk.size;

			if( lseek( Sfd[ nsfd ], may_address , SEEK_SET ) == -1L ){
				return Err;
				}

			if( writestruct( Sfd[ nsfd ], FREEBLOCK ) != Ok )
				return Err;

			if( Fblk.adress_next_free_block == NO_ADRESS ) {
				Sblk[nsfd].adress_last_free_block = may_address;
				if( wr_sblk( nsfd ) != Ok )
					return Err;
				return Ok;
				}


			continue;
			}

		may_address = Fblk.adress_next_free_block;

		}

	return Ok;

	}

bool
sfflush( int nsfd ){

	int	newfd;

	assert_sfd( nsfd );

	if( ( newfd = dup( Sfd[ nsfd ] ) ) < 0 )
		return Err;

	if( close(  newfd  ) < 0 )
		return Err;

	return Ok;
	}


int
sfload( int nsfd, int ndscr, char *fname, long baddrin, long baddrout, long size ){

	int	fp;
	long	pozin;

	assert_sfd( nsfd );

	if( ( fp = open( fname, O_RDWR|O_BINARY, 0666 ) ) == -1 ) {
                errno = EFAULT;
                return Err;
                }


	if( size  == ALL ){
		size = ( filelength( fp ) - baddrin );

		if( size <= 0 ){
			errno = EINVAL;
			close( fp );
			return Err;
			}
		}
	else{

		if( ( pozin = lseek( fp, 0L, SEEK_END ) ) == -1L ){
			close( fp );
			return Err;
			}

		if( baddrin + size > pozin || baddrin > pozin || baddrin < 0L ){
			close( fp );
			errno = EINVAL;
			return Err;
			}
		}


	if( dseek( nsfd, ndscr ) == Err ){
		close( fp );
		return Err;
		}

	if( readstruct( Sfd[ nsfd ], DESCRIPTION ) == Err ){
		close( fp );
		return Err;
		}

	assert_dscr( ndscr );


	if( baddrout < 0L || baddrout > Dscr.data_size ){
		close( fp );
		errno = EINVAL;
		return Err;
		}

	if( Dscr.data_size < baddrout + size ){

		if( ( ndscr = reallocbsf( nsfd, ndscr, 0L, baddrout + size + REZERVSIZE ) ) == Err ){
			close( fp );
			return Err;
			}

		if( dseek( nsfd, ndscr ) == Err ){
			close( fp );
			return Err;
			}

		if( readstruct( Sfd[ nsfd ], DESCRIPTION ) == Err ){
			close( fp );
			return Err;
			}

		}



	if( copyfilesfile( fp, nsfd, baddrin, Dscr.adress_data + baddrout, size ) == Err ){
		close( fp );
		return Err;
		}

	if( Dscr.real_space < baddrout + size ){

		Dscr.real_space = baddrout + size;

		if( dseek( nsfd, ndscr ) == Err ){
			close( fp );
			return Err;
			}

		if( writestruct( Sfd[ nsfd ], DESCRIPTION ) == Err ){
			close( fp );
			return Err;
			}
		}

	return ndscr;
	}

static	int
copyfilesfile( int fp, int nsfd, long addrin, long addrout, long size ){

	int	skey;
	long	ssize;
	char	*bff;

	ssize = size;
	skey = 1;

	while( (bff = malloc( (unsigned)( ssize ) ) ) == NULL ){
		skey++;
		ssize = ssize / skey;

		if( ssize < 1 ){
			skey = -1;
			break;
			}
		}


	if( skey > 0 ){
		int	k = 0;
		int	reads;
		long	sum;

		k = 0;
		reads = 0;
		sum = 0;

		if( skey * ssize < size )
			skey++;

		lseek( fp, addrin, SEEK_SET );

		lseek( Sfd[nsfd], addrout, SEEK_SET );


		while( k < skey ){

			reads = read( fp, bff, (unsigned)(int)( ssize ) );

			sum += reads;

			if( sum > size )
				reads -= (int)(sum - size);

			write( Sfd[nsfd], bff, reads );

			k++;
			}
		free( bff );
		return Ok;

		}
	else{
		free( bff );
		errno = EINVMEM;
		return Err;
		}
	}

int
sfsetspace( int nsfd, int ndscr, long new_space ){

	assert_sfd( nsfd );

	if( new_space < 0L ){
		errno = EINVAL;
		return Err;
		}

	if( dseek( nsfd, ndscr ) == Err ){
		return Err;
		}

	if( readstruct( Sfd[ nsfd ], DESCRIPTION ) == Err ){
		return Err;
		}

	assert_dscr( ndscr );

	if( new_space > Dscr.adress_data + Dscr.data_size ){
		errno = EINVAL;
		return Err;
		}

	Dscr.real_space = new_space;

	if( dseek( nsfd, ndscr ) == Err ){
		return Err;
		}

	if( writestruct( Sfd[ nsfd ], DESCRIPTION ) == Err ){
		return Err;
		}

	return Ok;

	}







