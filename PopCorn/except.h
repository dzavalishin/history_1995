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
// �������� ���⠫��� �������. �� ᫮������. �� ����� �������,
// �᫨ ����, �� �� ������ - ���� �� ��������.
//

class _Export Fail : public Exception
    {
    public:
        Fail( exc_s a_where, exc_s a_cause, exc_s a_data = "")
            : Exception( a_where, a_cause, a_data )
            { }
    };



//
// ��㬥�� �㭪樨 - �� � �����
//

// �ਬ��:
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
// � �⠯� �� ���ᠭ� ���� ������, ��� ���ᠭ� �� ⠪
//

// �ਬ��:
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
// �� 㤮���⢮७� ���譥� (�� �⭮襭�� � ��ࠬ��) �᫮��� -
// ��� 䠩��, ᡮ��� �᪮��, etc
//

// �ਬ��:
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
// ��ꥪ� �� � ⮬ ���ﭨ�, ����� ����� ��뢠�� ᥩ ��⮤.
//

// �ਬ��:
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
// ����� 䠩��, ��� ����� ������, ����� ᯨ᪠ - � ��饬, 䨭��.
//

// �ਬ��:
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
// �������� ���檨� � ��㤭���०������� �������.
// ���� �����⭮ �⢠���� � �� ��ᢥ稢���.
//

class _Export Error : public Exception
    {
    public:
        Error(exc_s a_where, exc_s a_cause, exc_s a_data )
            : Exception( a_where, a_cause, a_data )
            { }
    };





//
// ��� �����!! ���ࠧ�.
//

// �ਬ��:
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
// ����७��� �訡��. ���ࠧ�.
//

// �ਬ��:
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
// ^Break. �� ��㪠 �������� ⮫쪮 (?) � Stop_U1_On_Signal()
//

// �ਬ��:
//  throw Break( __FILE__ );

class _Export Break : public Error
    {
    public:
        Break(exc_s a_where, exc_s a_cause )
            : Error( a_where, a_cause, "" )
            { }
    };





#endif // EXCEPT_H



