/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: Frame, Framer, Probability
 *
 *      $Log: MpFrame.h $
 *      Revision 1.5  1996/07/10 18:27:24  dz
 *      something goes through, framer/frame classes
 *      are work somehow.
 *
 *      Revision 1.4  1996/07/09 16:03:58  dz
 *      debugging depacketizer
 *
 *      Revision 1.3  1996/07/09 02:12:58  dz
 *      Debugging has begone
 *
 *      Revision 1.2  1996/07/02 04:33:28  dz
 *      writing
 *
 *      Revision 1.1  1996/07/02 01:58:48  dz
 *      Initial revision
 *
 *
 *
 *
 *
\*/

#ifndef MPFRAME_H
#define MPFRAME_H

#include "MpUtil.h"

#include "vector.h"

#include "time.h"





// classes

const MP_Prob_Max = 100;

class MP_Prob   // Probability
    {
    protected:
        unsigned char  prob_v;
    
    public:
        MP_Prob() { prob_v = 0; }
        MP_Prob(int i) { prob_v = i; }
        
        bool           is_100_percent() const { return (prob_v >= MP_Prob_Max) ? Yes : No; }
        void           set_to_100_percent()   { prob_v = MP_Prob_Max; }
        void           set_to_0_percent()   { prob_v = 0; }
        
        int            operator >  ( const MP_Prob &p ) const { return prob_v >  p.prob_v; }
        int            operator <  ( const MP_Prob &p ) const { return prob_v <  p.prob_v; }
        int            operator == ( const MP_Prob &p ) const { return prob_v == p.prob_v; }
        int            operator != ( const MP_Prob &p ) const { return prob_v != p.prob_v; }
        
        MP_Prob &      operator =  ( const MP_Prob &p )       { prob_v = p.prob_v; return *this; }
        MP_Prob &      operator += ( const MP_Prob &p );
        MP_Prob        operator +  ( const MP_Prob &p ) const; //  { MP_Prob pp = *this; pp += p; return pp; }

        int prob() const { return prob_v; }
    };

extern const MP_Prob MP_75_percent;


class MP_Frame
    {
    public:
        ~MP_Frame();
        MP_Frame( MP_Off o, MP_Off s ) : off(o), size(s) {}
          //MP_Frame() : off(-1), size(-1) {}
        MP_Frame() {}
        
        MP_Off_t   off;  // NB - _absolute_, not relative offset
        MP_Off_t   size;
        
        // First byte that is _not_ ours
        MP_Off     end() const { return off+size; }
        
        bool       contains( const MP_Frame & ) const;
        bool       has_common_point_with( const MP_Frame & ) const;
        
        // Extend this frame to include arg.
        // Throw error if arg is not side by side with us
        MP_Frame & operator += ( const MP_Frame & );
        
        // make sure this frame doesn't contain given range
        // No errors.
        void       operator -= ( const MP_Frame & );
        
        int        operator == ( const MP_Frame &f ) const { return off==f.off && size==f.size; }

    };

class MP_WFrame : public MP_Frame
    {
    public:
        MP_Prob       prob; // Probability of correct reception
        time_t        sent; // When last packet (data or ack/nack) was sent
        
        MP_WFrame     operator +  ( const MP_WFrame &f1 );
            //{ MP_WFrame f(*this); f += f1; return f; }
        
        MP_WFrame &   operator += ( const MP_WFrame & );
    };


// Not multithread-protected.
// Call methods IN LOCKED STATE only!
class MP_Framer
    {
    private:
        int last_pos;

    protected:
        // rip off this frame from link. Element at index known
        // for sure to contain lower part of requested range.
        // Return index.
        // Note that if resulting frame is combined from two frames,
        // it's probability is set to lowest of both and time is
        // set to highest of both. See MP_WFrame operator +.
        // this is a worker for other methods.
        int                  rip_off( MP_Frame, int index );
        
        // Split frames[i] at position off, return index of leftmost part
        int                  split( int i, MP_Off off );
    
    public:
        MP_Framer()
            { last_pos = 0; }
        // All the frames here are sorted. Lower index corresponds
        // to lower offset. No overlapping allowed.
        vector<MP_WFrame>    frames;
        
        // We have at least one frame with probability < 100%
        // in range r
        bool                 have_non_acked( MP_Frame r );
        MP_Prob              lowest_prob();
        MP_WFrame &          next_with_prob(const MP_Prob &p);
        
        // rip off and DELETE this frame from link, possibly dividing
        // some el. into a pair. 
        void                 operator -= ( MP_Frame );
        
        // Just make framer to contatin ths frame - don't care, how.
        MP_WFrame &          find_or_rip_or_make( const MP_Frame& f );
        bool                 empty() const { return frames.size() == 0 ? Yes : No; }
        const MP_WFrame &    first() const
            { 
            if( empty() ) throw(Mp_Ex_Fail("MP_WFrame::first","empty",""));
            return frames[0];
            }

        void       dump();
    };




#endif  // MPFRAME_H

