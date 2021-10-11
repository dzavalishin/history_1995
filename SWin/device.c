/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: device classes impl.
 *
 *      $Log: device.c $
 *      Revision 1.13  1996/11/22 13:02:38  dz
 *      Nonrectangular (partially transparent) windows
 *
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




