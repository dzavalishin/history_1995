
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
