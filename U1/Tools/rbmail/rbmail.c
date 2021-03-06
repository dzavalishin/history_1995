/*
 * Receive batched mail - rewritten by Dmitry Zavalishin
 * Original version was made by avg
 *
 *    $Log: rbmail.c $
 *    Revision 1.10  1996/12/06 04:59:18  dz
 *    closing stdin after load and
 *    reopening as /dev/nul
 *
 *    Revision 1.9  1996/12/06 04:27:24  dz
 *    before changing again
 *
 *    Revision 1.8  1996/11/12 06:33:43  dz
 *    Removed infinite waits to
 *    overcome stalls because of never ending child :(
 *
 *    Revision 1.7  1996/09/22 04:14:12  dz
 *    rewritten version
 *
 *    Revision 1.5  1995/09/26 08:22:27  dz
 *    Waiting for all childs to end before exiting to make sure
 *    we're not preventing uuxqt from killing spool file.
 *
 *    Revision 1.4  1995/07/03  10:40:35  dz
 *    messages suppressed, batch error tolerance improved
 *
 *    Revision 1.2  1995/03/06  13:40:49  dz
 *    - don't die if wrong command line given, just process batch
 *    - don't flood system with a lots of rmails
 *
 *
 */

#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <dir.h>
#include <direct.h>
#include <time.h>
#include <process.h>
#include <ctype.h>


#define INCL_DOSPROCESS
#define INCL_DOSERRORS
#define INCL_DOSQUEUES
#define INCL_DOSFILEMGR
#include <OS2.H>

const max_header = 4096;

int	mail( char *args[] );
//void	dump_off( const char *err_msg, const char *hdr, const char* data, int len );
//void	corrupt_hdr( const char *details, const char *hdr );
//int	send_it( const char* data, int len, char *args[] );
void    send_it( const char* data, int len, char *args[] );



typedef const char *	cchar_ptr;
typedef cchar_ptr &	cchar_ptr_ref;





void do_one_piece( char *rargs[], const char* text, int len, int rcsum )
    {
    unsigned short csum = 0;
    for( const char *cp = text; cp-text < len; cp++ )
        {
        if( csum & 01 )          csum = (csum>>1) + 0x8000;
        else                     csum >>= 1;
        csum += *cp;
        csum &= 0xFFFF;
        }

    if( rcsum != csum )
        {
        printf("Our csum %x, stored %x!!\n", csum, rcsum );
        //throw( "Checksum error" );
        }

    send_it( text, len, rargs );
    }





//
// Spawn rmail, return descriptor of our end of it's pipe
//

int mail( char *args[] )
	{
        int rc;
	unsigned long rd, wr, temp;
	int stdin_save = dup( 0 );
        //close( 0 );

	if( DosCreatePipe( &rd, &wr, 4096 ) != 0 )  throw( "Can't create pipe" );
	//if( 0 != dup2( rd, 0 ) )                     throw( "Can't dup2" );
        temp = 0;
        rc = DosDupHandle( rd, &temp );
        if( 0 != rc )
            {
            DosClose( 0 );
            temp = 0xFFFFFFFF;
            rc = DosDupHandle( rd, &temp );
            if( 0 != rc )
                {
                printf("DosDupHandle rc = %d\n", rc );
                char buf[80];
                sprintf(buf,"DosDupHandle rc = %d", rc );
                throw( buf );
                }
            if( 0 != temp )
                {
                printf("DosDupHandle created nonzero handle = %d\n", temp );
                throw( "DosDupHandle created nonzero handle" );
                }
            }

	close( rd );
        DosSetFHState( wr, OPEN_FLAGS_NOINHERIT );

        if( spawnvp( P_NOWAIT, "rmail", args ) < 0 ) throw( "Can't run rmail" );

	if( 0 != dup2( stdin_save, 0 ) )             throw( "Can't dup2 back" );
	close( stdin_save );

	return wr;
	}


void dump_off( const char *err_msg, int hpos, const char* data, int len )
	{
	char msg[1000];

	char *dir;

//	if( NULL == (dir = getenv("RBMAIL")) )
		if( NULL == (dir = getenv("TMP")) )
			if( NULL == (dir = getenv("TEMP")) )
				dir = "\\";

// c set!
#define chdir _chdir
#define mkdir _mkdir

	printf("RBmail dump_off switches to dir %s\n", dir );

	chdir( dir );
	mkdir( "Errors" );
	char subd[300];
        sprintf(subd,"%s/errors", dir );
	printf("RBmail dump_off switches to dir %s\n", subd );
	chdir( subd );

//	char *tname = tempnam( dir, "Errors/Rbm" );
//	mktemp(tname);

	char tname[L_tmpnam+2] = "Err.tmp";
	tmpnam(tname);

	printf("RBmail dump_off file: %s\n", tname );
	FILE *ef = fopen( tname, "wb" );

	if( ef == NULL )
		{
		fprintf( stderr, "Can't save broken batch!\n" );
		exit(44);
		}

	fprintf( ef, "Error: %s at %d\n", err_msg, hpos );
	fwrite( data, 1, len, ef );

	int ee = ferror( ef );		// Error writing ef?

	fclose( ef );

	//rename(tname,subd);

	char *margs[3];

	margs[0] = "rmail";
	margs[1] = "postmaster";
	margs[2] = NULL;

	int pipe_d = mail( margs );	// Spawn rmail, return pipe to it
	FILE *mf = fdopen( pipe_d, "wb" );

	if( mf == NULL )
		throw( "Can't mail to postmaster!" );

	long tt = time(NULL);

	fprintf( mf, "From phantom!uucp %s", ctime(&tt));
	fprintf( mf, "From: uucp (UUCP Batch Mail Service)\n");
	fprintf( mf, "To: postmaster\n");
	fprintf( mf, "Subject: batch receiption failed\n\n");
	fprintf( mf, "ERROR: %s.\nBatch saved in %s\n\n", err_msg, tname );

	if( ee )
		fprintf( mf, "There was error writing %s\n\n", tname );

	fclose( mf );
	DosClose( pipe_d );

	exit( 33 );
	}





void
be_idle_a_bit( void )
	{
	static int	turn = 0;

// ???? ??????:
//   ?? ?????? ??? ????????? ???????? ?p????? ????????.
//   ??? ???????? ?? ???? idle priority, ????? ?p???p?????? ????,
//   ???? ???p???? ?????? ??????, ? ?? ????????? ???? ??? ?????.

	//fprintf( stderr, "rbmail: Sleeping\r");
	//DosSleep( 500 );
	//fprintf( stderr, "rbmail: Idle    \r");
	DosSetPriority (PRTYS_THREAD, PRTYC_IDLETIME, 15L, 0L );
	DosSleep( 32 );
	//DosSleep( 100 );
	//DosSleep( 100 );
	//DosSleep( 100 );
	DosSetPriority (PRTYS_THREAD, PRTYC_REGULAR, 0L, 0L );
	//fprintf( stderr, "                                        \r");



	if( ++turn > 9 )
		{
		PID		ch_pid;
		RESULTCODES	ch_rc;
//
// ?? ??? p????? ??????? ?p??????? - ???????? ?????!
//

                fprintf( stderr, "rbmail: waiting for some child to end\r");
                fflush( stderr );
                int sleep_times = 30;
                while(1)
                    {
                    APIRET rc = DosWaitChild( DCWA_PROCESSTREE, DCWW_NOWAIT, &ch_rc, &ch_pid, 0 );
                    if( rc == ERROR_CHILD_NOT_COMPLETE )
                        {
                        if(sleep_times--)
                            DosSleep(1000);
                        else
                            {
                            fprintf( stderr, "\nError! Some child still active. Moving on.\n" );
                            break;
                            }
                        }
                    else
                        break;
                    }
		fprintf( stderr, "rbmail: some child finished          \n");

		turn--;
		}
	}



void send_it( const char* data, int len, char *args[] )
	{
	int pipe_d = mail( args );	// Spawn rmail, return pipe to it
	ULONG actual;

	DosWrite( pipe_d, (void *)data, len, &actual );

	DosClose( pipe_d );

	be_idle_a_bit();

	if( actual != len ) throw( "Write error");
	}





char *get_one_arg( cchar_ptr_ref line /*, char *reason */ )
    {
    char *out;
    const char *begin;
    
      // printf( "Get one Arg got '%s'\n", line );
    
    begin = line;
    while( *line && *line != '\n' && *line != '\r' && !isspace(*line) )
        line ++;
    
    int len = line - begin;
    
    out = (char *) malloc( len + 1 );
    
    if( out == NULL )
        throw( "Out of memory" );
    
    strncpy( out, begin, len );
    out[len] = '\0';
    
    return out;
    }


const maxargs = 100;

void load_args( char *rargs[], const char *line /*, char *fail_reason */ )
    {
    int arg = 1;
    rargs[0] = "rmail"; // say, rmail. Don't care much...
    
    while(1)
        {
        
        while( isspace( *line ) && *line != '\r' && *line != '\n' )
            line++;
        
        if( *line == '\0' || *line == '\r' || *line == '\n' )
            break;
        
        rargs[arg] = get_one_arg( line ); //, fail_reason );
        
        arg++;
        
        if( arg >= maxargs )
            throw( "Too many args" );
        }
    
    rargs[arg] = NULL;
    }


int main( int ac, char *av[] )
    {
      //char headerline[max_header];
    int exit_code = 0;
    
    if( ac != 1 )
        {
        fprintf(stderr, "Wrong arg: %s\n\n", av[1] );
        fprintf(stderr, "Usage: rbmail < batchfile\n");
        
          // never give up! ???? ??? p????? ????????? - p????? ? ?p??????,
          // ? ???? uuxqt ???p? ??? ??? ????? ?????? ?????? - ????? ?? ?????? ?? ????
          // ? ?? ????? ??p??? ????? ?? ??p????? ???????.
          //		exit(64);       /* EX_USAGE */
        }

    _setmode( 0, O_BINARY );
    
    int   ssize = 0;
    char *store = 0;
    int   readsize = 100*1024;
    int   ndone = 0;
    int   nread;
    
    do
        {
        ssize += readsize;
        char *more = new char[ssize+1]; // extra char for \0
        
        if( store )
            {
            memcpy( more, store, ndone );
            delete [] store;
            }
        store = more;

        nread = read( 0, store+ndone, readsize );

        if( nread <= 0 ) break;
        ndone += nread;

        }
    while( nread >= readsize );

    printf("done reading %d bytes\n", ndone );
    store[ndone] = 0;
    
    char *cp = store;
    char *diagheader = store;

    close(0); // we don't need it any more

    if( open( "/dev/nul", O_RDONLY ) != 0 )
        {
        printf("can't open /dev/nul as stdin: %d\n", errno );
        dump_off( "can't open /dev/nul as stdin", diagheader-store, store, ndone );
        return 33; // exit
        }
    
    try
        {
        while( *cp )
            {
            char *hline = cp;
            diagheader = cp;

            while( *cp != '\n' )
                if( *cp++ == 0 )
                    throw( "missing newline" );
            cp++;

            int len, csum, pos;
            if( 2 != sscanf( hline, "%d %x%n", &len, &csum, &pos ) )
                throw( "no len or checksum" );

            hline += pos;

            char *rargs[maxargs+2];
            char reason[200];
            load_args( rargs, hline ); //, reason );
        
            do_one_piece( rargs, cp, len, csum );
            cp += len;

            for( char **ap = &rargs[1]; *ap; ap++ )
                free( *ap );
            }
        }
    catch( char *error )
        {
        printf("exception: %s\n", error );
        dump_off( error, diagheader-store, store, ndone );
        exit_code = 33;
        }
    
      // wait for all children to die
    PID		ch_pid;
    RESULTCODES	ch_rc;
    fprintf( stderr, "rbmail: waiting for all childern to end...");
    fflush( stderr );
    int maxwait = 180*10; // 180 tenths of second
    while( 0 == DosWaitChild( DCWA_PROCESSTREE, DCWW_NOWAIT, &ch_rc, &ch_pid, 0 ) )
        {
        if( maxwait-- )
            {
            DosSleep(100); // .1 sec
            if( !(maxwait % 10) )
                fprintf( stderr, "%4d\b\b\b\b", maxwait );
            }
        else
            {
            fprintf( stderr, " (some still alive, but) ");
            break;
            }
        }
    fprintf( stderr, " done.\n");

    return exit_code;
    }
