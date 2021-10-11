head	1.25;
access;
symbols;
locks;
comment	@ * @;


1.25
date	96.11.23.10.19.59;	author dz;	state Exp;
branches;
next	1.24;

1.24
date	96.11.22.13.02.38;	author dz;	state Exp;
branches;
next	1.23;

1.23
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.22;

1.22
date	96.11.15.21.37.58;	author dz;	state Exp;
branches;
next	1.21;

1.21
date	96.11.15.13.12.49;	author dz;	state Exp;
branches;
next	1.20;

1.20
date	96.10.11.17.02.37;	author dz;	state Exp;
branches;
next	1.19;

1.19
date	96.10.10.23.37.07;	author dz;	state Exp;
branches;
next	1.18;

1.18
date	96.10.07.19.40.27;	author dz;	state Exp;
branches;
next	1.17;

1.17
date	96.10.07.11.15.09;	author dz;	state Exp;
branches;
next	1.16;

1.16
date	96.10.03.07.43.57;	author dz;	state Exp;
branches;
next	1.15;

1.15
date	96.10.02.11.01.11;	author dz;	state Exp;
branches;
next	1.14;

1.14
date	96.10.01.16.32.27;	author dz;	state Exp;
branches;
next	1.13;

1.13
date	96.10.01.05.27.39;	author dz;	state Exp;
branches;
next	1.12;

1.12
date	96.09.30.14.18.41;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.09.29.05.54.19;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.09.28.22.58.52;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.09.28.14.03.17;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.09.27.07.26.39;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.09.27.00.33.20;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.09.26.20.46.52;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.09.26.06.06.18;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.09.21.13.26.57;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.09.20.14.40.18;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.12.01.12.51;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.09.14.53.22;	author dz;	state Exp;
branches;
next	;


desc
@@


1.25
log
@transparency, clock
@
text
@
//#define INCL_KBD
//#define INCL_VIO
//#include <os2.h>

#include "sw_device.h"
#include "sw_controls.h"
#include "sw_ctl_list.h"
#include "sw_ctl_entry.h"
#include "sw_tail.h"
#include "sw_clock.h"



sw_vio_device d;

sw_abstract_win *logwp, *quewp, *menuwp, *dialog_wp;

class sw_desktop_focus_changer : virtual public sw_focus_changer
    {
    public:
        virtual bool pre_focus_changer( const sw_event &e )
            {
            switch( e.fkey() )
                {
                case sw_key::tab:
                    focused_child( focused_child() == quewp ? logwp : quewp );
                    break;
                case sw_key::f10:
                    menuwp->push_focus();
                    break;
                case sw_key::f9:
                    dialog_wp->push_focus();
                    break;
                default:
                    return No;
                }
            return Yes;
            }
        
        virtual bool post_focus_changer( const sw_event &e )
            { return No; }
    };


const sw_color cyan_on_blue (sw_color::LightCyan,sw_color::Blue);
const sw_color white_on_blue(sw_color::White,sw_color::Blue);
const sw_color yellow_on_blue(sw_color::Yellow,sw_color::Blue);
const sw_color black_on_gray(sw_color::Black,sw_color::LightGray);
const sw_color yellow_on_magenta(sw_color::Yellow,sw_color::Magenta);
const sw_color white_on_green(sw_color::White,sw_color::Green);
const sw_color black_on_lightgreen(sw_color::Black,sw_color::LightGreen);
const sw_color black_on_blue(sw_color::Black,sw_color::Blue);

    
class sw_test_desktop : public sw_desktop_focus_changer, public sw_bordered_win
    {
    public: sw_test_desktop( sw_abstract_win &parent, sw_rect pos_size )
        : sw_bordered_win( parent, pos_size, Double_Border, cyan_on_blue, cyan_on_blue, ' ' )
        {};
    };

void go()
    {
    
    sw_rect desk_rect(0,1,d.xs(),d.ys()-2);
    int act_x = d.xs() - 26;
    
    //sw_bordered_win desktop( d, desk_rect, Double_Border, cyan_on_blue, cyan_on_blue, ' ' );

    class dialog : public sw_bordered_win, public sw_dialog_focus_changer
        {
        sw_title  title;
        public: dialog()
            :
        sw_bordered_win( d, sw_rect(30,5,45,12),
                        Single_Border, black_on_gray, black_on_gray, ' '
                       ),
        title( *this, " Send File ", black_on_gray )
            {}
        }
    send_dialog;
    
    sw_3d_entry send_addr( send_dialog, sw_rect(11,2,21,1), white_on_green, black_on_lightgreen, black_on_gray );
    send_addr.value("2:5020/");
    send_addr.label("Address: ", black_on_gray );
    sw_3d_entry send_file( send_dialog, sw_rect(11,4,21,1), white_on_green, black_on_lightgreen, black_on_gray );
    send_file.value("*.*");
    send_file.label("File:    ", black_on_gray );

    sw_text_button b_ok    ( send_dialog, sw_point(  2, 10 ), " Ok ",     white_on_green, black_on_lightgreen, black_on_gray );
    sw_text_button b_cancel( send_dialog, sw_point(  8, 10 ), " Cancel ", white_on_green, black_on_lightgreen, black_on_gray );
    sw_text_button b_help  ( send_dialog, sw_point( 37, 10 ), " Help ",   white_on_green, black_on_lightgreen, black_on_gray );

    //sw_onechar_button send_b1( send_dialog, sw_point( 34, 2 ), white_on_green );
    sw_text_button b_1     ( send_dialog, sw_point( 34, 2 ), "",   white_on_green, black_on_lightgreen, black_on_gray );

    send_addr.focus();

    dialog_wp = &send_dialog;

    
    sw_test_desktop desktop( d, desk_rect );
    
    //sw_label desk_l1( desktop, sw_point(3,0),       " LOG ",    white_on_blue );
    sw_lamp desk_l1( desktop, sw_point(3,0),       " LOG ",    white_on_blue, yellow_on_blue );
    //loglp = &desk_l1;
    
    sw_label desk_l2( desktop, sw_point(act_x+2,0), " Action ", white_on_blue );
    sw_label desk_l3( desktop, sw_point(act_x-6,0), " 1 ",      white_on_blue );

    sw_bordered_win
        top_bar(
                d, sw_rect( sw_point(0,0), sw_size(d.xs(),1)),
                No_Border, black_on_gray,
                black_on_gray, ' '
               );

    sw_label tb_ll1( top_bar, sw_point(1,0),             "2:5020/32@@FidoNet",    black_on_gray );
    sw_label tb_ll2( top_bar, sw_point(tb_ll1.xs()+2,0), "³",                    black_on_gray );
    sw_label tb_ll3( top_bar, sw_point(tb_ll1.xs()+4,0), "The Brake! g703/0004", black_on_gray );

    sw_label tb_rl1( top_bar, sw_point(d.xs()-14,0),     "³",                    black_on_gray );
    sw_label tb_rl2( top_bar, sw_point(d.xs()-12,0),     "36:13",                black_on_gray );
    sw_label tb_rl3( top_bar, sw_point(d.xs()-6,0),      "³",                    black_on_gray );
    sw_label tb_rl4( top_bar, sw_point(d.xs()-4,0),      "28%",                  black_on_gray );
    
    sw_bordered_win
        bot_bar(
                d, sw_rect( sw_point(0,d.ys()-1), sw_size(d.xs(),1)),
                No_Border, black_on_gray,
                black_on_gray, ' '
               );

    sw_label bb_ll1( bot_bar, sw_point( 1,0),         "  92m",    black_on_gray );
    sw_label bb_ll2( bot_bar, sw_point( 8,0),         "³",        black_on_gray );
    sw_label bb_ll3( bot_bar, sw_point(10,0),         "Thu 12/09 09:08:33",        black_on_gray );
    sw_label bb_ll4( bot_bar, sw_point(29,0),         "³",        black_on_gray );
    sw_label bb_ll5( bot_bar, sw_point(31,0),         "",        black_on_gray );
    sw_label bb_ll6( bot_bar, sw_point(35,0),         "³",        black_on_gray );
    sw_label bb_ll7( bot_bar, sw_point(37,0),         "",        black_on_gray );
    sw_label bb_ll8( bot_bar, sw_point(42,0),         "³",        black_on_gray );
    
    //sw_label bb_rl1( bot_bar, sw_point(d.xs()-42,0),  "DTR CTS RTS RI CD RQ SQ",                    black_on_gray );
    sw_label bb_rl2( bot_bar, sw_point(d.xs()-13,0),  "³",                    black_on_gray );
    sw_label bb_rl3( bot_bar, sw_point(d.xs()-11,0),  "COM2 57600",                    black_on_gray );

    sw_color lamp0(sw_color::Black,sw_color::LightGray);
    sw_color lamp1(sw_color::White,sw_color::Red);

    sw_lamp lamp[7] =
        {
        sw_lamp( bot_bar, sw_point(d.xs()-42,0), "DTR",  lamp0, lamp1 ),
        sw_lamp( bot_bar, sw_point(d.xs()-38,0), "CTS",  lamp0, lamp1 ),
        sw_lamp( bot_bar, sw_point(d.xs()-34,0), "RTS",  lamp0, lamp1 ),
        sw_lamp( bot_bar, sw_point(d.xs()-30,0), "RI",   lamp0, lamp1 ),
        sw_lamp( bot_bar, sw_point(d.xs()-27,0), "CD",   lamp0, lamp1 ),
        sw_lamp( bot_bar, sw_point(d.xs()-24,0), "RQ",   lamp0, lamp1 ),
        sw_lamp( bot_bar, sw_point(d.xs()-21,0), "SQ",   lamp0, lamp1 )
        };

    /*
    for( int lamp_v = 0; lamp_v < 0x80; lamp_v++ )
        {
        for( int nl = 0; nl < 7; nl++ )
            lamp[nl].value(0x01 & (lamp_v>>nl));
        }
    */

    sw_bordered_win
        mid_bar(
                desktop, sw_rect( sw_point(0,10), sw_size(d.xs(),1)),
                No_Border, cyan_on_blue,
                cyan_on_blue, 'Ä'
               );

    sw_label mb_ll1( mid_bar, sw_point( 0,0),         "Ç",               cyan_on_blue  );
      //sw_label mb_ll2( mid_bar, sw_point( 3,0),         " Queue: Direct ", white_on_blue );
    sw_lamp  mb_ll2( mid_bar, sw_point( 3,0),         " Queue: Direct ", white_on_blue , yellow_on_blue );
    //quelp = &mb_ll2;

    sw_label mb_rl1( mid_bar, sw_point( act_x-1,0),   "Á",               cyan_on_blue  );
    sw_label mb_rl2( mid_bar, sw_point( d.xs()-14,0), " Priority ",     white_on_blue );
    sw_label mb_rl3( mid_bar, sw_point( d.xs()-1,0),  "¶",               cyan_on_blue );

    sw_bordered_win
        ver_bar(
                desktop, sw_rect( sw_point(act_x-1,0), sw_size(1,10)),
                No_Border, cyan_on_blue,
                cyan_on_blue, '³'
               );
    sw_label vb_rl1( desktop, sw_point( act_x-1,0),   "Ñ",               cyan_on_blue  );

    sw_bordered_win log_w
        (
         desktop, sw_rect(1,1,act_x-2,9),
         No_Border, sw_color::Yellow, cyan_on_blue, ' '
        );

    sw_tail tail( log_w, sw_rect(0,0,act_x-2,9), 50, sw_color::Blue, sw_color::Magenta );

    sw_bordered_win act_w
        (
         desktop, sw_rect(act_x,1,d.xs()-act_x-1,9),
         No_Border, sw_color::Yellow, cyan_on_blue, ' '
        );

    vector <sw_list_elem_base*> listel;
    
    sw_simple_list que_w
        (
         desktop, sw_rect(1,12,d.xs()-2,d.ys()-15),
         listel,
         white_on_blue, yellow_on_magenta
        );
    que_w.focus_tree();
    sw_label que_l1( desktop, sw_point(3,11),  "Node                        Con/Bad  Mail  Files ÚI¿ÚC¿ÚD¿ÚN¿ÚH¿R State",         cyan_on_blue  );

    for( int i = 0; i < 100; i++ )
        {
        string s, a, m1, m2;

        a.print("%d:%d/%d.%d",
                (rand() * 4) / RAND_MAX,
                (rand() * 6000) / RAND_MAX,
                (rand() * 400) / RAND_MAX,
                (rand() * 35) / RAND_MAX
               );
        
        s.print("n %-25s    0/0  %5dK%5dK  ..............F. Disable   ",
                a.c_str(),
                (rand() * 9000) / RAND_MAX,
                (rand() * 9000) / RAND_MAX
               );
        
        listel.push_back( new sw_static_list_elem( s ) );
        }

    sw_vmenu cx_menu2( d, 0, sw_rect(15,14,25,6), black_on_gray, yellow_on_magenta );
    sw_menu_elem_base cx2_e1("Rescan Queue   Alt-Q",cx_menu2);
    sw_menu_elem_base cx2_e2("Sort by Node      ^N",cx_menu2);
    sw_menu_elem_base cx2_e3("Sort by Flavour   ^F",cx_menu2);
    sw_menu_elem_base cx2_e4("Sort by Size      ^S",cx_menu2);

    //sw_border cx_menu2_border( cx_menu2, Dot_Border, white_on_blue );
    
    que_w.context_menu( &cx_menu2 );
    

    logwp = &log_w; // for our special windows focus switcher
    quewp = &que_w;

    class sw_lamp_focus_hook : public sw_hook_base
        {
        sw_lamp &l;
        virtual bool pre( const sw_event &e ) { return No; }
        virtual bool post( const sw_event &e ) 
            {
            if( e == sw_event(sw_event::InFocus) )
                {
                l.value(1);
                return Yes;
                }
            if( e == sw_event(sw_event::NotInFocus) )
                {
                l.value(0);
                return Yes;
                }
            return No;
            }
        
        public:
            sw_lamp_focus_hook( sw_event_handler&hooked, sw_lamp &ll )
                :
            sw_hook_base(hooked), l(ll)
                {}
        } lhook1( log_w, desk_l1 ), lhook2(que_w,mb_ll2);

      // Set initial focus after setting up hooks
      // so that hooks will see focus change
    //en.focus_tree();
    tail.focus_tree();

    tail.add("Press F9 to test dialog");
    tail.add("Press F10 to test menus");
    tail.add("Press Shift-F10 to test context menus");


    sw_vmenu cx_menu1( d, 0, sw_rect(15,4,25,6), black_on_gray, yellow_on_magenta );
    sw_menu_elem_base cx_e1("Save Log Window   ^S",cx_menu1);
    sw_menu_elem_base cx_e2("Add Line To Log   ^A",cx_menu1);
    sw_menu_elem_base cx_e3("Find              ^F",cx_menu1);
    sw_menu_elem_base cx_e4("Find Again        ^L",cx_menu1);

    tail.context_menu( &cx_menu1 );
    
    sw_label aw_l1( act_w, sw_point( 3,1),  "Mode:",  cyan_on_blue  );
    sw_label aw_l2( act_w, sw_point( 3,3),  "Call:",  cyan_on_blue  );
    sw_label aw_l3( act_w, sw_point( 3,4),  "Used:",  cyan_on_blue  );
    sw_label aw_l4( act_w, sw_point( 2,6),  "Event:", cyan_on_blue  );
    sw_label aw_l5( act_w, sw_point( 3,7),  "Time:",  cyan_on_blue  );

    sw_label aw_r1( act_w, sw_point( 9,1),  "Abort",         white_on_blue  );
    sw_label aw_r2( act_w, sw_point( 9,3),  "2:5020/144",    white_on_blue  );
    sw_label aw_r3( act_w, sw_point( 9,4),  "26400, 01:25",  white_on_blue  );
    sw_label aw_r4( act_w, sw_point( 9,6),  "UUCP Poll",     white_on_blue  );
    sw_label aw_r5( act_w, sw_point( 9,7),  "11:00",         white_on_blue  );


    sw_hmenu mainmenu( d, sw_rect(0,0,desktop.xs(),1), black_on_gray, yellow_on_magenta );

    sw_vsubmenu vmenu1( "File ", d, mainmenu, sw_rect(2,1,15,7), black_on_gray, yellow_on_magenta );
    sw_menu_elem_base e1("Open",vmenu1);
    sw_menu_elem_base e2("Save",vmenu1);
    sw_menu_elem_base e3("Save As",vmenu1);
    sw_menu_elem_base e4("Save All",vmenu1);
    sw_menu_elem_base e5("Print",vmenu1);
    sw_menu_elem_base e6("Close",vmenu1);
    sw_menu_elem_base e7("Exit",vmenu1);
    
    sw_vsubmenu vmenu2( "Edit ", d, mainmenu, sw_rect(10,1,15,7), black_on_gray, yellow_on_magenta );

    sw_vsubmenu vmenu21( "Copy ", d, vmenu2, sw_rect(15,3,12,4), black_on_gray, yellow_on_magenta );
    sw_menu_elem_base e8("As Entry",vmenu21);
    sw_menu_elem_base e9("As List",vmenu21);

    sw_menu_elem_base e11("Hold",vmenu2);
    sw_menu_elem_base e12("UnHold",vmenu2);
    sw_menu_elem_base e13("ReAddress",vmenu2);
    sw_menu_elem_base e14("Detach",vmenu2);

    sw_menu_elem_base e15("Browse",mainmenu);
    sw_menu_elem_base e16("Quit",mainmenu);

    sw_menu_push_focus to_send_dialog( "Send", mainmenu , send_dialog );
    
    menuwp = &mainmenu;

    //mainmenu.back_focus( &desktop );

    sw_clock clock( d, sw_point(30,8) );
    clock.top();

    
    KBDKEYINFO ki;

    while(1)
        {
        d.flush();
        KbdCharIn( &ki, IO_WAIT, 0 );

//cx_menu2_border.top();
        
        if( ki.chScan == 0x2d && (ki.fsState & sw_event::Alt))
            break;

        if( !d.event( sw_event(ki.chChar, ki.chScan, ki.fsState) ) )
            {
            if(
               //(ki.fsState & (RShift|LShift)) &&
               //(!(ki.fsState & (LControl|LAlt|RControl|RAlt))) &&
               ki.chScan == sw_key::sf10
              )
                d.event( sw_event( sw_event::ContextMenu ) );
            }

        string s;
        s.print( "a %02x s %02x", ki.chChar, ki.chScan );
        aw_r4.text( s );
        s.print( "shift %04x", ki.fsState );
        aw_r5.text( s );

        s.print( "key a %02x s %02x shift %04x", ki.chChar, ki.chScan, ki.fsState );
        tail.add(s);
        }
    
    }


int main()
    {
    try { go(); }
    catch( sw_Exception e )
        {
        cout << "Exception in " << e.where.c_str()
             << ": " << e.cause.c_str() 
             << " (" << e.data.c_str() << ")\n";
        }

    }

@


1.24
log
@Nonrectangular (partially transparent) windows
@
text
@d11 1
d245 1
a245 1
    sw_border cx_menu2_border( cx_menu2, Dot_Border, white_on_blue );
d341 4
d352 1
a352 1
cx_menu2_border.top();
@


1.23
log
@Before changing event class
@
text
@d243 2
d347 2
@


1.22
log
@preliminary mouse support done
@
text
@@


1.21
log
@update
@
text
@d2 1
a2 1
#define INCL_KBD
d4 1
a4 1
#include <os2.h>
@


1.20
log
@context menus done, menus code fixed to
support lone vertical menus correctly.
@
text
@a280 13
    tail.add("Log line #1");
    tail.add("Log line #2");
    tail.add("Log line #3");
    tail.add("Long-Long Log line #4");
    tail.add("Long-Long Log line #5");
    tail.add("Long-Long Log line #6");
    tail.add("Long-Long Long-Long Log line #7");
    tail.add("Long-Long Long-Long Log line #8");
    tail.add("Long-Long Long-Long Log line #9");
    tail.add("Long-Long Long-Long Log line #10");
    tail.add("Long-Long Long-Long Log line #11");
    tail.add("Long-Long Long-Long Log line #12");
    tail.add("Long-Long Long-Long Log line #13");
d363 3
@


1.19
log
@reorganized devices classes, generalized
videomemory-style devices
@
text
@d238 8
d296 10
d361 9
a369 1
        d.event( sw_event(ki.chChar, ki.chScan, ki.fsState) );
d372 1
a372 1
        s.print( "s %02x a %02x", ki.chChar, ki.chScan );
@


1.18
log
@new 3d entry fields
@
text
@d29 1
a29 1
                    menuwp->focus_tree();
d75 1
a75 1
        sw_bordered_win( d, sw_rect(30,5,35,9),
d83 1
a83 1
    sw_3d_entry send_addr( send_dialog, sw_rect(2,2,21,1), white_on_green, black_on_lightgreen, black_on_gray );
d85 2
a86 1
    sw_3d_entry send_file( send_dialog, sw_rect(2,4,21,1), white_on_green, black_on_lightgreen, black_on_gray );
d88 8
a95 1
    sw_3d_entry send_type( send_dialog, sw_rect(2,6,21,1), white_on_green, black_on_lightgreen, black_on_gray );
a96 4
    sw_text_button b_ok( send_dialog, sw_point( 24, 2 ), "   Ok   ",     white_on_green, black_on_lightgreen, black_on_gray );
    sw_text_button b_cancel( send_dialog, sw_point( 24, 4 ), " Cancel ", white_on_green, black_on_lightgreen, black_on_gray );
    sw_text_button b_help( send_dialog, sw_point( 24, 6 ), "  Help  ",   white_on_green, black_on_lightgreen, black_on_gray );
    
d331 1
a331 1
    mainmenu.back_focus( &desktop );
@


1.17
log
@3d entry fields, wrong way
@
text
@d83 1
a83 1
    sw_entry send_addr( send_dialog, sw_rect(2,2,21,2), white_on_green, black_on_lightgreen, black_on_gray );
d85 1
a85 1
    sw_entry send_file( send_dialog, sw_rect(2,4,21,2), white_on_green, black_on_lightgreen, black_on_gray );
d87 1
a87 1
    sw_entry send_type( send_dialog, sw_rect(2,6,21,2), white_on_green, black_on_lightgreen, black_on_gray );
@


1.16
log
@buttons
@
text
@d83 1
a83 1
    sw_entry send_addr( send_dialog, sw_rect(2,2,20,1), white_on_green, black_on_lightgreen );
d85 1
a85 1
    sw_entry send_file( send_dialog, sw_rect(2,4,20,1), white_on_green, black_on_lightgreen );
d87 2
a88 1
    sw_entry send_type( send_dialog, sw_rect(2,6,20,1), white_on_green, black_on_lightgreen );
d282 2
@


1.15
log
@focusable
@
text
@d16 1
a16 2
sw_abstract_win *logwp, *quewp, *menuwp;
//int in_menu = 0;
d29 4
a32 4
                    //if( in_menu )                        focused_child()->focus_tree();
                    //else
                        menuwp->focus_tree();
                    //in_menu = (bool)!in_menu;
d45 8
a52 5
sw_color cyan_on_blue (sw_color::LightCyan,sw_color::Blue);
sw_color white_on_blue(sw_color::White,sw_color::Blue);
sw_color yellow_on_blue(sw_color::Yellow,sw_color::Blue);
sw_color black_on_gray(sw_color::Black,sw_color::LightGray);
sw_color yellow_on_magenta(sw_color::Yellow,sw_color::Magenta);
d54 1
d75 2
a76 2
        sw_bordered_win( d, sw_rect(30,5,30,8),
                        Single_Border, yellow_on_blue, yellow_on_blue, ' '
d78 1
a78 1
        title( *this, " Send File ", yellow_on_blue )
d83 1
a83 1
    sw_entry send_addr( send_dialog, sw_rect(2,2,20,1), black_on_gray, sw_color(sw_color::Black,sw_color::LightCyan) );
d85 1
a85 1
    sw_entry send_file( send_dialog, sw_rect(2,4,20,1), black_on_gray, sw_color(sw_color::Black,sw_color::LightCyan) );
d87 4
a90 1
    sw_entry send_type( send_dialog, sw_rect(2,5,20,1), black_on_gray, sw_color(sw_color::Black,sw_color::LightCyan) );
d93 3
@


1.14
log
@bugfixes - a lot of
@
text
@d69 1
d74 2
a75 1
                       )
@


1.13
log
@dialogs, basically written
@
text
@d80 4
a83 2
    sw_entry send_file( send_dialog, sw_rect(2,3,20,1), black_on_gray, sw_color(sw_color::Black,sw_color::LightCyan) );
    send_addr.value("*.*");
@


1.12
log
@log control written
@
text
@d67 17
a180 4
    //sw_entry en( log_w, sw_rect(5,3,20,1), cyan_on_blue, yellow_on_blue );
    sw_entry en( log_w, sw_rect(5,3,20,1), black_on_gray, sw_color(sw_color::Black,sw_color::LightCyan) );
    en.value("Hi there!");

d252 1
a252 1
    en.focus_tree();
d285 7
a291 7
    vmenu1.add( new sw_menu_elem_base("Open") );
    vmenu1.add( new sw_menu_elem_base("Save") );
    vmenu1.add( new sw_menu_elem_base("Save As") );
    vmenu1.add( new sw_menu_elem_base("Save All") );
    vmenu1.add( new sw_menu_elem_base("Print") );
    vmenu1.add( new sw_menu_elem_base("Close") );
    vmenu1.add( new sw_menu_elem_base("Exit") );
d296 7
a302 2
    vmenu21.add( new sw_menu_elem_base("As Entry") );
    vmenu21.add( new sw_menu_elem_base("As List") );
d304 2
a305 4
    vmenu2.add( new sw_menu_elem_base("Hold") );
    vmenu2.add( new sw_menu_elem_base("UnHold") );
    vmenu2.add( new sw_menu_elem_base("ReAddress") );
    vmenu2.add( new sw_menu_elem_base("Detach") );
d307 1
a307 2
    mainmenu.add( new sw_menu_elem_base("Browse") );
    mainmenu.add( new sw_menu_elem_base("Quit") );
d320 1
a320 1
        if( ki.chScan == 0x2d )
@


1.11
log
@external array for list
@
text
@d10 1
d160 1
a160 1
         desktop, sw_rect(1,1,act_x-3,9),
d168 1
a169 1
    
d240 1
d242 13
@


1.10
log
@menu item classes written (wrong way yet)
@
text
@d175 2
d180 2
a181 1
         white_on_blue, yellow_on_blue
d186 3
a188 30
    que_w.add("d 2:50/0@@FidoNet                1/14   3.1K    -   ......M..M...... *Nothing*");
    que_w.add("d 2:5020/50@@FidoNet            56/0    126K    -   .........M...... 28800/ARQ/");
    que_w.add("d 2:5020/251@@FidoNet           64/0      -     -   ................ 16800/ARQ/");
    que_w.add("d 2:5020/269@@FidoNet           82/0      -     -   ................ 28800/ARQ/");
    que_w.add("d 2:5020/411@@FidoNet           27/1      -     -   ................ *Nothing* ");
    que_w.add("d 2:5020/509@@FidoNet          161/0      -     -   ................ 26400/ARQ/");
    que_w.add("n 2:50/317.137@@FidoNet          0/0      -   209K  ..............F. Disable   ");
    que_w.add("n 2:50/325.55@@FidoNet           0/0      -    21K  ..............F. Disable   ");
    que_w.add("n 2:50/325.64@@FidoNet           0/0      -   7.8K  ..............F. Disable   ");
    que_w.add("n 2:50/325.75@@FidoNet           0/0      -    17K  ..............F. Disable   ");
    que_w.add("n 2:50/330.50@@FidoNet           0/0      -   9.3K  ..............F. Disable   ");
    que_w.add("n 2:50/502.23@@FidoNet           0/0      -     -   ................ Disable   ");
    que_w.add("n 2:5001/20.30@@FidoNet          0/0      -     -   ................ Disable   ");
    que_w.add("n 2:5001/211.3@@FidoNet          0/0      -     -   ................ Disable   ");
    que_w.add("n 2:5002/18.10@@FidoNet          0/0      -    30K  ..............F. Disable   ");
    que_w.add("n 2:5003/16@@FidoNet             0/0      -     -   ................ Disable   ");
    que_w.add("n 2:5003/22.5@@FidoNet           0/0      -     -   ................ Disable   ");
    que_w.add("n 2:5005/49.1@@FidoNet           0/0      -    12K  ..............F. Disable   ");
    que_w.add("n 2:5012/1.136@@FidoNet          0/0      -     -   ................ Disable   ");
    que_w.add("n 2:5012/10.115@@FidoNet         0/0      -    14K  ..............F. Disable   ");
    que_w.add("n 2:5015/7.10@@FidoNet           0/0      -   578K  ..............F. Disable   ");
    que_w.add("n 2:5019/1.33@@FidoNet           0/0      -    65K  ..............F. Disable   ");
    que_w.add("n 2:5019/9.6@@FidoNet            0/0      -     -   ................ Disable   ");
    que_w.add("n 2:5020/14.33@@FidoNet          0/0      -     -   ................ Not worked");
    que_w.add("n 2:5020/22.43@@FidoNet          0/0      -     -   ................ Not worked");
    que_w.add("n 2:5020/37.124@@FidoNet         0/0      -     -   ................ Not worked");
    que_w.add("n 2:5020/68.36@@FidoNet          0/0      -    30K  ..............F. Not worked");
    que_w.add("n 2:5020/72@@FidoNet             0/12     -   1.4M  ...........F..F. NO CARRIER");
    que_w.add("n 2:5020/91@@FidoNet             0/0      -    27K  ..............F. Not worked");

d190 15
d256 8
a263 4
    sw_vmenu vmenu1( d, &mainmenu, sw_rect(2,1,15,7), black_on_gray, yellow_on_magenta );
    vmenu1.add( "Open" );      vmenu1.add( "Save" );    vmenu1.add( "Save As" );
    vmenu1.add( "Save All" );  vmenu1.add( "Print" );   vmenu1.add( "Close" );
    vmenu1.add( "Exit" );
d265 1
a265 1
    sw_vmenu vmenu2( d, &mainmenu, sw_rect(13,1,15,7), black_on_gray, yellow_on_magenta );
d267 8
a274 12
    sw_vmenu vmenu21( d, &vmenu2, sw_rect(17,3,12,7), black_on_gray, yellow_on_magenta );
    vmenu21.add( "As Entry" );    vmenu21.add( "As List" );

    vmenu2.add( "Copy ", vmenu21 );
    vmenu2.add( "Hold" );       vmenu2.add( "UnHold" );
    vmenu2.add( "ReAddress" );  vmenu2.add( "Detach" );

    mainmenu.add( "File ", vmenu1 );
    mainmenu.add( "Edit ", vmenu2 );
    mainmenu.add( "Browse" );
    mainmenu.add( "Quit" );
    //mainmenu.focus_tree();
d276 3
@


1.9
log
@update
@
text
@d275 1
a275 1
    vmenu2.add( "Copy ", &vmenu21 );
d279 2
a280 2
    mainmenu.add( "File ", &vmenu1 );
    mainmenu.add( "Edit ", &vmenu2 );
@


1.8
log
@bugfixes
@
text
@d15 2
a16 1
sw_abstract_win *logwp, *quewp;
d23 14
a36 2
            if( e.fkey() != sw_key::tab ) return No;
            focused_child( focused_child() == quewp ? logwp : quewp );
d58 1
a58 1
int main()
a261 2
#if 0
    sw_hmenu mainmenu( desktop, sw_rect(0,0,desktop.xs(),1), black_on_gray, yellow_on_magenta );
d263 6
a268 4
    sw_vmenu vmenu1( desktop, &mainmenu, sw_rect(2,1,30,7), black_on_gray, yellow_on_magenta );
    vmenu1.add( "Item 1" );    vmenu1.add( "Item 2" );    vmenu1.add( "Item 3" );
    vmenu1.add( "Item 4" );    vmenu1.add( "Item 5" );    vmenu1.add( "Item 6" );
    vmenu1.add( "Item 7" );    vmenu1.add( "Item 8" );    vmenu1.add( "Item 9" );
d270 4
a273 1
    sw_vmenu vmenu2( desktop, &mainmenu, sw_rect(13,1,30,7), black_on_gray, yellow_on_magenta );
d275 3
a277 2
    sw_vmenu vmenu21( desktop, &vmenu2, sw_rect(17,3,15,7), black_on_gray, yellow_on_magenta );
    vmenu21.add( "Item 1" );    vmenu21.add( "Item 2" );
d279 9
a287 11
    vmenu2.add( "SubSub 1", &vmenu21 );
    vmenu2.add( "Item 2" );    vmenu2.add( "Item 3" );
    vmenu2.add( "Item 4" );    vmenu2.add( "Item 5" );    vmenu2.add( "Item 6" );
    vmenu2.add( "Item 7" );    vmenu2.add( "Item 8" );    vmenu2.add( "Item 9" );
    
    mainmenu.add( "SubMenu 1", &vmenu1 );
    mainmenu.add( "SubMenu 2", &vmenu2 );
    mainmenu.add( "Item 3" );
    mainmenu.add( "Item 4" );
    mainmenu.focus_tree();
#endif 
d309 14
@


1.7
log
@viobuf, optimization (sort of:)
@
text
@d249 1
d258 6
a263 1
    vmenu2.add( "Item 1" );    vmenu2.add( "Item 2" );    vmenu2.add( "Item 3" );
d272 1
@


1.6
log
@menus basically done
@
text
@d271 1
@


1.5
log
@update
@
text
@d36 1
d249 17
d273 1
a273 1
        if( ki.chChar == 0x1B )
@


1.4
log
@generalized event model
@
text
@d16 1
a16 1
  /*sw_lamp         *loglp, *quelp;*/
a22 2
            //static n = 1;
            //n = !n;
a23 2
            //if( loglp ) loglp->value( n );
            //if( quelp ) quelp->value( !n );
d152 3
a154 1
        
@


1.3
log
@update
@
text
@d16 1
a16 2
sw_lamp         *loglp, *quelp;

d23 5
a27 5
            static n = 0;
            n = !n;
            focused_child( n ? logwp : quewp );
            if( loglp ) loglp->value( n );
            if( quelp ) quelp->value( !n );
d60 1
a60 1
    loglp = &desk_l1;
d133 1
a133 1
    quelp = &mb_ll2;
a154 1
    en.focus_tree();
d203 1
a203 1
    
d206 31
@


1.2
log
@works a little
@
text
@d2 4
d8 4
d15 34
d51 5
d57 40
a96 32
    sw_bordered_win w1(
                    d,
                    sw_rect( sw_point(0,0), (sw_size&)d)
                   );

    sw_onechar_button b11( w1, sw_point(2,0) );
    char *ltext = " Hello, World! ";
    int llen = strlen(ltext);
    sw_label l11(
                 w1,
                   //sw_point( (d.xs()/2)-llen/2, 1 ),
                 sw_point( 1, 1 ),
                 ltext, llen, sw_color::LightGreen
                );

    sw_bordered_win w21(
                       w1, sw_rect(20, 7, 35, 10),
                       Single_Border, sw_color::LightRed,
                       sw_color::LightGreen
                      );

    sw_bordered_win w22(
                        w1, sw_rect(25, 15, 15, 3),
                        Single_Border, sw_color::LightCyan,
                        sw_color::DarkGray
                       );

    sw_bordered_win w211(
                        w21, sw_rect(5, 2, 15, 3),
                        Single_Border, sw_color::LightCyan,
                        sw_color::DarkGray
                       );
d98 3
d102 16
a117 1
    for( int i = 0; i < 20; i++ )
d119 2
a120 3
        w22.move( 1,0 );
        w211.move( 1,0 );
        b11.state(i&1);
d122 83
d206 15
a220 3
    for( i = 0; i < 10; i++ )     w21.move( 1,0 );
    w22.bot();
    for( i = 0; i < 20; i++ )     w211.move( -1,0 );
d222 1
d224 3
a226 1
      //w1.fill( sw_color::Red, '*', sw_rect( 1, 1, 4, 4 ) );
d228 2
a229 1
    }
d231 1
d233 8
@


1.1
log
@Initial revision
@
text
@d3 1
d5 1
a5 1
sw_ansi_device d;
d10 35
a44 1
    char str[] =
d46 4
a49 9
        'a',1, 'b',2, 'c',3, 'd',4, 'e',5,
        'f',6, 'g',7, 'h',8, 'i',9, 'j',10
        };

    d.fill( sw_color::Cyan, '°' );
    d.put_up(
             sw_rect(10, 10, 5, 2),
             str
            );
d51 7
d59 1
@
