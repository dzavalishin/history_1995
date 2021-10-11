head	1.7;
access;
symbols;
locks;
comment	@ * @;


1.7
date	96.07.10.18.27.24;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.07.10.11.31.06;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.07.09.02.12.58;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.07.02.04.33.28;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.07.02.01.55.57;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.07.01.14.46.23;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.06.30.22.58.25;	author dz;	state Exp;
branches;
next	;


desc
@@


1.7
log
@something goes through, framer/frame classes
are work somehow.
@
text
@/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: data tarnsfer window classes impl.
 *
 *      $Log: MpWin.h $
 *
 *
\*/

#include "MpUtil.h"
#include "MpWin.h"



MP_Window::MP_Window( int size )
    {
    wbuf = new char [size+1]; // +1 for any case :)
    if( wbuf == NULL ) throw(Mp_Ex_NoMemory("MP_Window::MP_Window"));
    wf.off = 0;
    wf.size = 0; // Window is empty yet
    
    wsize = size;
    aborted = No;
    }
        
MP_Window::~MP_Window()
    {
    delete [] wbuf;
    }
        
MP_Window::abort()
    {
    aborted = Yes;
    client_data_ready.post();
    }



// -------------------------------------------------------------------------





bool MP_Receive_Window::completed_up_to(MP_Off_t pos)
    {
    MP_SLKey key(lock);
    // return Yes if we received everything up to
    // pos _AND_ passed it all to the client
    
    if( wf.off >= pos && wf.size == 0 ) return Yes;
    
    return No;
    }

MP_Frame MP_Receive_Window::put( const MP_Frame &_f, const char* data )
    {
    MP_SLKey key(lock);

    MP_Frame ack = _f, f = _f;

      // Packet is completely below OR ABOVE the buffer??
      // Ack everything up to the begin of the buffer, and do nothing else
      // BUG. here will be good to ack lower part of window, if it marked
      // as received.
    if( f.end() <= wf.off || f.off >= (wf.off+wsize) )
        {
        ack.off = 0;
        ack.size = wf.off;
        return ack;
        }
    
      // We got a packet which begins on the lower border of window
      // or below it? Ack all the data below window together with it.
    if( f.off <= wf.off )
        {
        ack.size = ack.end();
        ack.off = 0;
        }

      // Some part of it is below our window?
      // Cut it off
    if( f.off < wf.off )
        {
        int cut = wf.off-f.off;
        ack.size -= cut;
        ack.off += cut;
        f.size -= cut;
        f.off += cut;
        data += cut;
        }

    // Some part of it is above our window?

    if( f.end() > (wf.off+wsize) )
        {
        int cut = f.end() - (wf.off+wsize);
        f.size -= cut;
        ack.size -= cut;
        }

    MP_Off pos = f.off - wf.off; // offset in a window
    memcpy( wbuf+pos, data, f.size );
    wf.size = wf.size < pos+f.size ? pos+f.size : wf.size;
    mark_as_received(f);

      // in fact, it is only needed to post it when
      // data on the lower border of win is received, but
      // we'll make sure we haven't missed something... :)
    client_data_ready.post();
    
    return ack;
    }




// Client (reliable) side.
int MP_Receive_Window::read( char *data, int dsize )   // Blocking!!
    {
    if( dsize <= 0 ) return 0;
    if( aborted ) throw(Mp_Ex_Abort("MP_Receive_Window::read"));
    
    int done = 0;
    while(1)
        {
            { // key-locked area
            MP_SLKey key(lock);
            if( !frames.empty() )
                {
                MP_WFrame f = frames.first();
                if( f.off == wf.off && f.prob.is_100_percent() )
                    {
                    if( f.size > dsize ) f.size = dsize;
                    dsize -= f.size;
                    done += f.size;
                    memcpy( data, wbuf, f.size );
                    data += f.size;

                    memcpy( wbuf, wbuf+f.size, wsize-f.size );
                    wf.off += f.size;
                    wf.size -= f.size;

                    if( wf.size < 0 )
                        throw(Mp_Ex_Fail("MP_Receive_Window::read","wf.size < 0",""));

                    frames -= f;
                    }
                }
            } // end of key-locked area

        if( dsize == 0 || aborted ) return done;
        
        client_data_ready.sleep();
        }
    }


// -------------------------------------------------------------------------


// Client (reliable) side.
int MP_Send_Window::write( char *data, int dsize )   // Blocking!!
    {
    if( dsize <= 0 ) return 0; // Or treat as EOF??
    if( aborted ) throw(Mp_Ex_Abort("MP_Send_Window::write"));

    //MP_Message("MP_Send_Window::write( \"%.*s\", %d )", dsize, data, dsize );
    
    int done = 0;
    while(1)
        {
            { // key-locked area
            MP_SLKey key(lock);
            if( wf.size < wsize )
                {
                int len = wsize - wf.size;
                if( len > dsize ) len = dsize;
                dsize -= len;
                done += len;
                memcpy( wbuf+wf.size, data, len );
                MP_Message("MP_Send_Window::write: put at offset %d", wf.size );
                mark_as_non_received( MP_Frame(wf.size,len));
                wf.size += len; // Not before!
                }
            } // end of key-locked area

        if( dsize == 0 || aborted ) return done;

        client_data_ready.sleep();
        }
    }





@


1.6
log
@MP_Receive_Window::put - incoming packet validation
redone. Now checking and processing correctly all
possible cases.
@
text
@d1 10
d49 1
d58 1
a58 1
MP_Frame MP_Receive_Window::put( const MP_Frame &f, const char* data )
d60 1
a60 1
      //if( !wf.contains( f ) ) throw(Mp_Ex_Fail("MP_Receive_Window::put","block is out of window",""));
d62 1
a62 1
    MP_Frame ack = f;
d68 1
a68 1
    if( f.end() <= wf.off || f.off >= wend )
d71 1
a71 1
        ack.size = fw.off;
d97 1
a97 1
    if( f.end() > wend )
d99 1
a99 1
        int cut = f.end() - wend;
d106 1
d108 7
a117 12
    // Ask sender to skip this many bytes from the current start of window.
    // Upon the return from this method window must start from new pos.
    // How:
    // Send ack for all the data from 0 to this pos, move window
    // to a new start correcting Framer state, return.
    // Note that state machine must ack all the data up to the window
    // start on the receprion of data, which belongs to the under-the-window
    // part of stream. These acks are not depend on history and not TOO
    // critical, so it seems to be wise to send 'em no more frequent,
    // than normal data turnaround time. See MpStat.h
//void MP_Receive_Window::skip( MP_Off_t );

d140 1
@


1.5
log
@Debugging has begone
@
text
@d47 1
a47 1
void MP_Receive_Window::put( const MP_Frame &f, const char* data )
d49 46
a94 3
    if( !wf.contains( f ) ) throw(Mp_Ex_Fail("MP_Receive_Window::put","block is out of window",""));
    MP_Off_t woff = f.off - wf.off; // offset in a window
    memcpy( wbuf+woff, data, f.size );
d161 2
d176 3
@


1.4
log
@writing
@
text
@d25 1
a25 1
    aborted = No;
d73 1
a73 1
    if( dsize <= 0 ) return 0; // Or treat as EOF??
@


1.3
log
@writing
@
text
@a4 122
const MP_Prob MP_75_percent(75);


bool MP_Frame::contains( const MP_Frame &f ) const
    {
    return ( off <= f.off && end() >= f.end() ) ? Yes : No;
    }

bool MP_Frame::has_common_point_with( const MP_Frame &f ) const
    {
    return ( off < f.end() || f.off < end() ) ? Yes : No;
    }



MP_Frame & MP_Frame::operator += ( const MP_Frame &f )
    {
    if( off+size < f.off && f.off+f.size < off )
        throw(Mp_Ex_Fail("MP_Frame::operator +=","Frames are too distant",""));
    
    if( f.off < off ) off = f.off;
    if( f.off+f.size > off+size ) size = f.off+f.size - off;
    return *this;
    }

MP_WFrame & MP_WFrame::operator += ( const MP_WFrame &f )
    {
    ((MP_Frame*)this)->operator += ( (const MP_Frame&)f );
    
    if( f.sent > sent ) sent = f.sent;
    if( f.prob < prob ) prob = f.prob;
    return *this;
    }

MP_WFrame  MP_WFrame::operator +  ( const MP_WFrame &f1 )
    { 
    MP_WFrame f(*this); 
    f += f1; 
    return f; 
    }

// -------------------------------------------------------------------------

// Split frames[i] at position off, return index of leftmost part
int MP_Framer::split( int i, MP_Off off )
    {
    if( frames[i].off > off || frames[i].end() < off )
        throw(Mp_Ex_Fail("MP_Framer::split","Inconsistent args",""));

    MP_WFrame newf = frames[i];
    newf.size = off-newf.off;

    frames[i].size -= off-frames[i].off;
    frames[i].off = off;

    frames.insert( frames.begin()+i, newf );
    return i;
    }


// Effect:
//     rip off this frame, keeping it in a link.
//     Don't extend anything, just divide/join ops allowed.

// Return:
//     index.

// Prerequisites:
//     Element at index known for sure to contain some
//     part of requested range.
//     (Or we'll throw an exception into your face! :)


// Notes:
//     If resulting frame is combined from two frames,
//     it's probability is set to lowest of both and time is
//     set to highest of both. See MP_WFrame operator +.
//
//     This method is a worker for others.

int MP_Framer::rip_off( MP_Frame f, int i )
    {
    throw(Mp_Ex_Fail("MP_Framer::rip_off","Not written",""));
    
    
    if( !f.has_common_point_with(frames[i]) )
        throw(Mp_Ex_Fail("MP_Framer::rip_off","Inconsistent args",""));

    if( frames[i].off < f.off )
        {
        // Frame in a framer begins before our rip-off, so
        // first divide it into a pair.
        i = split( i, f.off );
        i++;
        }
    
    // Here we sure that frames[i].off >= f.off
    
    if( frames[i].end() > f.end() )
        {
        // Frame in a framer ends after our rip-off, so
        // divide it into a pair.
        i = split( i, f.end() );
        // here we know that frames[i] has no
        // parts, that extend beyond the borders
        // of f. So return result.
        return i;
        }
    
    if( frames[i+1].off < f.end() && frames[i+1].end() > f.end() )
        {
        // Next frame in a framer contains end of our rip-off, so
        // divide it into a pair too.
        int next = split( i+1, f.end() );
        return i;
        }
    
    

    }

// -------------------------------------------------------------------------
a139 30




// -------------------------------------------------------------------------

MP_Prob & MP_Prob::operator += ( const MP_Prob &p )
    {
    // Summ of probablities in mpipe's way :)
    
    // Have 100%? Result is surely 100%
    if( prob_v >= MP_Prob_Max || p.prob_v >= MP_Prob_Max )
        {
        prob_v = MP_Prob_Max;
        return *this;
        }
    
    prob_v = (prob_v+p.prob_v) << 1; // (a+b) / 2
    
    // Didn't had 100%? Result is surely NOT 100%
    if( prob_v >= MP_Prob_Max ) prob_v = MP_Prob_Max - 1;
    return *this;
    }

MP_Prob MP_Prob::operator + ( const MP_Prob &p ) const 
    { 
    MP_Prob pp = *this; 
    pp += p; 
    return pp; 
    }
@


1.2
log
@writing framer methods
@
text
@d5 14
d27 1
a33 5
    //if( off+size < f.off && f.off+f.size < off )
    //    throw(Mp_Ex_Fail("MP_WFrame::operator +=","Frames are too distant",""));
    //if( f.off < off ) off = f.off;
    //if( f.off+f.size > off+size ) size = f.off+f.size - off;
    
d36 1
d65 6
a70 8
// rip off this frame, keeping it in a link. Element at index known
// for sure to contain some part of requested range. 
// (Or we'll throw it into your face! :)
// Return index.
// Note that if resulting frame is combined from two frames,
// it's probability is set to lowest of both and time is
// set to highest of both. See MP_WFrame operator +.
// this is a worker for other methods.
d72 14
a85 1
int MP_Framer::rip_off( MP_Frame f, index i )
d87 3
d97 1
a97 7
        MP_WFrame newf = frames[i];
        newf.size = f.off-newf.off;
        
        frames[i].size -= f.off-frames[i].off;
        frames[i].off = f.off;
        
        frames.insert( frames.begin()+i, newf );
d101 1
a101 1
    // Here we guaranteed that frames[i].off >= f.off
d107 13
a119 9
        MP_WFrame newf = frames[i];
        newf.size = f.off-newf.off;
        
        frames[i].size -= f.off-frames[i].off;
        frames[i].off = f.off;
        
        frames.insert( frames.begin()+i, newf );
        i++;
        
d123 1
d201 23
a223 19
        lock.lock();
        MP_Frame f = frames.first();
        if( f.off == wf.off && f.prob.is_100_percent() )
            {
            if( f.size > dsize ) f.size = dsize;
            dsize -= f.size;
            done += f.size;
            memcpy( data, wbuf, f.size );
            
            memcpy( wbuf, wbuf+f.size, wsize-f.size );
            wf.off += f.size;
            wf.size -= f.size;
            
            if( wf.size < 0 )
                throw(Mp_Ex_Fail("MP_Receive_Window::read","wf.size < 0",""));
            
            frames -= f;
            }
        lock.unlock();
d244 12
a255 12
        
        lock.lock();
        if( wf.size < wsize )
            {
            int len = wsize - wf.size;
            if( len > dsize ) len = dsize;
            dsize -= len;
            done += len;
            memcpy( wbuf+wf.size, data, len );
            }
        lock.unlock();
        
d257 1
a257 1
        
d266 1
d268 24
@


1.1
log
@Initial revision
@
text
@d2 1
d6 129
a136 1
#error not written
d139 3
d144 97
@
