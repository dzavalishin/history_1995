head	1.14;
access;
symbols;
locks;
comment	@ * @;


1.14
date	96.11.22.13.02.38;	author dz;	state Exp;
branches;
next	1.13;

1.13
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	96.10.11.17.02.37;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.10.10.23.37.07;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.10.01.16.32.27;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.10.01.05.27.39;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.09.29.05.54.19;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.09.29.03.37.08;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.09.28.22.58.52;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.09.28.14.03.17;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.09.27.07.26.09;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.09.26.20.46.52;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.21.08.07.06;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.20.14.40.18;	author dz;	state Exp;
branches;
next	;


desc
@@


1.14
log
@Nonrectangular (partially transparent) windows
@
text
@
#include "sw_ctl_list.h"
#include "sw_except.h"


bool sw_list_base::event_preprocessor( const sw_event &e )
    {
    sw_point p;
    if( our_mouse_event( p, e) )
        {
          //if( in_focus )
        switch( e.info() )
            {
            case sw_event::Mouse1Press:
            //case sw_event::Mouse2Press:
            //case sw_event::Mouse3Press:
                set_pos( p.y() + shift_v /*+ (border_type_v==No_Border ? 0 : -1) */ );
                break;
            }
        return No;
        }
    
    switch( e.fkey() )
        {
        case sw_key::up:    prev(); return Yes;
        case sw_key::down:  next(); return Yes;

        case sw_key::pgup:  pgup(); return Yes;
        case sw_key::pgdn:  pgdn(); return Yes;
            
        case sw_key::home:  home(); return Yes;
        case sw_key::end:   end();  return Yes;
            
        }

    return No;
    }


void sw_list_base::refresh(const sw_rect&a)
    {
    int end = a.yend() > ys() ? ys() : a.yend();
    for( int i = a.y() < 0 ? 0 : a.y(); i < end; i++ )
        display_elem( shift_v+i );
    }

void sw_list_base::set_pos(int np)
    {
    int sz = size();
    
    if( np == pos_v )  return;
    if( np < 0 )       np = 0;
    if( np >= sz )     np = sz - 1;
    
    int old_pos = pos_v;
    pos_v = np;

    int old_shift = shift_v;
    
    if( pos_v-1 < shift_v ) shift_v = pos_v - 1;
    if( shift_v < 0 )       shift_v = 0;

    int ve = visible_elems();
    // We contain less that we can show here
    if( sz <= ve ) shift_v = 0;
    else
        {
        if( pos_v+2 >= shift_v+ve )
            shift_v = pos_v - ve + 2;
        if( shift_v >= sz - ve )
            shift_v = sz - ve;
        }
    
    if( old_shift == shift_v )
        {
        display_elem( old_pos );
        display_elem( pos_v );
        }
    else
        {
        for( int i = 0; i < visible_elems(); i++ )
            display_elem( shift_v+i );
        }
    }


// -------------------------------------------------------------------------------

/*
void sw_simple_list::add( sw_list_elem_base *el , int pos )
    {
    if( pos < 0 || pos > elem_v.size() )
        {
        elem_v.push_back(el);
        display_elem( elem_v.size()-1 );
        }
    else
        {
        elem_v[pos] = el;
        int end = pos+ys(); // for sure
        for( int i = pos; i < end; i++ )
            display_elem( i );
        }
    }

void sw_simple_list::del( int pos )
    {
    if( pos < 0 || pos > elem_v.size() )
        throw sw_Fail("sw_simple_list::del","invalid position","");
    elem_v.erase(elem_v.begin()+pos);
    int end = pos+ys(); // for sure
    for( int i = pos; i < end; i++ )
        display_elem( i );
    }

void sw_simple_list::clear()
    {
    while(elem_v.size()) elem_v.pop_back();
    int end = ys(); // for sure
    for( int i = 0; i < end; i++ )
        display_elem( i );
    }
*/
void sw_simple_list::display_elem( int line )
    {
    sw_point here(0,line-shift_v);
    
    if( line < shift_v || line >= shift_v+size() ) return; // out of view
    sw_color c = (line == pos_v && in_focus) ? scolor_v : lcolor_v;
    if( line < 0 || line >= elem_v.size() )
        fill( lcolor_v, ' ', sw_rect(here,sw_size(xs(),1)) );
    else
        put( here, c, elem_v[line]->display() );
    }
        



  // -------------------------------------------------------------------------------

void sw_menu_base::add( sw_menu_elem_base *el )
    {
    if( elem_v.size() )
        el->xpos_v = (*(elem_v.end()-1))->xpos_v + 2 + (*((elem_v.end()-1)))->length();
    else
        el->xpos_v = 2;

    elem_v.push_back( el );
    display_elem(elem_v.size()-1); // Bug! Disables scrolling completely!
    }

void sw_menu_base::del( const sw_menu_elem_base *el )
    {
    for( int i = elem_v.size(); i--; )
        if( elem_v[i] == el ) elem_v.erase( elem_v.begin()+i );
    }

/*
void sw_menu_base::clear()
    {
    while(elem_v.size()) elem_v.pop_back();
    }
*/


void sw_menu_base::scan_up( int pos )
    {
    try { do_scan_up(pos); } catch( sw_EOF )
        { try { do_scan_dn(pos); } catch( sw_EOF ) {} }
    }

void sw_menu_base::scan_dn( int pos )
    {
    try { do_scan_dn(pos); } catch( sw_EOF )
        { try { do_scan_up(pos); } catch( sw_EOF ) {} }
    }

void sw_menu_base::do_scan_up( int pos )
    {
    if( pos < 0 ) pos = 0;
    int ie = elem_v.size();
    for( int i = pos; i < ie; i++ )
        if( elem_v[i]->selectable )
            {
            set_pos(i);
            return;
            }
    throw sw_EOF("sw_menu_base::do_scan_up","","");
    }

void sw_menu_base::do_scan_dn( int pos )
    {
    int ie = elem_v.size();    if( pos >= ie ) pos = ie - 1;
    for( int i = pos; i >= 0; i-- )
        if( elem_v[i]->selectable )
            {
            set_pos(i);
            return;
            }
    throw sw_EOF("sw_menu_base::do_scan_dn","","");
    }

void sw_menu_base::activate() // currently selected one
    {
    if(elem_v[pos_v]->auto_dismiss())
        dismiss();
    elem_v[pos_v]->action();
    }





  // -------------------------------------------------------------------------------

void sw_hmenu::display_elem( int nelem )
    {
    if( nelem < 0 || nelem >= elem_v.size() ) return;
    sw_point here(elem_v[nelem]->xpos_v,0);
    put( here, nelem == pos_v ? scolor_v : lcolor_v, elem_v[nelem]->display() );
    }

int sw_hmenu::visible_elems() const
    {
    int end = elem_v.size();

    if( end == 0 ) return 1; // pretend that one elem is surely visible
    
    int hsize = xs();
    int basepos = elem_v[pos_v]->xpos_v;
    int visible = 1;
    for( int i = shift_v+1; i < end; i++ )
        {
        if( (elem_v[i]->xpos_v+elem_v[i]->length()) < hsize-1 )
            visible++;
        else
            break;
        }
    return visible;
    }

int sw_hmenu::elem_at( int xpos ) const
    {
    int end = elem_v.size();
    for( int i = shift_v; i < end; i++ )
        {
        if(
           (xpos >= elem_v[i]->xpos_v) &&
           (xpos < ((elem_v[i]->xpos_v)+(elem_v[i]->length())))
           //&& (i == end-1 || xpos < elem_v[i+1]->xpos_v)
          )
            return i;
        }
    return -1;
    }

bool sw_hmenu::event_preprocessor( const sw_event &e )
    {
    sw_point p;
    if( our_mouse_event( p, e) )
        {
          //if( in_focus )
            {
            int el = elem_at(p.x());
            if( el >=0 ) set_pos( el );
            }
        switch( e.info() )
            {
            case sw_event::Mouse1DoubleClick:
                activate();
                return Yes;
            }
        return No;
        }
/*
    switch( e.info() )
        {
        case sw_event::InFocus:
            must_pop = Yes;           // We must pop focus just once
            return Yes;
        case sw_event::NotInFocus:
            //dismiss();
            return Yes;
        }
*/
    switch( e.fkey() )
        {
        case sw_key::left:  prev(); break;
        case sw_key::right: next(); break;
        case sw_key::home:  home(); break;
        case sw_key::end:   end(); break;
        case sw_key::esc:   dismiss(); break;

        case sw_key::down:
        case sw_key::enter:
              //if(elem_v[pos_v]->auto_dismiss()) dismiss();
              //elem_v[pos_v]->action();
            activate();
            break;

        default: return No;
        }
    return Yes;
    }

void sw_hmenu::refresh(const sw_rect&a)
    {
    fill(lcolor_v, ' ',a);
    int end = shift_v+visible_elems();
    for( int i = shift_v; i < end; i++ )
        display_elem( i );
    }

void sw_hmenu::dismiss()
    {
    visible(No);
    pop_focus();
    }

void sw_hmenu::left_from_submenu()
    {
    prev(); 
    elem_v[pos_v]->submenu_action();
    }

void sw_hmenu::right_from_submenu()
    {
    next(); 
    elem_v[pos_v]->submenu_action();
    }

  // -------------------------------------------------------------------------------

int sw_vmenu::visible_elems() const
    {
    return ys()-2;
    }

void sw_vmenu::display_elem( int nelem )
    {
    if( nelem < 0 || nelem >= elem_v.size() ) return;
    sw_color col = nelem == pos_v ? scolor_v : lcolor_v;
    string s;
    elem_v[nelem]->display(s, xs()-2);
    put( sw_point(1,nelem-shift_v+1), col, s );
    }

void sw_vmenu::refresh(const sw_rect&_sq)
    {
    sw_rect a;
    if( !a.intersection( _sq, sw_rect(0,0,xs(),ys()) )) return;
    fill(lcolor_v, ' ',a,Yes);
    border(lcolor_v,Single_Border);
    
    int end = shift_v+visible_elems();
    for( int i = shift_v; i < end; i++ )
        display_elem( i );
    }


void sw_vmenu::dismiss()
    {
    visible(No);
    pop_focus();
    }


void sw_vmenu::left_from_submenu()
    {
    if(base_menu_v)
        {
        pop_focus();
        visible(No);
        base_menu_v->left_from_submenu();
        }
    }

void sw_vmenu::right_from_submenu()
    {
    if(base_menu_v)
        {
        pop_focus();
        visible(No);
        base_menu_v->right_from_submenu();
        }
    }


bool sw_vmenu::event_preprocessor( const sw_event &e )
    {
    sw_point p;
    if( our_mouse_event( p, e) )
        {
          //if( in_focus )
        set_pos( p.y() + shift_v - 1/*+ (border_type_v==No_Border ? 0 : -1) */ );
        switch( e.info() )
            {
            case sw_event::Mouse1DoubleClick:
                activate();
                return Yes;
            }
        return No;
        }

    switch( e.fkey() )
        {
        case sw_key::up:   prev(); break;
        case sw_key::down: next(); break;
            
        case sw_key::left:
            if(base_menu_v)
                {
                pop_focus();
                visible(No);
                base_menu_v->left_from_submenu();
                }
            break;
            
        case sw_key::right:
            if(base_menu_v)
                {
                pop_focus();
                visible(No);
                base_menu_v->right_from_submenu();
                }
            break;
            
        case sw_key::pgup:  pgup(); break;
        case sw_key::pgdn:  pgdn(); break;
        case sw_key::home:  home(); break;
        case sw_key::end:   end(); break;
        case sw_key::esc:   dismiss(); break;
            
        case sw_key::enter:
            activate();
            break;
            
        default:
            return No;
        }
    return Yes;
    }




  // -------------------------------------------------------------------------------


void sw_menu_submenu::action()         { submenu_v.push_focus(); }
void sw_menu_submenu::submenu_action() { submenu_v.push_focus(); }

  // -------------------------------------------------------------------------------


sw_menu_elem_base::sw_menu_elem_base(const string &t, sw_menu_base &base_menu)
    :
text_v(t), base_menu_v(base_menu)
    {
    selectable = Yes;
    base_menu_v.add(this);
    }

sw_menu_elem_base::~sw_menu_elem_base()
    {
    base_menu_v.del(this);
    }



@


1.13
log
@Before changing event class
@
text
@d8 15
d25 2
a26 2
        case sw_key::up:    prev(); break;
        case sw_key::down:  next(); break;
d28 2
a29 2
        case sw_key::pgup:  pgup(); break;
        case sw_key::pgdn:  pgdn(); break;
d31 2
a32 2
        case sw_key::home:  home(); break;
        case sw_key::end:   end(); break;
a33 1
        default: return No;
d35 2
a36 1
    return Yes;
d203 8
d242 14
d259 27
d296 3
a298 2
            if(elem_v[pos_v]->auto_dismiss()) dismiss();
            elem_v[pos_v]->action();
a316 1
      //if(back_focus_v)  back_focus_v->focus();
d391 14
d435 1
a435 2
            if(elem_v[pos_v]->auto_dismiss()) dismiss();
            elem_v[pos_v]->action();
d438 2
a439 1
        default: return No;
a446 2


a449 2
//void sw_menu_submenu::action()         { submenu_v.focus_tree(); }
//void sw_menu_submenu::submenu_action() { submenu_v.focus_tree(); }
@


1.12
log
@context menus done, menus code fixed to
support lone vertical menus correctly.
@
text
@@


1.11
log
@reorganized devices classes, generalized
videomemory-style devices
@
text
@d300 1
a300 1
    if(base_menu_v)                    base_menu_v->focus_tree();
a305 1
    visible(No);
d308 2
a309 1
        base_menu_v->focus_tree();
a315 1
    visible(No);
d318 2
a319 1
        base_menu_v->focus_tree();
a332 1
            visible(No);
d335 2
a336 1
                base_menu_v->focus_tree();
a341 1
            visible(No);
d344 2
a345 1
                base_menu_v->focus_tree();
d374 4
a377 2
void sw_menu_submenu::action()         { submenu_v.focus_tree(); }
void sw_menu_submenu::submenu_action() { submenu_v.focus_tree(); }
@


1.10
log
@bugfixes - a lot of
@
text
@d252 2
a253 1
    if(back_focus_v)  back_focus_v->focus();
@


1.9
log
@dialogs, basically written
@
text
@d34 2
d38 1
a38 1
    if( np >= size() )  np = size() - 1;
d48 1
d50 1
a50 1
    if( size() < visible_elems() ) shift_v = 0;
d53 4
a56 4
        if( pos_v+2 >= shift_v+visible_elems() )
            shift_v = pos_v - visible_elems() + 2;
        if( shift_v >= size() - visible_elems() )
            shift_v = size() - visible_elems();
d114 1
d118 1
a118 1
        put( here, line == pos_v ? scolor_v : lcolor_v, elem_v[line]->display() );
a143 7
void sw_menu_base::del( int pos )
    {
    if( pos < 0 || pos > elem_v.size() )
        throw sw_Fail("sw_menu_base::del","invalid position","");
    elem_v.erase(elem_v.begin()+pos);
    }

a196 1
      //put( here, nelem == pos_v ? scolor_v : lcolor_v, (elem_v[nelem]->text_v).c_str() );
a221 8
    /*
    if( e.type() == sw_event::Info )
        {
        //if( e.info() == sw_event::NotInFocus )            bot();
        if( e.info() == sw_event::InFocus )               visible(1);
        }
    */

a225 1
            
d228 1
d231 4
a234 1
        case sw_key::enter: elem_v[pos_v]->action(); break;
a235 2
        case sw_key::esc:   dismiss(); break;
            
a325 8
      /*
       if( e.type() == sw_event::Info )
       {
      //if( e.info() == sw_event::NotInFocus )            bot();
      if( e.info() == sw_event::InFocus )               visible(1);
      }
      */
    
a350 1
            
d353 1
d355 4
a358 2
        case sw_key::enter: elem_v[pos_v]->action(); break;
        case sw_key::esc:   dismiss(); break;
@


1.8
log
@external array for list
@
text
@d396 12
@


1.7
log
@using sw_list_elem_base
@
text
@a9 2
        //case sw_key::left:  focus_to_left_child(); break;
        //case sw_key::right: focus_to_right_child(); break;
d71 1
a71 1

d105 1
a105 1

d133 7
d151 1
a151 1

a190 5
void sw_menu_base::action()
    {
    elem_v[pos_v]->action();
    }

d243 1
a243 1
        case sw_key::enter: action(); break;
a265 8
/*
void sw_hmenu::action()
    {
      //if( elem_v[pos_v].submenu_v )        elem_v[pos_v].submenu_v->focus_tree();
    elem_v[pos_v].action();
    }
    */

d268 1
a268 1
    prev(); //if(elem_v[pos_v]->submenu_v)        action();
d274 1
a274 1
    next(); //if(elem_v[pos_v]->submenu_v)        action();
a288 2
#if 1
      //string s; s.print( " %-*.*s", xs()-2, xs()-2, (elem_v[nelem]->text_v).c_str() );
a291 7
#else
    sw_point here(1,nelem-shift_v+1);
    put( here, col, (elem_v[nelem].text_v).c_str() );
    int l = elem_v[nelem].text_v.length();
    if( l < xs()-4 )
        fill( col, ' ', sw_rect(l+2, nelem-shift_v+1, xs()-l-4, 1), No );
#endif
a312 7
/*
void sw_vmenu::action()
    {
    if( elem_v[pos_v].submenu_v )      elem_v[pos_v].submenu_v->focus_tree();
    }
*/

d374 1
a374 1
        case sw_key::enter: action(); break;
@


1.6
log
@menu item classes written (wrong way yet)
@
text
@d74 1
a74 1
void sw_simple_list::add( const string & s, int pos )
d76 1
a76 1
    if( pos < 0 || pos > text_v.size() )
d78 2
a79 2
        text_v.push_back(s);
        display_elem( text_v.size()-1 );
d83 1
a83 1
        text_v[pos] = s;
d92 1
a92 1
    if( pos < 0 || pos > text_v.size() )
d94 1
a94 1
    text_v.erase(text_v.begin()+pos);
d102 1
a102 1
    while(text_v.size()) text_v.pop_back();
d113 1
a113 1
    if( line < 0 || line >= text_v.size() )
d116 1
a116 1
        put( here, line == pos_v ? scolor_v : lcolor_v, text_v[line].c_str() );
@


1.5
log
@update
@
text
@d124 1
a124 1
void sw_menu_base::add( const string & s, sw_menu_base *b, int pos )
d126 4
a129 7
    if( pos == -1 )
        {
        if( elem_v.size() )
            pos = (elem_v.end()-1)->xpos_v + 2 + ((elem_v.end()-1)->text_v).length();
        else
            pos = 2;
        }
d131 1
a131 1
    elem_v.push_back( elem(s,pos,Yes,b) );
d152 1
a152 3
        {
        try { do_scan_dn(pos); } catch( sw_EOF ) {}
        }
d158 1
a158 3
        {
        try { do_scan_up(pos); } catch( sw_EOF ) {}
        }
d163 1
d166 1
a166 1
        if( elem_v[i].selectable )
d176 1
d178 1
a178 1
        if( elem_v[i].selectable )
d186 4
d199 3
a201 2
    sw_point here(elem_v[nelem].xpos_v,0);
    put( here, nelem == pos_v ? scolor_v : lcolor_v, (elem_v[nelem].text_v).c_str() );
d211 1
a211 1
    int basepos = elem_v[pos_v].xpos_v;
d215 1
a215 1
        if( (elem_v[i].xpos_v+elem_v[i].text_v.length()) < hsize-1 )
d266 1
d269 2
a270 2
    if( elem_v[pos_v].submenu_v )
        elem_v[pos_v].submenu_v->focus_tree();
d272 1
d276 2
a277 3
    prev();
    if(elem_v[pos_v].submenu_v)
        action();
d282 2
a283 3
    next();
    if(elem_v[pos_v].submenu_v)
        action();
a295 1
    sw_point here(2,nelem-shift_v+1);
d298 4
a301 2
    string s; s.print( " %-*.*s", xs()-2, xs()-2, (elem_v[nelem].text_v).c_str() );
    put( here, col, s );
d303 1
d330 1
d335 1
a335 1

d407 11
@


1.4
log
@bugfixes
@
text
@d52 2
a53 2
        if( pos_v+1 >= shift_v+visible_elems() )
            shift_v = pos_v - visible_elems() + 1;
a237 4
        //case sw_key::up:    prev(); break;
            
        //case sw_key::pgup:  pgup(); break;
        //case sw_key::pgdn:  pgdn(); break;
d243 1
a243 2
        case sw_key::enter:
            action(); break;
d245 2
d263 1
d288 5
d296 47
a342 2
    sw_point here(0,nelem-shift_v);
    put( here, nelem == pos_v ? scolor_v : lcolor_v, (elem_v[nelem].text_v).c_str() );
d345 1
a345 1
bool sw_vmenu::event_preprocessor( const sw_event &e )
d347 2
a348 2
    /*
    if( e.type() == sw_event::Info )
d350 2
a351 2
        //if( e.info() == sw_event::NotInFocus )            bot();
        if( e.info() == sw_event::InFocus )               visible(1);
d353 13
a365 2
    */
        
a371 1
              //bot();
a374 1
                //base_menu_v->visible(1);
a380 1
              //bot();
a383 1
                //base_menu_v->visible(1);
d388 1
a388 1

a401 56

void sw_vmenu::refresh(const sw_rect&a)
    {
    fill(lcolor_v, ' ',a);
    int end = shift_v+visible_elems();
    for( int i = shift_v; i < end; i++ )
        display_elem( i );
    }

void sw_vmenu::dismiss()
    {
      //bot();
    visible(No);
    if(base_menu_v)
        {
        //base_menu_v->visible(1);
        base_menu_v->focus_tree();
        }
    }

void sw_vmenu::action()
    {
    if( elem_v[pos_v].submenu_v )
        {
        //elem_v[pos_v].submenu_v->visible(1);
        elem_v[pos_v].submenu_v->focus_tree();
        }
    }



void sw_vmenu::left_from_submenu()
    {
      //bot();
    visible(No);
    if(base_menu_v)
        {
        //base_menu_v->visible(1);
        base_menu_v->focus_tree();
        base_menu_v->left_from_submenu();
        }
    }

void sw_vmenu::right_from_submenu()
    {
      //bot();
    visible(No);
    if(base_menu_v)
        {
        //base_menu_v->visible(1);
        base_menu_v->focus_tree();
        base_menu_v->right_from_submenu();
        }
    }


@


1.3
log
@menus basically done
@
text
@a115 1
          //throw sw_Fail("sw_simple_list::display_line","invalid position","");
d135 1
a135 2
    display_elem(elem_v.size()-1);
    //visible_v = elem_v.size(); // Bug! Disables scrolling completely!
a264 1
    //bot();
a270 2
        {
        //elem_v[pos_v].submenu_v->visible(1);
a271 1
        }
@


1.2
log
@in progress
@
text
@d122 294
@


1.1
log
@Initial revision
@
text
@d31 1
a31 1
        display_line( shift_v+i );
d49 1
a49 1
    if( size() < ys() ) shift_v = 0;
d52 4
a55 2
        if( pos_v+1 >= shift_v+ys() )    shift_v = pos_v - ys() + 1;
        if( shift_v >= size() - ys() )   shift_v = size() - ys();
d60 2
a61 2
        display_line( old_pos );
        display_line( pos_v );
d65 2
a66 2
        for( int i = 0; i < ys(); i++ )
            display_line( shift_v+i );
d79 1
a79 1
        display_line( text_v.size()-1 );
d86 1
a86 1
            display_line( i );
d97 1
a97 1
        display_line( i );
d105 1
a105 1
        display_line( i );
d108 1
a108 1
void sw_simple_list::display_line( int line )
@
