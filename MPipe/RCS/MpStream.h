head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	96.07.07.10.01.14;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.07.02.01.55.57;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.06.30.16.11.38;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
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
 *      $Log: MpStream.h $
 *      Revision 1.2  1996/07/02 01:55:57  dz
 *      writing
 *
 *      Revision 1.1  1996/06/30 16:11:38  dz
 *      Initial revision
 *
 *
 *
 *
\*/


#ifndef MPSTREAM_H
#define MPSTREAM_H

// MPipe stream control


// Basic Mpipe functionality is a bidirectional binary stream.
// File transfer and any other operatins are performed
// by sending stream package, which contains
// type name \0, length in ascii \0 and corresponding data.
// Type name is not case sensitive


const char *  MP_T_Chan   = "Chan";   // Channel allocation
const char *  MP_T_COff   = "COff";   // Channel deallocation
const char *  MP_T_File   = "File";   // File transfer
const char *  MP_T_FAnn   = "FAnn";   // File transfer announce
const char *  MP_T_FRec   = "FRec";   // File receipt
const char *  MP_T_FReq   = "FReq";   // File request
const char *  MP_T_Host   = "Host";   // For use in EMSI-less operation - address, passwords, etc
const char *  MP_T_Info   = "Info";   // Host Info - free space, date/time, etc - addition to id
const char *  MP_T_EMSI   = "EMSI";   // Just pure EMSI_DAT - instead of previous
const char *  MP_T_Rest   = "Rest";   // File transmission restart info
const char *  MP_T_ReAl   = "ReAl";   // Resolved Exlers. Hm..? Oh no, sorry - Resolves Aliases, sure.

const char *  MP_T_Over   = "Over";   // Means "I'm done, waiting just for you"
const char *  MP_T_Turn   = "Turn";   // Means "end of my turn, now you go" in case MP_O_Asymm is negotiated




// NB!
//
// All the names here are sent in Unicode if MP_O_Unicode negotiated

// NB!
//
// No field except for file data can be larger that half the window size!


// Info:
//
// header, unix time of this host \0, free space (worst possible dest!) \0,
// { El. Type \0, [name] \0, Len, Data, }
//
// Common types: OS, OPNAME, OPEMAIL, MAILER
//
// For example: 
//    "OS\0", "\0", 13, "OS/2 Warp 3.0"
//    "OPNAME\0", "\0", ??, "Dmitry Zavalishin"
//    "OPEMAIL\0", "INET\0", ??, "dz@@phantom.ru"
//    "OPEMAIL\0", "FIDO\0", ??, "2:5020/32"
//    "MAILER\0", "\0", ??, "The Brake! V1.0.g701/0004"


// File transfer announce:
//
// header, { unix attrib record, long name \0, 8.3 name \0, }
//
// Used to inform receiver about file we going to send next.
// More than one such package can be sent.
// Receiver can reply to this stuff with receipt and force
// us to skip file transmission even before we started


// File transfer:
//
// header, unix attrib record, long name \0, 8.3 name \0,
// { El. Type \0, [name] \0, Len, Data, }
//
// Common types: ACL, EA, DATA
//
// For example: 
//    "DATA\0", "resource fork\0", 135, <135 bytes of Mac res. fork>
//    "DATA\0", "mine\0", 3000, <3000 bytes of NT file substream named "mine">
//    "DATA\0", "\0", 100, <100 bytes of unix, DOS, OS/2 file>
//
// If element size is not known at the moment of transfer it is sent as a
// separate stream of type File (see below). In this case len is set to 0,
// and data contains channel number in ascii terminated by \0.
// All the large parst should be sent as a separate stream to make sure
// transmission is easily abortable.

// Freq:
//
// header, unix time for update or 0 for freq \0, long name \0, 8.3 name \0,
// [password] \0
//
// there may be either file name only or complete or incomplete
// path given. It's up to the sender to specify


// Chan:
//
// header, number \0, type \0, prio \0, [encryption key _name_] \0
//
// Channels - types:
//    0   - control (channel creation, receipts, hangup)
//    1   - main (main data link, allways allocated first)
//    ?   - chat (for Fido mailer sysops)
//    ?   - IP (tcp/ip encapsulation)
//    ?   - BBS/rlogin (access to BBS during file transfer in Fido mailers)
//    ?   - Freq (for concurrent freq transfer and better prio control)
//    ?   - Netmail (for concurrent netmail transfer and better prio control)
//    ?   - File (for sending file which size is not known.)
//
// Prio: from 0 to 254, usual is 128.
//
// Key Name: used by both sides as index into their key storage or
//           as a ticket to obtain key from some server, NEVER as key itself,
//           NEVER as a seed for a key generator.
//
// channel 0 prio is 255.
//    

// COff:
//
// header, number \0, reason \0
//
// Reasons:
//    Ok    - all done
//    Abort - one side asked to abort connection




// FRec
//
// header, long name just as in File \0, result \0
//
// Results:
//    Ok      - file received, allright. (May be deleted, if k/s)
//    NoSpace - file can't be received due to disk space limit
//    Fail    - other (unknown) failure
//    Skip    - there was some reason to skip the file, DON'T 
//              send now or next time

// FRestart
//
// header, long name \0, size we got \0, info \0
//
// Info:
//    Restart - go send it from given pos
//    Keep    - Do not send now, keep for following session
//    Skip    - there was some reason to skip the file, DON'T resend next time

// ReAl
//
// header, [info] \0, { freq \0, { resolved as name \0, rinfo \0, } \0 }
//
// Sent in responce to the freq to inform freq'ing
// which request was treated as alias and which real
// file names correspond to it.
//
// info - common info about this freq, system freq policy, etc

// Turn
//
// Header
//
// Sent by the side which is current sender in asymmetric transfer
// to pass "right to send" to the other side. It is possible to
// get it back. Side, which received "Turn" and has nothing
// to send right now should send "Over", not "Turn".
// 

// Over
//
// Header
//
// Sent by the side which has nothing to send, and going to
// hang up as soon as it will receive Over from other side
//
// It is recommended to send Turn, not Over, on a _symmetric_
// channel if other side is still sending something to you, but
// you have nothing to send back.
// This will let you check periodically for a new files to
// send and resume sending if new files are found for sending.
//
// NB!
//
// You just _have_ to reply with Over if you got Turn or Over and
// have no files to send NOW. It's up to you, whether count
// chat in process as file, or not, but, in general,
// any open channel except for 0/1 can be counted for file transfer
// here.



#endif // MPSTREAM_H

@


1.2
log
@writing
@
text
@d8 3
d36 1
d43 4
d75 9
d114 1
a114 1
// header, number \0, type \0, winsize \0, prio \0, [encryption key _name_] \0
d117 8
a124 8
//    0/1 - control (channel creation, receipts, hangup)
//    2/3 - main (main data link, allways allocated first)
//    ?/? - chat (for Fido mailer sysops)
//    ?/? - IP (tcp/ip encapsulation)
//    ?/? - BBS/rlogin (access to BBS during file transfer in Fido mailers)
//    ?/? - Freq (for concurrent freq transfer and better prio control)
//    ?/? - Netmail (for concurrent netmail transfer and better prio control)
//    ?/? - File (for sending file which size is not known.)
d132 2
a133 2
// channel 0/1 setup is:
//    0/1, control, 32Kbytes, 255, ""
d154 2
a155 1
//    Skip    - there was some reason to skip the file, DON'T resend next time
d166 19
d186 20
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: Recode.c $
d17 2
d33 1
a33 1
const char *  MP_T_File   = "FRec";   // File receipt
d35 2
a36 2
const char *  MP_T_HostId = "HostId"; // For use in EMSI-less operation - address, passwords, etc
const char *  MP_T_HostId = "Info";   // Host Info - free space, date/time, etc - pair to id
d38 1
a38 1
const char *  MP_T_FRes   = "Rest";   // File transmission restart info
d151 2
@
