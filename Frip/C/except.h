/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	exception info classes
 *
 *      $Log: except.h $
 *      Revision 1.2  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.1  1996/08/04 17:28:17  dz
 *      Initial revision
 *
 *
 *
\*/

#ifndef EXCEPT_H
#define EXCEPT_H

//#include "style.h"
#include "strng.h"
//#include "stdlib.h"

//class string;

class Empty_Ex
    {
    };

class Nomem_Ex : public Empty_Ex
    {
    };

// Eceptions

class General_Ex : public Empty_Ex
    {
    public:
        string   where, what, why;

        void print() const;
    };

class Ex_Abort : public General_Ex
    {
    public:
        Ex_Abort( const char* wh )
            {
            where = wh;
            what = "operation aborted";
            why = "";
            }
    };


class Ex_EOF : public General_Ex
    {
    public:
        Ex_EOF( const char* wh )
            {
            where = wh;
            what = "EOF";
            why = "";
            }
        Ex_EOF( const char* wh,  const char* wy )
            {
            where = wh;
            what = "EOF";
            why = wy;
            }
    };


class Ex_NoMemory : public General_Ex
    {
    public:
        Ex_NoMemory( const char* wh )
            {
            where = wh;
            what = "out of memory";
            why = "";
            }
    };


class Ex_Fail : public General_Ex
    {
    public:
        Ex_Fail( const char* wh, const char* wa, const char* wy )
            {
            where = wh;
            what = wa;
            why = wy;
            }
    };

class Ex_Arg : public General_Ex
    {
    public:
        Ex_Arg( const char* wh, const char* wy )
            {
            where = wh;
            what = "Invalid argument";
            why = wy;
            }

        Ex_Arg( const char* wh, const char* wa, const char* wy )
            {
            where = wh;
            what = wa;
            why = wy;
            }
    };

class Ex_Errno : public General_Ex
    {
    public:
        Ex_Errno( const char* wh, const char* wa, long e )
            {
            where = wh;
            what = wa;
            char es[100];
            sprintf( es, "%ld", e );
            why = es;
            }
    };


/*
class Ex_Disconnect : public General_Ex
    {
    public:
        Ex_Disconnect( const char* wh )
            {
            where = wh;
            what = "channel disconnect";
            why = "";
            }
    };
*/

#endif // EXCEPT_H




