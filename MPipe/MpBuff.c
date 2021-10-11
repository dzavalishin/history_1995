/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	: Session class impl.
 *
 *      $Log: MpBuff.c $
 *      Revision 1.4  1996/07/10 08:19:31  dz
 *      debugging. packetizing, packet queues
 *      and wakeup/init state machine seems to be ok.
 *
 *      Revision 1.3  1996/07/09 16:03:58  dz
 *      debugging depacketizer
 *
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

#include "MpBuff.h"


void sucker_run( void *o )
    {
    //MP_Message("Sucker thread this = %x, %x", o, ((MP_Buffer*)o));
    try {  ((MP_Buffer*)o)->sucker(); }
    catch( Mp_Ex ex )
        {
        MP_Message("Exc in %s: %s (%s), buff. sucker thread",
                   ex.where.c_str(), ex.what.c_str(), ex.why.c_str() );
        }
    ((MP_Buffer*)o)->s_tid = 0;
    }


void MP_Buffer::sleep()
    {
    overflow = Yes;
    s_over.sleep();
    overflow = No;
    }

void MP_Buffer::wakeup()
    {
    if(overflow) s_over.post();
    }

void MP_Buffer::wait_for_data( int len )
    {
    while(used < len) 
        {
        underflow = Yes;
        s_under.sleep();
        if( !connected() ) throw Mp_Ex_Disconnect("MP_Buffer::wait_for_data");
        underflow = No;
        }
    }



void MP_Buffer::sucker()
    {
    while(!stop_v)
        {
        const rbs = 128;
        char rbuf[rbs];
        
        //MP_Message("sucker reading");
        int r = port.read( rbuf, rbs );
        //MP_Dump("sucker read", rbuf, r );
        
        if( r < 0 ) { stop_v = Yes; break; }
        
        int rbshift = 0;
        while( r > 0 )
            {
            while(used >= size) sleep();
            if( stop_v ) return;
            
                {
                MP_SLKey key( discard ); // Prevent discard from happening here
                int len = size-used;
                if( len > r ) len = r;
                memcpy( buf+used, rbuf+rbshift, len );
                r -= len;
                used += len;
                rbshift += len;
                if( underflow ) s_under.post();
                }
            }
        
        }
    }


void MP_Buffer::operator += ( int len )
    {
    //MP_Message("MP_Buffer::operator += (%d), used %d", len, used );
    if( len == 0 ) return;
    MP_SLKey key( discard ); // Prevent sucker from messing around
    if( len <= 0 || len >= used )
        used = 0; // just kill all.
    else
        {
        //MP_Message( "memcpy( 0x%x, 0x%x, %d ), used = %d, len = %d", buf, buf+len, used - len, used, len );
        memcpy( buf, buf+len, used - len );
        used -= len;
        }
    wakeup();
    }



void MP_Buffer::scan( char c )
    {

    while(1)
        {
            {
            MP_SLKey key( discard ); // Prevent sucker from messing around
            //MP_Message("scan 0x%x start, %d used", c, used );
            //MP_Dump( "scan buf", buf, used );
            for( char *cp = buf; cp-buf < used; cp++ )
                if( *cp == c )
                    {
                    //MP_Message("scan 0x%x found 0x%x at %d, %d used", c, *cp, cp-buf, used );
                    cp++;
                    operator += (cp-buf);
                    return;
                    }
            
            //MP_Message("scan 0x%x found nothing, %d used", c, used );
            // we scanned all the buffer and nothing is of interest,
            // so clear it
            used = 0;
            }

        if( !connected() ) throw Mp_Ex_Disconnect("MP_Buffer::wait_for_data");
        wait_for_data();
        if( !connected() ) throw Mp_Ex_Disconnect("MP_Buffer::wait_for_data");
        }

    }
