/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	:	
 *
 *      $Log: MpState.h $
 *      Revision 1.4  1996/07/09 02:12:58  dz
 *      Debugging has begone
 *
 *      Revision 1.3  1996/07/07 10:01:14  dz
 *      *** empty log message ***
 *
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

#ifndef MPSTATE_H
#define MPSTATE_H

const Fatal_Timeout  = 1000* 5*60; // 5 minutes
const SEOF_Timeout   = 1000* 10;   // 10 seconds
const SStop_Timeout  = 1000* 10;   // 10 seconds
const Wakeup_Timeout = 1000* 2;    // 2 seconds

enum MP_State
    {
    
    MP_SS_None   = '0',    // Not started yet
    //MP_SS_Wakeup = 'W',
    MP_SS_Init   = 'I',
    MP_SS_Data   = 'D',
    MP_SS_SEOF   = 'E',
    MP_SS_SStop  = 'S',    // Sender stopped
    MP_SS_RStop1 = '1',
    MP_SS_RStop2 = '2'
    
    };



/*
 
Init:
 
  Sender state machines  of caller and answerer start in Init state
  sending wakeup packets until
  Init packet is received. Each time Wakeup is received Init is sent back.
  (Even in Data state). One Init is sent immediately after first Wakeup
  just to speed things up on a clear lines. After Init is received state
  changed to Data. In Init state all the packets except for Init/Wakeup are
  ignored. In Data state Init packets are ignored. Receiver state
  machine is started in data state when sender switches to Data.

ZModem-like challenge mechanism:

  can be implemented by inserting challenge number in Wakeup AND Init
  packet sent by answering side of protocol and sending it back in 
  Init packets sent in answer. If so, calling side will no send Init 
  until it got one (or Wakeup before) from answering side.
  
  
Final:
  
  Sender switches to SEOF state from Data state if nothing more to send
  and all the data is acked. It is possible for it to switch back to Data
  if Nak received.
  EOF packet carries EOF position to the receiver and is sent in SEOF state
  periodically (see SEOF_Timeout). First time it is sent when sender
  gets sure it knows where EOF is.
  
  When Stop1 is received by sender, state switches to SStop. 
  In SStop state SStop packet is sent at the entry to the state, 
  in responce to anything we get and after SStop_Timeout of silence.
  Sender awaits hangup in this state.

  Receiver switches to RStop1 state when EOF packet is received AND
  all the data is received ok. In RStop1 state RStop1 is sent once AND in
  responce to anything we get except for SStop, which is causes transition
  to RStop2.
  
  RStop2 state: send RStop2, wait for other channel to go to
  SStop, hangup.
  

Data transfer:

  Sender looks up packet which has the lowest probability of being
  received and sends it. If all is acked, goes to Stop1.
  
  Usually receiver sends Ack each time it got something correctly. If
  received packet is positioned below current window, all the
  data from 0 to the bottom of window ack'ed. Window slides
  up as it's lower side received correctly. 
  
Acks groupping:
  
  It is up to receiver to group Acks, but Ack should surely be sent 
  near the middle of window or when packet disassembly code recognizes 
  burst of junk of a "packet-looking" :) size. Say, > 128 bytes.
  
  I suppose the best way to combine acks is wait for the nex packet
  some time, and if next packet data frame is aside to the frame
  we going to ack - extend ack range to cover new packet. If range 
  to ack is equal or more than half of window, send pending ack now. 
  If timed out waiting for a new data packet - send pending ack too.
  Else - wait for a next data packet.
  
  Note that this stuff is not fully compatible with link delay estimate
  system. Such an extended ack gives us twice as long delay as usual.
  Maybe it will be better to send it as a special packet type to
  make sure estimation will work correctly.
  
  Addinionally, receiver sends Nak when non-received hole is 
  encountered at the beginning of a window.
  
  Both sender and receiver mark send time for each interval of
  a window. Sender uses it to calc. turnaround time by substracting
  packet send time from it's ack reception time.
  
  Sender and receiver try not to resend data blocks or acks/nacks
  until turnaround time passed last send.
  
  When sender sent a block and did not receiver ack in 
  (1.5 * turnaround time), it sends a query packet to
  force receiver to resend ack/nack for that block.
  This is done in attempt to prevent data resend in a case
  ack was lost or corrupted by line noise.
  
  If EOF pos is known and data after EOF pos is nacked, EOF
  is resent.
  
  Slow stop mode: if MP_O_NoOver option is negotiated, sender will
  switch to slow mode at the end of file, when EOF is known and
  all the blockks are sent. In this mode data block is sent only in
  responce to Nak and Query packet is sent after each data block
  and once in (1.5 * turnaround time). 
  If not in MP_O_NoOver mode, sender will flood channel repeating
  remaining unacked data packets.
  
Example:
  
---------------------------------------------------------------------------
Caller send ch / recv ch               Receiver send ch / recv ch

               W                                        W

               I                                        I
           D                                        D
                     A                                        A
           D                                        D
                     A                                        A
           D                                        D
                     A                                        A
           E                                        E
  
           D                                        D
           E         A                              E         A
                     1                                        1
           S                                        S
                     2                                        2
                     
---------------------------------------------------------------------------

Very simple example, though :)
                     
*/



#endif // MPSTATE_H

