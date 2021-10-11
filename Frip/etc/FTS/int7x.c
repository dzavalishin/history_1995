#include "int7x.h"


ostream & operator << ( ostream &os, const int7x &av )
    {
    unsigned long v = av.value;
    unsigned char c;
    
    do
        {
        c = v & 0x7F;
        v >>= 7;
        
        if( v ) c |= 0x80;
        
        os.put(c);
        if( os.fail() )
            return os;
        } while( v );
    
    return os;
    }

istream & operator >> ( istream &is, int7x &v )
    {
    long res = 0;
    int shift = 0;
    unsigned char c;
    
    do
        {
        is.get(c);
        if( is.fail() || is.eof() )
            return is;
        
        res += (((unsigned)c) & 0x7F) << ((shift++)*7);
        
        } while( ((unsigned)c) & 0x80u );
    
    v.value = res;
    
    return is;
    }
