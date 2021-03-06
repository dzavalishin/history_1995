head	1.6;
access;
symbols;
locks;
comment	@ * @;


1.6
date	96.11.22.13.02.38;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.11.15.21.37.58;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.11.15.16.29.28;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.10.10.23.37.07;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.10.10.10.46.53;	author dz;	state Exp;
branches;
next	;


desc
@@


1.6
log
@Nonrectangular (partially transparent) windows
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: OS/2 specific device classes impl.
 *
 *      $Log: device_os2.c $
 *      Revision 1.5  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.4  1996/11/15 21:37:58  dz
 *      preliminary mouse support done
 *
 *      Revision 1.3  1996/11/15 16:29:28  dz
 *      mouse event generator written
 *
 *      Revision 1.2  1996/10/10 23:37:07  dz
 *      reorganized devices classes, generalized
 *      videomemory-style devices
 *
 *      Revision 1.1  1996/10/10 10:46:53  dz
 *      Initial revision
 *
 *
 *      
 *
\*/


#include "sw_device.h"
#include <io.h>




static sw_size get_vio_size()
    {
    VIOMODEINFO mi;
    mi.cb = sizeof(mi);
    int rc = VioGetMode( &mi, 0);
    if( rc )
        {
        printf("VioGetMode error: %d\n", rc );
        throw sw_Fail("get_vio_size","VioGetMode failed","");
        }
    return sw_size(mi.col, mi.row);
    }

sw_vio_base::sw_vio_base() : sw_2b_video_memory_device( get_vio_size() ), mouse( *this )
    {
    saved_screen = 0;
    vm_alloc();
    }

sw_vio_base::~sw_vio_base()
    {
    vm_free();
    }

void sw_vio_base::vm_alloc()
    {
    ULONG buf;
    USHORT bsize;
    if( VioGetBuf( &buf, &bsize, 0 ) )
        throw sw_Fail("sw_vio_base::sw_vio_base","VioGetBuf failed","");
    unsigned short sel = buf >> 16;
    unsigned short off = buf & 0xFFFFu;
    video_memory = (char *) MAKEP(sel,off);
    
    int len = xs()*ys()*2;
    saved_screen = new char[len];
    memcpy( saved_screen, video_memory, len ); // save user's screen
    }

void sw_vio_base::vm_free()
    {
    if(saved_screen)
        {
        int len = xs()*ys()*2;
        memcpy( video_memory, saved_screen, len ); // restore user's screen
        VioShowBuf( 0, len, 0 );
        delete [] saved_screen;
        saved_screen = 0;
        }
    }




void sw_vio_base::do_flush(const sw_rect &_p)
    {
    sw_rect p;
    if( !p.intersection( _p, sw_rect(0,0,xs(),ys()) ) )        return;

    int off = p.y()  * xs() * 2;
    int len = p.ys() * xs() * 2;

    mouse.hide( p );
    
    if( VioShowBuf( off, len, 0 ) )
        throw sw_Fail( "sw_vio_base::do_flush", "VioShowBuf error", "" );

    mouse.show();
    }

void sw_vio_base::mouse_hide( const sw_rect &r )
    {
    mouse.hide( r );
    }

void sw_vio_base::mouse_show()
    {
    mouse.show();
    }


  // -----------------------------------------------------------------------

sw_vio_device::sw_vio_device()  {}
sw_vio_device::~sw_vio_device() {}

const char *sw_vio_device::name() { return "vio"; }

  // -----------------------------------------------------------------------

sw_avio_device::sw_avio_device()  {}
sw_avio_device::~sw_avio_device() {}

const char *sw_avio_device::name() { return "avio"; }

  // -----------------------------------------------------------------------

enum
    {
    mou_b1 = MOUSE_BN1_DOWN,
    mou_b2 = MOUSE_BN2_DOWN,
    mou_b3 = MOUSE_BN3_DOWN,

    drag_b1 = MOUSE_MOTION_WITH_BN1_DOWN,
    drag_b2 = MOUSE_MOTION_WITH_BN2_DOWN,
    drag_b3 = MOUSE_MOTION_WITH_BN3_DOWN,

    any_b1 = MOUSE_BN1_DOWN|MOUSE_MOTION_WITH_BN1_DOWN,
    any_b2 = MOUSE_BN2_DOWN|MOUSE_MOTION_WITH_BN2_DOWN,
    any_b3 = MOUSE_BN3_DOWN|MOUSE_MOTION_WITH_BN3_DOWN

    };


void sw_os2_mouse_handler::send_event( sw_point p, sw_event::it i )
    {
    sw_event e( p, i );

      //fprintf( stderr, "me (%d,%d)\t%d\n", p.x(), p.y(), (int)i );
    
    if( capture_v )
        {
        if( capture_v->event(e) )
            return;
        // event is not processed? Try to translate it to some standard command and send again
        if( dev_v.mouse_event_translator(e) )
            capture_v->event(e);
        }
    else
        {
        if( dev_v.mouse_event(e))
            return;
        if( dev_v.mouse_event_translator(e) )
            dev_v.mouse_event(e);
        }

    
    }

const click_time = 300; // msec max between press and release
const dclick_time = 1000; // msec max between 1st release and 2nd press


void sw_os2_mouse_handler::run()
    {
    static USHORT old_event_mask = 0;      // dunno why, but just static works, but not auto.
    static USHORT new_event_mask = 0x7Fu;  // ditto.
    
    if( MouOpen( 0, (PHMOU)&h ) )
        return; // Can't open, just die. There may be no mouse on system.

    MouGetEventMask( &old_event_mask, h );
    MouSetEventMask( &new_event_mask, h );

    
    show(); // Show pointer

    int this_time = 0;
    int prev_time = 0;

    int this_state = 0;
    int prev_state = 0;

    int b1_press_time = 0, b2_press_time = 0, b3_press_time = 0;
    int b1_click_time = 0, b2_click_time = 0, b3_click_time = 0;
    
    sw_point prev_pos( -1, -1 ), this_pos( -1, -1 );
    sw_point b1_click_pos( -1, -1 ), b2_click_pos( -1, -1 ), b3_click_pos( -1, -1 );

    while( !asked_to_stop() )
        {
        MOUEVENTINFO mi;

        USHORT wait_mode = 1; // wait
        MouReadEventQue( &mi, &wait_mode, h );

        prev_time = this_time;
        this_time = mi.time; // milliseconds

        prev_state = this_state;
        this_state = mi.fs;

        prev_pos = this_pos;
        this_pos = sw_point( mi.col, mi.row );

        //int same_pos = this_pos == prev_pos;
        
        if( (this_state & any_b1) && !(prev_state & any_b1) ) // b1 press
            {
            send_event( this_pos, sw_event::Mouse1Press );
            if( b1_click_pos == this_pos  && this_time - b1_click_time < dclick_time )
                {
                send_event( this_pos, sw_event::Mouse1DoubleClick );
                b1_press_time = 0; // Prevent sequential doubleclicks
                }
            else
                b1_press_time = this_time;
            b1_click_pos = this_pos;
            }
        if( (prev_state & any_b1) && !(this_state & any_b1) ) // b1 release
            {
            send_event( this_pos, sw_event::Mouse1Release );
            if( b1_click_pos == this_pos && this_time - b1_press_time < click_time )
                {
                send_event( this_pos, sw_event::Mouse1Click );
                b1_click_time = this_time;
                }
            }
        if( this_state & drag_b1 ) // b1 drag
            send_event( this_pos, sw_event::Mouse1Drag );


        if( (this_state & any_b2) && !(prev_state & any_b2) ) // b2 press
            {
            send_event( this_pos, sw_event::Mouse2Press );
            if( b2_click_pos == this_pos  && this_time - b2_click_time < dclick_time )
                {
                send_event( this_pos, sw_event::Mouse2DoubleClick );
                b2_press_time = 0; // Prevent sequential doubleclicks
                }
            else
                b2_press_time = this_time;
            b2_click_pos = this_pos;
            }
        if( (prev_state & any_b2) && !(this_state & any_b2) ) // b2 release
            {
            send_event( this_pos, sw_event::Mouse2Release );
            if( b2_click_pos == this_pos  && this_time - b2_press_time < click_time )
                {
                send_event( this_pos, sw_event::Mouse2Click );
                b2_click_time = this_time;
                }
            }
        if( this_state & drag_b2 ) // b2 drag
            send_event( this_pos, sw_event::Mouse2Drag );


        if( (this_state & any_b3) && !(prev_state & any_b3) ) // b3 press
            {
            send_event( this_pos, sw_event::Mouse3Press );
            if( b3_click_pos == this_pos  && ((this_time - b3_click_time) < dclick_time) )
                {
                send_event( this_pos, sw_event::Mouse3DoubleClick );
                b3_press_time = 0; // Prevent sequential doubleclicks
                }
            else
                b3_press_time = this_time;
            b3_click_pos = this_pos;
            }
        if( (prev_state & any_b3) && !(this_state & any_b3) ) // b3 release
            {
            send_event( this_pos, sw_event::Mouse3Release );
            if( b3_click_pos == this_pos  && this_time - b3_press_time < click_time )
                {
                send_event( this_pos, sw_event::Mouse3Click );
                b3_click_time = this_time;
                }
            }
        if( this_state & drag_b3 ) // b3 drag
            send_event( this_pos, sw_event::Mouse3Drag );

        if( this_state == MOUSE_MOTION ) // just move
            send_event( this_pos, sw_event::MouseMove );


        dev_v.flush();

          // all the buttons are released, reset capture
        if( capture_v && !(this_state & 0x7E) )
            {
            capture_v->event( sw_event::EndOfCapture );
            capture_v = 0;
            }
        
        }

    MouSetEventMask( &old_event_mask, h );
    }

void sw_os2_mouse_handler::hide( const sw_rect &where )
    {
    NOPTRRECT reg;

    reg.row = where.y();
    reg.col = where.x();
    reg.cRow = where.yend();
    reg.cCol = where.xend();
    
    MouRemovePtr( &reg, h );
    }

void sw_os2_mouse_handler::show()
    {
    MouDrawPtr( h ); // Show pointer
    }















@


1.5
log
@Before changing event class
@
text
@d8 3
d98 2
d102 7
d111 6
d181 3
d187 4
d227 1
d229 4
a232 1
            b1_press_time = this_time;
d252 1
d254 4
a257 1
            b2_press_time = this_time;
d277 1
d279 4
a282 1
            b3_press_time = this_time;
d297 2
d311 2
@


1.4
log
@preliminary mouse support done
@
text
@d8 3
d135 19
a154 2
    if( capture_v )            capture_v->event(e);
    else                       dev_v.mouse_event(e);
d198 1
a198 1
        if( (this_state & mou_b1) && !(prev_state & mou_b1) ) // b1 press
d243 1
a243 1
            if( b3_click_pos == this_pos  && this_time - b3_click_time < dclick_time )
d250 1
a250 1
            send_event( this_pos, sw_event::Mouse2Release );
@


1.3
log
@mouse event generator written
@
text
@d8 3
a26 4
#define INCL_MOU
#define INCL_KBD
#define INCL_VIO
#include <os2.h>
d120 6
a125 1
    drag_b3 = MOUSE_MOTION_WITH_BN3_DOWN
d134 1
a134 1
    else                       dev_v.event(e);
d186 1
a186 1
        if( (prev_state & mou_b1) && !(this_state & mou_b1) ) // b1 release
d199 1
a199 1
        if( (this_state & mou_b2) && !(prev_state & mou_b2) ) // b2 press
d207 1
a207 1
        if( (prev_state & mou_b2) && !(this_state & mou_b2) ) // b2 release
d220 1
a220 1
        if( (this_state & mou_b3) && !(prev_state & mou_b3) ) // b3 press
d228 1
a228 1
        if( (prev_state & mou_b3) && !(this_state & mou_b3) ) // b3 release
@


1.2
log
@reorganized devices classes, generalized
videomemory-style devices
@
text
@d8 4
a42 15
#if 0
sw_vio_base::sw_vio_base() : sw_device( get_vio_size() )
    {
    ULONG buf;
    USHORT bsize;
    if( VioGetBuf( &buf, &bsize, 0 ) )
        throw sw_Fail("sw_vio_base::sw_vio_base","VioGetBuf failed","");
    unsigned short sel = buf >> 16;
    unsigned short off = buf & 0xFFFFu;
    viobuf = (char *) MAKEP(sel,off);

    int len = xs()*ys()*2;
    saved_screen = new char[len];
    memcpy( saved_screen, viobuf, len ); // save user's screen
    }
d44 1
a44 9
sw_vio_base::~sw_vio_base()
    {
    int len = xs()*ys()*2;
    memcpy( viobuf, saved_screen, len ); // restore user's screen
    VioShowBuf( 0, len, 0 );
    delete [] saved_screen;
    }
#else
sw_vio_base::sw_vio_base() : sw_2b_video_memory_device( get_vio_size() )
d83 1
a83 1
#endif
d97 29
a125 2
#if 0
void sw_vio_base::fill_cw( const sw_abstract_win &who, sw_color col, char ch, sw_rect _p )
d127 1
a127 1
    invalidate(_p);
d129 23
a151 2
    sw_rect p;
    if( !p.intersection( _p, sw_rect(0,0,xs(),ys()) ) )        return;
d153 4
a156 11
    //int inp_bpl = _p.xs() * 2;
    int scr_bpl = xs() * 2;
    int map_bpl = xs();
    
    char *vio_line = viobuf+(p.y()*scr_bpl);
    const sw_abstract_win * const * map_line = map.mapp()+(p.y()*map_bpl);
    
    int pxshift = p.x()*2;
    char cv = col.value();
    
    for( int vcount = p.ys(); vcount--; )
d158 15
a172 2
        //const char *lp = in_line;
        //in_line += inp_bpl;
d174 30
a203 7
        char *vp = vio_line+pxshift;
        vio_line += scr_bpl;
        
        const sw_abstract_win * const * mp = map_line+p.x();
        map_line += map_bpl;
        
        for( int hcount = p.xs(); hcount--; )
d205 2
a206 1
            if( &who == *mp++ )
d208 2
a209 2
                *vp++ = ch;
                *vp++ = cv;
d211 17
a227 1
            else
d229 2
a230 1
                vp+=2;
d233 14
d250 1
a250 1
void sw_vio_base::border_cw( const sw_abstract_win &who, sw_color col, sw_border_t t )
d252 8
d261 2
a262 1
void sw_vio_base::put_cw( const sw_abstract_win &who, const sw_rect &_p, const char *in_line )
d264 4
a267 4
    invalidate(_p);
    
    sw_rect p;
    if( !p.intersection( _p, sw_rect(0,0,xs(),ys()) ) )        return;
a268 3
    int inp_bpl = _p.xs() * 2;
    int scr_bpl = xs() * 2;
    int map_bpl = xs();
a269 2
    char *vio_line = viobuf+(p.y()*scr_bpl);
    const sw_abstract_win * const * map_line = map.mapp()+(p.y()*map_bpl);
a270 6
    int pxshift = p.x()*2;
    
    for( int vcount = p.ys(); vcount--; )
        {
        const char *lp = in_line;
        in_line += inp_bpl;
a271 2
        char *vp = vio_line+pxshift;
        vio_line += scr_bpl;
a272 2
        const sw_abstract_win * const * mp = map_line+p.x();
        map_line += map_bpl;
a273 16
        for( int hcount = p.xs(); hcount--; )
            {
            if( &who == *mp++ )
                {
                *vp++ = *lp++;
                *vp++ = *lp++;
                }
            else
                {
                vp+=2; lp+=2;
                }
            }
        }
    }
#endif
  // -----------------------------------------------------------------------
a274 2
sw_vio_device::sw_vio_device()  {}
sw_vio_device::~sw_vio_device() {}
a275 1
const char *sw_vio_device::name() { return "vio"; }
a276 1
  // -----------------------------------------------------------------------
a277 2
sw_avio_device::sw_avio_device()  {}
sw_avio_device::~sw_avio_device() {}
a278 1
const char *sw_avio_device::name() { return "avio"; }
a279 1
  // -----------------------------------------------------------------------
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: device.c $
d63 1
a63 1
sw_vio_base::sw_vio_base() : sw_device( get_vio_size() )
d66 1
d71 1
d74 1
a74 1
char *sw_vio_base::vm_alloc()
d82 1
a82 1
    char *viobuf = (char *) MAKEP(sel,off);
d86 1
a86 3
    memcpy( saved_screen, viobuf, len ); // save user's screen

    return viobuf;
d89 1
a89 1
void sw_vio_base::vm_free(char *viobuf);
d94 1
a94 1
        memcpy( viobuf, saved_screen, len ); // restore user's screen
d214 2
a215 2
sw_avio_device::sw_vio_device()  {}
sw_avio_device::~sw_vio_device() {}
@
