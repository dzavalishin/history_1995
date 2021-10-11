/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: data tarnsfer window classes impl.
 *
 *      $Log: MpWin.c $
 *      Revision 1.7  1996/07/10 18:27:24  dz
 *      something goes through, framer/frame classes
 *      are work somehow.
 *
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


  // Set probability to 100%
  // BUG!
  // (Ineffective impl., in fact.)
  // Sometimes this thing rips off part of 100% block just to set it to
  // 100%. It should be better to check range probability first and
  // do nothing if given or larger frame is already marked as received.
  // See #if-ed code below
void MP_Window::mark_as_received(const MP_Frame & f)
    {
    MP_Message("swin:: marking as received [%d,%d[", f.off, f.end() );
    MP_SLKey key(lock);
#if CHECK_PROB_FIRST
    if( marked_as_received(f) ) return;
#endif
    MP_WFrame &wf = frames.find_or_rip_or_make( f );
    wf.prob.set_to_100_percent();
    frames.dump();
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


MP_Frame MP_Send_Window::find_frame_to_send()  // throws MP_EOF in none found
    {
    MP_SLKey key(lock);
    MP_Prob p = frames.lowest_prob();
    return frames.next_with_prob( p );
    }


bool     MP_Send_Window::have_non_acked()      // We have at least one non-acked frame
    {
    MP_SLKey key(lock);
      // we can't have non-acked ones if win is empty!
    if( wf.size == 0 ) return No; 
    return frames.have_non_acked(wf);
    }

  // Set probability to 0%
void MP_Send_Window::mark_as_non_received( const MP_Frame & f )
    {
    MP_Message("swin:: marking as non-received [%d,%d[", f.off, f.end() );
    MP_SLKey key(lock);
    MP_WFrame &wf = frames.find_or_rip_or_make( f );
    wf.prob.set_to_0_percent();
    frames.dump();
    }

  // Increase probability, but NEVER make it 100%.
  // Use known info on packet drop rate to adjust probability
void MP_Send_Window::mark_as_sent(const MP_Frame & f)
    {
    MP_Message("swin:: marking as sent [%d,%d[", f.off, f.end() );
    MP_SLKey key(lock);
    MP_WFrame &wf = frames.find_or_rip_or_make( f );
    wf.prob += MP_75_percent; // BUG! here we should use drop rate
    wf.sent = time(0);
    frames.dump();
    }

  // we asked about this. it should trigger
  // ack/nak and so must be used to count turnaround time
void MP_Send_Window::mark_as_queried(const MP_Frame & f)
    {
    MP_Message("swin:: marking as queried [%d,%d[", f.off, f.end() );
    MP_SLKey key(lock);
    MP_WFrame &wf = frames.find_or_rip_or_make( f );
    wf.sent = time(0);
    frames.dump();
    }

const char* MP_Send_Window::get( const MP_Frame &f )
    {
    return wbuf + (f.off - wf.off);
    }



