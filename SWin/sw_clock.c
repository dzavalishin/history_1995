/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: clock class impl.
 *
 *      $Log: sw_clock.c $
 *      Revision 1.3  1996/11/23 10:19:59  dz
 *      transparency, clock
 *
 *      Revision 1.2  1996/11/22 18:18:01  dz
 *      ticks
 *
 *      Revision 1.1  1996/11/22 17:37:34  dz
 *      Initial revision
 *
 *
 *
 *
\*/



#include "sw_clock.h"
#include "sw_device.h"


static char image[] =
    "    ± ± ± ± ± ± ± ± ± ± ±     "
    "  ±     ù     ù     ù     ±   "
    "±   ù                   ù   ± "
    "±           J u v           ± "
    "± ù       4 2 : 7 3       ù ± "
    "±       7 2 , 1 9 0 6       ± "
    "±   ù                   ù   ± "
    "  ±     ù     ù     ù     ±   "
    "    ± ± ± ± ± ± ± ± ± ± ±     "
    ;

static const char *mtable[12] =
    {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    
static const sw_point tick[12] =
    {
    sw_point(7,1),  sw_point(10,1),
    sw_point(12,2), sw_point(13,4), sw_point(12,6),
    sw_point(10,7),  sw_point(7,7),  sw_point(4,7),
    sw_point(2,6),  sw_point(1,4),  sw_point(2,2),
    sw_point(4,1)
    };

void sw_clock::put_tick()
    {
    int new_tick = now.tm_sec / 5;
    int odd_second = now.tm_sec % 2;

    sw_color tick_col = odd_second ? ticks_on_col : ticks_off_col;

    if( new_tick != old_tick )
        put( tick[old_tick], display_col, 'ù' );
    put( tick[new_tick], tick_col, 'ù' );
    old_tick = new_tick;

    put( sw_point(7,4), odd_second ? display_col : time_col, ":" );
    }

void sw_clock::put_time()
    {
    char buf[20];

    sprintf( buf, "%2.2d:%2.2d", now.tm_hour, now.tm_min );
    put( sw_point(5,4), time_col, buf );
    if(now.tm_sec % 2)
        put( sw_point(7,4), display_col, ":" );

    sprintf( buf, "%3.3s", mtable[now.tm_mon] ); // mon
    put( sw_point(6,3), display_col, buf );
    
    sprintf( buf, "%2.2d,%4.4d", now.tm_mday, now.tm_year );
    put( sw_point(4,5), display_col, buf );

    old_min = now.tm_min;
    }

void sw_clock::get_time()
    {
    sysdep_getlocaltime( now );
    }


void sw_clock::refresh(const sw_rect &_sq)
    {
    sw_rect sq;
    if( !sq.intersection( _sq, sw_rect(0,0,xs(),ys()) )) return;
    sw_round_win::refresh(sq);
    fill( display_col, ' ', sq );

    put( sw_rect(0,0,xs(),ys()), image );

    for( int t = 0; t < 12; t++ )
        put( tick[t], display_col, 'ù' );

    put_tick();
    put_time();
    
    //first_refresh = No;
    }

void sw_clock::run()
    {
    static sec = 0;
    while(1)
        {
        get_time();
        put_tick();

        if( old_min != now.tm_min )
            put_time();
        
        device.flush();
        sleep(1000);
        }
    }


sw_clock::sw_clock(
                   sw_abstract_win &parent, sw_point pos,
                   sw_color _border_col,
                   sw_color _display_col,   sw_color _time_col,
                   sw_color _ticks_off_col, sw_color _ticks_on_col
                  )
    : sw_round_win( parent, sw_rect(pos,sw_size(15,9)) ),
    border_col(_border_col),
    display_col(_display_col), time_col(_time_col),
    ticks_off_col(_ticks_off_col), ticks_on_col(_ticks_on_col)
    {
    old_tick = 0;
    get_time();
    
    for( int i = 0; ++i < sizeof(image); )
        {
        image[i] = border_col.value();
        i++;
        }

    refresh(sw_rect(sw_point(0,0),*this));

    //first_refresh = Yes;
    
    sw_thread::begin();
    }

sw_clock::~sw_clock()
    {
    }


