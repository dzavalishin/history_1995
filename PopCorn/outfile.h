/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Dmitry Zavalishin
 *
 *	Module 	: worker class for mailbox parser
 *
 *      $Log: outfile.h $
 *      Revision 1.1  1996/09/17 23:20:43  dz
 *      Initial revision
 *
 *
 *
 *
 *
\*/

#include "PopCorn.h"
#include "except.h"

#include <stdio.h>


class outfile
    {
    private:
        FILE * fp_v;
        string fn_v;

    public:
        outfile( const string &fn ) { open(fn); }
        outfile(  ) { fp_v = 0; }
        ~outfile() { if(fp_v) abort(); }

        void open( const string &fn )
            {
            fn_v = fn;
            fp_v = fopen( fn_v, "w" );
            if( fp_v == NULL ) throw Fail("outfile::open","open error",fn_v);
            }
        void close() { if( fp_v ) fclose( fp_v ); fp_v = 0; }
        void abort() { close(); unlink( fn_v ); }
        void puts(const string &s) { fputs( s, fp_v ); fputs( "\n", fp_v ); }

    };




