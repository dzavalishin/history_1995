/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: round window class
 *
 *      $Log: sw_roundwin.h $
 *      Revision 1.2  1996/11/23 10:19:59  dz
 *      transparency, clock
 *
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
