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
date	96.07.10.11.57.27;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.07.10.10.09.28;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.07.09.16.03.58;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.07.09.02.12.58;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.07.02.04.33.28;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.07.02.01.58.48;	author dz;	state Exp;
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
 *	Module 	: Frame, Framer, Probability - implementation
 *
 *      $Log: MpFrame.c $
 *      Revision 1.6  1996/07/10 11:57:27  dz
 *      find/rip/make for fixed
 *
 *
 *
 *
\*/

#include "MpUtil.h"
#include "MpFrame.h"


void MP_Framer::operator -= ( MP_Frame f )
    {
    MP_Message("Framer -= [%d,%d[", f.off, f.end() );
    //int end = frames.size();
    for( int i = 0; i < frames.size(); i++ )
        {
        if( f.contains((MP_Frame&)frames[i])  )
            {
            MP_Message("Framer -= kills el %d, [%d,%d[", i, frames[i].off, frames[i].end() );
            frames.erase(frames.begin()+i);
            i--; // continue process at the same index
            continue;
            }
        
        if( ((MP_Frame&)frames[i]).contains(f) )
            {
            if( frames[i].off != f.off )
                {
                MP_Message("Framer -= splits el %d, [%d,%d[", i, frames[i].off, frames[i].end() );
                  // split frame in two parts, take second one
                i = 1+split( i, f.off );
                }
            if( f.contains((MP_Frame&)frames[i])  )
                {
                MP_Message("Framer -= kills el %d, [%d,%d[", i, frames[i].off, frames[i].end() );
                frames.erase(frames.begin()+i);
                i--; // continue process at the same index
                continue;
                }
            ((MP_Frame&)frames[i]) -= f;
            // By the way, it seems possible to return here, not continue
            continue;
            }
            
        if( frames[i].has_common_point_with(f) )
            {
            ((MP_Frame&)frames[i]) -= f;
            continue;
            }
        }
    dump();
    }



bool MP_Framer::have_non_acked( MP_Frame r )
    {
    for( int i = frames.size(); i--; )
        {
        if( !frames[i].has_common_point_with(r) )
            continue;
        if( !frames[i].prob.is_100_percent() )
            return Yes;
        }
    return No;
    }
        
MP_Prob MP_Framer::lowest_prob()
    {
    MP_Prob p;
    p.set_to_100_percent();
    for( int i = frames.size(); i--; )
        {
        if( frames[i].prob < p )
            p = frames[i].prob;
        }
    return p;
    }
        
MP_WFrame & MP_Framer::next_with_prob(const MP_Prob &p)
    {
    int end = frames.size();

    for( int i = last_pos; i < end; i++ )
        if( frames[i].prob == p || frames[i].prob < p )
            {
            last_pos = i;
            return frames[i];
            }
    for( i = 0; i <= last_pos; i++ )
        if( frames[i].prob == p || frames[i].prob < p )
            {
            last_pos = i;
            return frames[i];
            }
    throw(Mp_Ex_EOF("MP_WFramer::next_with_prob"));
    }









// Split frames[i] at position off, return index of leftmost part
int MP_Framer::split( int i, MP_Off off )
    {
    MP_Message("Framer split( %d, [%d])", i, off );
    dump();
    if( frames[i].off > off || frames[i].end() < off )
        throw(Mp_Ex_Fail("MP_Framer::split","Inconsistent args",""));

    if( frames[i].off == off || frames[i].end() == off )
        {
        MP_Message("Will not split, margin is on interval boundary");
        return i;
        }
    
    MP_WFrame newf = frames[i];
    newf.size = off-newf.off;

    frames[i].size -= off-frames[i].off;
    frames[i].off = off;

    frames.insert( frames.begin()+i, newf );
    dump();
    return i;
    }


// Effect:
//     rip off this frame, keeping it in a link.
//     Don't extend anything, just divide ops are allowed.

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
    //throw(Mp_Ex_Fail("MP_Framer::rip_off","Not written",""));
    
    MP_Message("Framer rip_off( [%d,%d[, %d)", f.off, f.end(), i );
    dump();
    
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
        dump();
        return i;
        }
    
    if( frames[i+1].off < f.end() && frames[i+1].end() > f.end() )
        {
        // Next frame in a framer contains end of our rip-off, so
        // divide it into a pair too.
        int next = split( i+1, f.end() );
        dump();
        return i;
        }
    
    return i;
    }

MP_WFrame & MP_Framer::find_or_rip_or_make( const MP_Frame& f )
    {
    MP_Message("Framer find_or_rip_or_make( [%d,%d[ )", f.off, f.end() );
    dump();

    int end = frames.size();

    for( int i = 0; i < end; i++ )
        {
        if( frames[i].off >= f.end() )
            {
            // We found a frame that follows one we need?
            // Create and insert new
            MP_WFrame wf;
            (MP_Frame&)wf = f;
            wf.prob.set_to_0_percent();
            wf.sent = 0l;
            frames.insert( frames.begin()+i, wf );
            dump();
            return frames[i];
            }
        if( frames[i].has_common_point_with(f) )
            {
            int pos = rip_off( f, i );
            // here we must have either one (at pos)
            // or two (at pos and pos+1) frames, that
            // have common points with f
            if( frames[pos+1].has_common_point_with(f) )
                {
                // go combine 'eir properties
                if( frames[pos+1].prob < frames[pos].prob )
                    frames[pos].prob = frames[pos+1].prob;
                
                if( frames[pos+1].sent > frames[pos].sent )
                    frames[pos].sent = frames[pos+1].sent;
                frames.erase( frames.begin()+pos+1 );
                }
            // here at pos we must have the _only_
            // farme that has common points with f.
            // simply make it occupy range we need.
            frames[pos].off  = f.off;
            frames[pos].size = f.size;
            dump();
            return frames[pos];
            }
        }

      // Create new and add at the end
    MP_WFrame wf;
    (MP_Frame&)wf = f;
    wf.prob.set_to_0_percent();
    wf.sent = 0l;
    frames.push_back( wf );
    dump();
    return *(frames.end()-1);
    }


void MP_Framer::dump()
    {
#if DEBUG
    MP_Message("Framer contents:");
    int end = frames.size();
    
    for( int i = 0; i < end; i++ )
        {
        MP_Message(
                   "%3d    [ %3d,%3d [    p%d",
                   i,
                   frames[i].off, frames[i].end(),
                   frames[i].prob.prob()
                  );
        }
#endif
    }




  // -------------------------------------------------------------------------






bool MP_Frame::contains( const MP_Frame &f ) const
    {
    return ( off <= f.off && end() >= f.end() ) ? Yes : No;
    }

bool MP_Frame::has_common_point_with( const MP_Frame &f ) const
    {
    return ( off >= f.end() || f.off >= end() ) ? No : Yes;
    }



MP_Frame & MP_Frame::operator += ( const MP_Frame &f )
    {
    if( off+size < f.off && f.off+f.size < off )
        throw(Mp_Ex_Fail("MP_Frame::operator +=","Frames are too distant",""));
    
    if( f.off < off ) off = f.off;
    if( f.off+f.size > off+size ) size = f.off+f.size - off;
    return *this;
    }

void MP_Frame::operator -= ( const MP_Frame &f )
    {
    if( !has_common_point_with(f) )
        return;
    
    MP_Message("MP_Frame:: [%d,%d[ -= [%d,%d[", off, end(), f.off, f.end() );
    
    if( f.off > off )
        {
        if( f.end() < end() )
            throw(Mp_Ex_Fail("MP_Frame::operator -=","He's inside of us",""));

        size = f.off - off;
        MP_Message("MP_Frame:: -= result is [%d,%d[", off, end() );
        return;
        }


    if( end() <= f.end() )
        throw(Mp_Ex_Fail("MP_Frame::operator -=","He's contains us",""));

    // he's beginning before us or at the same position
    size = end()-f.end();
    off  = f.end();
    MP_Message("MP_Frame:: -= result is [%d,%d[", off, end() );
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

MP_Frame::~MP_Frame()
    {} // To keep STL happy



// -------------------------------------------------------------------------


const MP_Prob MP_75_percent(75);




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


1.6
log
@find/rip/make for fixed
@
text
@d7 4
a10 1
 *      $Log: MpFrame.h $
a18 3
MP_Frame::~MP_Frame()
    {} // To keep STL happy

d22 3
a24 2
    int end = frames.size();
    for( int i = 0; i++; i < end )
d28 1
d30 1
d36 15
a50 4
            // First, split frame in two parts
            int pos = split( i, f.off );
            // range to cut off is in the second part.
            ((MP_Frame&)frames[pos+1]) -= f;
d60 1
a114 59
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

void MP_Frame::operator -= ( const MP_Frame &f )
    {
    if( !has_common_point_with(f) )
        return;
    
    if( contains(f) )
        throw(Mp_Ex_Fail("MP_Frame::operator -=","He's inside of us",""));
    
    if( f.off > off )
        {
        size = f.off - off;
        return;
        }
    
    size -= f.size;
    off  += f.size;
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
d119 2
d124 6
d137 1
d164 1
a164 1
    throw(Mp_Ex_Fail("MP_Framer::rip_off","Not written",""));
d166 2
d190 1
d199 1
d202 2
a203 1

d208 3
d224 1
d248 1
d259 1
d264 100
d365 1
@


1.5
log
@find_or_rip_or_make returned no
value if win was empty.
@
text
@d1 11
d240 1
a240 12
    if( end == 0 )
        {
          // Create and insert new
        MP_WFrame wf;
        (MP_Frame&)wf = f;
        wf.prob.set_to_0_percent();
        wf.sent = 0l;
        frames.insert( frames.begin(), wf );
        return frames[0];
        }
    
    for( int i = 0; i++; i < end )
d278 7
a284 1
    throw Mp_Abort("MP_Framer::find_or_rip_or_make found nothing to return?");
@


1.4
log
@debugging depacketizer
@
text
@d228 12
d277 2
@


1.3
log
@Debugging has begone
@
text
@a64 1
    static int last_pos = 0;
@


1.2
log
@writing
@
text
@d5 4
@


1.1
log
@Initial revision
@
text
@d3 80
a82 1
#include "MpWin.h"
a83 1
const MP_Prob MP_75_percent(75);
d108 20
d165 1
a165 1
//     Don't extend anything, just divide/join ops allowed.
a218 35
    
    

    }

// -------------------------------------------------------------------------


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
    aborted = No;
    client_data_ready.post();
    }



// -------------------------------------------------------------------------


a219 10


bool MP_Receive_Window::completed_up_to(MP_Off_t pos)
    {
    // return Yes if we received everything up to
    // pos _AND_ passed it all to the client
    
    if( wf.off >= pos && wf.size == 0 ) return Yes;
    
    return No;
d222 1
a222 1
void MP_Receive_Window::put( const MP_Frame &f, const char* data )
d224 2
a225 29
    if( !wf.contains( f ) ) throw(Mp_Ex_Fail("MP_Receive_Window::put","block is out of window",""));
    MP_Off_t woff = f.off - wf.off; // offset in a window
    memcpy( wbuf+woff, data, f.size );
    mark_as_received(f);
    }


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



// Client (reliable) side.
int MP_Receive_Window::read( char *data, int dsize )   // Blocking!!
    {
    if( dsize <= 0 ) return 0; // Or treat as EOF??
    if( aborted ) throw(Mp_Ex_Abort("MP_Receive_Window::read"));
    
    int done = 0;
    while(1)
d227 18
a244 3
            { // key-locked area
            MP_SLKey key(lock);
            if( !frames.empty() )
d246 7
a252 17
                MP_WFrame f = frames.first();
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
d254 7
a260 5
            } // end of key-locked area

        if( dsize == 0 || aborted ) return done;
        
        client_data_ready.sleep();
d267 1
a268 20
// Client (reliable) side.
int MP_Send_Window::write( char *data, int dsize )   // Blocking!!
    {
    if( dsize <= 0 ) return 0; // Or treat as EOF??
    if( aborted ) throw(Mp_Ex_Abort("MP_Send_Window::write"));
    
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
                }
            } // end of key-locked area
a269 5
        if( dsize == 0 || aborted ) return done;

        client_data_ready.sleep();
        }
    }
a270 5




// -------------------------------------------------------------------------
@
