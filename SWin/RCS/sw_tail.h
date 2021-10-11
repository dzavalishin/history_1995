head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	96.11.18.15.47.54;	author dz;	state Exp;
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
date	96.09.30.09.47.49;	author dz;	state Exp;
branches;
next	;


desc
@@


1.4
log
@Before changing event class
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: colored log display header
 *
 *      $Log: sw_tail.h $
 *      Revision 1.3  1996/09/30 14:18:41  dz
 *      log control written
 *
 *      Revision 1.2  1996/09/30 12:51:06  dz
 *      *** empty log message ***
 *
 *      Revision 1.1  1996/09/30 09:47:49  dz
 *      Initial revision
 *
 *
 *      
 * TODO: ...
 *
\*/

#include "sw_ctl_list.h"
#include "sw_sema.h"
#include <vector.h>

//#include <io.h>
//#include <stdio.h>
#include <string.h>
//#include <process.h>
//#include <ctype.h>

//#define INCL_DOSDATETIME     /* Timer support    */
//#define INCL_DOSSEMAPHORES   /* Semaphore values */
//#define INCL_DOSERRORS       /* error codes      */
//#define INCL_DOSPROCESS
//#include <os2.h>




class sw_tail : public sw_list_base
    {
    public:
        typedef int cid;
        
    private:
        class line
            {
            protected:
                string      text;
                int         time; // since line was placed here
                int         changed;
                cid         current_color;
                cid         recommended_color;

            public:
                line(const string &s);
                line();
                void           touch()              { changed = 1; }
                void           untouch()            { changed = 0; }
                int            is_changed() const   { return changed; }
                cid            get_current_color()  { return current_color; }
                int            get_time()           { return time; }
                const string & get_text()           { return text; }

                //void           draw( int y, int sel, sw_color sc );
                //void           cond_draw( int y, int sel, sw_color sc );
                void           tick();
                int            repaint( cid prev_line );
                void           set( const char *_text );
                void           scroll( line &l );
            };


        vector <line>      lines;
        int                max_v;
          //sw_color           lcolor_v, scolor_v;

        sw_color           scolor_v;

        unsigned long      disp_sema;

        
    public:
        sw_tail( sw_abstract_win &parent, const sw_rect &r, int maxlines, sw_color back, sw_color sc );
        
        ~sw_tail()
            {}
        
        virtual int          visible_elems() const { return ys(); }
        virtual int          size() const { return lines.size(); }
        virtual void         display_elem( int line );
        
        virtual void         cond_display_all();
        
        void                 add( const string & str );

        void                 tick();
    };
@


1.3
log
@log control written
@
text
@d8 3
@


1.2
log
@*** empty log message ***
@
text
@d8 3
d74 1
d83 1
a83 1
        sw_tail( sw_abstract_win &parent, const sw_rect &r, sw_color sc );
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: tail.c $
d18 1
d21 2
a22 2
#include <io.h>
#include <stdio.h>
d24 2
a25 10
#include <process.h>
#include <ctype.h>

#define INCL_DOSDATETIME     /* Timer support    */
#define INCL_DOSSEMAPHORES   /* Semaphore values */
#define INCL_DOSERRORS       /* error codes      */
#define INCL_DOSPROCESS
#include <os2.h>

static char *Id = "$Id: tail.c 1.1 1996/09/30 09:43:52 dz Exp $";
d27 5
d38 30
a67 25
    
    
    class line
        {
        protected:
            string      text;
            int         time; // since line was placed here
            int         changed;
            sw_color    current_color;
            sw_color    recommended_color;

        public:
            line();
            void touch() { changed = 1; }
            color get_current_color() { return current_color; }
            int get_time() { return time; }
            const char *get() { return text; }

            void draw( int y );
            void cond_draw( int y );
            void tick();
            int repaint( sw_color prev_line );
            void set( const char *_text );
            void scroll( line &l );
        };
d70 2
a71 2
    vector <line>      lines;
    sw_color           lcolor_v, scolor_v;
d73 1
d75 3
d79 1
a79 7
        tail(
             sw_abstract_win &parent, const sw_rect &r,
             sw_color lc, sw_color sc
            )
            :
        sw_list_base( parent, rect ), lcolor_v(lc), scolor_v(sc),
        {}
d81 1
a81 1
        ~tail()
d86 1
a86 4
        virtual void         display_elem( int line )
            {
            lines[line].draw(line-shift_v);
            }
d88 1
a88 6
        virtual void         cond_display_all()
            {
            int ie = visible_elems();
            for( int i = 0; i < ie; i++ )
                lines[i+shift_v].cond_draw( i );
            }
d90 1
a90 24
        void add( const string & str )
            {
              // Очень молодая строка доскроллилась доверху - подождем чуток!
              //if( lines[0].get_time() < 20 ) DosSleep( 500 );

            for( int i = 0; i < window-1; i++ )
                lines[i].scroll( lines[i+1] );
            lines[window-1].set( str.substr(0,xs()) );
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
d92 1
@
