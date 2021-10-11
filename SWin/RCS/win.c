head	1.19;
access;
symbols;
locks;
comment	@ * @;


1.19
date	96.11.22.13.02.38;	author dz;	state Exp;
branches;
next	1.18;

1.18
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.17;

1.17
date	96.11.15.21.37.58;	author dz;	state Exp;
branches;
next	1.16;

1.16
date	96.10.07.19.40.27;	author dz;	state Exp;
branches;
next	1.15;

1.15
date	96.10.03.07.43.57;	author dz;	state Exp;
branches;
next	1.14;

1.14
date	96.10.02.11.01.11;	author dz;	state Exp;
branches;
next	1.13;

1.13
date	96.10.01.16.32.27;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	96.10.01.05.27.39;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.09.30.14.18.41;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.09.28.14.03.17;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.09.27.04.54.43;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.09.26.22.07.12;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.09.26.20.46.52;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.09.21.13.26.57;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.09.21.08.07.06;	author dz;	state Exp;
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


1.19
log
@Nonrectangular (partially transparent) windows
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: window classes impl.
 *
 *      $Log: win.c $
 *      Revision 1.18  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.17  1996/11/15 21:37:58  dz
 *      preliminary mouse support done
 *
 *      Revision 1.16  1996/10/07 19:40:27  dz
 *      new 3d entry fields
 *
 *      Revision 1.15  1996/10/03 07:43:57  dz
 *      buttons
 *
 *      Revision 1.14  1996/10/02 11:01:11  dz
 *      focusable
 *
 *      Revision 1.13  1996/10/01 16:32:27  dz
 *      bugfixes - a lot of
 *
 *      Revision 1.12  1996/10/01 05:27:39  dz
 *      dialogs, basically written
 *
 *      Revision 1.11  1996/09/30 14:18:41  dz
 *      log control written
 *
 *      Revision 1.9  1996/09/27 04:54:43  dz
 *      fast fill - now we're passing fill data
 *      down to device and do actual fill there.
 *      Prepared to do same thing with border, but
 *      not sure yet I really will.
 *
 *      Revision 1.8  1996/09/26 22:07:12  dz
 *      faster map rebuild. not much, though.
 *
 *      Revision 1.7  1996/09/26 20:46:52  dz
 *      menus basically done
 *
 *      Revision 1.6  1996/09/21 13:26:57  dz
 *      generalized event model
 *
 *      Revision 1.5  1996/09/21 08:07:06  dz
 *      last version with distinct broadcast/event entry points
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



#include "sw_win.h"
#include "sw_device.h"

sw_win_ops::sw_win_ops()
    {}


// ----------------------- sw_abstract_win -------------------------


void sw_abstract_win::put( const sw_point &p, sw_color c, char ch )
    {
    char buf[2];
    
    buf[0] = ch;
    buf[1] = c.value();
    
    put( sw_rect(p,sw_size(1,1)), buf );
    }

void sw_abstract_win::put( const sw_point &p, sw_color c, const char *s )
    {
    const maxb = 1024;
    char buf[maxb+1];

    int w = 0;
    for( char *bp = buf; bp - buf < maxb && *s; w++ )
        {
        *bp++ = *s++;
        *bp++ = c.value();
        }

    put( sw_rect(p,sw_size(w,1)), buf );
    }

void sw_abstract_win::put( const sw_rect &p, const char *char_and_attr_map )
    {
    put_cw( *this, p, char_and_attr_map );
    }

void sw_abstract_win::fill( sw_color col, char ch, sw_rect r, bool excl_border )
    {
    if(excl_border)
        {
        if( r.x() == 0 ) r.x(1);
        if( r.y() == 0 ) r.y(1);
        if( r.xend() == xs() ) r.xend(xs()-1);
        if( r.yend() == ys() ) r.yend(ys()-1);
        }
    fill_cw( *this, col, ch, r );
    }

void sw_abstract_win::border( sw_color col, sw_border_t bt )
    {
    static char *bchars[] =
        {
        "      ",
        "ÄÚ¿ÀÙ³",        "ÍÉ»È¼º",
        "ÄÖ·Ó½º",        "ÍÕ¸Ô¾³",
        "ÛÛÛÛÛÛ",        "ùùùùùù"
        };

    fill( col, bchars[bt][0], sw_rect(1,0,xs()-2,1), No );       // up hor line
    fill( col, bchars[bt][0], sw_rect(1,ys()-1,xs()-2,1), No );  // lo hor line

    put( sw_rect(0,0,1,1), col, bchars[bt][1] );                 // ul corner
    put( sw_rect(xs()-1,0,1,1), col, bchars[bt][2] );            // ur corner
    put( sw_rect(0,ys()-1,1,1), col, bchars[bt][3] );            // ll corner
    put( sw_rect(xs()-1,ys()-1,1,1), col, bchars[bt][4] );       // lr corner

      // BUG! Here we can get better performance if
      // a special vfill method will be added
    fill( col, bchars[bt][5], sw_rect(0,1,1,ys()-2), No );       // left ver line
    fill( col, bchars[bt][5], sw_rect(xs()-1,1,1,ys()-2), No );  // right ver line
    }

void sw_abstract_win::refresh(const sw_rect &sq)
    {
    for( int i = 0; i < child.size(); i++ )
        {
        sw_rect sq1 = sq;
        sq1.move( -(child[i]->x()), -(child[i]->y()) );
        child[i]->refresh(sq1);
        }
    }



sw_abstract_win::sw_abstract_win( const sw_size &s, sw_device &dev )
    :
sw_size(s), map(s), device(dev)
    {
    map.clear(this);
    focusable_v = Yes; // Most windows are
    }

sw_abstract_win::~sw_abstract_win()
    {
    device.clear_win_from_focus_stack(*this);
    }

void sw_abstract_win::resize(const sw_size &s)
    {
    sw_size::resize(s);
    map.resize(s);
    }

void sw_abstract_win::do_rebuild_map(const sw_rect&r) // rebuilds map of child windows
    {
    map.clear(this);
    for( int i = 0; i < child.size(); i++ )
        {
        sw_rect r1 = r;
        r1.move( -(child[i]->x()), -(child[i]->y()) );
        child[i]->do_rebuild_map(r1);
        map.load( child[i]->map, *((sw_point*)child[i]) );
        }
    }

void sw_abstract_win::do_rebuild_map_to_root(const sw_rect& r)
    {
    do_rebuild_map(r);
    }

void sw_abstract_win::rebuild_map(const sw_rect& r)
    {
    do_rebuild_map_to_root(r);
    }



void sw_abstract_win::attach_child( sw_win &w )
    {
    child.push_back( &w );
    rebuild_map(w);
    }



sw_abstract_win::child_iterator sw_abstract_win::find_child( const sw_win &w )
    {
    for( child_iterator i = child.begin(); i < child.end(); i++ )
        {
        if( *i == &w )
            return i;
        }
    throw sw_Fail("sw_abstract_win::find_child","no such child","");
    }

void sw_abstract_win::erase_child( const sw_win &w )
    {
    child.erase( find_child( w ) );
    }



void sw_abstract_win::detach_child( sw_win &w )
    {
    // make sure we'll not rip off focused window
    if( focused_child() == &w ) focused_child(0);
    erase_child( w );
    rebuild_map(w);
    //refresh(w);
    }

void sw_abstract_win::top_child( sw_win &w ) // move child to top
    {
    erase_child( w );
    child.push_back( &w );
    rebuild_map(w);
    refresh(w);
    }

void sw_abstract_win::bot_child( sw_win &w ) // move child to bottom
    {
    erase_child( w );
    child.insert( child.begin(), &w );
    rebuild_map(w);
    refresh(w);
    }

  // set child ch to be above ref
void sw_abstract_win::above_child( sw_win &ch, const sw_win &ref )
    {
    child_iterator refc = find_child( ref );
    erase_child( ch );
    child.insert( refc+1, &ch );
    rebuild_map(ch);
    refresh(ch);
    }

  // set child ch to be below ref
void sw_abstract_win::below_child( sw_win &ch, const sw_win &ref )
    {
    child_iterator refc = find_child( ref );
    erase_child( ch );
    child.insert( refc, &ch );
    rebuild_map(ch);
    refresh(ch);
    }




bool sw_abstract_win::pass_broadcast_to_children( const sw_event &e )
    {
    int nonforce = !e.forced();
    for( int i = 0; i < child.size(); i++ )
        if( child[i]->event(e) && nonforce ) return Yes;
    return No;
    }



void sw_abstract_win::push_focus()
    {
    device.push_focus(*this);
    }

void sw_abstract_win::get_focus_iterator(child_iterator &i)
    {
    sw_abstract_win * now = focused_child();
    for( i = child.begin(); i != child.end(); i++ )
        if( *i == now )
            return;
    throw sw_Fail("sw_abstract_win::get_focus_iterator","not a focus!","");
    }

void sw_abstract_win::increment_focus_iterator(child_iterator &i)
    {
    i++;
    if( i == child.end() ) i = child.begin();
    }

void sw_abstract_win::decrement_focus_iterator(child_iterator &i)
    {
    if( i == child.begin() ) i = child.end();
    i--;
    }


void sw_abstract_win::focus_to_next_child()
    {
    if( child.empty() ) return;
    
    child_iterator i;
    get_focus_iterator(i);

    child_iterator prev_pos = i;

    do
        {
        increment_focus_iterator(i);
        if( i == prev_pos )             // we made a full circle
            return;
        }
    while( !((*i)->focusable()) );
    
    focused_child(*i);
    }

void sw_abstract_win::focus_to_prev_child()
    {
    if( child.empty() ) return;

    child_iterator i;
    get_focus_iterator(i);
    
    child_iterator prev_pos = i;
    
    do
        {
        decrement_focus_iterator(i);
        if( i == prev_pos )             // we made a full circle
            return;
        }
    while( !((*i)->focusable()) );
    
    focused_child(*i);
    return;
    }


// BUG!! We should write a real search code here
void sw_abstract_win::focus_to_left_child()
    {
    focus_to_prev_child();
    }

void sw_abstract_win::focus_to_right_child()
    {
    focus_to_next_child();
    }

void sw_abstract_win::focus_to_up_child()
    {
    focus_to_prev_child();
    }

void sw_abstract_win::focus_to_down_child()
    {
    focus_to_next_child();
    }

void sw_abstract_win::pop_focus()
    {
    device.pop_focus();
    }


int sw_abstract_win::our_mouse_event( sw_point &out, const sw_event &in ) const // boolean
    {
    return 0; // pos is not valid
    }


  // --------------------------- sw_win -----------------------------


sw_win::sw_win( sw_abstract_win &parent, sw_rect pos_size )
    : w(parent), sw_point(pos_size), sw_abstract_win(pos_size,parent.device)
    {
    w.attach_child( *this );
    calculate_absolute_window_posiotion(); // Shouldn't be redefined, I suppose
    }

sw_win::~sw_win()
    {
    w.detach_child( *this );
    w.refresh(sw_rect((sw_point&)*this,(sw_size&)*this));
    }

void sw_win::calculate_absolute_window_posiotion()
    {
    absolute_positon_v = w.absolute_positon();
    move_to_win( absolute_positon_v );
    for( child_iterator i = child.begin(); i != child.end(); i++ )
        (*i)->calculate_absolute_window_posiotion(); // forse them to recalc positions too
    }

int sw_win::our_mouse_event( sw_point &out, const sw_event &in ) const // boolean
    {
    if( !in.mouse() ) return 0; // pos is not valid
    out.x( in.pos().x() - absolute_positon_v.x() );
    out.y( in.pos().y() - absolute_positon_v.y() );
      //return ((sw_rect)*this).includes( out );
    if( out.x() < 0 || out.x() > xs() || out.y() < 0 || out.y() > ys() )
        return 0;
    return 1;
    }


void sw_win::move( int _x, int _y )
    {
    sw_rect r1 ((sw_point&)*this,(sw_size&)*this);
    sw_point::move(_x,_y);
    sw_rect r2 ((sw_point&)*this,(sw_size&)*this);
    r2.summ( r1, r2 );

    calculate_absolute_window_posiotion(); // we are moved!
            
    w.rebuild_map(r2);
    w.refresh(r2);
    }

void sw_win::resize(const sw_size &s)
    {
    sw_rect r1 ((sw_point&)*this,(sw_size&)*this);
    sw_abstract_win::resize(s);
    sw_rect r2 ((sw_point&)*this,(sw_size&)*this);
    r2.summ( r1, r2 );
    w.rebuild_map(r2);
    w.refresh(r2);
    }

void sw_win::put_cw( const sw_abstract_win &who, const sw_rect &_p, const char *char_and_attr_map )
    {
    sw_rect p = _p;
    move_to_win( p );
    w.put_cw( who, p, char_and_attr_map );
    }

void sw_win::fill_cw( const sw_abstract_win &who, sw_color col, char c, sw_rect _p )
    {
    sw_rect p = _p;
    move_to_win( p );
    w.fill_cw( who, col, c, p );
    }
    

void sw_win::border_cw( const sw_abstract_win &who, sw_color col, sw_border_t t )
    {
    }


void sw_win::do_rebuild_map_to_root(const sw_rect& r)
    {
    sw_rect p = r;
    move_to_win( p );
    w.do_rebuild_map_to_root(p);
    }

void sw_win::top() { w.top_child( *this ); }
void sw_win::bot() { w.bot_child( *this ); }
void sw_win::above( const sw_win &ref )  { w.above_child( *this, ref ); }
void sw_win::below( const sw_win &ref )  { w.below_child( *this, ref ); }



void sw_win::focus()      { visible(Yes); w.focused_child( this ); }
void sw_win::focus_tree() { visible(Yes); w.focused_child( this ); w.focus_tree(); }



  // ------------------------ sw_bordered_win --------------------------



void sw_bordered_win::refresh(const sw_rect &_sq)
    {
    sw_rect sq;
    if( !sq.intersection( _sq, sw_rect(0,0,xs(),ys()) )) return;
    sw_abstract_win::refresh(sq);
    fill(fill_color_v,fill_char_v,sq,(bool)(border_type_v!=No_Border));
    if(border_type_v!=No_Border)
        border(border_color_v,border_type_v);
    }

sw_bordered_win::sw_bordered_win(
                                 sw_abstract_win &parent,
                                 sw_rect pos_size,
                                 sw_border_t border_type,
                                 sw_color border_color,
                                 sw_color fill_color,
                                 char fill_char
                                )
    : sw_win( parent, pos_size ),
    border_type_v(border_type),
    border_color_v(border_color),
    fill_color_v(fill_color),
    fill_char_v(fill_char)
    {
      //w.refresh(sw_rect((sw_point&)*this,(sw_size&)*this));
    refresh(sw_rect(sw_point(0,0),*this));
    }

sw_bordered_win::~sw_bordered_win()
    {}


  // ------------------------ sw_border --------------------------



void sw_border::refresh(const sw_rect &_sq)
    {
    sw_rect sq;
    if( !sq.intersection( _sq, sw_rect(0,0,xs(),ys()) )) return;
    sw_abstract_win::refresh(sq);
    //fill(fill_color_v,fill_char_v,sq,(bool)(border_type_v!=No_Border));
    //if(border_type_v!=No_Border)
    border(border_color_v,border_type_v);
    }

sw_border::sw_border(
                           sw_win &parent,
                           //sw_rect pos_size,
                           sw_border_t border_type,
                           sw_color border_color
                           //sw_color fill_color,
                           //char fill_char
                          )
    : sw_win( parent.w, sw_rect(parent.x()-1,parent.y()-1,parent.xs()+2,parent.ys()+2) ),
    border_type_v(border_type),
    border_color_v(border_color)
    //fill_color_v(fill_color),
    //fill_char_v(fill_char)
    {
      //w.refresh(sw_rect((sw_point&)*this,(sw_size&)*this));
    sw_rect r = (sw_rect)(*this);
    r.resize( sw_size(-2, -2) );
    r.move( 1, 1 );
    // a hole inside of us will be transparent
    map.set_transparent( r );
    refresh(sw_rect(sw_point(0,0),*this));
    //top();
    }

sw_border::~sw_border()
    {}


@


1.18
log
@Before changing event class
@
text
@d8 3
d160 1
d372 5
d380 1
d402 10
d509 42
@


1.17
log
@preliminary mouse support done
@
text
@d8 3
a28 3
 *      Revision 1.10  1996/09/28 14:03:17  dz
 *      update
 *
a46 3
 *      Revision 1.4  1996/09/20 14:40:18  dz
 *      update
 *
a49 3
 *      Revision 1.2  1996/09/09 14:53:22  dz
 *      update
 *
a158 1

a194 15
/*
void sw_abstract_win::erase_child( sw_win &w )
    {
    for( int i = 0; i < child.size(); i++ )
        {
        if( child[i] == &w )
            {
            child.erase( child.begin() + i );
            return;
            }
        }
    throw sw_Fail("sw_abstract_win::erase_child","no such child","");
    }
*/

a195 10
#if 0
child_iterator find_child( sw_win &w ) const
    {
    for( int i = 0; i < child.size(); i++ )
        {
        if( child[i] == &w )
            return child.begin()+i;
        }
    throw sw_Fail("sw_abstract_win::erase_child","no such child","");
    }
a196 15
void sw_abstract_win::erase_child( sw_win &w )
    {
    for( int i = 0; i < child.size(); i++ )
        {
        if( child[i] == &w )
            {
            child.erase( child.begin() + i );
            return;
            }
        }
    throw sw_Fail("sw_abstract_win::erase_child","no such child","");
    }


#else
a211 1
#endif
a224 13
#if 0
    for( int i = 0; i < child.size(); i++ )
        {
        if( child[i] == &w )
            {
            child.erase( child.begin() + i );
            child.push_back( &w );
            rebuild_map(w);
            refresh(w);
            break;
            }
            }
#else
a228 1
#endif
a232 13
#if 0
    for( int i = 0; i < child.size(); i++ )
        {
        if( child[i] == &w )
            {
            child.erase( child.begin() + i );
            child.insert( child.begin(), &w );
            rebuild_map(w);
            refresh(w);
            break;
            }
        }
#else
a236 1
#endif
a269 10
/*
bool sw_abstract_win::pass_event_to_position( const sw_event &e )
    {
    sw_rect r, point( e.pos(),sw_size(1,1) );
    for( int i = 0; i < child.size(); i++ )
        if( r.intersection( point, *((sw_rect*)child[i]) ) )
            if( child[i]->event(e) ) return Yes;
    return No;
    }
*/
d375 1
d384 9
d400 3
a403 1
    //r2.move( x(), y() );
a413 1
    //r2.move( x(), y() );
@


1.16
log
@new 3d entry fields
@
text
@d8 3
d345 11
@


1.15
log
@buttons
@
text
@d8 3
d199 28
a226 1
void sw_abstract_win::detach_child( sw_win &w )
a227 2
    // make sure we'll not rip off focused window
    if( focused_child() == &w ) focused_child(0);
d233 1
a233 1
            break;
d236 28
d270 1
d281 7
a287 1
        }
d292 1
d304 26
d332 3
d507 4
@


1.14
log
@focusable
@
text
@d8 3
d198 2
@


1.13
log
@bugfixes - a lot of
@
text
@d8 3
d149 1
d251 1
a251 1
void sw_abstract_win::focus_to_next_child()
a253 1
    vector<sw_win *>::iterator i;
a255 3
            {
            if(i == child.end()-1) focused_child(*child.begin());
            else                   focused_child(*(i+1));
d257 34
a290 2
            }
    if( !child.empty() ) focused_child(*child.begin());
d295 11
a305 7
    sw_abstract_win * now = focused_child();
    vector<sw_win *>::iterator i;
    for( i = child.end(); i-- != child.begin(); )
        if( *i == now )
            {
            if(i == child.begin()) focused_child(*(child.end()-1));
            else                   focused_child(*(i-1));
d307 5
a311 2
            }
    if( !child.empty() ) focused_child(*child.begin());
@


1.12
log
@dialogs, basically written
@
text
@d8 3
d371 2
a372 2
void sw_win::focus()      { w.focused_child( this ); visible(Yes); }
void sw_win::focus_tree() { w.focused_child( this ); w.focus_tree(); visible(Yes); }
@


1.11
log
@log control written
@
text
@d8 3
d51 1
d237 62
@


1.10
log
@update
@
text
@d8 3
a107 9
/*
    char str[sw_max_width*2];
    
    for( int x = xs(); x--; )
        {
        str[x<<1    ] = bchars[bt][0];
        str[(x<<1)+1] = col.value();
        }
*/
a121 64





#if 0
void sw_abstract_win::fill( sw_color col, char ch, sw_rect r, bool excl_border )
    {
    char str[sw_max_width*2];

    for( int x = xs(); x--; )
        {
        str[x<<1    ] = ch;
        str[(x<<1)+1] = col.value();
        }

    if(excl_border)
        {
        if( r.x() == 0 ) r.x(1);
        if( r.y() == 0 ) r.y(1);
        if( r.xend() == xs() ) r.xend(xs()-1);
        if( r.yend() == ys() ) r.yend(ys()-1);
        }
    int ye = r.yend();
    for( int y = r.y(); y < ye; y++ )
        put( sw_rect(r.x(),y,r.xs(),1), str );
    }

void sw_abstract_win::border( sw_color col, sw_border_t bt )
    {
    static char *bchars[] =
        {
        "      ",
        "ÄÚ¿ÀÙ³",        "ÍÉ»È¼º",
        "ÄÖ·Ó½º",        "ÍÕ¸Ô¾³",
        "ÛÛÛÛÛÛ",        "ùùùùùù"
        };
    char str[sw_max_width*2];
    
    for( int x = xs(); x--; )
        {
        str[x<<1    ] = bchars[bt][0];
        str[(x<<1)+1] = col.value();
        }

    str[0] = bchars[bt][1];
    str[(xs()-1)*2] = bchars[bt][2];
    put( sw_rect(0,0,xs(),1), str );
    str[0] = bchars[bt][3];
    str[(xs()-1)*2] = bchars[bt][4];
    put( sw_rect(0,ys()-1,xs(),1), str );

    for( int y = ys()-1; y-- > 1; )
        {
        str[0] = bchars[bt][5];
        str[1] = col.value();
        put( sw_rect(0,y,1,1), str );
        put( sw_rect(xs()-1,y,1,1), str );
        }
    
    }
#endif


d134 3
a136 1
sw_abstract_win::sw_abstract_win( const sw_size &s ) : sw_size(s), map(s)
d238 1
a238 1
    : w(parent), sw_point(pos_size), sw_abstract_win(pos_size)
@


1.9
log
@fast fill - now we're passing fill data
down to device and do actual fill there.
Prepared to do same thing with border, but
not sure yet I really will.
@
text
@d8 6
d104 2
d113 8
a120 14

      // upper hor line
    fill( col, bchars[bt][0], sw_rect(1,0,xs()-2,1), No );
      // lower hor line
    fill( col, bchars[bt][0], sw_rect(1,ys()-1,xs()-2,1), No );

      // ul corner
    put( sw_rect(0,0,1,1), col, bchars[bt][1] );
      // ur corner
    put( sw_rect(xs()-1,0,1,1), col, bchars[bt][2] );
      // ll corner
    put( sw_rect(0,ys()-1,1,1), col, bchars[bt][3] );
      // lr corner
    put( sw_rect(xs()-1,ys()-1,1,1), col, bchars[bt][4] );
d124 2
a125 4
      // left ver line
    fill( col, bchars[bt][5], sw_rect(0,1,1,ys()-2), No );
      // left ver line
    fill( col, bchars[bt][5], sw_rect(xs()-1,1,1,ys()-2), No );
@


1.8
log
@faster map rebuild. not much, though.
@
text
@d8 3
d79 19
d99 1
a99 1

d102 1
a102 1
        str[x<<1    ] = ch;
d106 27
d134 5
a138 1
    if(excl_border)
d140 2
a141 2
        for( int y = ys()-1; y-->1; )
            put( sw_rect(1,y,xs()-2,1), str );
d143 1
a143 6
    else
        {
        for( int y = ys(); y--; )
            put( sw_rect(0,y,xs(),1), str );
        }
#else
a153 1
#endif
a155 1

d189 1
a342 1
    //if( &who != this ) who.
d346 13
@


1.7
log
@menus basically done
@
text
@d8 3
d173 1
a173 1
void sw_abstract_win::do_rebuild_map() // rebuilds map of child windows
d178 3
a180 1
        child[i]->do_rebuild_map();
d185 1
a185 1
void sw_abstract_win::do_rebuild_map_to_root()
d187 1
a187 1
    do_rebuild_map();
d190 1
a190 1
void sw_abstract_win::rebuild_map()
d192 1
a192 1
    do_rebuild_map_to_root();
d200 1
a200 1
    rebuild_map();
d213 1
a213 1
    rebuild_map();
d225 3
a227 1
            rebuild_map();
a229 1
    refresh(w);
d240 3
a242 1
            rebuild_map();
a244 1
    refresh(w);
d277 1
a277 1
    w.rebuild_map();
d288 1
a288 1
    w.rebuild_map();
d301 1
a301 1
void sw_win::do_rebuild_map_to_root()
d303 3
a305 1
    w.do_rebuild_map_to_root();
@


1.6
log
@generalized event model
@
text
@d8 3
d302 2
a303 2
void sw_win::focus()      { w.focused_child( this ); }
void sw_win::focus_tree() { w.focused_child( this ); w.focus_tree(); }
@


1.5
log
@last version with distinct broadcast/event entry points
@
text
@d8 3
d237 1
a237 1
void sw_abstract_win::pass_broadcast_to_children( const sw_event &e )
d239 1
d241 2
a242 1
        child[i]->broadcast(e);
@


1.4
log
@update
@
text
@d8 3
d197 1
d199 2
@


1.3
log
@works a little
@
text
@d8 3
d34 6
d44 11
d97 2
a98 1
void sw_abstract_win::border( sw_color col, sw_border_t t )
d100 7
d111 1
a111 1
        str[x<<1    ] = 'Ä';
d115 2
a116 2
    str[0] = 'Ú';
    str[(xs()-1)*2] = '¿';
d118 2
a119 2
    str[0] = 'À';
    str[(xs()-1)*2] = 'Ù';
d124 1
a124 1
        str[0] = '³';
d228 7
d246 1
a246 1
    //w.refresh(sw_rect((sw_point&)*this,(sw_size&)*this));
d284 7
@


1.2
log
@update
@
text
@d8 3
d22 2
a23 1
sw_win_ops::sw_win_ops() {}
d29 1
a29 1
void sw_abstract_win::put( sw_point &p, sw_color c, char ch )
d33 1
a33 1
void sw_abstract_win::put( sw_point &p, sw_color c, const char *s )
d37 4
d42 1
a42 1
void sw_abstract_win::fill( sw_color col, char ch )
d52 23
a74 3
    for( int y = ys(); y--; )
        put_up( sw_rect(0,y,xs(),1), str );
    
d77 1
a77 1
void sw_abstract_win::refresh()
d79 23
a101 2
    for( int i = 0; i < child.size(); i++ )
        child[i]->refresh();
d104 2
a105 1
void sw_abstract_win::put_cw( const sw_win &who, const sw_rect &p, const char *char_and_attr_map )
d107 6
a116 1

d119 1
d127 1
a127 1
void sw_abstract_win::resize( int x, int y )
d129 2
a130 2
    sw_size::resize(x,y);
    map.resize(sw_size(x,y));
d133 1
a133 1
void sw_abstract_win::rebuild_map() // rebuilds map of child windows
d135 1
a135 1
    map.clear();
d137 14
a150 1
        map.mark( i, *(child[i]) );
d154 1
d169 1
d179 1
a179 1
            child.insert( child.begin(), &w );
d183 1
d193 1
a193 1
            child.push_back( &w );
d197 1
d211 1
d215 1
a215 1
void sw_win::move( int x, int y )
d217 4
a220 1
    sw_point::move(x,y);
d222 2
a223 1
    w.refresh();
d226 1
a226 1
void sw_win::resize( int x, int y )
d228 4
a231 1
    sw_win::resize(x,y);
d233 2
a234 1
    w.refresh();
d237 1
a237 5

  // --------------------------- sw_map -----------------------------


void sw_map::allocme()
d239 4
a242 4
    int bytes = s.xs() * s.ys();
    v = new char[ bytes ];
    if( v == NULL ) throw sw_Out_Of_Memory("sw_map::allocme");
    clear();
d245 1
a245 1
void sw_map::freeme()
d247 1
a247 2
    delete [] v;
    v = NULL;
a249 4
sw_map::sw_map( const sw_size &_s ) : s(_s)
    {
    allocme();
    }
d251 5
a255 1
sw_map::~sw_map()
d257 6
a262 8
    freeme();
    }
        
void sw_map::resize( const sw_size &delta )
    {
    freeme();
    s.resize( delta.xs(), delta.ys() );
    allocme();
d265 13
a277 1
void sw_map::clear()
d279 2
a280 1
    memset( v, -1, s.xs()*s.ys() ); // -1 == uninitialized
d283 2
a284 14
void sw_map::mark( int index, sw_win &w )
    {
    int xbegin = w.x();
    int xend = w.x() + w.xs();
    if( xend > s.xs() ) xend = s.xs();
    
    int ybegin = w.y();
    int yend = w.y() + w.ys();
    if( yend > s.ys() ) yend = s.ys();
    
    for( int yi = ybegin; yi < yend; yi++ )
        for( int xi = xbegin; xi < xend; xi++ )
            v[xi+(yi*s.xs())] = index;
    }
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: Tic.c $
d19 3
d24 62
a85 1
void sw_abstract_win::attach_child( sw_abstract_win &w )
d91 1
a91 1
void sw_abstract_win::detach_child( sw_abstract_win &w )
d101 25
d128 82
@
