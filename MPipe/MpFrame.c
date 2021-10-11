/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: Frame, Framer, Probability - implementation
 *
 *      $Log: MpFrame.c $
 *      Revision 1.7  1996/07/10 18:27:24  dz
 *      something goes through, framer/frame classes
 *      are work somehow.
 *
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
    //MP_Message("Framer -= [%d,%d[", f.off, f.end() );
    //int end = frames.size();
    for( int i = 0; i < frames.size(); i++ )
        {
        if( f.contains((MP_Frame&)frames[i])  )
            {
            //MP_Message("Framer -= kills el %d, [%d,%d[", i, frames[i].off, frames[i].end() );
            frames.erase(frames.begin()+i);
            i--; // continue process at the same index
            continue;
            }
        
        if( ((MP_Frame&)frames[i]).contains(f) )
            {
            if( frames[i].off != f.off )
                {
                //MP_Message("Framer -= splits el %d, [%d,%d[", i, frames[i].off, frames[i].end() );
                  // split frame in two parts, take second one
                i = 1+split( i, f.off );
                }
            if( f.contains((MP_Frame&)frames[i])  )
                {
                //MP_Message("Framer -= kills el %d, [%d,%d[", i, frames[i].off, frames[i].end() );
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


  // BUG! (Really bug)
  // Sometimes this stuff jumps across frames, leaving non-sent
  // holes behind. It will be better to keep not a frame number in
  // last_pos, but stream offset. Note that receiver should nak
  // any holes it will encounter at the beginning of its window.
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
    //MP_Message("Framer split( %d, [%d])", i, off );
    dump();
    if( frames[i].off > off || frames[i].end() < off )
        throw(Mp_Ex_Fail("MP_Framer::split","Inconsistent args",""));

    if( frames[i].off == off || frames[i].end() == off )
        {
        //MP_Message("Will not split, margin is on interval boundary");
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
    
    //MP_Message("Framer rip_off( [%d,%d[, %d)", f.off, f.end(), i );
    dump();
    
    if( !f.has_common_point_with(frames[i]) )
        throw(Mp_Ex_Fail("MP_Framer::rip_off","Inconsistent args",""));

    if( frames[i].off < f.off )
        {
        //MP_Message("Framer rip_off: frames[i %d].off %d < f.off %d", i, frames[i].off, f.off  );
        // Frame in a framer begins before our rip-off, so
        // first divide it into a pair.
        i = split( i, f.off );
        i++;
        }
    
    // Here we sure that frames[i].off >= f.off
    
    if( frames[i].end() > f.end() )
        {
        //MP_Message("Framer rip_off: frames[i %d].end() %d > f.end() %d", i, frames[i].end(), f.end()  );
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
        //MP_Message("Framer rip_off: frames[i+1].off < f.end() && frames[i+1].end() > f.end()"  );
        // Next frame in a framer contains end of our rip-off, so
        // divide it into a pair too.
        int next = split( i+1, f.end() );
        dump();
        return i;
        }
    
    //MP_Message("Framer rip_off: final return"  );
    return i;
    }

MP_WFrame & MP_Framer::find_or_rip_or_make( const MP_Frame& f )
    {
    //MP_Message("Framer find_or_rip_or_make( [%d,%d[ )", f.off, f.end() );
    dump();

    //int end = frames.size();

    for( int i = 0; i < frames.size(); i++ )
        {
        //MP_Message("find_or_rip_or_make is at frames[%d] [%d,%d[ )", i, frames[i].off, frames[i].end() );
        
        if( frames[i].off >= f.end() )
            {
            //MP_Message("find_or_rip_or_make frames[i].off %d >= f.end() %d", frames[i].off, f.end() );
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
            //MP_Message("find_or_rip_or_make frames[%d].has_common_point_with(f)", i );
            int pos = rip_off( f, i );
              // here we must have either one (at pos)
              // or more (at pos and pos+1...) frames, that
              // have common points with f. Note that because of
              // rip_off, no frames will span thet border of interval
              // we finding_or_ripping_or_makeing.
            while( (pos+1 < frames.size()) && frames[pos+1].has_common_point_with(f) )
                {
                // go combine 'eir properties
                if( frames[pos+1].prob < frames[pos].prob )
                    frames[pos].prob = frames[pos+1].prob;
                
                if( frames[pos+1].sent > frames[pos].sent )
                    frames[pos].sent = frames[pos+1].sent;
                frames.erase( frames.begin()+pos+1 );
                pos = rip_off( f, pos ); // Because there may be more frames to join
                }

            //MP_Message("find_or_rip_or_make setting frames[%d] to f", pos );
            // here at pos we must have the _only_
            // frame that has common points with f.
            // simply make it occupy range we need.
            frames[pos].off  = f.off;
            frames[pos].size = f.size;
            dump();
            return frames[pos];
            }
        }

      // Create new and add at the end
    //MP_Message("find_or_rip_or_make - create new and add at the end" );
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
#if DEBUG && 0
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
    
    //MP_Message("MP_Frame:: [%d,%d[ -= [%d,%d[", off, end(), f.off, f.end() );
    
    if( f.off > off )
        {
        if( f.end() < end() )
            throw(Mp_Ex_Fail("MP_Frame::operator -=","He's inside of us",""));

        size = f.off - off;
        //MP_Message("MP_Frame:: -= result is [%d,%d[", off, end() );
        return;
        }


    if( end() <= f.end() )
        throw(Mp_Ex_Fail("MP_Frame::operator -=","He's contains us",""));

    // he's beginning before us or at the same position
    size = end()-f.end();
    off  = f.end();
    //MP_Message("MP_Frame:: -= result is [%d,%d[", off, end() );
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

      //MP_Message("prob(%d) += %d", prob_v, p.prob_v );

      // Have 100%? Result is surely 100%
    if( prob_v >= MP_Prob_Max || p.prob_v >= MP_Prob_Max )
        {
        prob_v = MP_Prob_Max;
        return *this;
        }
    
    prob_v = (prob_v+p.prob_v) >> 1; // (a+b) / 2
    
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




