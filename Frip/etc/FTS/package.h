#include "style.h"

class part;


class package
    {

public:
    static const char *file_extension;
    
    package( const string &dir, const string &file );  // open existing
    package( const string &dir );                      // create new
    
    ~package();
    
    success   push( const package &);
    success   pop( const package &);
    success   pop( void );
    
    
    const part &    top();
    const part &    bottom();
    };


class part
    {
    friend package;
    package &carrier;
    streampos begin, treamoff len;
    part( package &c, streampos b, treamoff l )
        : carrier(c), begin(b), len(l)
        {}
public:
    
    const part &    below();   // to the beginning of file
    const part &    above();   // to the end of file
    
    }




