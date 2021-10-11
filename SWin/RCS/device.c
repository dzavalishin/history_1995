head	1.13;
access;
symbols;
locks;
comment	@ * @;


1.13
date	96.11.22.13.02.38;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.10.10.23.37.07;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.10.01.05.27.39;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.09.30.14.18.41;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.09.27.07.26.39;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.09.27.04.54.43;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.09.27.03.50.45;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.09.27.00.33.20;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.09.20.14.40.18;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.09.12.01.12.51;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.09.14.53.22;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.09.01.47.48;	author dz;	state Exp;
branches;
next	;


desc
@@


1.13
log
@Nonrectangular (partially transparent) windows
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: device classes impl.
 *
 *      $Log: device.c $
 *      Revision 1.12  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.11  1996/10/10 23:37:07  dz
 *      reorganized devices classes, generalized
 *      videomemory-style devices
 *
 *      Revision 1.10  1996/10/01 05:27:39  dz
 *      dialogs, basically written
 *
 *      Revision 1.9  1996/09/30 14:18:41  dz
 *      log control written
 *
 *      Revision 1.8  1996/09/27 07:26:39  dz
 *      bugfixes
 *
 *      Revision 1.7  1996/09/27 04:54:43  dz
 *      fast fill - now we're passing fill data
 *      down to device and do actual fill there.
 *      Prepared to do same thing with border, but
 *      not sure yet I really will.
 *
 *      Revision 1.6  1996/09/27 03:50:45  dz
 *      optimized once again
 *
 *      Revision 1.5  1996/09/27 00:33:20  dz
 *      viobuf, optimization (sort of:)
 *
 *      Revision 1.3  1996/09/12 01:12:51  dz
 *      works a little
 *
 *      Revision 1.1  1996/09/09 01:47:48  dz
 *      Initial revision
 *
 *      
 *
\*/


#include "sw_device.h"
#include <io.h>



sw_device::sw_device( const sw_size &s ) : sw_abstract_win(s,*this)
    {
    have_invalid_v = Yes;
    invalid_v = sw_rect(0,0,s.xs(),s.ys()); // invalidate all the screen
    sw_device::calculate_absolute_window_posiotion(); // we do it our own way
    }

sw_device::~sw_device() {}

void sw_device::invalidate( const sw_rect &r )
    {
    if( have_invalid_v )
        invalid_v.summ(invalid_v,r);
    else
        {
        invalid_v = r;
        have_invalid_v = Yes;
        }
    }

void sw_device::flush()
    {
    if( !have_invalid_v ) return;
    do_flush(invalid_v);
    have_invalid_v = No;
    }

void sw_device::resize( const sw_size &s )
    {
    throw sw_Fail("sw_device::resize","Device is not resizeable", name() );
    }

void sw_device::push_focus()
    {
    sw_abstract_win *w = deep_focus();
    if( w == 0 ) sw_Fail("sw_device::push_focus","No focus yet", "" );
    focus_stack.push( w );
    }

void sw_device::push_focus(sw_abstract_win &new_focus)
    {
    push_focus();
    new_focus.focus_tree();
    }

void sw_device::pop_focus()
    {
    if( focus_stack.empty() ) sw_Fail("sw_device::pop_focus","Stack is empty", "" );
    focus_stack.top()->focus_tree();
    focus_stack.pop();
    }

void sw_device::clear_win_from_focus_stack(sw_abstract_win &w)
    {
    // BUG!!
    }

void sw_device::mouse_hide( const sw_rect & ) {}
void sw_device::mouse_show() {}


  // -----------------------------------------------------------------------


void sw_device::mouse_capture( sw_event_handler *c )
    {
    }

bool sw_device::mouse_event( const sw_event &e )
    {
    sw_abstract_win *wp = map.win_at( e.pos() );
    if( wp )
        return wp->event( e );
    else
        return No;
    }

bool sw_device::mouse_event_translator( sw_event &e )
    {

    switch( e.info() )
        {
        // Right click must call context menu by default
        case sw_event::Mouse2Click:
            e = sw_event( e.pos(), sw_event::ContextMenu ); break;

        default:  return No; // No translation available
        };
    
    return Yes;
    }


  // -----------------------------------------------------------------------


void sw_2b_video_memory_device::fill_cw( const sw_abstract_win &who, sw_color col, char ch, sw_rect _p )
    {
    invalidate(_p);
    
    sw_rect p;
    if( !p.intersection( _p, sw_rect(0,0,xs(),ys()) ) )        return;
    
      //int inp_bpl = _p.xs() * 2;
    int scr_bpl = xs() * 2;
    int map_bpl = xs();
    
    char *vio_line = video_memory+(p.y()*scr_bpl);
    const sw_abstract_win * const * map_line = map.mapp()+(p.y()*map_bpl);
    
    int pxshift = p.x()*2;
    char cv = col.value();

    mouse_hide( p );
    
    for( int vcount = p.ys(); vcount--; )
        {
          //const char *lp = in_line;
          //in_line += inp_bpl;
        
        char *vp = vio_line+pxshift;
        vio_line += scr_bpl;
        
        const sw_abstract_win * const * mp = map_line+p.x();
        map_line += map_bpl;
        
        for( int hcount = p.xs(); hcount--; )
            {
            if( &who == *mp++ )
                {
                *vp++ = ch;
                *vp++ = cv;
                }
            else
                {
                vp+=2;
                }
            }
        }

    mouse_show();

    }

void sw_2b_video_memory_device::border_cw( const sw_abstract_win &who, sw_color col, sw_border_t t )
    {
    }

void sw_2b_video_memory_device::put_cw( const sw_abstract_win &who, const sw_rect &_p, const char *in_line )
    {
    invalidate(_p);
    
    sw_rect p;
    if( !p.intersection( _p, sw_rect(0,0,xs(),ys()) ) )        return;
    
    int inp_bpl = _p.xs() * 2;
    int scr_bpl = xs() * 2;
    int map_bpl = xs();
    
    char *vio_line = video_memory+(p.y()*scr_bpl);
    const sw_abstract_win * const * map_line = map.mapp()+(p.y()*map_bpl);
    
    int pxshift = p.x()*2;
    
    mouse_hide( p );

    for( int vcount = p.ys(); vcount--; )
        {
        const char *lp = in_line;
        in_line += inp_bpl;
        
        char *vp = vio_line+pxshift;
        vio_line += scr_bpl;
        
        const sw_abstract_win * const * mp = map_line+p.x();
        map_line += map_bpl;
        
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

    mouse_show();
    }




@


1.12
log
@Before changing event class
@
text
@d8 3
d104 7
d164 2
d191 3
d216 2
d242 2
@


1.11
log
@reorganized devices classes, generalized
videomemory-style devices
@
text
@d8 4
a32 3
 *      Revision 1.4  1996/09/20 14:40:18  dz
 *      update
 *
a35 3
 *      Revision 1.2  1996/09/09 14:53:22  dz
 *      update
 *
d53 1
d55 1
d99 33
@


1.10
log
@dialogs, basically written
@
text
@d8 3
a48 3
  //#define INCL_KBD
#define INCL_VIO
#include <os2.h>
a101 2


a103 29
static sw_size get_vio_size()
    {
    VIOMODEINFO mi;
    //mi.col = 80; mi.row = 25;
    mi.cb = sizeof(mi);
    int rc = VioGetMode( &mi, 0);
    if( rc )
        {
        printf("VioGetMode: %d\n", rc );
        throw sw_Fail("get_vio_size","VioGetMode failed","");
        }
    //printf("VioGetMode: %d x %d\n", mi.col, mi.row );
    return sw_size(mi.col, mi.row);
    }

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
d105 1
a105 22
sw_vio_base::~sw_vio_base()
    {
    int len = xs()*ys()*2;
    memcpy( viobuf, saved_screen, len ); // restore user's screen
    VioShowBuf( 0, len, 0 );
    delete [] saved_screen;
    }

void sw_vio_base::do_flush(const sw_rect &_p)
    {
    sw_rect p;
    if( !p.intersection( _p, sw_rect(0,0,xs(),ys()) ) )        return;

    int off = p.y()  * xs() * 2;
    int len = p.ys() * xs() * 2;

    if( VioShowBuf( off, len, 0 ) )
        throw sw_Fail( "sw_vio_base::do_flush", "VioShowBuf error", "" );
    }


void sw_vio_base::fill_cw( const sw_abstract_win &who, sw_color col, char ch, sw_rect _p )
d112 1
a112 1
    //int inp_bpl = _p.xs() * 2;
d116 1
a116 1
    char *vio_line = viobuf+(p.y()*scr_bpl);
d124 2
a125 2
        //const char *lp = in_line;
        //in_line += inp_bpl;
d148 1
a148 1
void sw_vio_base::border_cw( const sw_abstract_win &who, sw_color col, sw_border_t t )
d151 2
a152 1
void sw_vio_base::put_cw( const sw_abstract_win &who, const sw_rect &_p, const char *in_line )
d158 1
a158 1

d162 2
a163 2

    char *vio_line = viobuf+(p.y()*scr_bpl);
d165 1
a165 1

d172 1
a172 1

d175 1
a175 1

d178 1
a178 1

a192 125

  // -----------------------------------------------------------------------

//void sw_vio_device::put_up( const sw_rect &p, const char *char_and_attr_map )  { }

sw_vio_device::sw_vio_device()  {}
sw_vio_device::~sw_vio_device() {}

const char *sw_vio_device::name() { return "vio"; }

  // -----------------------------------------------------------------------

sw_tty_base::sw_tty_base( ostream& s) : os_v(s)
    {}

void sw_tty_base::tty_write( const char *s, int count )
    {
    os_v.write(s,count);
    }

  // -----------------------------------------------------------------------
        

void sw_ansi_device::put_cw( const sw_abstract_win &who, const sw_rect &_p, const char *ca )
    {
    char cur_color;
    sw_rect p = _p;
    int bytes_per_line = _p.xs() * 2;

    //who.move_to_win(p);
    
    p.cut( *this );
    if( p.xs() <= 0 || p.ys() <= 0 ) return;
    
    ansi_set_color( cur_color = ca[1] ); // Set first attribute
    
    for( int yi = p.y(); yi < p.yend(); yi++ )
        {
        ansi_gotoxy( p.x(), yi );
        const char *lp = ca;
        ca += bytes_per_line;
        for( int xi = p.x(); xi < p.xend(); xi++ )
            {
#if 1
            if( !map.belongs( &who, xi, yi ) )
               tty_write("\x1B[C", 3);
            else
#endif
                {
                if( lp[1] != cur_color ) ansi_set_color( cur_color = lp[1] );
                tty_write(lp,1);
                }
            lp += 2;
            }
        }
    
    }

void sw_ansi_device::fill_cw( const sw_abstract_win &who, sw_color col, char ch, sw_rect _p )
    {
    }

void sw_ansi_device::border_cw( const sw_abstract_win &who, sw_color col, sw_border_t t )
    {
    }


/*
void sw_ansi_device::put_up( const sw_rect &_p, const char *ca )
    {
    char cur_color;
    sw_rect p = _p;
    int bytes_per_line = _p.xs() * 2;

    p.cut( *this );
    if( p.xs() <= 0 || p.ys() <= 0 ) return;

    ansi_set_color( cur_color = ca[1] ); // Set first attribute

    for( int yi = p.y(); yi < p.yend(); yi++ )
        {
        ansi_gotoxy( p.x(), yi );
        const char *lp = ca;
        ca += bytes_per_line;
        for( int xi = p.x(); xi < p.xend(); xi++ )
            {
            if( lp[1] != cur_color ) ansi_set_color( cur_color = lp[1] );
            tty_write(lp,1);
            lp += 2;
            }
        }
    
    }
*/
sw_ansi_device::sw_ansi_device()  {}
sw_ansi_device::~sw_ansi_device() {}


int ansi_col_tab[] = { 30, 34, 32, 36, 31, 35, 33, 37 };

void sw_ansi_device::ansi_set_color( char color )
    {
    unsigned char fg  =  color & 0x7;
    int           fgi =  color & 0x8;
    unsigned char bg  = (color & 0x70) >> 4;
    int           bgi =  color & 0x80;

    char s[40];
    sprintf( s, "\x1B[%d;%d", ansi_col_tab[fg], 10+ansi_col_tab[bg] );
    if( fgi ) strcat( s, ";1" );
    if( bgi ) strcat( s, ";5" );
    strcat( s, "m" );
    tty_write( s, strlen(s) );
    }

void sw_ansi_device::ansi_gotoxy( int x, int y )
    {
    char s[40];
    sprintf(s,"\x1B[%d;%dH", y+1, x+1 );
    tty_write( s, strlen(s) );
    }


const char *sw_ansi_device::name() { return "ansi"; }

@


1.9
log
@log control written
@
text
@d8 3
d57 45
@


1.8
log
@bugfixes
@
text
@d8 3
d48 1
a48 1
sw_device::sw_device( const sw_size &s ) : sw_abstract_win(s)
@


1.7
log
@fast fill - now we're passing fill data
down to device and do actual fill there.
Prepared to do same thing with border, but
not sure yet I really will.
@
text
@d8 6
d45 5
a49 1
sw_device::sw_device( const sw_size &s ) : sw_abstract_win(s) {}
d93 1
a93 1
void sw_vio_base::do_flush(const sw_rect &r)
d95 5
a99 2
    int off = r.y()  * xs() * 2;
    int len = r.ys() * xs() * 2;
d101 2
a102 1
    VioShowBuf( off, len, 0 );
@


1.6
log
@optimized once again
@
text
@d8 3
d92 1
a92 2
#if SLOOOOOW
void sw_vio_base::put_cw( const sw_abstract_win &who, const sw_rect &_p, const char *ca )
d96 12
a107 2
    sw_rect p = _p;
    int bytes_per_line = _p.xs() * 2;
d109 1
a109 5
    p.cut( *this );
    if( p.xs() <= 0 || p.ys() <= 0 ) return;

    char *violine = viobuf+(p.y()*xs()*2);
    for( int yi = p.y(); yi < p.yend(); yi++ )
d111 10
a120 6
        const char *lp = ca;
        ca += bytes_per_line;
        char *vp = violine;
        violine += xs()*2;
        int xe = p.xend();
        for( int xi = p.x(); xi < xe; )
d122 9
a130 13
            int count = 0;  int x = xi;
            count = map.not_belong_count( &who, xi, yi );
            if( count > (xe-xi) ) count = (xe-xi);
            xi += count;
            lp += count;
            lp += count;
            count = map.belong_count( &who, xi, yi );
            x = xi;
            if( count > (xe-xi) ) count = (xe-xi);
            xi += count;
            count *= 2;
            memcpy( vp+(x*2), lp, count );
            lp += count;
a132 1
    
d134 4
a137 1
#else
a178 1
#endif
d236 9
@


1.5
log
@viobuf, optimization (sort of:)
@
text
@d8 3
d89 1
a89 1

a96 2
      //who.move_to_win(p);
    
a110 4
            /*
            while( (!map.belongs( &who, xi, yi )) && xi < xe )
            lp += 2, xi++;
            */
a120 1
              //if( count != 0 )         VioWrtCellStr( (PCH)lp, count, yi, x, 0 );
d127 11
d139 31
@


1.4
log
@update
@
text
@d8 3
d50 1
a50 1
    printf("VioGetMode: %d x %d\n", mi.col, mi.row );
d56 27
d85 1
a85 1
sw_vio_base::~sw_vio_base() {}
d89 2
d98 2
a99 1
    
d104 2
d110 1
d112 6
a117 7
                lp += 2, xi++;
#if 0
            count = 0;          x = xi;
            while( map.belongs( &who, xi, yi ) && xi < xe )
                count += 2, xi++;

            if( count != 0 )         VioWrtCellStr( (PCH)lp, count, yi, x, 0 );
a118 1
#else
d124 2
a125 1
            if( count != 0 )         VioWrtCellStr( (PCH)lp, count, yi, x, 0 );
a126 1
#endif
@


1.3
log
@works a little
@
text
@d8 3
d25 5
d39 9
a47 1
    VioGetMode( &mi, 0);
@


1.2
log
@update
@
text
@d8 3
a18 1

d28 11
a38 1
sw_vio_base::sw_vio_base() {}
d41 42
d85 1
a85 3
void sw_vio_device::put_up( const sw_rect &p, const char *char_and_attr_map )
    {
    }
d94 4
a97 1
int sw_tty_base::tty_write( const char *s, int count )
d99 1
a99 1
    return os_v.write(s,count);
d105 36
d145 1
a145 1
    int orig_width = _p.xs();
d155 2
a156 2
        char *lp = ca;
        ca += orig_width;
d166 1
a166 1

@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: Tic.c $
d18 91
@
