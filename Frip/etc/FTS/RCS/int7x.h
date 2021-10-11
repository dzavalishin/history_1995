head	1.1;
access;
symbols;
locks;
comment	@ * @;


1.1
date	96.01.24.22.18.58;	author dz;	state Exp;
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
#include <iostream.h>

#if 0
class int7x_exception
    {
    };

class int7x_read_error_exception : public int7x_exception
    {
    };
#endif


class int7x
    {
    unsigned long value;
    
public:
    operator unsigned long () const { return value; }
    int7x( unsigned long v ) : value(v) {}
    int7x( const int7x &v ) : value(v.value) {}
    
    friend ostream & operator << ( ostream &, const int7x & );
    friend istream & operator >> ( istream &, int7x & );
    };


ostream & operator << ( ostream &, const int7x & );
istream & operator >> ( istream &, int7x & );
    



@
