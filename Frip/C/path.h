/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	class 'path' definition
 *
 *      $Log: path.h $
 *      Revision 1.1  1996/12/28 03:11:55  dz
 *      Initial revision
 *
 *
\*/

#include <strng.h>

#ifndef PATH_H
#define PATH_H

#ifdef unix
#  define BASE_FOR_PATH string
const char FN_SLASH = '/';
#else
#  define BASE_FOR_PATH istring
const char FN_SLASH = '\\';
#endif

  //
  // Absence of trailing '/' is guaranteed

class _Export path
    : public BASE_FOR_PATH
    {
    public:
        path( const path &          p ) : BASE_FOR_PATH(p) {}
        path( const string &        p ) : BASE_FOR_PATH(p) { fix(); }
        path( const char *          p ) : BASE_FOR_PATH(p) { fix(); }

        void fix();
    
        void replace_dir ( path dir ); // Treats arg as just a directory
        void replace_name( path dir ); // Extracts name from arg
        void replace_ext ( path dir ); // Extracts extension from arg

        path dir()  const;  // Cuts off last
        path name() const;
        path ext()  const;

        void to_slashes();
        void to_backslashes();

    
        void make_relative_to_dir( path dir );
    };


//path make_relative_to_dir( path in, path dir )
path make_relative_to_start_dir( path in );

#ifndef PATH_C
#undef BASE_FOR_PATH
#endif // PATH_C


#endif // PATH_H



