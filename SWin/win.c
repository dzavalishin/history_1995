/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: window classes impl.
 *
 *      $Log: win.c $
 *      Revision 1.19  1996/11/22 13:02:38  dz
 *      Nonrectangular (partially transparent) windows
 *
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

void sw_abstract_win::fill( sw_color col, char ch, sw_rect r /*, bool excl_border */ )
    {
/*
    if(excl_border)
        {
        if( r.x() == 0 ) r.x(1);
        if( r.y() == 0 ) r.y(1);
        if( r.xend() == xs() ) r.xend(xs()-1);
        if( r.yend() == ys() ) r.yend(ys()-1);
        }
*/
    fill_cw( *this, col, ch, r );
    }

void sw_abstract_win::border( sw_color col, sw_border_t bt )
    {
    static char *bchars[] =
        {
        "      ",
        "�ڿ�ٳ",        "�ɻȼ�",
        "�ַӽ�",        "�ոԾ�",
        "������",        "������"
        };

    fill( col, bchars[bt][0], sw_rect(1,0,xs()-2,1) );           // up hor line
    fill( col, bchars[bt][0], sw_rect(1,ys()-1,xs()-2,1) );      // lo hor line

    put( sw_rect(0,0,1,1), col, bchars[bt][1] );                 // ul corner
    put( sw_rect(xs()-1,0,1,1), col, bchars[bt][2] );            // ur corner
    put( sw_rect(0,ys()-1,1,1), col, bchars[bt][3] );            // ll corner
    put( sw_rect(xs()-1,ys()-1,1,1), col, bchars[bt][4] );       // lr corner

      // BUG! Here we can get better performance if
      // a special vfill method will be added
    fill( col, bchars[bt][5], sw_rect(0,1,1,ys()-2) );           // left ver line
    fill( col, bchars[bt][5], sw_rect(xs()-1,1,1,ys()-2) );      // right ver line
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
    our_border = 0;
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

void sw_win::visible( bool yn)
    {
    if( yn ) top(); else bot();
    if(our_border)
        (*our_border).visible(yn);
    }

void sw_win::move( int _x, int _y )
    {
    sw_rect r1 ((sw_point&)*this,(sw_size&)*this);
    sw_point::move(_x,_y);
    sw_rect r2 ((sw_point&)*this,(sw_size&)*this);
    r2.summ( r1, r2 );

    calculate_absolute_window_posiotion(); // we are moved!

    if(our_border)
        (*our_border).move(_x,_y);
            
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
    if(our_border)
        (*our_border).resize(s);
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

void sw_win::top()
    {
    w.top_child( *this );
    if(our_border)
        w.below_child( *our_border, *this );
    }
void sw_win::bot()
    {
    w.bot_child( *this );
    if(our_border)
        w.bot_child( *our_border );
    }
void sw_win::above( const sw_win &ref )
    {
    w.above_child( *this, ref );
    if(our_border)
        w.below_child( *our_border, *this );
    }
void sw_win::below( const sw_win &ref )
    {
    w.below_child( *this, ref );
    if(our_border)
        w.below_child( *our_border, *this );
    }



void sw_win::focus()      { visible(Yes); w.focused_child( this ); }
void sw_win::focus_tree() { visible(Yes); w.focused_child( this ); w.focus_tree(); }



  // ------------------------ sw_bordered_win --------------------------



void sw_bordered_win::refresh(const sw_rect &_sq)
    {
    sw_rect sq;
    if( !sq.intersection( _sq, sw_rect(0,0,xs(),ys()) )) return;
    sw_abstract_win::refresh(sq);
    fill(fill_color_v,fill_char_v,sq);
    //fill(fill_color_v,fill_char_v,sq,(bool)(border_type_v!=No_Border));
    //if(border_type_v!=No_Border)        border(border_color_v,border_type_v);
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
    //border_type_v(border_type),
      //border_color_v(border_color),
    border_v(*this,border_type,border_color),
    fill_color_v(fill_color), fill_char_v(fill_char)
    {
    refresh(sw_rect(sw_point(0,0),*this));
    }

sw_bordered_win::~sw_bordered_win()
    {}


  // ------------------------ sw_border --------------------------



void sw_border::refresh(const sw_rect &_sq)
    {
    sw_abstract_win::refresh(_sq);
    sw_rect sq,sq1;
    if(
       sq.intersection( _sq, sw_rect(0,0,xs(),1) ) ||
       sq.intersection( _sq, sw_rect(0,0,1,ys()) ) ||
       sq.intersection( _sq, sw_rect(0,ys()-1,xs(),1) ) ||
       sq.intersection( _sq, sw_rect(xs()-1,0,1,ys()) )
      )
        border(border_color_v,border_type_v);
    }

sw_border::sw_border(
                           sw_win &parent,
                           sw_border_t border_type,
                           sw_color border_color
                          )
    : sw_win( parent.w, sw_rect(parent.x()-1,parent.y()-1,parent.xs()+2,parent.ys()+2) ),
    border_type_v(border_type),
    border_color_v(border_color),
    bordered_window(parent)
    {
      //w.refresh(sw_rect((sw_point&)*this,(sw_size&)*this));
    sw_rect r(0,0,xs(),ys());
    if( border_type != No_Border )
        {
        // a hole inside of us will be transparent
        r.resize( sw_size(-2, -2) );
        r.move( 1, 1 );
        }
      // if No_Border, we'll make transparent all the window completely
    map.set_transparent( r );
    if( border_type != No_Border )
        parent.register_border(this);
    else
        bot(); // go bottom and don't give parent chance to get us up
    refresh(sw_rect(sw_point(0,0),*this));
    }

sw_border::~sw_border()
    {
    bordered_window.register_border(0);
    }


