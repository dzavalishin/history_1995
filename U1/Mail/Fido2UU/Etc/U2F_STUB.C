//
//  Copyright (C) 1993 by Dmitry Zavalishin.
//

#include <stdio.h>
#include <process.h>

// 
// This tiny thingie can be used to run UU2FIDO from UUPC/Extended RMAIL
// as a gateway (See UUCP/Ext. HOSTPATH file).
//

void
main( int ac, char **av )
    {
    char    arg[200];

    sprintf( arg, "%s@%s", av[3], av[2] );

    execlp( "UU2FIDO", "UU2FIDO", arg, NULL );

    perror("Can't execute UU2FIDO");
    exit(-1);
    }

