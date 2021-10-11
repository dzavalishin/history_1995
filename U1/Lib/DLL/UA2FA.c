/************************ UUCP interface library ***************************\
 *
 *  Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   UUCP to FIDO address conversion
 *
 *      $Log: UA2FA.c $
 *      Revision 1.3  1995/11/05 13:52:48  dz
 *      current.
 *
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:07:42  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:07:42  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   07 Jun 1993 18:03:00   dz
 *      Initial revision.
 *
 *
\*/

#include    "uucpaddr.h"
#include    <fidoaddr.h>
#include    <log.h>

#include    <stdlib.h>
#include    <malloc.h>
#include    <ctype.h>



static const    uname_len = 50;

struct p2
    {
    fido_user   fa;
    char        uname[uname_len];
    bool        gzone;
    bool        gnet;
    bool        gnode;
    bool        gpoint;
    };


static bool
get_fragm( uucp_addr::el_type t, const char *el, int len, void *arg )
    {
    p2  &p = *(p2*) arg;
    int i;


    if( len < 1 )
        {
//        error( EI_None, "Ua2Fa - El. length = 0!");
        return Yes;
        }


    char *copy = (char *) malloc( len+1 );
    char *csave = copy;

    if( copy == NULL )
        {
        error( EI_None, "Out of memory" );
        return Yes;
        }

    strncpy( copy, el, len );
    copy[len] = '\0';

    switch( t )
        {
    case uucp_addr::UU_name:
        i = len > (uname_len-1) ? (uname_len-1) : len;
        strncpy( p.uname, el, i );
        p.uname[i] = '\0';
//        strip_wrong_characters( p.uname );
        p.fa.name( p.uname );
        break;

    case uucp_addr::UU_bang:
        // Just one element?
        if( NULL == strchr( copy, '.' ) )
            {
            if( isdigit(copy[0]) )
                {
                short int v = (short int) atoi( copy );

                if( p.gpoint )
                    error( EI_None, "Too many digital fields in address");
                else if( p.gnode )
                    p.fa.point( v ), p.gpoint = Yes;
                else if( p.gnet )
                    p.fa.node( v ), p.gnode = Yes;
                else if( p.gzone )
                    p.fa.net( v ), p.gnet = Yes;
                else
                    p.fa.zone( v ), p.gzone = Yes;

                break;
                }

            if( !isdigit( copy[1] ) )
                break;

            short int v = (short) atoi( copy+1 );

            switch( copy[0] )
                {
            case 'p':   
            case 'P':   p.fa.point( v ), p.gpoint = Yes; break;
            case 'f':   
            case 'F':   p.fa.node( v ), p.gnode = Yes;   break;
            case 'n':   
            case 'N':   p.fa.net( v ), p.gnet = Yes;     break;
            case 'z':   
            case 'Z':   p.fa.zone( v ), p.gzone = Yes;   break;
                }

            break;
            }

        // Domain. fall through...

    case uucp_addr::UU_domain:
    case uucp_addr::UU_via:

        if( tolower(copy[0]) == 'p' && isdigit( copy[1] ) )
            {
            short int v = (short) atoi( ++copy );
            while( isdigit( *copy ) )
                copy++;

            if( (!*copy) || *copy == '.' )
                p.gpoint = Yes, p.fa.point( v );

            if( *copy != '.' )
                break;

            copy++;
            }

        if( tolower(copy[0]) == 'f' && isdigit( copy[1] ) )
            {
            short int v = (short) atoi( ++copy );
            while( isdigit( *copy ) )
                copy++;

            if( (!*copy) || *copy == '.' )
                p.gnode = Yes,  p.fa.node( v );

            if( *copy != '.' )
                break;

            copy++;
            }

        if( tolower(copy[0]) == 'n' && isdigit( copy[1] ) )
            {
            short int v = (short) atoi( ++copy );
            while( isdigit( *copy ) )
                copy++;

            if( (!*copy) || *copy == '.' )
                                p.gnet = Yes,   p.fa.net( v );

            if( *copy != '.' )
                break;

            copy++;
            }

        if( tolower(copy[0]) == 'z' && isdigit( copy[1] ) )
            {
            short int v = (short) atoi( ++copy );
            while( isdigit( *copy ) )
                copy++;

            if( (!*copy) || *copy == '.' )
                p.gzone = Yes,  p.fa.zone( v );

            }
        break;


    default:
        error( EI_None, "Unknown UUCP addr frag. type %d", (int) t );
        free(csave);
        return Yes;
        }

    free(csave);
    return Yes;
    }


bool uucp_addr::
convert_to_fido( fido_user &a, char *uname, int uname_len ) const
    {
    p2      par;

    debug("Converting %s to fido address", addr_v );

    par.uname[0] = '\0';
    par.gzone =
    par.gnet =
    par.gnode =
    par.gpoint = No;

    if( traverse( get_fragm, (void *) &par ) == Err )
        {
        error( EI_None, "Can't parse uucp addr '%s'", addr_v );
        return Err;
        }

    strncpy( uname, par.uname, uname_len );
    uname[uname_len-1] = '\0';

    if( par.gzone && par.gnet && par.gnode && strlen( par.fa.name() ) )
        {
        debug( "pfnz conversion done" );
        a = par.fa;
        a.normalize_name();
        return Yes;
        }

    return No;
    }














