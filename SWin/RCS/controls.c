head	1.9;
access;
symbols;
locks;
comment	@ * @;


1.9
date	96.11.23.10.19.59;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.11.18.16.16.26;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.11.15.21.37.58;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.10.11.17.02.37;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.10.10.23.37.07;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.10.03.07.43.57;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.20.14.40.18;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.12.01.12.51;	author dz;	state Exp;
branches;
next	;


desc
@works a little
@


1.9
log
@transparency, clock
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: control window classes impl.
 *
 *      $Log: controls.c $
 *      Revision 1.8  1996/11/18 16:16:26  dz
 *      Event class now has no type field/method, but
 *      has info() == KeyPress and delivery mode == Mouse.
 *      If event::mouse() is nonzero, pos is valid.
 *
 *      Revision 1.7  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
 *      Revision 1.6  1996/11/15 21:37:58  dz
 *      preliminary mouse support done
 *
 *      Revision 1.5  1996/10/11 17:02:37  dz
 *      context menus done, menus code fixed to
 *      support lone vertical menus correctly.
 *
 *      Revision 1.4  1996/10/10 23:37:07  dz
 *      reorganized devices classes, generalized
 *      videomemory-style devices
 *
 *      Revision 1.3  1996/10/03 07:43:57  dz
 *      buttons
 *
 *      Revision 1.2  1996/09/20 14:40:18  dz
 *      update
 *
 *      Revision 1.1  1996/09/12 01:12:51  dz
 *      Initial revision
 *
 *
 *      
 *
\*/

#include "sw_controls.h"
#include "sw_ctl_list.h"
#include "sw_device.h" // flush



void sw_control::label( const char *text, sw_color color )
    {
    del_label();
    label_v = new sw_label( w, sw_point( x() - strlen(text), y() ), text, color );
    }

void sw_control::del_label()
    {
    if( label_v ) delete label_v;
    label_v = 0;
    }

void sw_control::context_menu(sw_menu_base *m)
    {
    delete context_menu_v;
    context_menu_v = m;
    }

bool sw_control::event_postprocessor( const sw_event &e )
    {
    if( context_menu_v && e.info() == sw_event::ContextMenu )
        {
        context_menu_v->push_focus();
        return Yes;
        }

    return No;
    }


  // -------------------------------------------------------------------------

sw_label::sw_label(
                   sw_abstract_win &parent, sw_point pos,
                   const char *_text, sw_color color
                  )
    :
color_v(color),
sw_control( parent, sw_rect(pos,sw_size(strlen(_text),1))  )
    {
    text_v = 0;
    len_v = 0;
    text(_text);
    focusable_v = No; // Don't try to set focus to us
    }

sw_label::sw_label( sw_abstract_win &parent, sw_color color ) :
color_v(color), sw_control( parent, sw_rect(0,0,1,1)  )
    {
    text_v = 0;
    len_v = 0;
    }


void sw_label::text( const char *s )
    {
    delete [] text_v;
    len_v = strlen(s);
    text_v = new char[len_v*2];
    if(len_v-xs()) resize(sw_size(len_v-xs(),0));
    for( int i = 0; i < len_v*2; i++ )
        {
        text_v[i++] = *s ? *s++ : ' ';
        text_v[i] = color_v.value();
        }
    refresh(sw_rect(0,0,len_v,1));
    }

sw_label::~sw_label()
    {
    delete [] text_v;
    }
        
void sw_label::refresh(const sw_rect&a)
    {
    if( text_v == 0 ) return;
    sw_rect r;
      //if( r.intersection(a, sw_rect(0,0,len_v,1) ) )
      //  put( r, text_v+(r.x()*2) );
    put( sw_rect(0,0,len_v,1), text_v );
    }



  // -------------------------------------------------------------------------

void sw_abs_button::button_press()
    {
    state(1);
    device.flush();
    //sw_sleep(128); BUG!! Here we must send us a 'release' message somehow?
    state(0);
    }

bool sw_abs_button::event_preprocessor( const sw_event &e )
    {
    switch( e.info() )
        {
        case sw_event::Mouse1Press:
            focus_tree();
            state(1);
            device.mouse_capture( this );
            return Yes;
            
              //        case sw_event::Mouse1Release:
        case sw_event::EndOfCapture:
            state(0);
            return Yes;
        }
    
    return No;
    }


  // -------------------------------------------------------------------------

sw_onechar_button::sw_onechar_button
    (
     sw_abstract_win &parent, sw_point pos,
     sw_color color, char _c1, char _c2
    ) : sw_abs_button( parent, sw_rect(pos,sw_size(1,1)) )
    {
    c1[0] = _c1;
    c2[0] = _c2;
    c1[1] = color.value();
    c2[1] = color.value();
    }

void sw_onechar_button::refresh(const sw_rect&)
    {
    put( sw_rect(0,0,1,1), state_v ? c2 : c1 );
    }
    

  // -------------------------------------------------------------------------

sw_text_button::sw_text_button
    (
     sw_abstract_win &parent, sw_point pos, const string &text,
     sw_color c1, sw_color cs, sw_color c2
    )
    :
sw_abs_button( parent, sw_rect(pos,sw_size(text.length()+1,2)) ),
c1_v(c1), cs_v(cs), c2_v(c2),
text_v( text ), l( text.length() ),
lower_line_1('�',text.length()), lower_line_2(' ',text.length()+1)
    {
    map.set_transparent( sw_point(0,ys()-1) );    // 1 above
    }

void sw_text_button::refresh(const sw_rect&)
    {
    if( state_v )
        {
        put( sw_point(1,0), in_focus ? cs_v : c1_v, text_v );
        put( sw_point(0,0), c2_v, " " );
        put( sw_point(0,1), c2_v, lower_line_2 );
        }
    else
        {
        put( sw_point(0,0), in_focus ? cs_v : c1_v, text_v );
        put( sw_point(l,0), c2_v, "�" );
        put( sw_point(0,1), c2_v, " " );
        put( sw_point(1,1), c2_v, lower_line_1 );
        }
    }


  // -------------------------------------------------------------------------


@


1.8
log
@Event class now has no type field/method, but
has info() == KeyPress and delivery mode == Mouse.
If event::mouse() is nonzero, pos is valid.
@
text
@d8 5
d194 1
@


1.7
log
@Before changing event class
@
text
@d8 3
d62 1
a62 1
    if( e.type() == sw_event::Info || e.type() == sw_event::Mouse )
d64 2
a65 5
        if( context_menu_v && e.info() == sw_event::ContextMenu )
            {
            context_menu_v->push_focus();
            return Yes;
            }
d67 1
@


1.6
log
@preliminary mouse support done
@
text
@d8 3
d59 1
a59 1
    if( e.type() == sw_event::Info )
d140 1
@


1.5
log
@context menus done, menus code fixed to
support lone vertical menus correctly.
@
text
@d8 4
d130 18
@


1.4
log
@reorganized devices classes, generalized
videomemory-style devices
@
text
@d8 4
d27 1
d35 1
a35 1
    l = new sw_label( w, sw_point( x() - strlen(text), y() ), text, color );
d40 8
a47 2
    if( l ) delete l;
    l = 0;
d50 12
@


1.3
log
@buttons
@
text
@d8 3
d25 18
d45 3
a47 2
                   char *_text, sw_color color
                  ) :
d49 1
a49 1
sw_win( parent, sw_rect(pos,sw_size(strlen(_text),1))  )
d54 1
d58 1
a58 1
color_v(color), sw_win( parent, sw_rect(0,0,1,1)  )
d157 3
@


1.2
log
@update
@
text
@d8 3
d20 1
d72 10
d83 1
d102 33
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: sw_win.h $
d20 1
a20 1
                   char *_text, int len = 0, sw_color color = sw_color::Yellow
a21 1
len_v(len?len:strlen(_text)),
d23 1
a23 1
sw_win( parent, sw_rect(pos,sw_size(len?len:strlen(_text),1))  )
d25 2
a26 1
    text_v = new char[len_v*2];
d29 9
a37 1
        
d40 4
d49 1
d59 1
d73 1
a73 1
     sw_color color = sw_color::Yellow, char _c1 = '', char _c2 = 9
@
