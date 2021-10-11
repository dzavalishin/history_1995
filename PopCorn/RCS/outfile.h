head	1.1;
access;
symbols
	Ver_075:1.1;
locks;
comment	@ * @;


1.1
date	96.09.17.23.20.43;	author dz;	state Exp;
branches;
next	;


desc
@@


1.1
log
@Initial revision
@
text
@/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Dmitry Zavalishin
 *
 *	Module 	: worker class for mailbox parser
 *
 *      $Log:  $
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




@
