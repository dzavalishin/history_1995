/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	exception info classes
 *
 *      $Log: zexcept.h $
 *      Revision 1.1  1996/09/23 03:19:42  dz
 *      Initial revision
 *
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

#include "zstrng.h"

class zxEmpty_Ex
    {
    };

class zxNoMem : public zxEmpty_Ex
    {
    };

// Eceptions

class zxGeneral_Ex : public zxEmpty_Ex
    {
    public:
        string   where, what, why;

        void print() const;
    };

class zxAbort : public zxGeneral_Ex
    {
    public:
        zxAbort( const string & wh )
            {
            where = wh;
            what = "operation aborted";
            why = "";
            }
    };


class zxEOF : public zxGeneral_Ex
    {
    public:
        zxEOF( const string & wh )
            {
            where = wh;
            what = "EOF";
            why = "";
            }
        zxEOF( const string & wh,  const string & wy )
            {
            where = wh;
            what = "EOF";
            why = wy;
            }
    };


class zxNoMemory : public zxGeneral_Ex
    {
    public:
        zxNoMemory( const string & wh )
            {
            where = wh;
            what = "out of memory";
            why = "";
            }
    };


class zxFail : public zxGeneral_Ex
    {
    public:
        zxFail( const string & wh, const string & wa, const string & wy )
            {
            where = wh;
            what = wa;
            why = wy;
            }
    };

class zxArg : public zxGeneral_Ex
    {
    public:
        zxArg( const string & wh, const string & wy )
            {
            where = wh;
            what = "Invalid argument";
            why = wy;
            }

        zxArg( const string & wh, const string & wa, const string & wy )
            {
            where = wh;
            what = wa;
            why = wy;
            }
    };

class zxErrno : public zxGeneral_Ex
    {
    public:
        zxErrno( const string & wh, const string & wa, long e )
            {
            where = wh;
            what = wa;
            char es[100];
            sprintf( es, "%ld", e );
            why = es;
            }
    };


/*
class zxDisconnect : public zxGeneral_Ex
    {
    public:
        zxDisconnect( const string & wh )
            {
            where = wh;
            what = "channel disconnect";
            why = "";
            }
    };
*/

#endif // EXCEPT_H




