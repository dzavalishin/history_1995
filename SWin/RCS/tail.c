head	1.7;
access;
symbols;
locks;
comment	@ * @;


1.7
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.11.15.13.12.49;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.10.02.11.01.11;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.10.01.16.32.27;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.09.30.14.18.41;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.30.12.51.06;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.30.09.43.52;	author dz;	state Exp;
branches;
next	;


desc
@@


1.7
log
@Before changing event class
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: colored log display
 *
 *      $Log: tail.c $
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

static char *Id = "$Id: tail.c 1.6 1996/11/15 13:12:49 dz Exp $";



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











@


1.6
log
@update
@
text
@d8 3
d41 1
a41 1
static char *Id = "$Id: tail.c 1.5 1996/10/02 11:01:11 dz Exp $";
@


1.5
log
@focusable
@
text
@d8 3
d38 1
a38 1
static char *Id = "$Id: tail.c 1.4 1996/10/01 16:32:27 dz Exp $";
d181 5
a185 1
    if( lines.size() > max_v ) lines.erase(lines.begin());
@


1.4
log
@bugfixes - a lot of
@
text
@d8 3
d35 1
a35 1
static char *Id = "$Id: tail.c 1.3 1996/09/30 14:18:41 dz Exp $";
d166 1
a166 1
            if( lno == pos_v ) c.back( scolor_v.value() );
@


1.3
log
@log control written
@
text
@d8 3
d32 1
a32 1
static char *Id = "$Id: tail.c 1.2 1996/09/30 12:51:06 dz Exp $";
d144 1
a144 1
        if( lno == pos_v ) c.back( scolor_v.value() );
@


1.2
log
@*** empty log message ***
@
text
@d8 3
d21 1
a23 1
//#include <io.h>
d25 1
a25 7
//#include <string.h>
//#include <process.h>
//#include <ctype.h>

//#define INCL_DOSDATETIME     /* Timer support    */
//#define INCL_DOSSEMAPHORES   /* Semaphore values */
//#define INCL_DOSERRORS       /* error codes      */
d29 1
a29 1
static char *Id = "$Id: tail.c 1.1 1996/09/30 09:43:52 dz Exp $";
d52 2
a53 2

static const sw_color cname_from_cid[] =
a83 13
/*
void sw_tail::line::draw( int y, int sel, sw_color sc )
    {
    put( sw_point(0,y), sel ? sc : cname_from_cid[current_color], text );
    changed = 0;
    }

void sw_tail::line::cond_draw( int y, int sel, sw_color sc )
    {
    if( !changed ) return;
    draw( y, sel, sc );
    }
    */
d136 1
a136 1
        fill( cname_from_cid[0], ' ', sw_rect(0,lno-shift_v,xs(),1) );
d140 3
a142 3
        put( sw_point(0,lno-shift_v),
            lno == pos_v ? scolor_v : cname_from_cid[l.get_current_color()],
            l.get_text() );
d154 1
a154 1
            fill( cname_from_cid[0], ' ',  sw_rect(0,i+shift_v,xs(),1) );
d159 3
a161 3
            put( sw_point(0,i),
                lno == pos_v ? scolor_v : cname_from_cid[l.get_current_color()],
                l.get_text() );
d169 5
a173 7
      // Очень молодая строка доскроллилась доверху - подождем чуток!
      //if( lines[0].get_time() < 20 ) DosSleep( 500 );
    
    for( int i = 0; i+1 < lines.size(); i++ )
        lines[i].scroll( lines[i+1] );
      //lines[lines.size()-1].set( str.substr(0,xs()) );
    lines.push_back( str.substr(0,xs()) );
d189 1
d208 4
a211 1
sw_tail::sw_tail( sw_abstract_win &parent, const sw_rect &r, sw_color sc )
d213 1
a213 1
sw_list_base( parent, r ), scolor_v(sc)
d216 2
a220 20
/*

int main( int ac, char **av )
    {
    
    while(1)
        {
        char str[100];
        if( fgets( str, 79, stdin ) == NULL )
            break;
        char *c = strpbrk( str, "\r\n" );
        if( c ) *c = 0;
        t.add( str );
        
        }
    
    return 0;
    }

*/
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: sw_ctl_list.h $
d17 1
a17 1
#include "sw_ctl_list.h"
d20 9
a28 9
#include <io.h>
#include <stdio.h>
#include <string.h>
#include <process.h>
#include <ctype.h>

#define INCL_DOSDATETIME     /* Timer support    */
#define INCL_DOSSEMAPHORES   /* Semaphore values */
#define INCL_DOSERRORS       /* error codes      */
d32 1
a32 1
static char *Id = "$Id: ftail.c 1.2 1996/05/12 01:45:59 dz Rel $";
d38 3
a40 3
void wait_sema();
void close_sema();
void create_sema();
a42 2
void set_color( int time );
void gotoxy( int, int y );
d44 1
a44 1
static cid cid_from_time( int time )
d58 2
a59 2
    sw_color::White, sw_color::yellow, sw_color::lightred,
    sw_color::lightmagenta, sw_color::brown, sw_color::darkgray
a68 143
class sw_tail : public sw_list_base
    {
    
    
    class line
        {
        protected:
            string      text;
            int         time; // since line was placed here
            int         changed;
            sw_color    current_color;
            sw_color    recommended_color;
        public:
            line() 
                {
                time = 0;
                changed = 1;
                current_color = 0;
                recommended_color = 0;
                }
            
            void touch() { changed = 1; }
            
            void draw( int y )
                { 
                put( sw_point(0,y), current_color, text );
                changed = 0;
                }
            
            void cond_draw( int y )
                { 
                if( !changed ) return;
                draw( y );
                }
            
            void tick() 
                { 
                if( time <= max_time ) 
                    {
                    time++; 
                    recommended_color = color::cid_from_time(time);
                    } 
                }
            
            /* arg - color of line above. */
            /* returns 1 if color changed */
            int repaint( color prev_line )
                {
                color our_recommended_color = recommended_color;
                // предотвратить слипание цветов
                if( our_recommended_color >= prev_line )
                    {
                    if( our_recommended_color <= 0 )
                        return 0;
                    our_recommended_color = our_recommended_color.prev();
                    }
                if( our_recommended_color <= current_color )
                    return 0;
                
                current_color = our_recommended_color;
                touch();
                return 1;
                }

            color get_current_color() { return current_color; }
            int get_time() { return time; }

            void set( const char *_text )
                {
                text = _text;
                time = 0;
                current_color = color::cid_from_time(time);
                touch(); 
                }

            const char *get() { return text; }

            void scroll( line &l )
                {
                text = l.text;
                time = l.time;
                current_color = l.current_color;
                recommended_color = l.recommended_color;
                touch();
                }
        };

    
    vector <line>      lines;
    sw_color           lcolor_v, scolor_v;
    
    
public:
    tail(
         sw_abstract_win &parent, const sw_rect &r,
         sw_color lc, sw_color sc
        )
        :
    sw_list_base( parent, rect ), lcolor_v(lc), scolor_v(sc),
        {}
    ~tail()
        {}

    virtual int          visible_elems() const { return ys(); }
    virtual int          size() const { return lines.size(); }
    virtual void         display_elem( int line )
        {
        lines[line].draw(line-shift_v);
        }
    
    virtual void         cond_display_all()
        {
        int ie = visible_elems();
        for( int i = 0; i < ie; i++ )
            lines[i+shift_v].cond_draw( i );
        }

    void add( const string & str )
        {
        // Очень молодая строка доскроллилась доверху - подождем чуток!
        //if( lines[0].get_time() < 20 ) DosSleep( 500 );
        
        for( int i = 0; i < window-1; i++ )
            lines[i].scroll( lines[i+1] );
        lines[window-1].set( str.substr(0,xs());
        cond_display_all();
        }
    
    tick()
        {
        for( int i = 0; i < window; i++ )
            lines[i].tick();
        
        color new_color = 8;
        for( i = 0; i < window; i++ )
            {
            if( lines[i].repaint( new_color ) )
                new_color = lines[i].get_current_color();
            }
        
        }
    
};
d71 1
a71 1
sw_tail::line::line()
d79 1
a79 1
void sw_tail::line::touch()
d81 1
d83 2
d87 2
a88 1
void sw_tail::line::draw( int y )
d90 1
a90 1
    put( sw_point(0,y), current_color, text );
d94 1
a94 1
void sw_tail::line::cond_draw( int y )
d97 1
a97 1
    draw( y );
d99 1
d106 1
a106 1
        recommended_color = color::cid_from_time(time);
d112 1
a112 1
int sw_tail::line::repaint( color prev_line )
d114 1
a114 1
    color our_recommended_color = recommended_color;
d120 2
a121 1
        our_recommended_color = our_recommended_color.prev();
a130 10
sw_color sw_tail::line::get_current_color()
    {
    return current_color;
    }

int sw_tail::line::get_time()
    {
    return time;
    }

d135 1
a135 1
    current_color = color::cid_from_time(time);
a138 5
const string & sw_tail::line::get()
    {
    return text;
    }

a147 1
                            
d149 13
a161 2
tail   t(20);
HEV     disp_sema;
d163 24
d188 8
a195 1
extern "OPTLINK" void ticker(void *val)
d197 5
a201 1
    while(1)
d203 2
a204 10
        //gotoxy( 0, 23 );
        //fprintf( stdout, "O" );
        t.tick();
        DosPostEventSem( disp_sema );

        //gotoxy( 0, 23 );
        //fprintf( stdout, "o" );
        //fflush( stdout );

        DosSleep( 1000 );
d206 1
d210 6
a215 1
extern "OPTLINK" void shower(void *val)
d217 1
a217 1
    char marker = 'a'-1;
d220 4
a223 9
        marker++;
        if( marker > 'z' ) marker = 'a';
        
        gotoxy( 0, 23 );
        fprintf( stdout, "%c", toupper( marker ) );
        t.cond_display_all();
        gotoxy( 0, 23 );
        fprintf( stdout, "%c", tolower( marker ) );
        fflush( stdout );
d225 5
a229 2
        wait_sema();
        }
d233 1
a237 5
    create_sema();
    
    _beginthread( ticker, 0, 16*1024, 0 );
    _beginthread( shower, 0, 16*1024, 0 );
    
a245 1
        DosPostEventSem( disp_sema );
a248 3
    close_sema();
    
    
d252 1
a256 44




void create_sema()
    {
    APIRET  rc            = NO_ERROR;            /* Return code               */
 
    rc = DosCreateEventSem( NULL,
                           &disp_sema,
                           0 /* DC_SEM_SHARED */,
                           FALSE);         /* Semaphore is in RESET state  */
    if (rc != NO_ERROR)
        {
        printf("DosCreateEventSem error: return code = %u\n", rc);
        exit( 1 );
        }
 
    }

void close_sema()
    {
    APIRET rc = DosCloseEventSem(disp_sema);      /* Get rid of semaphore       */
    if (rc != NO_ERROR) 
        printf("DosCloseEventSem error: return code = %u", rc);
    }
 

void wait_sema( void )
    {

    APIRET  rc;         /* Return code                    */
 
    rc = DosWaitEventSem( disp_sema, SEM_INDEFINITE_WAIT );
 
    if (rc != 0) 
        {
        printf("DosWaitEventSem error: return code = %ld", rc);
        return;
        }
    
    ULONG res;
    DosResetEventSem( disp_sema, &res );
    }
@
