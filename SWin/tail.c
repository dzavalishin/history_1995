/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: colored log display
 *
 *      $Log: tail.c $
 *      Revision 1.7  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.6  1996/11/15 13:12:49  dz
 *      update
 *
 *      Revision 1.5  1996/10/02 11:01:11  dz
 *      focusable
 *
 *      Revision 1.4  1996/10/01 16:32:27  dz
 *      bugfixes - a lot of
 *
 *      Revision 1.3  1996/09/30 14:18:41  dz
 *      log control written
 *
 *      Revision 1.2  1996/09/30 12:51:06  dz
 *      *** empty log message ***
 *
 *      Revision 1.1  1996/09/30 09:43:52  dz
 *      Initial revision
 *
 *
 *      
 * TODO: ...
 *
\*/

#include "sw_tail.h"
#include "sw_device.h"
#include <vector.h>

#include <stdlib.h>

#define INCL_DOSPROCESS
#include <os2.h>

static char *Id = "$Id: tail.c 1.7 1996/11/18 15:47:54 dz Exp $";



const max_time = 170;

void wait_sema(void);
void close_sema(void);
void create_sema(void);



static sw_tail::cid cid_from_time( int time )
    {
      // можно выразить в общем виде
    if( time <  10 ) return 0;
    if( time <  20 ) return 1;
    if( time <  40 ) return 2;
    if( time <  80 ) return 3;
    if( time < 160 ) return 4;
    return 5;
    }
        
static const n_col_from_cid = 6;
static sw_color col_from_cid[n_col_from_cid] =
    {
    sw_color::White, sw_color::Yellow, sw_color::LightRed,
    sw_color::LightMagenta, sw_color::Brown, sw_color::DarkGray
    };










sw_tail::line::line(const string &s) : text( s )
    {
    time = 0;
    changed = 1;
    current_color = 0;
    recommended_color = 0;
    }

sw_tail::line::line()
    {
    time = 0;
    changed = 1;
    current_color = 0;
    recommended_color = 0;
    }


void sw_tail::line::tick() 
    {
    if( time <= max_time )
        {
        time++;
        recommended_color = cid_from_time(time);
        }
    }

  /* arg - color of line above. */
  /* returns 1 if color changed */
int sw_tail::line::repaint( sw_tail::cid prev_line )
    {
    cid our_recommended_color = recommended_color;
      // предотвратить слипание цветов
    if( our_recommended_color >= prev_line )
        {
        if( our_recommended_color <= 0 )
            return 0;
          //our_recommended_color = our_recommended_color.prev();
        our_recommended_color--;
        }
    if( our_recommended_color <= current_color )
        return 0;

    current_color = our_recommended_color;
    touch();
    return 1;
    }

void sw_tail::line::set( const char *_text )
    {
    text = _text;
    time = 0;
    current_color = cid_from_time(time);
    touch();
    }

void sw_tail::line::scroll( line &l )
    {
    text = l.text;
    time = l.time;
    current_color = l.current_color;
    recommended_color = l.recommended_color;
    touch();
    }


void sw_tail::display_elem( int lno )
    {
    if( lno < 0 || lno >= lines.size() )
        fill( col_from_cid[0], ' ', sw_rect(0,lno-shift_v,xs(),1) );
    else
        {
        line &l = lines[lno];
        sw_color c = col_from_cid[l.get_current_color()];
        if( lno == pos_v && in_focus ) c.back( scolor_v.value() );
        put( sw_point(0,lno-shift_v), c, l.get_text() );
        l.untouch();
        }
    }

void sw_tail::cond_display_all()
    {
    int ie = visible_elems();
    for( int i = 0; i < ie; i++ )
        {
        int lno = i+shift_v;
        if( lno < 0 || lno >= lines.size() )
            fill( col_from_cid[0], ' ',  sw_rect(0,i+shift_v,xs(),1) );
        else
            {
            line &l = lines[lno];
            if( !l.is_changed() ) continue;
            sw_color c = col_from_cid[l.get_current_color()];
            if( lno == pos_v && in_focus ) c.back( scolor_v.value() );
            put( sw_point(0,i), c, l.get_text() );
            l.untouch();
            }
        }
    }

void sw_tail::add( const string & str )
    {
    int atend = pos_v == size() - 1;
    string s1; s1.print( "%-*.*s", xs(), xs(), str.c_str() );
    lines.push_back( s1 );
    if( lines.size() > max_v )
        {
        lines.erase(lines.begin());
        display_all();
        }
    if( atend )                set_pos( size() - 1 );
    cond_display_all();
    }

void sw_tail::tick()
    {
    for( int i = 0; i < lines.size(); i++ )
        lines[i].tick();
    
    sw_tail::cid new_color = 8;
    for( i = 0; i < lines.size(); i++ )
        {
        if( lines[i].repaint( new_color ) )
            new_color = lines[i].get_current_color();
        }
    cond_display_all();
    device.flush();
    }







extern "OPTLINK" void sw_tail_ticker(void *val)
    {
    sw_tail *obj = (sw_tail*)val;
    while(1)
        {
        DosSleep( 1000 );
        obj->tick();
        }
    }

sw_tail::sw_tail(
                 sw_abstract_win &parent, const sw_rect &r, int maxlines,
                 sw_color bg, sw_color sc
                )
    :
sw_list_base( parent, r ), scolor_v(sc), max_v(maxlines)
    {
    _beginthread( sw_tail_ticker, 0, 16*1024, this );
    for( int i = 0; i < n_col_from_cid; i++ )
        col_from_cid[i].back(bg.value());
    }











