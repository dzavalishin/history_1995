/************************ UU2 Snews ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	Main
 *
 *      $Log: main.c $
 *      Revision 1.5  1995/11/06 13:00:40  dz
 *      Removed old code
 *
 *      Revision 1.4  1995/08/07 13:57:10  dz
 *      Cleanup
 *
 *      Revision 1.3  1995/08/07 13:28:26  dz
 *      Using class App
 *
 *      Revision 1.2  1995/04/09  23:24:27  dz
 *      IBM C Set version
 *
 *      Revision 1.1  1995/03/11  18:31:14  dz
 *      Initial revision
 *
 *      
 *         Rev 1.7   07 Jun 1993 17:09:46   dz
 *      Announces, copyrights, etc.
 *      
 *         Rev 1.6   28 Nov 1992 22:18:52   dz
 *      -q flag - don't print announce string
 *      
 *         Rev 1.5   28 Nov 1992 22:15:16   dz
 *      Announce
 *      
 *         Rev 1.4   23 Oct 1992 14:56:00   dz
 *      Usage
 *      
 *         Rev 1.3   18 Jun 1992 11:07:58   dz
 *      update
 *      
 *         Rev 1.2   31 Mar 1992 15:15:12   dz
 *      snews
 *      
 *         Rev 1.1   04 Feb 1992 00:00:14   dz
 *      -i flag, program name var.
 *      
 *         Rev 1.0   06 Oct 1991 00:17:28   dz
 *      Initial revision.
 *      
 *
\*/

#include	"snews.h"
#include	"..\..\version.h"



int
SnewsApp::Run()
    {
    bool	got_pkt = No;
    bool	killhim = Yes;
    bool	err = No;
    bool	scan_msgs = No;
    
    verbose = No;

    while( 1 )
        {
        switch( GetOpt() )
            {
            case o_Opt:
                switch( OptChar )
                    {
                    case 's':        scan_msgs = Yes;      continue;
                
                    case 'd':        killhim = No;         continue;
                
                    case 'v':        verbose = Yes;        continue;
                
                    case 'i':        continue;     // just ignore one arg
                    }
                // fall through...
                
            case o_Err:
                fprintf( stderr, "Unknown option '%c' ignored\n", OptChar );
                continue;
                
                
            case o_Arg:
                got_pkt = Yes;
                if( unpack( OptArg ) != Err )
                    {
                    if( killhim )
                        {
                        chmod( OptArg, 0666 );
                        unlink( OptArg );
                        }
                    }
                else
                    err = Yes;
                continue;
                
            case o_Eof:
                break;
                
            }
        break;
        }
    
    
    if( (!got_pkt) && (!scan_msgs) )
        PrintUsage( "No packet name or -s flag given" );
    
    if( scan_msgs )
        scan();
    
    if( !verbose )
        fprintf( stderr, "\n" );   // Newline after dots
    
    return err ? 1 : 0;
    }


#define OPTS	"i:sdv"

int
    main( int ac, char **av )
    {
    U1App::program_name( "U1 Snews " U1_VER_STR " (" __DATE__ ")" );
    U1App::_announceable = Yes;
    U1App::_usage = 
        "Personal args:\n"
        "    -i <arg>    - Ignore <arg> completely\n"
        "    -s          - Scan netmail for fileattaches with packets\n"
        "    -d          - Don't delete packets, given on command line\n"
        "    -v          - Verbose operation\n"
        "    <name>      - unpack and delete packet <name>\n"
        "                  (Wildcards allowed in <name>)\n"
        ;
    
    SnewsApp	app;
    
    app.Setup( ac, av, OPTS );
    
    return app.Run();
    }




