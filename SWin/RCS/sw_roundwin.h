head	1.2;
access;
symbols;
locks;
comment	@ * @;


1.2
date	96.11.23.10.19.59;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.11.22.14.55.54;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@transparency, clock
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: round window class
 *
 *      $Log: sw_roundwin.h $
 *      Revision 1.1  1996/11/22 14:55:54  dz
 *      Initial revision
 *
 *
 *      
 *
\*/

#ifndef SW_ROUNDWIN_H
#define SW_ROUNDWIN_H

class sw_round_win;
//class sw_abstract_win;
inline void destroy(sw_round_win**) {}
//inline void destroy(sw_abstract_win**) {}


#include "sw_win.h"



// In fact, this is window with rounded corners, not a real circle-formed one

class sw_round_win : public sw_win
    {
    private:
        
    public:
        sw_round_win( sw_abstract_win &parent, sw_rect pos_size );
        virtual ~sw_round_win();
        virtual void refresh(const sw_rect&); // redisplays self and children
    };



#endif // SW_ROUNDWIN_H
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: sw_win.h $
a33 4
        //sw_border_t  border_type_v;
        //sw_color     border_color_v;
          //sw_color     fill_color_v;
          //char         fill_char_v;
d36 2
a37 9
        round_win(
                  sw_abstract_win &parent,
                  sw_rect pos_size,
                  //sw_border_t border_type = Single_Border,
                  //sw_color border_color = sw_color::Yellow
                    //sw_color fill_color = sw_color::Cyan,
                    //char fill_char = '°'
                 );
        virtual ~round_win();
@
