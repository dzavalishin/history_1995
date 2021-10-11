head	1.6;
access;
symbols;
locks;
comment	@ * @;


1.6
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.10.07.19.40.27;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.10.07.11.15.09;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.10.01.16.32.27;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.28.14.03.17;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.20.14.40.18;	author dz;	state Exp;
branches;
next	;


desc
@@


1.6
log
@Before changing event class
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: entry fileds classes impl.
 *
 *      $Log: ctl_entry.c $
 *      Revision 1.5  1996/10/07 19:40:27  dz
 *      new 3d entry fields
 *
 *      Revision 1.4  1996/10/07 11:15:09  dz
 *      3d entry fields, wrong way
 *
 *      Revision 1.3  1996/10/01 16:32:27  dz
 *      bugfixes - a lot of
 *
 *      Revision 1.2  1996/09/28 14:03:17  dz
 *      update
 *
 *      Revision 1.1  1996/09/20 14:40:18  dz
 *      Initial revision
 *
 *      
 *
\*/


#include "sw_ctl_entry.h"
#include "sw_except.h"


bool sw_entry_base::event_preprocessor( const sw_event &e )
    {
    char c[2];
    char a = c[0] = e.akey();
    char f = e.fkey();
    
    if(
       ((a != 0 && a != 0xe0) || (a == 0xe0 && (f==0||f==0x23))) &&
       f != 0x0e &&  // pass backspace key
       f != 0x01 &&  // pass esc key
       f != 0x1C     // pass enter key
      )
        {
        c[1] = 0;
        insert_here(c);
        display_part( pos_v, pos_v+xs() );
        next(); // advance cursor
        return Yes;
        }


    switch( f )
        {
        case sw_key::left:  prev();         break;
        case sw_key::right: next();         break;

        case sw_key::cleft: prev_word();    break;
        case sw_key::cright:next_word();    break;

        case sw_key::home:  home();         break;
        case sw_key::end:   end();          break;

        case sw_key::del:
            delete_here();
            display_part( pos_v, pos_v+xs() );
            break;

        case sw_key::bs:
            if( pos_v > 0 )
                {
                prev();
                delete_here();
                display_part( pos_v, pos_v+xs() );
                }
            break;

        default:
            return No;
        }
    
    return Yes;
    }


void sw_entry_base::refresh(const sw_rect&a)
    {
    display_part(
                 shift_v+(a.x() < 0 ? 0 : a.x()),
                 shift_v+(a.xend() > xs() ? xs() : a.xend())
                );
    }

void sw_entry_base::set_pos(int np)
    {
    //int vs = xs()-1; // visible size of line
    int vs = xs(); // visible size of line
    int sz = size(); // characters in string
    
    if( np == pos_v )  return;
    if( np < 0  )  np = 0;
    if( np > sz )  np = sz;

    
    int old_pos = pos_v;
    pos_v = np;
    
    int old_shift = shift_v;
    
    if( pos_v-1 < shift_v ) shift_v = pos_v - 1;
    if( shift_v < 0 )       shift_v = 0;
    
      // We contain less that we can show here
    if( sz <= vs ) shift_v = 0;
    else
        {
        if( pos_v+1 >= shift_v+vs )  shift_v = pos_v - vs + 2;
        if( shift_v-1 >= sz - vs )   shift_v = (sz - vs) + 1;
        }
    
    if( old_shift == shift_v )
        {
        display_part( old_pos, old_pos );
        display_part( pos_v, pos_v );
        }
    else
        display_part( shift_v, shift_v+vs );
    }


  // -------------------------------------------------------------------------------


sw_entry::sw_entry( sw_abstract_win &parent, const sw_rect &rect, sw_color lc, sw_color sc /*, sw_color shadow_c */ )
    : sw_entry_base( parent, rect ), lcolor_v(lc), scolor_v(sc) //, shadow_v(shadow_c)
    {
    refresh(sw_rect(sw_point(0,0),*this));
    }

void sw_entry::display_part( int from, int to )
    {
    sw_point here(sw_point(from-shift_v,0));
      //int vs = xs()-1; // visible size of line
    int vs = xs(); // visible size of line
    int sz = size(); // characters in string

    if( from < 0 ) from = 0;
    if( to >= sz ) to = sz-1;
    if( sz-shift_v < vs )
        fill( lcolor_v, '°', sw_rect(sz-shift_v,0,vs-(sz-shift_v),1) );

    if( !(to < shift_v || from >= shift_v+sz) )
        put( sw_point(0,0), lcolor_v, text_v.substr(shift_v,vs) );

    if( in_focus )
        {
        if( pos_v >= sz )
            put( sw_point(pos_v-shift_v,0), scolor_v, "°" );
        else
            put( sw_point(pos_v-shift_v,0), scolor_v, text_v.substr(pos_v,1) );
        }
    }

void sw_entry::insert_here( string s )
    {
    if(pos_v == 0) text_v = s + text_v.substr(pos_v);
    else           text_v = text_v.substr(0,pos_v) + s + text_v.substr(pos_v);
    }

void sw_entry::delete_here()
    {
    text_v.strip(pos_v,1);
    display_part( pos_v, pos_v+1 );
    }

void sw_entry::prev_word()
    {
    for( int i = pos_v-1; i >= 0 && text_v[i] == ' '; i-- )
        ;
    
    for( ; i > 0; i-- )
        if( text_v[i-1] == ' ' )
            {
            set_pos(i);
            return;
            }
    set_pos(0);
    }

void sw_entry::next_word()
    {
    for( int i = pos_v+1; i < size() && text_v[i] != ' '; i++ )
        ;
    
    for( ; i < size(); i++ )
        if( text_v[i] != ' ' )
            {
            set_pos(i);
            return;
            }
    set_pos(size());
    }





#if 0
void sw_entry::refresh(const sw_rect&a)
    {
    sw_entry_base::refresh(a);
    put( sw_point(xs()-1,0), shadow_v, "Ü" );
    if( a.y() < 2 || a.yend() > 1 )
        {
        put( sw_point(0,1), shadow_v, " " );
        put( sw_point(1,1), shadow_v, lower_line_1 );
        }
    }
#endif



  // -------------------------------------------------------------------------------


void sw_3d_shadow::refresh( const sw_rect&a )
    {
    put( sw_point(xs()-1,0), shadow_v, "Ü" );
    //if( a.y() < 2 || a.yend() > 1 )
    //    {
      //put( sw_point(0,((sw_rect&)*this).yend()-1), shadow_v, " " );
      //put( sw_point(1,((sw_rect&)*this).yend()-1), shadow_v, lower_line );
    put( sw_point(0,ys()-1), shadow_v, " " );
    put( sw_point(1,ys()-1), shadow_v, lower_line );
    //    }
    }

sw_rect sw_3d_shadow::extend_rect( const sw_rect &r1 )
    {
    sw_rect r = r1;
    r.xs( r.xs()+1 );
    r.ys( r.ys()+1 );
    return r;
    }


@


1.5
log
@new 3d entry fields
@
text
@d8 3
@


1.4
log
@3d entry fields, wrong way
@
text
@d8 3
d93 2
a94 1
    int vs = xs()-1; // visible size of line
d131 2
a132 2
sw_entry::sw_entry( sw_abstract_win &parent, const sw_rect &rect, sw_color lc, sw_color sc, sw_color shadow_c )
    : sw_entry_base( parent, rect ), lcolor_v(lc), scolor_v(sc), shadow_v(shadow_c)
d140 2
a141 1
    int vs = xs()-1; // visible size of line
d205 1
a205 1

d216 18
d235 7
@


1.3
log
@bugfixes - a lot of
@
text
@d8 3
d90 3
d94 3
a96 2
    if( np < 0 )       np = 0;
    if( np > size() )  np = size();
d107 1
a107 1
    if( size() < xs() ) shift_v = 0;
d110 2
a111 2
        if( pos_v+1 >= shift_v+xs() )    shift_v = pos_v - xs() + 2;
        if( shift_v-1 >= size() - xs() )   shift_v = (size() - xs()) + 1;
d120 1
a120 1
        display_part( shift_v, shift_v+xs() );
d127 2
a128 2
sw_entry::sw_entry( sw_abstract_win &parent, const sw_rect &rect, sw_color lc, sw_color sc )
    : sw_entry_base( parent, rect ), lcolor_v(lc), scolor_v(sc)
a130 1
      //refresh((sw_rect&)*this);
d136 2
d140 3
a142 3
    if( to >= size() ) to = size()-1;
    if( size()-shift_v < xs() )
        fill( lcolor_v, '°', sw_rect(size()-shift_v,0,xs()-(size()-shift_v),1) );
d144 2
a145 2
    if( !(to < shift_v || from >= shift_v+size()) )
        put( sw_point(0,0), lcolor_v, text_v.substr(shift_v) );
d149 1
a149 1
        if( pos_v >= size() )
d200 11
@


1.2
log
@update
@
text
@d8 3
d130 1
a130 1
    
a132 2
    if( to < shift_v || from >= shift_v+size() ) return; // out of view

d136 10
a145 5
    put( sw_point(0,0), lcolor_v, text_v.substr(shift_v) );
    if( pos_v >= size() )
        put( sw_point(pos_v-shift_v,0), scolor_v, "°" );
    else
        put( sw_point(pos_v-shift_v,0), scolor_v, text_v.substr(pos_v,1) );
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: win.c $
d29 1
@
