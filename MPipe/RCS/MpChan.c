head	1.2;
access;
symbols;
locks;
comment	@ * @;


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


1.2
log
@Debugging has begone
@
text
@/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: Channels impl.
 *
 *      $Log: MpChan.c $
 *      Revision 1.1  1996/07/07 10:01:14  dz
 *      Initial revision
 *
 *
 *
 *
 *
\*/

#include "MpChan.h"


MP_Channels::MP_Channels( bool caller, MP_Port &_p ) 
    : port( _p ), schan( 3, NULL ), rchan( 3, NULL )
    {
    next_schan = 1;
    stop_us = No;
    is_caller = caller;
    
    MP_Message("Initializing channels mx receiver");
    r_tid = MP_Thread( rth_run, this ); // p thread must be running first
    MP_Message("Initializing channels mx sender");
    s_tid = MP_Thread( sth_run, this );
    
    // note that vectors have preallocated slots for these channels -
    // see schan/rchan initialization above.
    schan[0] = new MP_SC_Control( 0, sready, *this );
    rchan[0] = new MP_RC_Control( 0, *this );
    schan[1] = new MP_SC_Main( 1, sready, 127 ); // middle of prio scale
    rchan[1] = new MP_RC_Main( 1 );
    
    sready.post(); // Prevent deadlock
    }
        
MP_Channels::~MP_Channels()
    {
    abort();
    while( r_tid || s_tid )
        MP_Sleep(128);

    while( schan.size() ) { delete schan.back(); schan.pop_back(); }
    while( rchan.size() ) { delete rchan.back(); rchan.pop_back(); }
    }

void MP_Channels::abort()
    {
    stop_us = Yes;
    }
        
bool MP_Channels::schan_used(int ch)
    {
    for( int i = schan.size(); i-- > 0;  )
        if( schan[i]->number() == ch )
            return Yes;
    return No;
    }
        
int MP_Channels::get_new_schan_no()
    {
    int ch;

    do
        {
        ch = ((next_schan++) << 1) + (is_caller ? 1 : 0);
        } while( schan_used(ch) );
    return ch;
    }

// -------------------------------------------------------------------------


bool MP_Channels::try_to_send( int chan )
    {
    //const MP_CH_Chunk = 512; // Switch channels after max. MP_CH_Chunk bytes of data
    const bs = 256;
    char buf[bs+2];
    int len = schan[chan]->send( buf+2, bs );
    if( len == 0 ) return No;
    // BUG!
    // Here we must pass data through gzip compresion
    // and some encryption, if corresponding options
    // are in effect
    
    buf[0] = chan;
    buf[1] = len;
    
    port.write( buf, len+2 );
    
    return Yes;
    }

void MP_Channels::s_thread()     // sender/packer thread
    {
    
    while( !stop_us )
        {
        bool done_one = No;
        do
            {
            int end = schan.size();
            for( int i = 0; i < end; i++ )
                {
                if( try_to_send(i) )
                    {
                    done_one = Yes;
                    break; // Restart from highest prio
                    }
                }
            }
        while( !stop_us && done_one );
                
        // nobody gave us a data block :(
        // wait for them to get some
        sready.sleep();
        }
    
    }

void MP_Channels::r_thread()     // receiver/unpacker thread
    {
    int chan, len;
        
    while( !stop_us )
        {
        const bs = 256;
        char buf[bs];
        
        // read in 'for sure 'mode
        port.read( buf, 2, Yes );

        chan = buf[0];
        len  = buf[1];
        
        // read in 'for sure 'mode
        port.read( buf, len, Yes );
        
        if( chan > rchan.size() )
            {
            MP_Message("Got chunk for a wrong channel number %d", chan );
            continue;
            }
        
        rchan[chan]->receive( buf, len );
        }
    }



// -------------------------------------------------------------------------

void MP_Channels::sort_schannels() // In order of priority
    {
    }


static bool ieq( const char *s1, const char *s2 )
    { return strcmpi(s1, s2) ? No : Yes; }

MP_SChannel & MP_Channels::new_schan( const char* type, int prio, const char* param )
    {
    MP_SChannel *chp;
    int num = get_new_schan_no();
    
    // it is impossible to create one more control channel
    if(      ieq(type, "main" ) ) chp = new MP_SC_Main( num, sready, prio );
    else if( ieq(type, "chat" ) ) chp = new MP_SC_Chat( num, sready, prio );
    else if( ieq(type, "file" ) ) chp = new MP_SC_File ( num, sready, prio, param );
    
    while( schan.size() < num ) // Slot does not exist?
        schan.push_back(NULL);
    
    if( schan[num] != NULL )
        throw Mp_Ex_Fail("MP_Channels::new_schan", "channel number reuse", type );
    
    schan[num] = chp;
    
    sort_schannels(); // In order of priority
    
    return *chp;
    }

MP_RChannel & MP_Channels::new_rchan( const char* type, int num, const char* param )
    {
    MP_RChannel *chp;
    
    // it is impossible to create one more control channel
    if(      ieq(type, "main" ) ) chp = new MP_RC_Main( num );
    else if( ieq(type, "chat" ) ) chp = new MP_RC_Chat( num );
    else if( ieq(type, "file" ) ) chp = new MP_RC_File ( num );

    while( rchan.size() < num ) // Slot does not exist?
        rchan.push_back(NULL);
    
    if( rchan[num] != NULL )
        throw Mp_Ex_Fail("MP_Channels::new_rchan", "channel number reuse", type );
    
    rchan[num] = chp;
    
    return *chp;
    }



@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: MpChan.h $
d21 1
a21 1
    : p( _p ), schan( 3, NULL ), rchan( 3, NULL )
d26 5
a30 3

    r_tid = MP_Thread( rth_run ); // p thread must be running first
    s_tid = MP_Thread( sth_run );
d36 1
a36 1
    schan[1] = new MP_SC_Main( 1, sready );
d39 1
a77 13
MP_SChannel & new_schan( const char* type, int prio, const char* param )
    {
    
    sort_schannels(); // In order of priority
    }

MP_RChannel & new_rchan( const char* type, int prio, const char* param )
    {
    }


// -------------------------------------------------------------------------

d84 1
a84 1
    len = schan[i]->send( buf+2, bs );
d117 1
a117 1
        while( !stop_us && done_one )
d144 1
a144 1
        if( chan > rchan.size )
d152 55
@
