/************************ UUCP to FIDO gate ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *  Module  :   Main
 *
 *      $Log: Main.c $
 *      Revision 1.8  1995/11/05 13:50:34  dz
 *      Before fido_msg rewrite
 *
 *      Revision 1.7  1995/11/04 18:32:53  dz
 *      Now returning message to sender in a case of
 *      address conversion errors.
 *      Some debugging done.
 *
 *      Revision 1.6  1995/11/04 01:21:46  dz
 *      First RFC_Msg-based version, that compiles and does something
 *
 *      Revision 1.5  1995/11/02 14:25:20  dz
 *      Before switching to rfc_msg
 *
 *      Revision 1.4  1995/08/16 13:06:33  dz
 *      intermediate
 *
 *      Revision 1.3  1995/08/08 14:37:23  dz
 *      checkpoint
 *
 *      Revision 1.2  1995/08/02 13:07:40  dz
 *      closing all fd's except 0-2 on startup to prevent
 *      parent from deadlock if he forgot to close for me
 *      his end of pipe.
 *
 *      C Set fixes.
 *
 *      Revision 1.1  1995/03/07  00:20:29  dz
 *      Initial revision
 *
 *      
 *         Rev 1.9   12 Apr 1993 12:08:28   dz
 *      stdin replaced with in - support for uupc 5.06
 *      
 *         Rev 1.8   13 Mar 1993 23:03:52   dz
 *      Added ache's uucp 5.06 parameters-in-stdin support
 *      
 *         Rev 1.7   13 Mar 1993 22:29:24   dz
 *      Cosmetical...
 *      
 *         Rev 1.6   23 Oct 1992 14:46:38   dz
 *      Usage
 *      
 *         Rev 1.5   07 Apr 1992 12:27:08   dz
 *      -u flag
 *      
 *         Rev 1.4   03 Dec 1991 13:16:38   dz
 *      Program name variable added (for log file)
 *      
 *         Rev 1.3   11 Oct 1991 05:35:36   dz
 *      Stack size increased.
 *      Cosmetical changes.
 *      
 *         Rev 1.2   23 Sep 1991 17:25:20   dz
 *      UUPC sendmail-like flags parsed and ignored.
 *      
 *         Rev 1.1   27 Aug 1991 23:44:42   dz
 *      Sendmail replacement - first stage
 *      
 *         Rev 1.0   27 Aug 1991 07:44:44   dz
 *      Initial revision.
 *
 *
\*/

#include    "uu2fido.h"
#include    <rfc_msg.h>


static char _RCS_Info[] = "$Header: D:/U1/Mail/UU2Fido/RCS/Main.c 1.8 1995/11/05 13:50:34 dz Exp $";


Uu2FidoApp::Uu2FidoApp()
    {
    use_recoder = No;
    from_Unix   = No;
    got_job = No;
    }


#define	OPTS	"lr:f:h:x:uicmXZ"


int
Uu2FidoApp::Run()
    {
    bool   args_in_file = No;
    bool   got_job = No;
    bool   send_screwed = No;

    use_recoder = conf.code_fu_flag ? No : Yes;

    while( 1 )
        {
        switch( GetOpt() )
            {
        case o_Opt:
            switch( OptChar )
                {
            case 'i':   case 'v':   case 'm':
            case 'X':   case 'Z':              // uupc 5.06/ache
                break;

            case 'u':
                use_recoder = Yes;  // To turn recoder on
                from_Unix   = Yes;  // To pass -u option to sendmail, if chaining
                break;

            case 'x':
                conf.log_debug = Yes;
                break;

            case 'h':       // Number follows
                break;

            case 'r':
            case 'f':       // Next arg follows
                break;

            case 'l':       // Ok, take args from stdin (ache 5.06)
                if( conf.uucp == UUCP_Ache30 )
                    args_in_file = Yes;
                else
                    PrintUsage( "Option -l (load args from message) is valid for Ache UUCP mode only" );
                break;

            default:
                PrintUsage( "Bad option" );
                break;

                }
            continue;

        case o_Arg:
            debug( "Arg: %s", OptArg );

            if( !got_job )            // Загpужать письмо надо только один pаз
                {
                recoder   r( conf.code_uf.c_str() );

                msg.set_loader_recoder( 0 );

                if( use_recoder )
                    {
                    if(!r.valid())
                        error( EI_None, "Code table '%s' not found", (const char *)conf.code_uf.c_str() );
                    else
                    msg.set_loader_recoder( &r );
                    }

                got_job = Yes;
                msg.load( stdin );
                }

            if( send( msg, OptArg ) == Err )
                send_screwed = Yes;

            continue;

        case o_Err:
            fprintf( stderr, "Unknown option, ignored\n" );
            continue;

        case o_Eof:

            if( args_in_file )
                {
                args_in_file = No;
                load_args();
                continue;
                }

            break;

            }
        break;
        }


    return send_screwed ? 1 : 0;
     }


int
Uu2FidoApp::load_args()
    {
    char    buf[1024+2], *s;

    const   MAXARGS = 500;
    char    *nargv[MAXARGS];
    int     nargc = 0;

    bool    done = No;


    debug("args in file" );

    while( fgets(buf, sizeof(buf) - 1, stdin ) != NULL )
        {
        buf[sizeof(buf) - 1] = '\0';

        if(( s = strpbrk( buf, "\r\n" )) != NULL )
            *s = '\0';

        debug( "FileArg: %s", buf );

        if( strcmp( buf, "<<NULL>>" ) == 0 )
            {
            nargv[nargc] = NULL;

            Setup( nargc, nargv, OPTS );

            done = Yes;
            break;
            }
        else
            {
            if( nargc >= MAXARGS )
                error( EI_None, "Too many args (> %d)", MAXARGS );
            else
                {
                nargv[nargc] = strdup( buf );
                if( nargv[nargc] == NULL )
                    fatal( EC_Out_Of_Memory, EI_None, "Out of memory!" );

                nargc++;
                }
            }
        }

    if( ferror( stdin ) )
        fatal( EC_IO_Error, EI_Full, "Can't read stdin" );

    return 0;
    }




int
main( int ac, char **av )
    {
    U1App::program_name( "U1 UU2Fido " U1_VER_STR " (" __DATE__ ")" );
    U1App::_announceable = No;
    U1App::_usage = 
        "Personal args:\n"
        "       -l     \t- Load other args from stdin.\n"
        "                  Valid in Ache's uucp mode only.\n"
        "\n"
        "       -x<lvl>\t- Turns debugging on. Value of <lvl> is ignored\n"
        "\n"
        "       -u     \t- Turns code table convertor on.\n"
        "\n"
        "       -i     \t- \\\n"
        "       -v     \t-  \\\n"
        "       -m     \t-   \\\n"
        "       -X     \t-    \\ All of these\n"
        "       -Z     \t-    / are ignored.\n"
        "       -h<nn> \t-   /\n"
        "       -r<ff> \t-  /\n"
        "       -f<ff> \t- /\n"
        ;

    Uu2FidoApp	app;

    app.Setup( ac, av, OPTS );

    return app.Run();
    }





