/************************ Gate remote control ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :       Main
 *
 *      $Log: Main.c $
 *
 *
\*/

#include "batchmail.h"
#include "..\..\version.h"

#include <log.h>



static char _RCS_Info[] = "$Header: D:/U1/Tools/Remote/RCS/Main.c 1.3 1995/11/06 14:28:09 dz Exp $";


int BatchMailApp::Run()
    {
    int narg = 0;
    while( 1 )
        {
        switch( GetOpt() )
            {
            case o_Opt:
                switch( OptChar )
                    {
                    case 'f': flush_mail = Yes; continue;
                    }

            case o_Err:
                fprintf( stderr, "Unknown option, ignored\n" );
                continue;

            case o_Arg:
                if( narg++ == 0 )
                    host = OptArg;
                else
                    addr.push_back(OptArg);

            case o_Eof:
                break;

            }
        break;
        }

    if( narg > 1 )
        do_add_mail();

    if( flush_mail )
        do_flush_mail();

    if( narg < 2 && (!flush_mail) )
        {
        error(EI_None,"Nothing to do");
        return 33;
        }
    
    return 0;
    }


#define	OPTS	"f"


int main( int ac, char **av )
    {
    U1App::program_name( "U1 BatchMail " U1_VER_STR " (" __DATE__ ")" );
    U1App::_announceable = Yes;
    U1App::_usage =
        "Program args: [flags] host-name address...\n"
        "       -f     \t- flush stored mail now\n"
        ;

    BatchMailApp app;

    app.Setup( ac, av, OPTS );

    return app.Run();
    }







