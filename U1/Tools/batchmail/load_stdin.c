
#include <BatchMail.h>

#include <io.h>
#include <fcntl.h>
//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
  //#include <dir.h>
//#include <direct.h>
//#include <time.h>
//#include <process.h>
//#include <ctype.h>


const char *BatchMailApp::load_stdin()
    {
    _setmode( 0, O_BINARY );
    
    int   ssize = 0;
    char *store = 0;
    int   readsize = 100*1024;
    int   ndone = 0;
    int   nread;
    
    do
        {
        ssize += readsize;
        char *more = new char[ssize+1]; // extra char for \0
        
        if( store )
            {
            memcpy( more, store, ndone );
            delete [] store;
            }
        store = more;
        
        nread = read( 0, store+ndone, readsize );

        if( nread < 0 )
            throw( "BatchMailApp::load_stdin", "read error", "" );
        if( nread == 0 ) break;
        ndone += nread;
        
        }
    while( nread >= readsize );
    store[ndone] = 0;
    
    close(0); // we don't need it any more
    
    printf("done reading %d bytes from stdin\n", ndone );

    return store;
    }

