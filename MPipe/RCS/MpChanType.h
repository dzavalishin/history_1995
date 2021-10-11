head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	96.07.09.16.03.58;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.07.09.02.12.58;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.07.07.10.01.14;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@debugging depacketizer
@
text
@/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: Channel types
 *
 *      $Log: MpChanType.h $
 *      Revision 1.2  1996/07/09 02:12:58  dz
 *      Debugging has begone
 *
 *      Revision 1.1  1996/07/07 10:01:14  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#include "MpUtil.h"

class MP_Channel
    {
    protected:
        int     ch_no;
    public:
        //MP_Channel( int num ) : ch_no(num) {}
        
        int     number() const { return ch_no; }
        virtual const char *type_name() const = 0;
    };

// Receiving channel
class MP_RChannel : public MP_Channel
    {
    protected:
          //MP_RChannel() {}
    public:
        ~MP_RChannel() {}
        //MP_RChannel( int num ) : MP_Channel(num) {}
        
        // Called by multiplexer to pass received data
        virtual void receive( const char *data, int len ) = 0;
    };

// Sending one
class MP_SChannel : public MP_Channel
    {
    protected:
        MP_Sem &s_ready; // must be kicked eventually by schannel if it returned 0 from send lasttime
        int     prio;
        //MP_SChannel() {}
        MP_SChannel( MP_Sem &sr, int _p ) : s_ready(sr), prio(_p) {}
    public:
        ~MP_SChannel() {}
        //MP_SChannel( int num, MP_Sem &sender_ready ) : MP_Channel(num) {}
    
        // Called by multiplexer to get data to send
        virtual int send( const char *data, int len ) = 0;
    };


// -------------------------------------------------------------------------

class MP_Channels;

// Sending side of control channel
class MP_SC_Control : public MP_SChannel
    {
    private:
        MP_Channels & chan;
    public:
        MP_SC_Control( int num, MP_Sem &sr, MP_Channels &ch )
            : chan(ch), MP_SChannel(sr, 255) // max prio
            { 
            ch_no = num;
            if(ch_no > 1) throw Mp_Ex_Fail("MP_SC_Control","channel > 1",""); 
            }
        virtual const char *type_name() const;
        virtual int send( const char *data, int len );
    };

// Receiving side of control channel
class MP_RC_Control : public MP_RChannel
    {
    private:
        MP_Channels & chan;
    public:
        MP_RC_Control( int num, MP_Channels &ch  ) : chan(ch) 
            { ch_no = num; }
        virtual const char *type_name() const;
        virtual void receive( const char *data, int len );
    };




// -------------------------------------------------------------------------


// Sending side of main channel
class MP_SC_Main : public MP_SChannel
    {
    public:
        MP_SC_Main( int num, MP_Sem &sr, int prio ) : MP_SChannel(sr, prio)
            { 
            ch_no = num;
            if(ch_no < 2 || ch_no > 3) 
                throw Mp_Ex_Fail("MP_SC_Main","channel is not 2 or 3","");
            }
        virtual const char *type_name() const;
        virtual int send( const char *data, int len );
    };

// Receiving side of main channel
class MP_RC_Main : public MP_RChannel
    {
    public:
        MP_RC_Main( int num ) { ch_no = num; }
        virtual const char *type_name() const;
        virtual void receive( const char *data, int len );
    };




// -------------------------------------------------------------------------


// Sending side of chat channel
class MP_SC_Chat : public MP_SChannel
    {
    public:
        MP_SC_Chat( int num, MP_Sem &sr, int prio ) : MP_SChannel(sr,prio)
        { ch_no = num; }
        virtual const char *type_name() const;
        virtual int send( const char *data, int len );
    };

// Receiving side of chat channel
class MP_RC_Chat : public MP_RChannel
    {
    public:
        MP_RC_Chat( int num ) { ch_no = num; }
        virtual const char *type_name() const;
        virtual void receive( const char *data, int len );
    };




// -------------------------------------------------------------------------


// Sending side of file channel
class MP_SC_File : public MP_SChannel
    {
    const char *fname;
    public:
        MP_SC_File( int num, MP_Sem &sr, int prio, const char *name ) 
            : MP_SChannel(sr,prio)
        { ch_no = num; fname = name; }
        virtual const char *type_name() const;
        virtual int send( const char *data, int len );
    };

// Receiving side of file channel
class MP_RC_File : public MP_RChannel
    {
    const char *fname;
    public:
        MP_RC_File( int num, const char *name = NULL ) { ch_no = num; fname = name; }
        virtual const char *type_name() const;
        virtual void receive( const char *data, int len );
    };




@


1.2
log
@Debugging has begone
@
text
@d8 3
d19 1
a19 1
#include <MpUtil.h>
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: MpChan.h $
d16 1
d33 1
a33 1
        //MP_RChannel() {}
d35 1
d47 1
d49 1
a49 1
        MP_SChannel( MP_Sem &sr ) : s_ready(sr) {}
d51 1
d70 1
a70 1
            : chan(ch), MP_SChannel(sr)
d101 1
a101 1
        MP_SC_Main( int num, MP_Sem &sr ) : MP_SChannel(sr)
d130 1
a130 1
        MP_SC_Chat( int num, MP_Sem &sr ) : MP_SChannel(sr)
d154 1
d156 3
a158 3
        MP_SC_File( int num, MP_Sem &sr, const char *name ) 
            : MP_SChannel(sr)
        { ch_no = num; }
d166 1
d168 1
a168 1
        MP_RC_File( int num ) { ch_no = num; }
@
