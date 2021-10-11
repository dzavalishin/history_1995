head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	95.11.05.13.52.48;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.07.30.13.44.16;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.07.28.15.31.06;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.13.11.06.28;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.04.17;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@current.
@
text
@/************************ UUCP to FIDO convertor ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Log module
 *
 *      $Log: log.c $
 *      Revision 1.4  1995/07/30 13:44:16  dz
 *      Time representation changed to 24h
 *
 *      Revision 1.3  1995/07/28 15:31:06  dz
 *      there was fatal error in fatal() :) - misplaced va_end()
 *      improved diagnostic printout - errno text printed now.
 *
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *         Rev 1.11   07 Jun 1993 17:50:12   dz
 *      char -> uchar
 *      
 *         Rev 1.10   12 Mar 1993 07:33:38   dz
 *      Bink log style fixed
 *      
 *         Rev 1.9   12 Mar 1993 07:06:32   dz
 *      Log styles (Own, Bink, FroDo) introduced.
 *      
 *         Rev 1.8   15 Nov 1992 00:01:52   dz
 *      %-record fixes
 *      
 *         Rev 1.7   23 Oct 1992 15:14:44   dz
 *      error() & fatal() now have flag parm. - include
 *		DOS59 info, or not. Heap correctness check added.
 *      
 *         Rev 1.6   16 Oct 1992 03:37:00   dz
 *      fatal() redefined
 *      elapsed time counter, letters/sec counter
 *      
 *         Rev 1.5   30 Jun 1992 10:34:38   dz
 *      Open in denynone mode to allow nested gate modules execution
 *      Program run time counter added
 *      
 *         Rev 1.4   04 Feb 1992 22:02:32   dz
 *      Using program_name[] to identify program in log
 *      
 *         Rev 1.3   11 Oct 1991 05:03:54   dz
 *      open in binary mode
 *      
 *         Rev 1.2   12 Sep 1991 01:32:40   dz
 *      Open in append mode
 *      Time mark fixed - gmt->local
 *      
 *         Rev 1.1   28 Aug 1991 00:42:52   dz
 *      Open in DenyWrite mode
 *
 *         Rev 1.0   28 Aug 1991 00:37:18   dz
 *      Initial revision.
 *
 *
\*/

#include	"app.h"
#include	"log.h"
#include	"setup.h"
#include	"stat.h"

#include	"DLLInits.h"

#include	<stdio.h>
#include	<uu2io.h>
//#include	<dos.h>
#include	<fcntl.h>
#include	<stdlib.h>
#include	<time.h>
#include	<string.h>
//#include	<malloc.h>
#include	<share.h>
#include	<errno.h>


static void		track_add( char *str );

//const char     *program_name = "Unknown U1 module";

static FILE		*lf = NULL;
static char		buf[300];


/*
void
set_program_name( const char *pn ) 
	{
	program_name = pn;
	}
*/

static char *
timeline( char flag )
	{
	long            t;
	struct tm       *ts;
	static char     tline[60];

	time( &t );
	ts = localtime( &t );

	tline[0] = flag;			// First, insert flag character
	tline[1] = ' ';
	tline[2] = '\0';

	switch( conf.log_style )
		{
	case LOG_Bink:
//		strftime( tline+2, sizeof(tline)-2, "%d %b %X U1  ", ts );
		strftime( tline+2, sizeof(tline)-2, "%d %b %H:%M:%S U1  ", ts );
		break;

	case LOG_Frodo:
		strftime( tline+2, sizeof(tline)-2, "%H:%M:%S ", ts );
		break;

	default:
		strftime( tline+2, sizeof(tline)-2, "%H:%M:%S ", ts );
		break;
		}

	return tline;
	}




bool
open_log( const char *file_name )
	{
	int				fd;

//	at exit( close_log ); it is closed from DLL termination function


	if( lf != NULL )
		fclose( lf );

	fd	= _sopen( file_name, O_WRONLY|O_BINARY|O_CREAT|O_APPEND, SH_DENYNO, 0666 );

    if( fd < 0 )
		{
		error( EI_Full, "Can't create log file");
		return Err;
		}

    if( (lf = fdopen( fd, "ab" )) == NULL )
        {
		close( fd );
		error( EI_Full, "Can't create log file");
		return Err;
		}

	time_t			t;
	char     		tline[60];

	time( &t );
	struct tm	*tm_p = localtime( &t );

	switch( conf.log_style )
		{
	case LOG_Bink:
		strftime( tline, sizeof(tline), "%a %d %b %y", tm_p );
		sprintf( buf, "\r\n%s Begin %s, %s\r\n", timeline( '+' ), U1App::program_name(), tline );
		break;

	case LOG_Frodo:
		strftime( tline, sizeof(tline), "%a %d %b %y", tm_p );
		sprintf( buf, "----------  %s, %s\r\n", tline, U1App::program_name() );
		break;

	default:
		strftime( tline, sizeof(tline), "%a %b %d %H:%M:%S %Y", tm_p );
		sprintf( buf, "\r\n  --------  %s: %s\r\n", U1App::program_name(), tline );
		break;
		}

	fputs( buf, lf );                  // Write out
	return Ok;
	}





void
log( const char *level, const char *fmt, ... )
	{
	va_list argptr;

	if( strpbrk( (const char *)level, conf.log_level.c_str() ) || log_debug )
		{
		va_start(argptr, fmt);

		sprintf( buf, "%s ", timeline( *level ) );

		track_add( buf );
		vsprintf( buf, (const char*)fmt, argptr);
		track_add( buf );
		track_add( "\r\n" );

		va_end(argptr);
		}
	}


void
debug( const char *fmt, ... )
    {
    if( !log_debug )
		return;

	va_list argptr;

	va_start(argptr, fmt);

    sprintf( buf, "%s ", timeline( '-' ) );
	track_add( buf );
	vsprintf( buf, fmt, argptr);
	track_add( buf );
	track_add( "\r\n" );

	va_end(argptr);
	}



static void
put_ei( error_info_t ei )
	{
	if( ei == EI_None )
		return;

#ifdef __MSDOS__
	struct DOSERROR info;

	sprintf( buf, "%s  Info : ", timeline( '=' ) );
	track_add( buf );

	dosexterr(&info);

	sprintf( buf, "ExtErr: %d, Class: %x, Action: %x, Locus: %x\r\n",
		info.de_exterror,
		info.de_class,
		info.de_action,
		info.de_locus
		);

	track_add( buf );

	fprintf( stderr, "Info: %s", buf );
#else
	sprintf( buf, "%s  Info : ", timeline( '=' ) );
	track_add( buf );

#ifdef __IBMCPP__
	sprintf( buf, "DosErrno: %d, errno: %d (%s)\r\n", _doserrno, errno, strerror( errno ) );
#else
	sprintf( buf, "DosErrno: %d, errno: %d (%s)\r\n", _doserrno, errno, sys_errlist[errno] );
#endif

	track_add( buf );

	fprintf( stderr, "Info: %s", buf );

#endif
	}



void
warning( error_info_t ei, const char *fmt, ... )
    {
	va_list argptr;

	va_start(argptr, fmt);

	sprintf( buf, "%s Warning: ", timeline( '?' ) );
	track_add( buf );
	vsprintf( buf, fmt, argptr);
	track_add( buf );
	track_add( "\r\n" );

	fprintf ( stderr, "Warning: " );
	vfprintf( stderr, fmt, argptr);
	fprintf ( stderr, "\n" );

	put_ei( ei );

	va_end(argptr);
	}



void
error( error_info_t ei, const char *fmt, ... )
    {
	va_list argptr;

	va_start(argptr, fmt);

	sprintf( buf, "%s Error: ", timeline( '?' ) );
	track_add( buf );
	vsprintf( buf, fmt, argptr);
	track_add( buf );
	track_add( "\r\n" );

	fprintf ( stderr, "Error: " );
	vfprintf( stderr, fmt, argptr);
	fprintf ( stderr, "\n" );

	put_ei( ei );

	va_end(argptr);
	}



void
fatal( exit_code_t exit_code, error_info_t ei, const char *fmt, ... )
    {
	va_list argptr;

	va_start(argptr, fmt);

	sprintf( buf, "%s Fatal: ", timeline( '!' ) );
	track_add( buf );
	vsprintf( buf, fmt, argptr);
	track_add( buf );
	track_add( "\r\n" );

	fprintf( stderr, "Fatal: " );
	vfprintf( stderr, fmt, argptr);
	fprintf( stderr, "\n" );

	put_ei( ei );

	track_reset();

	va_end(argptr);

	exit( (int)exit_code );
	}


void
out_of_memory( void )
	{
	fatal( EC_Out_Of_Memory, EI_None, "Out of memory" );
	}






/*-------------------------------------------------------------------------*/
//	Errors tracking buffer
/*-------------------------------------------------------------------------*/

#define	TBUFS	(100*80)		// About 100 lines

static char		tbuf[TBUFS];

static char		*bufp = tbuf;


void
track_reset( void )
    {
	*bufp = '\0';							// Close buffer;
	if( lf && (bufp > tbuf) )				// Not empty?
		fputs( tbuf, lf );					// Write out

	bufp = tbuf;							// Reset pointer
	*bufp = '\0';							// Clear buffer;
	}


static long	program_start_time = 0l;

extern "SYSTEM" void
mark_program_start_time()
	{
	program_start_time = time(NULL);
	}

//#pra gma startup mark_program_start_time 64


extern "SYSTEM" void
close_log( void )
	{
	long	elap = (long)(time(NULL) - program_start_time);

	if( lf == NULL )
		return;

#if defined(__BORLANDC__) || defined(__TURBOC__)
	if( _HEAPCORRUPT == heapcheck() )
		error( EI_None, "Heap corrupt!" );
#endif

	if( elap <= 0 )			// Prevent divide error
		elap = 1;

    if( stat_mail_messages_processed > 0 || stat_news_messages_processed > 0 )
        log( "%", "Run %ld secs., Mail: %ld msgs, %ld.%02ld /s, News: %ld msgs, %ld.%02ld /s",
            elap,

            stat_mail_messages_processed, stat_mail_messages_processed/elap,
            (stat_mail_messages_processed*100/elap) % 100,

            stat_news_messages_processed, stat_news_messages_processed/elap,
            (stat_news_messages_processed*100/elap) % 100
        );

	track_reset();

	switch( conf.log_style )
		{
	case LOG_Bink:
		sprintf( buf, "%s End %s\r\n", timeline( '+' ), U1App::program_name() );
		break;

	case LOG_Frodo:
		sprintf( buf, "\r\n" );
		break;

	default:
		sprintf( buf, "  --------  End of %s\r\n", U1App::program_name() );
		break;
		}

	fputs( buf, lf );                  // Write out

	fclose( lf );
	lf = NULL;

	}



// Log mus be closed last
//#pra gma exit	close_log		64


static void
track_add( char *str )
    {

    while( *str )
        {
        if( bufp-tbuf >= TBUFS )
			track_reset();

		*bufp++ = *str++;
        }

	*bufp = '\0';
	}

char *
track_get( void )
    {
	return tbuf;
	}
@


1.4
log
@Time representation changed to 24h
@
text
@d7 4
a10 1
 *      $Log: Log.c $
d68 1
a68 1

d89 1
a89 1
const char     *program_name = "Unknown U1 module";
a91 1
bool			log_debug;
d95 1
d101 1
d175 1
a175 1
		sprintf( buf, "\r\n%s Begin %s, %s\r\n", timeline( '+' ), program_name, tline );
d180 1
a180 1
		sprintf( buf, "----------  %s, %s\r\n", tline, program_name );
d185 1
a185 1
		sprintf( buf, "\r\n  --------  %s: %s\r\n", program_name, tline );
d357 5
d434 1
a434 1
		sprintf( buf, "%s End %s\r\n", timeline( '+' ), program_name );
d442 1
a442 1
		sprintf( buf, "  --------  End of %s\r\n", program_name );
@


1.3
log
@there was fatal error in fatal() :) - misplaced va_end()
improved diagnostic printout - errno text printed now.
@
text
@d8 4
d116 2
a117 1
		strftime( tline+2, sizeof(tline)-2, "%d %b %X U1  ", ts );
d121 1
a121 1
		strftime( tline+2, sizeof(tline)-2, "%X ", ts );
d125 1
a125 1
		strftime( tline+2, sizeof(tline)-2, "%X ", ts );
@


1.2
log
@debug
@
text
@d8 3
d259 1
a259 1
	sprintf( buf, "DosErrno: %d, errno: %d\r\n", _doserrno, errno );
a333 3
	va_end(argptr);


d341 3
@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/uu2/vcs/log.c_v  $
d63 2
d67 1
a67 1
#include	<dos.h>
d72 2
a73 1
#include	<alloc.h>
d79 1
a79 1
extern char     program_name[];
d86 6
d132 3
d138 1
a138 1
	fd	= open( file_name, O_WRONLY|O_BINARY|O_CREAT|O_APPEND|O_DENYNONE, 0666 );
d186 1
a186 1
log( const uchar *level, const uchar *fmt, ... )
d197 1
a197 1
		vsprintf( buf, fmt, argptr);
d255 5
a259 4
	sprintf( buf, "DosErrno: %d, errno: %d (%s)\r\n",
		_doserrno, errno,
		sys_errlist[errno]
		);
d376 1
a376 1
static void
d382 1
a382 1
#pragma startup mark_program_start_time 64
d385 1
a385 1
static void
d439 1
a439 1
#pragma exit	close_log		64
@
