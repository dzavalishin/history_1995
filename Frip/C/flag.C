#include "frip.h"
#include "flag.h"


#include <iterator.h>

flags::~flags()
    {
    }


flags::flags( const string &f )
    {
    string s = f;
    string one;
    while( !s.is_empty() )
        {
        s.parse( one, ',', s );
        one.ucase();
        (*this) += one;
        }
    }
    

flags::operator string () const
    {
    string s;
    bool first = Yes;
    
    set<string, less<string> >::iterator i;
    for (i = v.begin (); i != v.end (); i++)
        {
        if( !first ) s += ",";
        first = No;
        s += *i;
        }

    return s;
    }


int flags::operator == ( const flags &f ) const
    {
    set<string, less<string> >::iterator i;
    
    for (i = v.begin (); i != v.end (); i++)
        {
        if( !f.v.count( *i ) )
            return 0;
        }
    
    for (i = f.v.begin (); i != f.v.end (); i++)
        {
        if( !v.count( *i ) )
            return 0;
        }
    
    return 1;
    }



flags & flags::operator += ( string s )
    {
    s.ucase();
    // if(!v.find(&s))  we need not check since it's a set, not just vector
    v.insert( s );
    return *this;
    }

flags & flags::operator -= ( string s )
    {
    s.ucase();
    v.erase( s );
    return *this;
    }

bool flags::has( string s ) const
    {
    s.ucase();
    return (v.count(s) == 0) ? No : Yes;
    }


void flags::extract( string &s, char key )
    {
    s.strip_ws();
    if( s[0] == key )
        {
        string fs;
        s.parse( fs, s );
        *this = flags( fs.substr( 1 ) );
        }
    }


void flags::clear()
    {
    while( v.size() )
        v.erase( v.begin() );
    }









