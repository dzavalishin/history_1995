head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.10.10.23.37.07;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.10.10.10.46.53;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@Before changing event class
@
text
@/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: tty-style device classes impl.
 *
 *      $Log: device_tty.c $
 *      Revision 1.2  1996/10/10 23:37:07  dz
 *      reorganized devices classes, generalized
 *      videomemory-style devices
 *
 *      Revision 1.1  1996/10/10 10:46:53  dz
 *      Initial revision
 *
 *
 *      
 *
\*/


#include "sw_device.h"
#include <io.h>


sw_tty_base_device::sw_tty_base_device( ostream& s)
    :
os_v(s),
sw_2b_video_memory_device(sw_size(80,24))
    {
    vm_alloc();
    }

sw_tty_base_device::~sw_tty_base_device()
    {
    vm_free();
    }


void sw_tty_base_device::tty_write( const char *s, int count )
    {
    os_v.write(s,count);
    }

void sw_tty_base_device::vm_alloc()
    {
    int len = xs()*ys()*2;

    video_memory = new char[len];
    memset( video_memory, ' ', len );
    screen_copy = new char[len];
    memset( screen_copy, ' ', len );
    }

void sw_tty_base_device::vm_free()
    {
    if(screen_copy)
        {
        delete [] screen_copy;
        screen_copy = 0;
        }
    if( video_memory )
        {
        delete [] video_memory;
        video_memory = 0;
        }
    }


  // -----------------------------------------------------------------------
        
#if 0
void sw_ansi_device::put_cw( const sw_abstract_win &who, const sw_rect &_p, const char *ca )
    {
    char cur_color;
    sw_rect p = _p;
    int bytes_per_line = _p.xs() * 2;

    //who.move_to_win(p);
    
    p.cut( *this );
    if( p.xs() <= 0 || p.ys() <= 0 ) return;
    
    ansi_set_color( cur_color = ca[1] ); // Set first attribute
    
    for( int yi = p.y(); yi < p.yend(); yi++ )
        {
        ansi_gotoxy( p.x(), yi );
        const char *lp = ca;
        ca += bytes_per_line;
        for( int xi = p.x(); xi < p.xend(); xi++ )
            {
#if 1
            if( !map.belongs( &who, xi, yi ) )
               tty_write("\x1B[C", 3);
            else
#endif
                {
                if( lp[1] != cur_color ) ansi_set_color( cur_color = lp[1] );
                tty_write(lp,1);
                }
            lp += 2;
            }
        }
    
    }

void sw_ansi_device::fill_cw( const sw_abstract_win &who, sw_color col, char ch, sw_rect _p )
    {
    }

void sw_ansi_device::border_cw( const sw_abstract_win &who, sw_color col, sw_border_t t )
    {
    }


/*
void sw_ansi_device::put_up( const sw_rect &_p, const char *ca )
    {
    char cur_color;
    sw_rect p = _p;
    int bytes_per_line = _p.xs() * 2;

    p.cut( *this );
    if( p.xs() <= 0 || p.ys() <= 0 ) return;

    ansi_set_color( cur_color = ca[1] ); // Set first attribute

    for( int yi = p.y(); yi < p.yend(); yi++ )
        {
        ansi_gotoxy( p.x(), yi );
        const char *lp = ca;
        ca += bytes_per_line;
        for( int xi = p.x(); xi < p.xend(); xi++ )
            {
            if( lp[1] != cur_color ) ansi_set_color( cur_color = lp[1] );
            tty_write(lp,1);
            lp += 2;
            }
        }
    
    }
    */
#endif

sw_ansi_device::sw_ansi_device()  {}
sw_ansi_device::~sw_ansi_device() {}


int ansi_col_tab[] = { 30, 34, 32, 36, 31, 35, 33, 37 };

void sw_ansi_device::tty_set_color( char color )
    {
    unsigned char fg  =  color & 0x7;
    int           fgi =  color & 0x8;
    unsigned char bg  = (color & 0x70) >> 4;
    int           bgi =  color & 0x80;

    char s[40];
    sprintf( s, "\x1B[%d;%d", ansi_col_tab[fg], 10+ansi_col_tab[bg] );
    if( fgi ) strcat( s, ";1" );
    if( bgi ) strcat( s, ";5" );
    strcat( s, "m" );
    tty_write( s, strlen(s) );
    }

void sw_ansi_device::tty_gotoxy( int x, int y )
    {
    char s[40];
    sprintf(s,"\x1B[%d;%dH", y+1, x+1 );
    tty_write( s, strlen(s) );
    }


const char *sw_ansi_device::name() { return "ansi"; }





@


1.2
log
@reorganized devices classes, generalized
videomemory-style devices
@
text
@d8 4
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: device.c $
d21 13
a33 2
sw_tty_base_device::sw_tty_base( ostream& s) : os_v(s)
    {}
d39 25
@
