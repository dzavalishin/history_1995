/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: round window classes impl.
 *
 *      $Log: sw_roundwin.c $
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



#include "sw_roundwin.h"




void sw_round_win::refresh(const sw_rect &_sq)
    {
    sw_rect sq;
    if( !sq.intersection( _sq, sw_rect(0,0,xs(),ys()) )) return;
    sw_abstract_win::refresh(sq);
    //fill(sw_color(sw_color::Red,sw_color::Red),'ø',sq /*,(bool)(border_type_v!=No_Border) */ );
    }

sw_round_win::sw_round_win( sw_abstract_win &parent, sw_rect pos_size )
    : sw_win( parent, pos_size )
    {

    // we'll make transparent a three positions at each corner
    map.set_transparent( sw_rect(0,0,2,1) );           // 2 in a row at upper left
    map.set_transparent( sw_point(0,1) );              // 1 below

    map.set_transparent( sw_rect(0,ys()-1,2,1) );      // 2 in a row at lower left
    map.set_transparent( sw_point(0,ys()-2) );         // 1 above

    map.set_transparent( sw_rect(xs()-2,0,2,1) );      // 2 in a row at upper right
    map.set_transparent( sw_point(xs()-1,1) );         // 1 below
    
    map.set_transparent( sw_rect(xs()-2,ys()-1,2,1) ); // 2 in a row at lower right
    map.set_transparent( sw_point(xs()-1,ys()-2) );    // 1 above

    // BUG!!
    // here we must update device map, isnt't?
    
    refresh(sw_rect(sw_point(0,0),*this));
    }

sw_round_win::~sw_round_win()
    {
    }


