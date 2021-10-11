/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Main
 *
 *      $Log: frip.C $
 *      Revision 1.23  1997/01/21 08:53:51  dz
 *      Make sure we have TZ var
 *
 *      Revision 1.22  1997/01/01 16:08:42  dz
 *      Before changing
 *
 *      Revision 1.21  1996/12/28 13:42:00  dz
 *      Ver. 27
 *
 *      Revision 1.20  1996/12/28 03:11:55  dz
 *      new string/istring/domain_s comparison ops
 *
 *      Revision 1.19  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.18  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.17  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.16  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.15  1996/07/31 07:00:23  dz
 *      FripMail written
 *
 *      Revision 1.14  1996/07/22 02:48:05  dz
 *      ver 17
 *
 *      Revision 1.13  1996/07/20 23:24:50  dz
 *      state save
 *
 *      Revision 1.12  1996/01/22 19:23:05  dz
 *      C Set, mostly works
 *
 *      Revision 1.11  1996/01/22 04:35:34  dz
 *      C Set & STL support
 *
 *      Revision 1.10  1996/01/18 00:56:53  dz
 *      Beta 15
 *
 *      Revision 1.9  1996/01/14 01:06:31  dz
 *      beta 14
 *
 *      Revision 1.8  1996/01/13 09:48:05  dz
 *      *** empty log message ***
 *
 *      Revision 1.7  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

//#include "version.h"
#include "frip.h"
#include "app.h"
//#include "lock.h"
//#include "rif.h"

#include <signal.h>
#include <malloc.h>
#include <ctype.h>

#ifdef __WATCOM_CPLUSPLUS__
#include <dos.h>
#endif


OS_Type os_type;



bool volatile Stop_Frip = No;

static int frip_exit_code = 0;
void add_exit_code( exit_code e ) { frip_exit_code |= e; }


static void sig_handler( int sig );

#ifdef __IBMCPP__
#  define SIG_FUNC_T (_SigFunc)
#else
#  define SIG_FUNC_T
#endif

static void sig_setter()
    {
    signal( SIGINT,   SIG_FUNC_T sig_handler );   //     an interactive attention (CTRL/C on keyboard) is signalled
    signal( SIGBREAK, SIG_FUNC_T sig_handler );   //     an interactive attention (CTRL/BREAK on keyboard) is signalled
    signal( SIGTERM,  SIG_FUNC_T sig_handler );   //     a termination request is sent to the program
#if 0
    signal( SIGUSR1,  SIG_IGN );                  //     OS/2 process flag A via DosFlagProcess
#endif
    signal( SIGUSR2,  SIG_IGN );                  //     OS/2 process flag B via DosFlagProcess
    signal( SIGUSR3,  SIG_IGN );                  //     OS/2 process flag C via DosFlagProcess
    }

static void sig_handler( int  )
    {
    //sleep(1);
    sig_setter();
    Stop_Frip = Yes;
    }



// NB!
string started_in_dir =
#ifdef unix
    "/"
#else
    "?:\\"
#endif
    ;

void go_dir( string dir )
    {
    char *dp = (char *)(const char *)dir;
    if( dp[1] == ':' )
        {
#ifdef __WATCOM_CPLUSPLUS__
        unsigned drives = 0, drive = tolower(dp[0]) - 'a' + 1;
        _dos_setdrive( drive, &drives );
#else
        _chdrive( tolower(dp[0]) - 'a' + 1);
#endif
        }
    chdir( dp );
    }

int main( int ac, char **av )
    {
    if( 0 == getenv("TZ") )
        {
#ifdef __NT__
        Warning("TZ variable is not set. Hope it's OK.");
#else
        Error("TZ variable is not set. I need to know your timezone to work correctly. Sorry :-(");
        return 33;
#endif
        }
        
    sig_setter();
    tzset();
    
    try { parse_config(); }
    catch( General_Ex ex )
        {
        cerr
            << "Setup loader failed\nException in " << ex.where.c_str()
            << ": " << ex.what.c_str()
            << " (" << ex.why.c_str() << ")\n"
            << "Can't configure self, stopped.\n";
        add_exit_code(Exit_Exception);
        return frip_exit_code;
        }

        {
        char a[200];
        getcwd( a, 200 );
        started_in_dir = a;
        }

    go_dir(conf.dir());


    try
        {
        FripApp app;
        app.frip( ac, av );
        } // app destructs here
    catch( General_Ex ex )
        {
        ex.print();
        cerr << "Unhandled exception, FRIP stopped.\n";
        add_exit_code(Exit_Exception);
        }

    /*
    try { ZipList_send(); }
    catch( General_Ex ex )
        {
        ex.print();
        add_exit_code(Exit_Exception);
        }
        */
    
#ifdef __DOS__
    go_dir(started_in_dir);
#endif

    return frip_exit_code;
    }


