head	1.2;
access;
symbols;
locks;
comment	@ * @;


1.2
date	96.12.28.13.42.00;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.12.28.03.11.55;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@Ver. 27
@
text
@
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
@


1.1
log
@Initial revision
@
text
@d4 1
d79 2
a80 1
    in.make_relative_to_dir( in );
@
