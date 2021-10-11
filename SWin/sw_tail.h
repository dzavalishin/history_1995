/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: colored log display header
 *
 *      $Log: sw_tail.h $
 *      Revision 1.4  1996/11/18 15:47:54  dz
 *      Before changing event class
 *
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
