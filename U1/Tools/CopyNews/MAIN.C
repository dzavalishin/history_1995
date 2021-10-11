/************************ UU2 CopyNews ***************************\
 *
 *  Copyright (C) 1993 by Infinity Soft
 *
 *  Module  :   Main
 *
 *      $Log:$
 *
 *
\*/

#include "copynews.h"

static char *        get_temp_name( const char *dir );
static bool          process( const char *file, const char *dst_dir );
static void          usage( const char * reason );

bool        f_silent = No, f_move = No;


int
main( int ac, char **av )
    {

	ac--;
	av++;

    while( **av == '-' && ac-- )
        {
		char *ap = *av++;

        for( ap++; *ap; ap++ )
            {
            switch( *ap  )
                {
			case 'q':
				f_silent = Yes;
				break;

			case 'm':
				f_move = Yes;
				break;

			default:
				usage("Error: Bad flag\n\n");
				exit(1);
				break;
				} /* switch */
			}
		}


    if( ac != 2 )
        {
        usage("There must be two arguments");
        exit(1);
        }

    char *list_file = av[0], *out_dir = av[1];


    FILE *list = fopen( list_file, "rb+" );
    if( list == NULL )
        {
        fprintf( stderr, "Can't open %s\n", list_file );
        exit(1);
        }

    const  bs = 200;
    char   b[bs+1];
    long   pos1, pos2;

    bool   has_err = No;

    while( pos1 = ftell( list ), NULL != fgets( b, bs, list ) )
        {
        pos2 = ftell( list );

        if( *b == ';' )                         // Our mark to skip...
            continue;

        char *p;
        if( NULL != (p = strpbrk( b, "\r\n" )) )
            *p = '\0';

        if( process( b, out_dir ) == Err )
            {
            fprintf( stderr, "Can't process %s\n", b );
            has_err = Yes;
            }
        else
            {
            if(
               (0 > fseek( list, pos1, SEEK_SET )) ||
               (EOF == putc( ';', list )) ||
               (0 > fseek( list, pos2, SEEK_SET ))
              )
                {
                fprintf( stderr, "Can't mark %s as processed\n", b );
//                exit(1);
                }
            }
        }

    if( !has_err )
        {
        fseek( list, 0, SEEK_SET );
        if( 0 != chsize( fileno(list), 0 ) )
            fprintf( stderr, "Can't truncate %s\n", list_file );

        fclose( list );

        if( 0 != unlink( list_file ))
            fprintf( stderr, "Can't delete %s\n", list_file );
        }
    else
        fclose( list );

    return 0;
    }




bool
process( const char *file, const char *dst_dir )
    {
    const char *out_file = get_temp_name( dst_dir );

    if( out_file == NULL )
        return Err;

    char of[300];

    strncpy( of, out_file, 300 );
    of[299] = '\0';

    return get_file( f_move ? GMove : GCopy , file, of );
    }

char *
get_temp_name( const char *dir )
    {
    char        fn[180];
    long        t = time(NULL) << 8;
    long        tt = t;

    const       max_try = 50;

    while( t - tt <= max_try )
        {
        sprintf( fn, "%s\\%08lX.a", dir, t++ );
        if( 0 != access( fn, 0 ) )
            return fn;

        }

    fprintf( stderr, "Too many tries - Can't invent temp file name\n");

    return NULL;
    }


void
usage( const char *msg )
    {
    printf(
//        "UU2 CopyNews vers. " UU2_VER_STR " (" __DATE__ ")\n"
        "UU2 CopyNews vers. 1.00 (" __DATE__ ")\n"
        "Copyright (C) 1991-1993 by Dmitry Zavalishin.\n"
        "\n"
        "%s\n\n"
        "This program will copy or move all files, which names are\n"
        "given in <src-file-list-file> to news spool <dst-dir>.\n"
        "\n"
        "Usage: copynews [-mq] src-file-list-file dst-dir\n"
        "\n"
        "Flags:\n"
        "\t-m - Move, not copy\n"
        "\t-q - Be quiet\n"
        ,
        msg
        );
    }
