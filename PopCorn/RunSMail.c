/************************ PopCorn server ***************************\
 *
 *      Copyright (C) 1991-1996 by Infinity Soft
 *
 *      Module  :	SendMail (or Rmail :) starter
 *
 *      $Log: RunSMail.c $
 *      Revision 1.1  1996/09/17 23:20:43  dz
 *      Initial revision
 *
 *
 *
\*/


#include	"popcorn.h"
#include	"except.h"

#include	<process.h>
#include	<stdio.h>
#include	<io.h>
#include	<time.h>

#include	<vector.h>

#define INCL_DOSQUEUES   /* Queue values */
#include <os2.h>


const string rmail_exe_name("rmail.exe");

  /*************************************************************
   Make time/date string withno '\n'
   *************************************************************/

static char *s_week[] =
    { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

static char *s_month[] =
    {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

static string rfc_timeline( time_t t = time(0), const char *tz = getenv("TZ") )
    {
    static char		tl[50];
    
    if( t == -1 )
        throw Invalid_Argument( "rfc_timeline", "time_t is out of range", "" );

    if( tz == 0 ) tz = "";
    
    struct tm * tb = localtime(&t);
        
      //*   [Weekday,] dd Mmm [19]yy hh:mm[:ss] Timezone
        
    sprintf( tl, "%s, %2d %.3s %d %02d:%02d:%02d %s",
            s_week[ (tb->tm_wday) % 7 ],    tb->tm_mday,
            s_month[ (tb->tm_mon) % 12 ],   tb->tm_year+1900,
            tb->tm_hour, tb->tm_min, tb->tm_sec, tz
           );
        
    return tl;
    }


string make_from_( string from )
    {
    string temp, dummy;
    if( from[0] == '@' )
        from.parse( dummy, ':', from );
    
    string d;
    bool repeat = Yes;
    while( repeat )
        {
        repeat = No;
        for( int i = from.length(); i--; )
            {
            if( from[i] == '@' || from[i] == '%' )
                {
                d = from.substr(i+1) + "!" + d;
                from.strip(i,0);
                repeat = Yes;
                break;
                }
            }
        }

    string out;
    
    if( d.length() || strchr(from, '!') )
        out = d + from + " " + rfc_timeline();
    else
        out = domain + "!" + from + " " + rfc_timeline();

    printf("from_ = %s\n", out.c_str() );
    return out;
    }








void run_rmail
    (
     const vector<string> &msg,
     const string &from,
     const vector<string> &to,
     const string &peer
    )
    {
    bool	err = No;
    int		ex;

    //debug("run_one( msg, %s, %s )", (const char *)from, (const char *)to);

    ulong    ReadHandle;    /* Pointer to the read handle      */
    ulong    WriteHandle;   /* Pointer to the write handle     */
    APIRET   rc;            /* Return code                     */
    const    PipeSize = 4096;

    rc = DosCreatePipe(&ReadHandle, &WriteHandle, PipeSize);
    if (rc != 0)
        {
        printf( "DosCreatePipe error: return code = %ld\n", rc );
        err = Yes;
        return;
        }

    int stdin_save = dup( 0 );

    DosClose( 0 );
    if( 0 != dup2( ReadHandle, 0 ) )
        {
        printf( "Can't dup2\n" );
        throw Invalid_Condition("run_uusmail","Can't dup2","");
        }
    DosClose( ReadHandle );

    DosSetFHState( WriteHandle, OPEN_FLAGS_NOINHERIT );
    DosSetFHState( stdin_save, OPEN_FLAGS_NOINHERIT );

    const char *argv[20];
    int         argc = 1;

    argv[0] = "rmail";

    //argv[argc++] = "-r";
    //argv[argc++] = from;

    for( int i = 0; i < to.size(); i++ )
        argv[argc++] = to[i];

    argv[argc++] = 0;     // End of args

    int pid;

    pid = spawnvp(
                 P_NOWAIT,
                 (char *)(const char *)rmail_exe_name,
                 // "./rmail.exe",
                 (char **)argv );
    if( pid < 0 )
        {
        printf( "Can't execute %s\n",  (const char *)rmail_exe_name );
        err = Yes;
        }
    else
        {
        FILE *fp = fdopen( WriteHandle, "w" );
        if( fp == NULL )
            {
            printf(  "Can't fdopen our end of pipe\n" );
            DosClose( WriteHandle );
            err = Yes;
            }

        fprintf( fp, "From %s\n", make_from_(from).c_str() );
        fprintf( fp,
                "Received: by %s (PopCorn " VERSION_STR ") from %s with SMTP; %s\n",
                domain.c_str(),
                peer.c_str(),
                rfc_timeline().c_str()
               );

          // Stuff data into the open end of pipe.
        for( i = 0; i < msg.size(); i++ )
            fprintf( fp, "%s\n", msg[i].c_str() );

        if( fp != NULL ) fclose( fp );
        }
        
    DosClose( 0 );
    if( 0 != dup2( stdin_save, 0 ) )
        {
        printf( "Can't dup2( %d, 0 )\n", stdin_save );
        err = Yes;
        }
    DosClose( stdin_save );
    //close( stdin_save );

    if( pid >= 0 )
        {
        int retcode;
        _cwait( &retcode, pid, WAIT_CHILD );
        if( retcode)
            {
            printf( "Error running %s: 0x%X\n", (const char *)rmail_exe_name, retcode );
            err = Yes;
            }
        }

    if( err )
        throw Invalid_Condition("run_uusmail","execution failed","");
    }


