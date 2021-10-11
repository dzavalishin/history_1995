/************************ PopCorn server ***************************\
 *
 *	Copyright (C) 1991-1996 by Dmitry Zavalishin
 *
 *	Module 	:	Exceptions
 *
 *      $Log: except.h $
 *      Revision 1.2  1996/09/17 23:20:43  dz
 *      First completely working POP3/ESMTP version
 *
 *      Revision 1.1  1996/09/13 07:24:48  dz
 *      Initial revision
 *
 *
 *
 *
\*/



#ifndef	EXCEPT_H
#define	EXCEPT_H

#include "strng.h"
//#include "compiler.h"
//#include "log.h"

//typedef const char *   exc_s;
typedef string   exc_s;

class _Export Exception
    {
    public:
        exc_s   where;   // Func. name, file
        exc_s   cause;   // Description of what happened
        exc_s   data;    // Offending data
        
        Exception( exc_s a_where, exc_s a_cause, exc_s a_data )
            { where = a_where; cause = a_cause; data = a_data; }
        
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

class _Export Fail : public Exception
    {
    public:
        Fail( exc_s a_where, exc_s a_cause, exc_s a_data = "")
            : Exception( a_where, a_cause, a_data )
            { }
    };



//
// Аргумент функции - не в кассу
//

// Пример:
//  throw Invalid_Argument
//     ( __FILE__, "Bad address domain part", string(ap) );


class _Export Invalid_Argument : public Fail
    {
    public:
        Invalid_Argument(exc_s a_where, exc_s a_cause, exc_s a_data )
            : Fail( a_where, a_cause, a_data )
            { }
    };

//
// В сетапе не описано нечто важное, или описано не так
//

// Пример:
//  throw Invalid_Setup
//     ( "", "", "" );

class _Export Invalid_Setup : public Fail
    {
    public:
        Invalid_Setup(exc_s a_where, exc_s a_cause, exc_s a_data )
            : Fail( a_where, a_cause, a_data )
            { }
    };

//
// Не удовлетворено внешнее (по отношению к прграмме) условие -
// нет файла, сбойнул сисколл, etc
//

// Пример:
//  throw Invalid_Condition
//     ( __FILE__, "returned error", "" );

class _Export Invalid_Condition : public Fail
    {
    public:
        Invalid_Condition(exc_s a_where, exc_s a_cause, exc_s a_data )
            : Fail( a_where, a_cause, a_data )
            { }
    };

//
// Объект не в том состоянии, когда можно вызывать сей метод.
//

// Пример:
//  throw Invalid_Object
//     ( __FILE__, "object is not ready for operation", "" );

class _Export Invalid_Object : public Fail
    {
    public:
        Invalid_Object(exc_s a_where, exc_s a_cause, exc_s a_data )
            : Fail( a_where, a_cause, a_data )
            { }
    };

//
// Конец файла, нет больше данных, конец списка - в общем, финиш.
//

// Пример:
//  throw XEOF
//     ( __FILE__, "end of file", "" );

class _Export XEOF : public Fail
    {
    public:
        XEOF(exc_s a_where, exc_s a_cause, exc_s a_data )
            : Fail( a_where, a_cause, a_data )
            { }
    };


// --------------------------------------------------------------------------

//
// Подкласс дурацких и труднопереживаемых обломов.
// Лучше аккуратно отвалить и не отсвечивать.
//

class _Export Error : public Exception
    {
    public:
        Error(exc_s a_where, exc_s a_cause, exc_s a_data )
            : Exception( a_where, a_cause, a_data )
            { }
    };





//
// Нет памяти!! Марразм.
//

// Пример:
//  throw Out_Of_Memory
//     ( __FILE__, "go buy large HD", "" );

class _Export Out_Of_Memory : public Error
    {
    public:
        Out_Of_Memory(exc_s a_where )
            : Error( a_where, "Out of memory", "" )
            { }
    };

//
// Внутренняя ошибка. Марразм.
//

// Пример:
//  throw Internal_Error
//     ( __FILE__, "crazy", "" );

class _Export Internal_Error : public Error
    {
    public:
        Internal_Error(exc_s a_where, exc_s a_cause, exc_s a_data )
            : Error( a_where, a_cause, a_data )
            { }
    };


//
// ^Break. Эта штука генерится только (?) в Stop_U1_On_Signal()
//

// Пример:
//  throw Break( __FILE__ );

class _Export Break : public Error
    {
    public:
        Break(exc_s a_where, exc_s a_cause )
            : Error( a_where, a_cause, "" )
            { }
    };





#endif // EXCEPT_H



