/************************ UU2 News Batcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Main
 *
 *      $Log: main.c $
 *      Revision 1.7  1996/05/21 00:21:04  dz
 *      cosmetical changes
 *
 *      Revision 1.6  1996/05/20 23:58:45  dz
 *      current
 *
 *      Revision 1.5  1995/08/07 05:15:02  dz
 *      Using App class
 *
 *      Revision 1.3  1995/08/02 13:30:34  dz
 *      cosmetical
 *
 *      Revision 1.2  1995/04/10  02:08:14  dz
 *      IBM C Set version
 *
 *      Revision 1.1  1995/03/11  18:26:07  dz
 *      Initial revision
 *
 *      Rev 1.0   26 Apr 1992 14:17:56   dz
 *      Initial revision.
 *
 *
\*/

#include	"batch.h"
#include	"..\..\version.h"

#include	"feeds.h"





#define	OPTS	"x"

int BatchApp::Run()
    {

    while( 1 )
        {
        switch( GetOpt() )
            {
            case o_Arg:
            case o_Opt:
            case o_Err:
                PrintUsage("Unknown option");
                return 0;

            case o_Eof:
                break;

            }
        break;
        }

    scan();

    if( sent_something && strlen( (const char*) conf.kick_uucp_flag.c_str()) != 0 )
        {
        FILE    *flag;

        if( ( flag = fopen( conf.kick_uucp_flag.c_str(), "w" ) ) == NULL )
            error( EI_Full, "Can't create flag file '%s'",
                  (const char *)conf.kick_uucp_flag.c_str() );
        else
            fclose( flag );
        }

    return 0;
    }





int main( int ac, char **av )
    {
    U1App::program_name( "U1 Batch " U1_VER_STR " (" __DATE__ ")" );
    // U1App::_usage = "Personal args: None\n";
    // U1App::_announceable = Yes;

    BatchApp	app;

    app.Setup( ac, av, OPTS );

    return app.Run();
    }


