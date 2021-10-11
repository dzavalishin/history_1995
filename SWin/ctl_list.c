
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
    fill(lcolor_v, ' ',a ); //,Yes);
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



