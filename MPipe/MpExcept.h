/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: util. types, etc
 *
 *      $Log: MpExcept.h $
 *      Revision 1.2  1996/07/07 10:01:14  dz
 *      *** empty log message ***
 *
 *      Revision 1.1  1996/07/06 05:13:48  dz
 *      Initial revision
 *
 *
 *
 *
 *
\*/

#ifndef MPEXCEPT_H
#define MPEXCEPT_H

//#include "style.h"
//#include "strng.h"
//#include "stdlib.h"

class string;



// Eceptions

class Mp_Ex
    {
    public:
        string   where, what, why;
    };

class Mp_Ex_Abort : public Mp_Ex
    {
    public:
        Mp_Ex_Abort( const char* wh )
            {
            where = wh;
            what = "operation aborted";
            why = "";
            }
    };


class Mp_Ex_EOF : public Mp_Ex
    {
    public:
        Mp_Ex_EOF( const char* wh )
            {
            where = wh;
            what = "EOF";
            why = "";
            }
    };


class Mp_Ex_NoMemory : public Mp_Ex
    {
    public:
        Mp_Ex_NoMemory( const char* wh )
            {
            where = wh;
            what = "out of memory";
            why = "";
            }
    };


class Mp_Ex_Fail : public Mp_Ex
    {
    public:
        Mp_Ex_Fail( const char* wh, const char* wa, const char* wy )
            {
            where = wh;
            what = wa;
            why = wy;
            }
    };

class Mp_Ex_Errno : public Mp_Ex
    {
    public:
        Mp_Ex_Errno( const char* wh, const char* wa, long e )
            {
            where = wh;
            what = wa;
            char es[100];
            sprintf( es, "%ld", e );
            why = es;
            }
    };



class Mp_Ex_Disconnect : public Mp_Ex
    {
    public:
        Mp_Ex_Disconnect( const char* wh )
            {
            where = wh;
            what = "channel disconnect";
            why = "";
            }
    };


#endif // MPEXCEPT_H




