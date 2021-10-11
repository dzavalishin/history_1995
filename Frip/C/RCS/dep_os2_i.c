head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@Version 25
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	OS Dependent Thingies - OS/2
 *
 *      $Log: dep_os2_i.c $
 *      Revision 1.2  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.1  1996/08/04 08:18:43  dz
 *      Initial revision
 *
 *      Revision 1.2  1996/01/14 01:06:31  dz
 *      beta 14
 *
 *      Revision 1.1  1996/01/07 20:21:01  dz
 *      Initial revision
 *
 *
 *
\*/

#include <os2.h>


#if 0
#define INCL_KBD
#include <os2.h>
#include <stdio.h>

kbd_binary_mode()
{
    KBDINFO ki;
    KbdGetStatus(&ki, 0);

    ki.fsMask &= ~KEYBOARD_ASCII_MODE;
    ki.fsMask |= KEYBOARD_BINARY_MODE;

    KbdSetStatus(&ki, 0);

    printf("Press ESC twice for exit...\n\n");

    INT     escTwice = 0;

    for (INT done = 0; !done;)
    {
        KBDKEYINFO kki;
        KbdCharIn(&kki, IO_WAIT, 0);

        if (kki.chChar == '\x1b')
        {
            if (escTwice++)
            {
                ++done;
                continue;
            }
        }
        else
            escTwice = 0;

        printf("char: '%c', ascii: %02X\n", kki.chChar, kki.chChar);
    }
    return 0;
}

#endif




#define SYSDEP_DO_COPY
static void Do_Copy( string from, string to )
    {
    APIRET rc = DosCopy( from, to, 0 );
    if( NO_ERROR != rc  ) throw Ex_Errno( "Do_Copy", "DosCopy error", rc );
    }


#if 0
bool frip_rename( string from, string to )
    {
    if( rename( from, to ) )
        {
        if( NO_ERROR != DosCopy( from, to, 0 ) )
            {
            Error("Can't rename or copy "+from+" to "+to);
            return Err;
            }
        if( unlink( from ) )
            {
            Error("Can't delete "+from);
            return Err;
            }
        }
    return Ok;
    }

bool frip_copy( string from, string to )
    {
    if( NO_ERROR != DosCopy( from, to, 0 ) )
        {
        Error("Can't rename or copy "+from+" to "+to);
        return Err;
        }
    return Ok;
    }
#endif






@


1.2
log
@Exceptions added, not compiled
@
text
@d8 3
d75 1
a75 1
    APIRET rc = DosCopy( from, to, 0 )
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: dep_os2.ic $
d70 1
a70 1
static bool Do_Copy( string from, string to )
d72 2
a73 3
    if( NO_ERROR != DosCopy( from, to, 0 ) )
        return Err;
    return Ok;
@
