/************************ SWin ***************************\
 *
 *	Copyright (C) 1996 by Dmitry Zavalishin
 *
 *	Module 	: dialog classes
 *
 *      $Log: sw_win.h $
 *
 *
\*/


class dialog : public sw_bordered_win, public sw_dialog_focus_changer
    {
    sw_title  title;
    public:
        dialog(
               sw_abstract_win &parent,
               sw_rect pos_size,
               const char *title,
               sw_border_t border_type = Single_Border,
               sw_color border_color = sw_color::Yellow,
               sw_color fill_color = sw_color::Cyan,
               char fill_char = ' '
              )
        :
        sw_bordered_win(
                        parent, pos_size,
                        border_type, border_color, fill_color, ' '
                       ),
        title( *this, title, border_color )
            {}
    };
