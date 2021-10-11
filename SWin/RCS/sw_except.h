head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.09.30.14.18.41;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.09.14.53.22;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.09.01.47.48;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
log
@Before changing event class
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Infinity Soft
 *
 *	Module 	:	Exceptions
 *
 *      $Log: sw_except.h $
 *      Revision 1.3  1996/09/30 14:18:41  dz
 *      log control written
 *
 *      Revision 1.2  1996/09/09 14:53:22  dz
 *      update
 *
 *      Revision 1.1  1996/09/09 01:47:48  dz
 *      Initial revision
 *
 *
 *
 *
\*/



#ifndef	SW_EXCEPT_H
#define	SW_EXCEPT_H

#include "strng.h"
//#include "compiler.h"
//#include "log.h"

//typedef const char *   exc_s;
typedef string   exc_s;

class _Export sw_Exception
    {
    public:
        exc_s   where;   // Func. name, file
        exc_s   cause;   // Description of what happened
        exc_s   data;    // Offending data
        
        sw_Exception( exc_s a_where, exc_s a_cause, exc_s a_data )
            : where(a_where), cause( a_cause ), data( a_data )
            {}

        sw_Exception( exc_s a_where, exc_s a_cause )
            : where( a_where ), cause( a_cause )
            {}
        
        void print_error();
/*
            {
            error
                ( 
                 EI_None, "Exception in %s: %s (%s)",
                 where.c_str(), cause.c_str(), data.c_str()
                );
            }
*/
    };

//
// Подкласс нефатальных обломов. Не сложилось. Не повод вешаться,
// если есть, что еще делать - надо это поделать.
//

class _Export sw_Fail : public sw_Exception
    {
    public:
        sw_Fail( exc_s a_where, exc_s a_cause, exc_s a_data = "")
            : sw_Exception( a_where, a_cause, a_data )
            { }
    };


class _Export sw_Errno : public sw_Exception
    {
    public:
        sw_Errno( const string & wh, const string & wa, long e )
            : sw_Exception( wh, wa )
            {
            char es[100];
            sprintf( es, "%ld", e );
            data = es;
            }
    };

//
// Аргумент функции - не в кассу
//

// Пример:
//  throw sw_Invalid_Argument
//     ( __FILE__, "Bad address domain part", string(ap) );


class _Export sw_Invalid_Argument : public sw_Fail
    {
    public:
        sw_Invalid_Argument(exc_s a_where, exc_s a_cause, exc_s a_data )
            : sw_Fail( a_where, a_cause, a_data )
            { }
    };

//
// В сетапе не описано нечто важное, или описано не так
//

// Пример:
//  throw sw_Invalid_Setup
//     ( "", "", "" );

class _Export sw_Invalid_Setup : public sw_Fail
    {
    public:
        sw_Invalid_Setup(exc_s a_where, exc_s a_cause, exc_s a_data )
            : sw_Fail( a_where, a_cause, a_data )
            { }
    };

//
// Не удовлетворено внешнее (по отношению к прграмме) условие -
// нет файла, сбойнул сисколл, etc
//

// Пример:
//  throw sw_Invalid_Condition
//     ( __FILE__, "returned error", "" );

class _Export sw_Invalid_Condition : public sw_Fail
    {
    public:
        sw_Invalid_Condition(exc_s a_where, exc_s a_cause, exc_s a_data )
            : sw_Fail( a_where, a_cause, a_data )
            { }
    };

//
// Объект не в том состоянии, когда можно вызывать сей метод.
//

// Пример:
//  throw sw_Invalid_Object
//     ( __FILE__, "object is not ready for operation", "" );

class _Export sw_Invalid_Object : public sw_Fail
    {
    public:
        sw_Invalid_Object(exc_s a_where, exc_s a_cause, exc_s a_data )
            : sw_Fail( a_where, a_cause, a_data )
            { }
    };

//
// Конец файла, нет больше данных, конец списка - в общем, финиш.
//

// Пример:
//  throw sw_EOF
//     ( __FILE__, "end of file", "" );

class _Export sw_EOF : public sw_Fail
    {
    public:
        sw_EOF(exc_s a_where, exc_s a_cause, exc_s a_data )
            : sw_Fail( a_where, a_cause, a_data )
            { }
    };


// --------------------------------------------------------------------------

//
// Подкласс дурацких и труднопереживаемых обломов.
// Лучше аккуратно отвалить и не отсвечивать.
//

class _Export sw_Error : public sw_Exception
    {
    public:
        sw_Error(exc_s a_where, exc_s a_cause, exc_s a_data )
            : sw_Exception( a_where, a_cause, a_data )
            { }
    };





//
// Нет памяти!! Марразм.
//

// Пример:
//  throw sw_Out_Of_Memory
//     ( __FILE__, "go buy large HD", "" );

class _Export sw_Out_Of_Memory : public sw_Error
    {
    public:
        sw_Out_Of_Memory(exc_s a_where )
            : sw_Error( a_where, "Out of memory", "" )
            { }
    };

//
// Внутренняя ошибка. Марразм.
//

// Пример:
//  throw sw_Internal_Error
//     ( __FILE__, "crazy", "" );

class _Export sw_Internal_Error : public sw_Error
    {
    public:
        sw_Internal_Error(exc_s a_where, exc_s a_cause, exc_s a_data )
            : sw_Error( a_where, a_cause, a_data )
            { }
    };


//
// ^Break. Эта штука генерится только (?) в Stop_U1_On_Signal()
//

// Пример:
//  throw sw_Break( __FILE__ );

class _Export sw_Break : public sw_Error
    {
    public:
        sw_Break(exc_s a_where, exc_s a_cause )
            : sw_Error( a_where, a_cause, "" )
            { }
    };





#endif // SW_EXCEPT_H



@


1.3
log
@log control written
@
text
@d8 3
@


1.2
log
@update
@
text
@d8 3
d39 6
a44 1
            { where = a_where; cause = a_cause; data = a_data; }
d72 11
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: Except.H $
d155 1
a155 1
class _Export sw_Error : public swception
d159 1
a159 1
            : swception( a_where, a_cause, a_data )
@
