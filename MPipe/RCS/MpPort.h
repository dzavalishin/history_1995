head	1.6;
access;
symbols;
locks;
comment	@ * @;


1.6
date	96.07.09.02.12.58;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.07.07.10.01.14;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.07.02.01.55.57;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.06.30.22.58.25;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.06.30.20.14.53;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.06.30.16.11.38;	author dz;	state Exp;
branches;
next	;


desc
@@


1.6
log
@Debugging has begone
@
text
@/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: port class
 *
 *      $Log: MpPort.h $
 *      Revision 1.5  1996/07/07 10:01:14  dz
 *      *** empty log message ***
 *
 *      Revision 1.4  1996/07/02 01:55:57  dz
 *      writing
 *
 *      Revision 1.3  1996/06/30 22:58:25  dz
 *      update
 *
 *      Revision 1.2  1996/06/30 20:14:53  dz
 *      update
 *
 *      Revision 1.1  1996/06/30 16:11:38  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#ifndef MPPORT_H
#define MPPORT_H

#include "style.h"

#include "MpUtil.h"


class MP_Port
    {
    protected:
        bool         connected_v;
        
    public:
        MP_Port() { connected_v = No; }
        ~MP_Port() { /*hangup();*/ }
        
        // Both read and write are blocking.
        //
        // read is guaranteed to return requested number of bytes
        // or throw exception if 'sure' flag is set.
        virtual int  read( char *data, int size, bool sure = No )  = 0;  
        virtual int  write( char *data, int size ) = 0;
        
        virtual void hangup() = 0;                       // Blocking!!

        virtual bool connected() const { return connected_v; }
    };


class MP_ComPort : public MP_Port
    {
    public:
        MP_ComPort( const char *name );
        ~MP_ComPort() { hangup(); }
            
        virtual int read( char *data, int size, bool sure = No );   // Blocking!!
        virtual int write( char *data, int size );  // Blocking!!
    
        virtual void hangup();                      // Blocking!!
    };

class MP_NPPort : public MP_Port
    {
    public:
        MP_NPPort( const char *name );
            
        virtual int read( char *data, int size, bool sure = No );
        virtual int write( char *data, int size ); 
        virtual void hangup();
    };

class MP_TcpPort : public MP_Port
    {
    public:
        MP_TcpPort( const char *name );
            
        virtual int read( char *data, int size, bool sure = No );  
        virtual int write( char *data, int size );
        virtual void hangup();                
    };

class MP_LoopPort : public MP_Port
    {
    private:
        MP_LoopPort * volatile    pair;
        MP_Sem                    pwait;
        MP_Sem                    rwait;
        MP_Sem                    wwait;
        MP_SpinLock               rlock;
        MP_SpinLock               wlock;
        volatile char *           wdata;
        volatile int              wlen;
            
    public:
        MP_LoopPort( MP_LoopPort *p = NULL) : pair(p), wlen(0) {}
            
        virtual void set_pair( MP_LoopPort &p ); // { pair = p; wait.post(); }
        
        virtual int read( char *data, int size, bool sure = No );
        virtual int write( char *data, int size );
        virtual void hangup();
    };




#endif // MPPORT_H


@


1.5
log
@*** empty log message ***
@
text
@d8 3
d64 1
a64 1
        virtual int read( char *data, int size );   // Blocking!!
d75 3
a77 3
        virtual int read( char *data, int size );   // Blocking!!
        virtual int write( char *data, int size );  // Blocking!!
        virtual void hangup();                      // Blocking!!
d85 3
a87 3
        virtual int read( char *data, int size );   // Blocking!!
        virtual int write( char *data, int size );  // Blocking!!
        virtual void hangup();                      // Blocking!!
d107 3
a109 3
        virtual int read( char *data, int size );   // Blocking!!
        virtual int write( char *data, int size );  // Blocking!!
        virtual void hangup();                      // Blocking!!
@


1.4
log
@writing
@
text
@d8 3
d42 6
a47 2
        virtual int  read( char *data, int size )  = 0;  // Blocking!!
        virtual int  write( char *data, int size ) = 0;  // Blocking!!
@


1.3
log
@update
@
text
@d5 1
a5 1
 *	Module 	:	
d8 3
d27 2
d75 22
@


1.2
log
@update
@
text
@d8 3
d19 4
d76 3
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: Recode.c $
d25 1
d29 3
a31 1
    
d40 1
d45 1
d55 1
d65 1
@
