/************************ UUCP to FIDO gate ***************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *  Module  :   Main
 *
 *      $Log: Main.c $
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
#include    "rfc_msg.h"


static char _RCS_Info[] = "$Header: D:/U1/Mail/UU2Fido/RCS/Main.c 1.6 1995/11/04 01:21:46 dz Exp $";


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
                log_debug = Yes;
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
//                msg.save( stdout );
                }

            send( msg, OptArg );
//            msg.save( stdout );

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


     return 0;
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
    Uu2FidoApp	app;

    U1App::_program_name = "U1 UU2Fido " U1_VER_STR " (" __DATE__ ")";
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
        "       -i     \t- \\n"
        "       -v     \t-  \\n"
        "       -m     \t-   \\n"
        "       -X     \t-    \\ All of these\n"
        "       -Z     \t-    // are ignored.\n"
        "       -h<nn> \t-   //\n"
        "       -r<ff> \t-  //\n"
        "       -f<ff> \t- //\n"
        ;

    app.Setup( ac, av, OPTS );

    return app.Run();
    }

















#if 0

char _program_name[] = "uu2fido " U1_VER_STR " (" __DATE__ ")";

static void     usage( char *msg );
//unsigned _stklen    = 16 * 1024;

//  If conf.code_fu_flag == Yes, than default mode is
//  withno recoding.

bool            use_recoder;
bool            from_Unix   = No;                   // -u flag
bool            got_job = No;

//  UUPC 5.06/Ache stores program parameters in the beginning of
//  file that we have at stdin. The next variable keeps position
//  in stdin, where the real text of letter begins.
//
long            text_start = 0;

//  To be sure our input is opened in binary mode, we're
//  setting handle 0 to binary mode and then fdopening it.
//  Here's the result of fdopen
FILE        *   in;


void
rewind_to_text()
    {
    fseek( in, text_start, SEEK_SET );
    fflush( in );
    }


static void
process_args( int ac, char **av )
    {
	bool            args_in_file = No;

    ac--;                                           // Skip exe name
    av++;


//    debug("process_args" );


    while( ac-- )
        {
        char *ap = *av++;
        if( *ap != '-' )                            // Address!
            {

            if( args_in_file )
                {
                error( EI_None, "Args in stdin are mixed with ones at command line!!" );
                error( EI_None, "Arg (%s) skipped", ap );
                continue;
                }

            got_job = Yes;

            debug( "Arg: %s", ap );

            rewind_to_text();

            RFC_Msg	msg;

            msg.load( in );
            msg.save( stdout );

//            send( ap );

            continue;
            }


        for( ap++; *ap; ap++ )                      // Option
            {

            debug("Flag: %c", *ap );

            switch( *ap  )                          // 1st level opt.
                {
            case 'i':
            case 'v':
			case 'm':
            case 'X':
            case 'Z':       // uupc 5.06/ache
                continue;

            case 'u':
                use_recoder = Yes;  // To turn recoder on
                from_Unix   = Yes;  // To pass -u option to sendmail, if chaining
                continue;

            case 'h':       // Number follows
            case 'x':
                break;

            case 'r':
            case 'f':       // Next arg follows
                av++;
                ac--;
                break;

// BUG!!! Check version of uucp before trying to
// do what this flag tells to!
            case 'l':       // Ok, take args from stdin (ache 5.06)
                args_in_file = Yes;
                break;

            default:
                error( EI_None, "Bad flag: %c", *ap );
                continue;
                } /* switch */

            break;

            } /* for */

        }

    // Inside this it is (mostly) Ache's code.

    if( args_in_file )
        {
        char    buf[1024+2], *s;

        const   MAXARGS = 500;
        char    *nargv[MAXARGS];
        int     nargc = 0;

        bool    done = No;


//        debug("args in file" );

        while( fgets(buf, sizeof(buf) - 1, in ) != NULL )
            {
            buf[sizeof(buf) - 1] = '\0';

            if(( s = strpbrk( buf, "\r\n" )) != NULL )
                *s = '\0';

            debug( "FileArg: %s", buf );

            if( strcmp( buf, "<<NULL>>" ) == 0 )
				{
                nargv[nargc] = NULL;

//              fflush( in );
                text_start = ftell( in );
                debug( "Text_start = %ld", text_start );
//              fflush( in );

                process_args( nargc, nargv );

                done = Yes;
                break;
                }
            else
                {
                if (nargc >= MAXARGS)
                    error( EI_None, "Too many args (> %d)", MAXARGS );
				else
                    {
                    nargv[nargc] = strdup(buf);
                    if( nargv[nargc] == NULL )
                        fatal( EC_Out_Of_Memory, EI_None, "Out of memory!" );

                    nargc++;
                    }
                }
            }

        if( !done )
            fatal(  EC_Dunno, EI_None, "End of arg list not found" );

        for( int i = nargc-1; i >= 0; i-- )
			if( nargv[i] != NULL )
				free( nargv[i] );
		}
	}




void
main( int ac, char **av )
	{
	for( int i = 3; i < 20; i++ )
		close( i );		// Блин гоpелый. Пеpед сетапом!

//	set_program_name ( _program_name );

	get_setup();

	use_recoder = conf.code_fu_flag ? No : Yes;


	FILE *tempfp;

	if( lseek( 0, 1L, SEEK_SET ) == -1L )
		{
//		debug("Pipe detected");

//		if( errno != ESPIPE )
		if( errno != EOS2ERR )
			error( EI_Full, "Can't seek on stdin, but errno = %d", errno );

		tempfp = tmpfile();
		if( tempfp == NULL )
			fatal( EC_OS_Error, EI_Full, "Can't create temp file to suck pipe in" );

		fseek( tempfp, 0L, SEEK_SET );

		const int	bs = 8196;
		static char	buf[bs];
		int	 	i;

		while( 1 )
			{
//			debug("Pipe read");
			i = read( 0, buf, bs );
//			debug("Pipe read done, %d", i );
			if( i < 0 )
				fatal( EC_OS_Error, EI_Full, "pipe read error" );

			if( i == 0 )
				break;

			if( fwrite( buf, 1, i, tempfp ) != i )
				fatal( EC_OS_Error, EI_Full, "tmp file write error" );
			}

//		debug("Pipe: got all" );
		fseek( tempfp, 0L, SEEK_SET );

		in = tempfp;
		}
	else
		{
		if( setmode( 0, O_BINARY ) < 0 )
			fatal( EC_OS_Error, EI_Full, "Can't switch stdin into the binary mode" );

		lseek( 0, 0L, SEEK_SET );

//		in = fdopen( 0, "rb" );
		in = fdopen( 0, "r" );
		if( in == NULL )
			fatal( EC_IO_Error, EI_Full, "Can't fdopen(0)" );
		}

	process_args( ac, av );

	if( !got_job )
		usage( "No address given" );

	exit(0);
	}






        /*************************************************************
                               Usage info
        *************************************************************/


void
usage( char *msg )
    {
    printf(
        "U1 UU2Fido vers. " U1_VER_STR " (" __DATE__ ")\n"
	U1_COPYRIGHT "\n"
        "\n"
        "%s\n\n"
        "Usage: uu2fido address [address...]\n"
        ,
        msg
        );
    }






#endif