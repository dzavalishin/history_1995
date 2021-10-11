head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	96.07.07.10.01.14;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.07.02.01.55.57;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.06.30.22.58.25;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.06.30.16.11.38;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
log
@*** empty log message ***
@
text
@/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	:	
 *
 *      $Log: MpOptions.h $
 *      Revision 1.3  1996/07/02 01:55:57  dz
 *      writing
 *
 *      Revision 1.2  1996/06/30 22:58:25  dz
 *      update
 *
 *      Revision 1.1  1996/06/30 16:11:38  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#ifndef MPOPTIONS_H
#define MPOPTIONS_H


// Пожалуй. надо передавать ЭТО три раза. Первый в смысле
// "я умею, если ты будешь", второй в смысле 
// "я буду, если ты умеешь" и третий -
// "я хочу, чтобы ты делал так"


// Upper bit of option means that it is
// an required option and both sides must turn
// it on if any one requested.

// Other options are turned on only if both sides are requested it


enum MP_Options
    {
    MP_O_GZip    = 0x00000001,    // Using inline gzip compression
    MP_O_Crypt   = 0x00000002,    // Using global crypto-protection, NOT a per-channel one
    
    // btoa overrides uue. default for wakeup/init is UUE only
    MP_O_UUE     = 0x80000004,    // All packets are sent uuencoded
    MP_O_BTOA    = 0x80000008,    // All packets are sent btoa-ed
    
    // do _both_ if both asked!
    MP_O_EMSI    = 0x00000010,    // EMSI handshake required
    MP_O_HostId  = 0x00000020,    // HostId handshake required
    
    MP_O_XON     = 0x80000040,    // Send XON between packets periodically
    MP_O_CRLF    = 0x80000080,    // Requests both sides to send a CRLF
                                  // after every packet
    
    // Reserved 100-800
    
    MP_O_Unicode = 0x00001000,    // Send all the names in Unicode
    MP_O_FN_RSX  = 0x00002000,    // This system is an RSX/VMS style one, treat ";number" as file version number
    MP_O_FN_DOS  = 0x00004000,    // This system is an MSDOS style one, treat "character:" as file drive name
    MP_O_FN_Unix = 0x00008000,    // This system is a Unix style one, filenames are case-dependent
    
    
    MP_O_NoOver  = 0x80010000,    // Channel is not our own, don't overload
    MP_O_Asymm   = 0x80020000,    // Answering side should hold its traffic where possible until
    MP_O_HBT     = 0x80040000,    // Requests both sides to send a HBT packet
                                  // once a minute if no other packets was sent
    
    // Reserved 100000-40000000
    
    MP_O_Last    = 0x80000000    // Not used
    };

const long our_options_on  = MP_O_HBT;
const long our_options_off = MP_O_GZip|MP_O_EMSI|MP_O_HostId|MP_O_Unicode;


#endif // MPOPTIONS_H

@


1.3
log
@writing
@
text
@d8 3
d26 6
d42 1
d44 3
a46 3
    // btoa overrides uue
    MP_O_UUE     = 0x80000002,    // All packets are sent uuencoded
    MP_O_BTOA    = 0x80000002,    // All packets are sent btoa-ed
d49 2
a50 2
    MP_O_EMSI    = 0x00000004,    // EMSI handshake required
    MP_O_HostId  = 0x00000010,    // HostId handshake required
d52 2
a53 2
    MP_O_XON     = 0x80000020,    // Send XON between packets periodically
    MP_O_CRLF    = 0x80000040,    // Requests both sides to send a CRLF 
d55 12
a66 1
    MP_O_HBT     = 0x80000100,    // Requests both sides to send a HBT packet
d69 1
a69 4
    MP_O_Unicode = 0x00000200,    // Send all the names in Unicode

    MP_O_NoOver  = 0x80000400,    // Channel is not our own, don't overload

@


1.2
log
@update
@
text
@d8 3
d19 2
d56 5
a60 2
long our_options_on  = MP_O_HBT;
long our_options_off = MP_O_GZip|MP_O_EMSI|MP_O_HostId|MP_O_Unicode;
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: Recode.c $
d20 3
a22 1
// it on if any one requested
d47 2
d51 2
a52 1

@
