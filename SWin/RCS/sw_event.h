head	1.12;
access;
symbols;
locks;
comment	@ * @;


1.12
date	96.11.18.16.16.26;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.11.18.15.47.54;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.11.15.21.37.58;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.11.15.16.29.28;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.10.11.17.02.37;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.09.28.14.03.17;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.09.28.11.55.51;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.09.26.20.46.52;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.09.26.06.06.18;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.09.21.13.26.57;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.09.21.08.07.06;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.09.20.14.40.18;	author dz;	state Exp;
branches;
next	;


desc
@@


1.12
log
@Event class now has no type field/method, but
has info() == KeyPress and delivery mode == Mouse.
If event::mouse() is nonzero, pos is valid.
@
text
@
#ifndef SW_EVENT
#define SW_EVENT

#include "sw_types.h"
#include <style.h>

class sw_key // used as a namespace for key definitions
    {
    public:
        enum k
            {
            none = 0,
            
            tab = 0x0F, ins = 0x52, del = 0x53, bs = 0x0e,
            space = 0x39, enter = 0x1C, esc = 0x01,
            
            left = 0x4B, right = 0x4D, up = 0x48, down = 0x50,
            cleft = 0x73, cright = 0x74, cup = 0x8D, cdown = 0x91,

            pgup = 0x49, pgdn = 0x51, home = 0x47, end = 0x4F,
            
            f1 = 0x3B, f2 = 0x3C, f3 = 0x3D, f4 = 0x3E, f5 = 0x3F, f6 = 0x40,
            f7 = 0x41, f8 = 0x42, f9 = 0x43, f10 = 0x44, f11 = 0x85, f12 = 0x86,

            cf1 = 0x5E,
            cf11 = 0x89,

            sf1 = 0x54, sf2 = 0x55, sf3 = 0x56, sf4 = 0x57, sf5 = 0x58, sf6 = 0x59,
            sf7 = 0x5A, sf8 = 0x5B, sf9 = 0x5C, sf10 = 0x5D, sf11 = 0x87,  sf12 = 0x88,

            af1 = 0x68,
            af11 = 0x8B

            
            };
    };
    


class sw_event
    {
    public:
          // Delivery:
          //   To focus until confirmed
          //   To focus, forced
          //   Broadcast until confirmed
          //   Broadcast, forced
          //   To window in given position (Mouse)
        enum md { Focus = 0x00, FFocus = 0x01, Broad = 0x02, FBroad = 0x03, Mouse = 0x04 };
        //enum et { Key, Mouse, Info };
        enum it
            {
            Nothing,
            
              // Focus related
            NotInFocus, InFocus,

              // Aliases
            ContextMenu, // sent as Info if not from mouse, else as Mouse.

              // Keyboard
            KeyPress,
            
              // Mouse
            Mouse1Click, Mouse1DoubleClick, Mouse1Drag, Mouse1Press, Mouse1Release,
            Mouse2Click, Mouse2DoubleClick, Mouse2Drag, Mouse2Press, Mouse2Release,
            Mouse3Click, Mouse3DoubleClick, Mouse3Drag, Mouse3Press, Mouse3Release,
            MouseMove,
            BeginOfCapture, EndOfCapture
            };
        enum st
            {
            NoShifts = 0,
            RShift = 0x0001, LShift = 0x0002,  Control = 0x0004, Alt = 0x0008,
            Scrolllock_on = 0x0010, Numlock_on = 0x0020, Capslock_on = 0x0040,
            Insert_on = 0x0080, LControl = 0x0100, LAlt = 0x0200,
            RControl = 0x0400, RAlt = 0x0800,
            Scrolllock = 0x1000, Numlock = 0x2000,
            Capslock = 0x4000, Sysreq = 0x8000
            };

    private:
        md             mode_v;
        //et             type_v;
        it             info_v;
        char           akey_v;
        short          vkey_v;
        st             shifts_v;
        sw_point       pos_v;
    public:
        sw_event( it info, md mode = Focus )
            : info_v(info), mode_v(mode),
            shifts_v(NoShifts), pos_v(0,0), akey_v(0), vkey_v(0)
            {}
        
        sw_event( const sw_point &pos, it info, md mode = Mouse )
            : pos_v(pos), info_v(info), mode_v(mode),
            shifts_v(NoShifts), akey_v(0), vkey_v(0)
            {}
        
        sw_event( char a, short v, short s, md mode = Focus )
            : akey_v(a), vkey_v(v), shifts_v((st)s), mode_v(mode),
            pos_v(0,0), info_v(KeyPress)
            {}


        int operator == ( const sw_event &e ) const;

        md             mode()   const { return mode_v; }
        int            broad()  const { return ((int)mode_v) & 0x02; }
        int            forced() const { return ((int)mode_v) & 0x01; }
        int            mouse()  const { return ((int)mode_v) & 0x04; } // event is from mouse
        
        //et             type() const { return type_v; }

        it             info() const { return info_v; }

        virtual short  fkey() const
            { return info_v == KeyPress ? vkey_v : 0; }

        virtual char   akey() const 
            { return info_v == KeyPress ? akey_v : 0; }

        virtual bool   shift() const 
            { return shifts_v & (RShift|LShift) ? Yes : No; }

        const sw_point & pos() const { return pos_v; }

    };


#endif // SW_EVENT

@


1.11
log
@Before changing event class
@
text
@d49 3
a51 2
        enum md { Focus = 0x000, FFocus = 0x001, Broad = 0x020, FBroad = 0x021 };
        enum et { Key, Mouse, Info };
d56 1
a56 1
            // Focus related
d59 1
a59 1
            // Aliases
d62 4
a65 1
            // Mouse
d69 1
a69 1
            MouseMove
d74 2
a75 1
            RShift = 1, LShift = 2,  Control = 0x0004, Alt = 0x0008,
d85 1
a85 1
        et             type_v;
d93 2
a94 2
            : info_v(info), type_v(Info), mode_v(mode),
            shifts_v(0), pos_v(0,0), akey_v(0), vkey_v(0)
d97 3
a99 3
        sw_event( const sw_point &pos, it info, md mode = Focus )
            : pos_v(pos), info_v(info), type_v(Mouse), mode_v(mode)
            shifts_v(0), akey_v(0), vkey_v(0)
d103 2
a104 2
            : akey_v(a), vkey_v(v), type_v(Key), shifts_v((st)s), mode_v(mode)
            pos_v(0,0), info_v(Nothing)
d113 1
d115 1
a115 1
        et             type() const { return type_v; }
d120 1
a120 1
            { return type_v == Key ? vkey_v : 0; }
d123 1
a123 1
            { return type_v == Key ? akey_v : 0; }
@


1.10
log
@preliminary mouse support done
@
text
@d13 2
d53 2
d59 1
a59 1
            ContextMenu,
d65 1
d88 3
a90 1
            : info_v(info), type_v(Info), mode_v(mode) {}
d93 3
a95 1
            : pos_v(pos), info_v(info), type_v(Mouse), mode_v(mode) {}
d98 3
a100 1
            : akey_v(a), vkey_v(v), type_v(Key), shifts_v((st)s), mode_v(mode) {}
@


1.9
log
@mouse event generator written
@
text
@d111 2
@


1.8
log
@context menus done, menus code fixed to
support lone vertical menus correctly.
@
text
@d5 1
d49 14
a62 1
        enum it { NotInFocus, InFocus, ContextMenu };
d80 1
d84 3
@


1.7
log
@update
@
text
@d26 2
a27 2
            sf1 = 0x54,
            sf11 = 0x87,
d48 1
a48 1
        enum it { NotInFocus, InFocus };
@


1.6
log
@broad() and forced() was incorrect
@
text
@d20 2
a21 3
            f1 = 0x3B, f2 = 0x3C, f3 = 0x3D, f4 = 0x3E,
            f5 = 0x3F, f6 = 0x40, f7 = 0x41, f8 = 0x42,
            f9 = 0x43, f10 = 0x44, f11 = 0x85, f12 = 0x86,
@


1.5
log
@menus basically done
@
text
@d78 2
a79 2
        int            broad()  const { return ((int)mode_v) && 0x02; }
        int            forced() const { return ((int)mode_v) && 0x01; }
@


1.4
log
@update
@
text
@d13 1
a13 1
            space = 0x39, enter = 0x1C,
d82 2
@


1.3
log
@generalized event model
@
text
@d13 1
@


1.2
log
@last version with distinct broadcast/event entry points
@
text
@d41 6
d48 1
a48 1
        enum it { NoFocus, Focus };
d60 1
d67 2
a68 2
        sw_event( it info )
            : info_v(info), type_v(Info) {}
d70 2
a71 2
        sw_event( char a, short v, short s )
            : akey_v(a), vkey_v(v), type_v(Key), shifts_v((st)s) {}
d75 4
@


1.1
log
@Initial revision
@
text
@d67 1
@
