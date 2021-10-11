
#define PATH_C
#include "path.h"
#include "app.h"

void path::fix()
    {
    strip_ws();

    if( length() == 0 )
        return;

    // This will crash if length == 0
    if(
       (*this)[length()-1] == '/'
#ifndef unix
       || (*this)[length()-1] == '\\'
#endif
      )
        {
        (*this)[length()-1] = 0;
        newlength();
        }

#ifdef unix
    to_slashes();
#else
    to_backslashes();
#endif

    }


void path::to_backslashes()
    {
    for( int i = length() - 1; i--; )
        if( (*this)[i] == '\\' ) (*this)[i] == '/';
    }

void path::to_slashes()
    {
    for( int i = length() - 1; i--; )
        if( (*this)[i] == '\\' ) (*this)[i] == '/';
    }

void path::make_relative_to_dir( path dir )
    {
    strip_ws();
    
#ifndef unix
    if( (*this)[0] == '/' ) (*this)[0] = '\\';
    if( (*this)[1] == '/' ) (*this)[1] = '\\';
    if( (*this)[2] == '/' ) (*this)[2] = '\\';
    
    if(
       ((*this)[1] == ':'  && (*this)[2] == '\\') ||
       ((*this)[0] == '\\' && (*this)[1] == '\\')
      )
        return; // Pathname is absolute
    
      // No drive letter ?
    if( (*this)[0] == '\\' )
        (*this) = dir.substr( 0, 3 ) + (*this);
    else
        (*this) = dir + "\\" + (*this);
    
#else
    
    if( (*this)[0] == '/' )
        return; // Pathname is absolute
    
    (*this) = dir + "/" + (*this);
    
#endif
    }

path make_relative_to_start_dir( path in )
    {
      //in.make_relative_to_dir( in );
    in.make_relative_to_dir( started_in_dir );
    return in;
    }
