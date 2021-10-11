head	1.1;
access;
symbols;
locks;
comment	@ * @;


1.1
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	;


desc
@@


1.1
log
@Initial revision
@
text
@
#ifndef BINBUF_H
#define BINBUF_H

class binbuf
    {
    public:
        char *b;
        
        binbuf( int size ) { b = new char[size]; }
        ~binbuf() { delete [] b; }
    };

#endif // BINBUF_H
@
