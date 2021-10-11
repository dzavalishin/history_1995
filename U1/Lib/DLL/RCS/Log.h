head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	95.11.05.13.52.48;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.13.11.06.28;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.04.17;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@current.
@
text
@/************************ UUCP to FIDO convertor ***************************\
 *
 *  Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Log module header
 *
 *      $Log: Log.h $
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

bool		log_debug;


#endif

@


1.2
log
@debug
@
text
@d8 3
d76 3
d81 3
@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/uu2/vcs/log.h_v  $
d57 1
a57 1
    EI_Full             = 0xF,
d60 5
a64 1
bool    open_log( const char *file_name );
d66 1
a66 1
void    log( const uchar *level, const uchar *fmt, ... );
d68 4
a71 4
void	warning( error_info_t ei, const char *fmt, ... );
void    error( error_info_t ei, const char *fmt, ... );
void    fatal( exit_code_t ec, error_info_t ei, const char *fmt, ... );
void    debug( const char *fmt, ... );
d73 2
a74 2
void    track_reset( void );            // Reset track buffer
char    *track_get( void );             // Get data
@
