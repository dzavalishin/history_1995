head	1.10;
access;
symbols;
locks;
comment	@ * @;


1.10
date	96.12.06.04.59.18;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.12.06.04.27.24;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.11.12.06.33.43;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.09.22.04.14.12;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.09.21.16.12.16;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	95.09.26.08.22.27;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.07.03.10.40.35;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.03.10.03.37.48;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.03.06.13.40.49;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	94.05.30.20.19.02;	author root;	state Exp;
branches;
next	;


desc
@starts rmail ok, but gives bad args
@


1.10
log
@closing stdin after load and
reopening as /dev/nul
@
text
@/*
 * Receive batched mail - rewritten by Dmitry Zavalishin
 * Original version was made by avg
 *
 *    $Log: rbmail.c $
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
        printf("Our csum %x, stored %x\n", csum, rcsum );
        throw( "Checksum error" );
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

// Идея такова:
//   мы только что отпустили сыновний пpоцесс погулять.
//   так поставим же себе idle priority, чтобы пpитоpмозить себя,
//   если загpузка машины велика, и не запускать пока еще детей.

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
// Мы уже pодили десяток пpоцессов - подождем чуток!
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
        
          // never give up! Если нас pуками запустили - pуками и пpишибут,
          // а если uuxqt сдуpу дал нам какой ключик лишний - лучше мы плюнем на него
          // и не будем теpять почту по дуpацким поводам.
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
@


1.9
log
@before changing again
@
text
@d6 3
a15 3
 *    Revision 1.6  1996/09/21 16:12:16  dz
 *    last before rewrite
 *
a22 3
 *    Revision 1.3  1995/03/10  03:37:48  dz
 *    cleanup
 *
a388 2
    //close(0); // we don't need it any more
    
d395 9
@


1.8
log
@Removed infinite waits to
overcome stalls because of never ending child :(
@
text
@d6 4
d255 1
a255 1
                    APIRET rc = DosWaitChild( DCWA_PROCESSTREE, DCWW_WAIT, &ch_rc, &ch_pid, 0 );
d442 1
a442 1
    while( 0 == DosWaitChild( DCWA_PROCESSTREE, DCWW_WAIT, &ch_rc, &ch_pid, 0 ) )
d445 1
d447 3
@


1.7
log
@rewritten version
@
text
@d6 3
d78 1
a78 1
        //throw( "Checksum error" );
d94 2
a95 1
	unsigned long rd, wr;
d97 1
d99 22
a120 2
	if ( DosCreatePipe( &rd, &wr, 4096 ) != 0 )  throw( "Can't create pipe" );
	if( 0 != dup2( rd, 0 ) )                     throw( "Can't dup2" );
d225 3
a227 3
	fprintf( stderr, "rbmail: Sleeping\r");
	DosSleep( 500 );
	fprintf( stderr, "rbmail: Idle    \r");
d229 4
a232 4
	DosSleep( 100 );
	DosSleep( 100 );
	DosSleep( 100 );
	DosSleep( 100 );
d234 1
a234 1
	fprintf( stderr, "                                        \r");
d246 19
a264 2
		fprintf( stderr, "rbmail: waiting for some child to end\r");
		DosWaitChild( DCWA_PROCESSTREE, DCWW_WAIT, &ch_rc, &ch_pid, 0 );
d387 2
d435 3
d439 11
a449 2
        ;
    
@


1.6
log
@last before rewrite
@
text
@d2 1
a2 1
 * Receive batched mail - Rewritten for OS/2 by Dmitry Zavalishin
d6 3
d13 2
a14 2
 * Revision 1.4  1995/07/03  10:40:35  dz
 * messages suppressed, batch error tolerance improved
d16 2
a17 2
 * Revision 1.3  1995/03/10  03:37:48  dz
 * cleanup
d19 3
a21 3
 * Revision 1.2  1995/03/06  13:40:49  dz
 * - don't die if wrong command line given, just process batch
 * - don't flood system with a lots of rmails
d27 1
d47 4
a50 3
void	dump_off( const char *err_msg, const char *hdr, const char* data, int len );
void	corrupt_hdr( const char *details, const char *hdr );
int	send_it( const char* data, int len, char *args[] );
a57 41
char *
get_one_arg( cchar_ptr_ref line, char *reason )
	{
	char *out;
	const char *begin;

// printf( "Get one Arg got '%s'\n", line );

	begin = line;
	while( *line && !isspace(*line) ) line ++;

	int len = line - begin;

	out = (char *) malloc( len + 1 );

	if( out == NULL )
		{
		strcpy( reason, "Out of memory" );
		return NULL;
		}

	strncpy( out, begin, len );
	out[len] = '\0';

	return out;
	}


int
load_args( char *rargs[], const char *line, char *fail_reason )
	{
	int arg = 1;

	rargs[0] = "rmail"; // say, rmail. Don't care much...

// printf( "LoadArgs got '%s'\n", line );

	while(1)
		{

		while( isspace( *line ) ) line++;
a58 2
		if( *line == '\0' )
			break;
a59 12
		rargs[arg] = get_one_arg( line, fail_reason );
		if( rargs[arg] == NULL )
			return -1;

		arg++;

		if( arg >= 98 )
			{
			strcpy( fail_reason, "Too many args" );
			return -1;
			}
		}
d61 16
a76 1
	rargs[arg] = NULL;
d78 2
a79 8
/*
for( int i = 0; i < 99; i++ )
  if( rargs[i] == NULL ) break;
  else printf("rarg[%d] = '%s', ", i, rargs[i] );
printf("\n");
*/
	return 0;
	}
a82 151
void
do_one_piece( const char *headerline )
	{
	char diagheader[max_header];
	char *rargs[100];
	const char *p;
	unsigned rcsum, csum;
	long     len;

	//	Parse header line
	rargs[0] = NULL;
	strcpy(diagheader, headerline);

	/* Length */
	p = headerline;
	len = 0;
	for( int i = 0 ; i < 7 ; i++ )
		{
		unsigned char c = *p++;

		if( c==' ' || c=='\t' )
			{
			p--;
			break;
			}

		if( '0' > c || c > '9' )
			corrupt_hdr( "bad length", diagheader );
		len = (len*10) + (c-'0');
		}

	if( *p++ != ' ' )
		corrupt_hdr( "no space after length", diagheader );

	/* Checksum */
	rcsum = 0;
	for( i = 0 ; i < 4 ; i++ )
		{
		unsigned char c = *p++;
		rcsum <<= 4;
		if( '0' <= c && c <= '9' )
			rcsum |= c-'0';
		else if( 'A' <= c && c <= 'F' )
			rcsum |= c-'A'+0xa;
		else if( 'a' <= c && c <= 'f' )
			rcsum |= c-'a'+0xa;
		else
			corrupt_hdr( "bad checksum field", diagheader );
		}

	if( *p != ' ' )
		corrupt_hdr( "no space after checksum", diagheader );

	/* Destination addresses */
	if( ! *p )
		corrupt_hdr( "no dest address", diagheader );


	char reason[200];
	if( load_args( rargs, p, reason ) != 0 )
		corrupt_hdr( reason, diagheader );

/*
for( int j = 0; j < 99; j++ )
  if( rargs[j] == NULL ) break;
  else printf("rarg[%d] = '%s', ", j, rargs[j] );
printf("\n");
*/

	// Read all the stuff in

	char *Letter = (char *) malloc( len+1 );

	if( Letter == NULL )
		corrupt_hdr("Out of memory", diagheader );

	int real_len = 0;
	if( (real_len = fread( Letter, 1, len, stdin )) != len )
		{
		char buf[100];
		sprintf(buf, "Can't read data - got %d instead of %d", real_len, len );
		dump_off( buf, diagheader, Letter, real_len );
		}

	Letter[len] = '\0';

	// Calc. checksum

	csum = 0;
	for( char *cp = Letter; *cp; cp++ )
		{
		if( csum & 01 )
			csum = (csum>>1) + 0x8000;
		else
			csum >>= 1;
		csum += *cp;
		csum &= 0xFFFF;
		}

//	if( rcsum != csum )
//		dump_off( "Checksum error", diagheader, Letter, real_len );

	if( send_it( Letter, real_len, rargs ) != 0 )
		dump_off( "Can't send", diagheader, Letter, real_len );

	for( char **ap = &rargs[1]; *ap; ap++ )
		free( *ap );

	free( Letter );
	}


int
main( int ac, char *av[] )
	{
	char headerline[max_header];

	if( ac != 1 )
		{
		fprintf(stderr, "Wrong arg: %s\n\n", av[1] );
		fprintf(stderr, "Usage: rbmail < batchfile\n");

// never give up! Если нас pуками запустили - pуками и пpишибут,
// а если uuxqt сдуpу дал нам какой ключик лишний - лучше мы плюнем на него
// и не будем теpять почту по дуpацким поводам.
//		exit(64);       /* EX_USAGE */
		}

	while( fgets(headerline, (sizeof headerline)-1, stdin) != NULL )
		{
		headerline[(sizeof headerline)-1] = '\0';

		char *p = headerline;
		while( *p != '\n' && *p ) p++;
		if( ! *p )
			corrupt_hdr( "missing newline", headerline );
		*p = '\0';

		do_one_piece( headerline );
		}


	// wait for all children to die
	PID		ch_pid;
	RESULTCODES	ch_rc;
	while( 0 == DosWaitChild( DCWA_PROCESSTREE, DCWW_WAIT, &ch_rc, &ch_pid, 0 ) )
		;

	return 0;
	}

d89 1
a89 2
int
mail( char *args[] )
a91 8

/*
for( int i = 0; i < 99; i++ )
  if( args[i] == NULL ) break;
  else printf("arg[%d] = '%s', ", i, args[i] );
printf("\n");
*/

d94 2
a95 5
	if ( DosCreatePipe( &rd, &wr, 4096 ) != 0 )
		return -1;

	if( 0 != dup2( rd, 0 ) )
		return -1;
d100 1
a100 7
// c set
#define P_NOWAITO P_NOWAIT
	if( spawnvp( P_NOWAITO, "rmail", args ) < 0 )
		return -1;

	if( 0 != dup2( stdin_save, 0 ) )
		return -1;
d102 1
d109 1
a109 2
void
dump_off( const char *err_msg, const char *hdr, const char* data, int len )
d148 1
a148 1
	fprintf( ef, "Error: %s\n%s\n", err_msg, hdr );
a150 7
	const bs = 16*1024;
	char buf[bs];
	int i;

	while( (i = fread( buf, 1, bs, stdin )) > 0 )
		fwrite( buf, 1, i, ef );

d155 1
a155 1
	rename(tname,subd);
d166 2
a167 5
	if( pipe_d < 0 || mf == NULL )
		{
		fprintf( stderr, "Can't mail to postmaster!\n" );
		exit(44);
		}
a186 12
void
corrupt_hdr( const char *details, const char *hdr )
	{
	char msg[1000];
	sprintf( msg, "Header line corrupted (%s)", details );

	// dump ther rest of file, send error message

	dump_off( msg, hdr, NULL, 0 );

	exit( 33 );
	}
d231 1
a231 2
int
send_it( const char* data, int len, char *args[] )
d234 1
a234 7
	unsigned long actual;

	if( pipe_d < 0 )
		{
		fprintf( stderr, "Can't mail!\n" );
		return -1;
		}
d242 1
a242 1
	return actual == len ? 0 : -1;
d246 150
@


1.5
log
@Waiting for all childs to end before exiting to make sure
we're not preventing uuxqt from killing spool file.
@
text
@d6 4
d325 3
a327 3
	if( NULL != (dir = getenv("RBMAIL")) )
		if( NULL != (dir = getenv("TMP")) )
			if( NULL != (dir = getenv("TEMP")) )
d333 3
d338 4
a341 1
	chdir( "Errors" );
d343 1
a343 1
//	char *tname = "ErXXXXXX";
d349 1
d372 2
d391 1
a391 1
	fprintf( mf, "From uucp %s", ctime(&tt));
@


1.4
log
@messages suppressed, batch eroor tolerance improved
@
text
@d6 3
d261 7
d420 1
a420 1
	DosSleep( 1000 );
d423 4
a426 4
	DosSleep( 500 );
	DosSleep( 500 );
	DosSleep( 500 );
	DosSleep( 500 );
@


1.3
log
@cleanup
@
text
@d6 3
d20 2
a21 1
#include <dir.h>
d35 1
a35 1
int	mail( char * const args[] );
d38 1
a38 1
int	send_it( const char* data, int len, char * const args[] );
d79 1
a79 1
	rargs[0] = "";
d136 7
d195 5
a199 1
		dump_off( "Can't read data", diagheader, Letter, real_len );
d268 1
a268 1
mail( char * const args[] )
d290 2
d316 3
d323 5
a327 2
	char *tname = "ErXXXXXX";
	mktemp(tname);
d409 1
a409 1
	fprintf( stderr, "rbmail: Sleeping\n");
d411 1
a411 1
	fprintf( stderr, "rbmail: Idle\n");
d418 1
a418 1
	fprintf( stderr, "rbmail: Regular priority\n");
d430 1
a430 1
		fprintf( stderr, "rbmail: waiting for some child to end\n");
d432 1
a432 1
		fprintf( stderr, "rbmail: some child finished\n");
d441 1
a441 1
send_it( const char* data, int len, char * const args[] )
@


1.2
log
@- don't die if wrong command line given, just process batch
- don't flood system with a lots of rmails
@
text
@d5 4
a8 1
 *    $Log:$
d10 1
a162 45
/*
	char **ap = &rargs[1];
	for(;;)
		{
		while( *p == ' ' || *p == '\t' ) p++;
		if( *p == '\0' )
			break;
		if( ap >= &rargs[99] )
			corrupt_hdr( "too many addresses", diagheader );

		*ap++ = p;
		const char *begin = p;
		for(;;)
			{
			if( *p == '"' )
				{
				do
					{
					p++;
					} while( *p && *p != '"' );
				if( *p ) p++;
				continue;
				}
			if( *p == ' ' )
				{
				*p++ = '\0';
				break;
				}
			if( *p == '\0' )
				break;
			p++;
			}

		*ap = (char *) malloc( p - begin + 1 );
		if( *ap == NULL )
			corrupt_hdr( "Out of memory", diagheader );
		strncpy( *ap, begin, p-begin );
		(*ap)[p-begin] = '\0';
		ap++;

		}

	*ap = NULL;
	if( ap == &rargs[1] )
		corrupt_hdr( "no dest address(es)", diagheader );
a163 2
*/

d223 4
@


1.1
log
@Initial revision
@
text
@d4 3
d16 1
d18 3
d27 4
a30 1
int	mail( char * const *args );
a31 4
void
dump_off( const char *err_msg, const char *hdr, const char* data, int len )
	{
	char msg[1000];
a32 1
	char *dir;
d34 2
a35 4
	if( NULL != (dir = getenv("RBMAIL")) )
		if( NULL != (dir = getenv("TMP")) )
			if( NULL != (dir = getenv("TEMP")) )
				dir = "\\";
a36 3
	chdir( dir );
	mkdir( "Errors" );
	chdir( "Errors" );
d38 5
a42 22
	char *tname = "ErXXXXXX";
	mktemp(tname);

	FILE *ef = fopen( tname, "wb" );

	if( ef == NULL )
		{
		fprintf( stderr, "Can't save broken batch!\n" );
		exit(44);
		}

	fprintf( ef, "Error: %s\n%s\n", err_msg, hdr );
	fwrite( data, 1, len, ef );

	const bs = 16*1024;
	char buf[bs];
	int i;

	while( (i = fread( buf, 1, bs, stdin )) > 0 )
		fwrite( buf, 1, i, ef );

	int ee = ferror( ef );		// Error writing ef?
d44 1
a44 1
	fclose( ef );
d46 2
a47 1
	char *margs[3];
d49 1
a49 3
	margs[0] = "rmail";
	margs[1] = "postmaster";
	margs[2] = NULL;
d51 1
a51 2
	int pipe_d = mail( margs );	// Spawn rmail, return pipe to it
	FILE *mf = fdopen( pipe_d, "wb" );
d53 1
a53 1
	if( pipe_d < 0 || mf == NULL )
d55 2
a56 2
		fprintf( stderr, "Can't mail to postmaster!\n" );
		exit(44);
d59 2
a60 1
	long tt = time(NULL);
d62 2
a63 5
	fprintf( mf, "From uucp %s", ctime(&tt));
	fprintf( mf, "From: uucp (UUCP Batch Mail Service)\n");
	fprintf( mf, "To: postmaster\n");
	fprintf( mf, "Subject: batch receiption failed\n\n");
	fprintf( mf, "ERROR: %s.\nBatch saved in %s\n\n", err_msg, tname );
a64 2
	if( ee )
		fprintf( mf, "There was error writing %s\n\n", tname );
d66 4
a69 2
	fclose( mf );
	DosClose( pipe_d );
d71 1
a71 2
	exit( 33 );
	}
d73 1
a73 5
void
corrupt_hdr( const char *details, const char *hdr )
	{
	char msg[1000];
	sprintf( msg, "Header line corrupted (%s)", details );
d75 2
a76 1
	// dump ther rest of file, send error message
d78 1
a78 1
	dump_off( msg, hdr, NULL, 0 );
d80 2
a81 2
	exit( 33 );
	}
d83 3
d87 1
a87 5
int
send_it( const char* data, int len, char * const args[] )
	{
	int pipe_d = mail( args );	// Spawn rmail, return pipe to it
	unsigned long actual;
d89 5
a93 4
	if( pipe_d < 0 )
		{
		fprintf( stderr, "Can't mail!\n" );
		return -1;
d96 1
a96 1
	DosWrite( pipe_d, (void *)data, len, &actual );
d98 7
a104 3
	DosClose( pipe_d );

	return actual == len ? 0 : -1;
a108 1

d114 1
a114 1
	/*const*/ char *p;
d159 1
d184 1
a184 1
//				*p++ = '\0';
d205 13
d244 2
a245 2
	if( rcsum != csum )
		dump_off( "Checksum error", diagheader, Letter, real_len );
d250 1
a250 1
	for( ap = &rargs[1]; *ap; ap++ )
d258 1
a258 1
main( int ac, char **)
d264 1
d266 1
a266 1
		exit(64);       /* EX_USAGE */
d292 1
a292 1
mail( char * const *args )
d296 7
d323 152
@
