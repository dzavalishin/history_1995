/************************ UUCP to FIDO convertor ***************************\
 *
 *  Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Log module header
 *
 *      $Log: Log.h $
 *      Revision 1.3  1995/11/05 13:52:48  dz
 *      current.
 *
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *         Rev 1.1   07 Jun 1993 17:58:00   dz
 *      update
 *      
 *         Rev 1.0   12 Sep 1991 01:28:50   dz
 *      Initial revision.
 *
 *
\*/


//#ifndef   FIDO_MSG_H
//#include <fido_msg.h>
//#endif

#ifndef LOG_H
#define LOG_H

#include    <stdarg.h>

#ifndef STYLE_H
#include    <style.h>
#endif

enum exit_code_t
    {
    EC_Incorrect_Setup  = 11,
    EC_IO_Error         = 12,
    EC_Out_Of_Memory    = 13,
    EC_User_Error       = 14,
    EC_OS_Error         = 15,
    EC_UU2_Error        = 16,
    EC_Dumb_Programmer  = 17,
    EC_Signal_Caught    = 18,

    EC_Got_Insane       = 32,
    EC_Dunno            = 33
    };

enum error_info_t
    {
    EI_None             = 0,
    EI_Errno            = 1,
    EI_Full             = 0xF
    };


void _Export	set_program_name( const char *pn );


bool _Export    open_log( const char *file_name );

void _Export    log( const char *level, const char *fmt, ... );

void _Export	warning( error_info_t ei, const char *fmt, ... );
void _Export    error( error_info_t ei, const char *fmt, ... );
void _Export    fatal( exit_code_t ec, error_info_t ei, const char *fmt, ... );
void _Export    debug( const char *fmt, ... );

void _Export    out_of_memory( void );


void _Export    track_reset( void );            // Reset track buffer
char _Export    *track_get( void );             // Get data

// /*extern*/ bool		log_debug;


#endif

