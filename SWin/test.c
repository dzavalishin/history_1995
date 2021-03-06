
//#define INCL_KBD
//#define INCL_VIO
//#include <os2.h>

#include "sw_device.h"
#include "sw_controls.h"
#include "sw_ctl_list.h"
#include "sw_ctl_entry.h"
#include "sw_tail.h"
#include "sw_clock.h"
#include "sw_dialog.h"



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
const sw_color blue_on_gray(sw_color::Blue,sw_color::LightGray);
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
    
    sw_rect desk_rect(1,2,d.xs()-2,d.ys()-4);
    int act_x = d.xs() - 26;

    dialog send_dialog(
                       d, sw_rect(30,5,45,12), " Send File ",
                       Single_Border, /*blue_on_gray*/ yellow_on_magenta, black_on_gray, '?'
                      );


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
                No_Border, yellow_on_magenta, black_on_gray, ' '
               );
    
    sw_label tb_ll1( top_bar, sw_point(1,0),             "2:5020/32@FidoNet",    black_on_gray );
    sw_label tb_ll2( top_bar, sw_point(tb_ll1.xs()+2,0), "?",                    black_on_gray );
    sw_label tb_ll3( top_bar, sw_point(tb_ll1.xs()+4,0), "The Brake! g703/0004", black_on_gray );

    sw_label tb_rl1( top_bar, sw_point(d.xs()-14,0),     "?",                    black_on_gray );
    sw_label tb_rl2( top_bar, sw_point(d.xs()-12,0),     "36:13",                black_on_gray );
    sw_label tb_rl3( top_bar, sw_point(d.xs()-6,0),      "?",                    black_on_gray );
    sw_label tb_rl4( top_bar, sw_point(d.xs()-4,0),      "28%",                  black_on_gray );
    

    sw_bordered_win bot_bar(
                            d, sw_rect( 0, d.ys()-1, d.xs(), 1 ),
                            No_Border, /*black_on_gray*/ yellow_on_magenta, black_on_gray, ' '
                           );
    //bot_bar.bot();
    
    sw_label bb_ll1( bot_bar, sw_point( 1,0),         "  92m",    black_on_gray );
    sw_label bb_ll2( bot_bar, sw_point( 8,0),         "?",        black_on_gray );
    sw_label bb_ll3( bot_bar, sw_point(10,0),         "Thu 12/09 09:08:33",        black_on_gray );
    sw_label bb_ll4( bot_bar, sw_point(29,0),         "?",        black_on_gray );
    sw_label bb_ll5( bot_bar, sw_point(31,0),         "",        black_on_gray );
    sw_label bb_ll6( bot_bar, sw_point(35,0),         "?",        black_on_gray );
    sw_label bb_ll7( bot_bar, sw_point(37,0),         "",        black_on_gray );
    sw_label bb_ll8( bot_bar, sw_point(42,0),         "?",        black_on_gray );
    
    //sw_label bb_rl1( bot_bar, sw_point(d.xs()-42,0),  "DTR CTS RTS RI CD RQ SQ",                    black_on_gray );
    sw_label bb_rl2( bot_bar, sw_point(d.xs()-13,0),  "?",                    black_on_gray );
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

    /**/

    sw_bordered_win
        mid_bar(
                desktop, sw_rect( sw_point(0,10), sw_size(d.xs(),1)),
                No_Border, cyan_on_blue,
                cyan_on_blue, '?'
               );

    sw_label mb_ll1( mid_bar, sw_point( 0,0),         "?",               cyan_on_blue  );
      //sw_label mb_ll2( mid_bar, sw_point( 3,0),         " Queue: Direct ", white_on_blue );
    sw_lamp  mb_ll2( mid_bar, sw_point( 3,0),         " Queue: Direct ", white_on_blue , yellow_on_blue );
    //quelp = &mb_ll2;

    sw_label mb_rl1( mid_bar, sw_point( act_x-1,0),   "?",               cyan_on_blue  );
    sw_label mb_rl2( mid_bar, sw_point( d.xs()-14,0), " Priority ",     white_on_blue );
    sw_label mb_rl3( mid_bar, sw_point( d.xs()-1,0),  "?",               cyan_on_blue );

    sw_bordered_win
        ver_bar(
                desktop, sw_rect( sw_point(act_x-1,0), sw_size(1,10)),
                No_Border, cyan_on_blue,
                cyan_on_blue, '?'
               );
    sw_label vb_rl1( desktop, sw_point( act_x-1,0),   "?",               cyan_on_blue  );

    sw_bordered_win log_w
        (
         desktop, sw_rect(0,0,act_x-0,11),
         No_Border, sw_color::Yellow, cyan_on_blue, ' '
        );

    sw_tail tail( log_w, sw_rect(0,0,act_x-2,9), 50, /*sw_color::Blue*/sw_color::Red, sw_color::Magenta );

    sw_bordered_win act_w
        (
         desktop, sw_rect(act_x,1,d.xs()-act_x-1,9),
         No_Border, sw_color::Yellow, cyan_on_blue, ' '
        );

    vector <sw_list_elem_base*> listel;
    
    sw_simple_list que_w
        (
         desktop, sw_rect(1,12,d.xs()-2, d.ys()-17 ),
         listel,
         white_on_blue, yellow_on_magenta
        );
    que_w.focus_tree();
    sw_label que_l1( desktop, sw_point(3,11),  "Node                        Con/Bad  Mail  Files ?I??C??D??N??H?R State",         cyan_on_blue  );

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

