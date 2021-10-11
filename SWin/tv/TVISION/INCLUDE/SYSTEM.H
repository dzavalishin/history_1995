/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   SYSTEM.H                                                              */
/*                                                                         */
/*   Copyright (c) Borland International 1991                              */
/*   All Rights Reserved.                                                  */
/*                                                                         */
/*   defines the classes THWMouse, TMouse, TEventQueue, TDisplay,          */
/*   TScreen, and TSystemError                                             */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#include <tvvo.h>

#if !defined( __EVENT_CODES )
#define __EVENT_CODES

/* Event codes */

const evMouseDown = 0x0001;
const evMouseUp   = 0x0002;
const evMouseMove = 0x0004;
const evMouseAuto = 0x0008;
const evKeyDown   = 0x0010;
const evCommand   = 0x0100;
const evBroadcast = 0x0200;

/* Event masks */

const evNothing   = 0x0000;
const evMouse     = 0x000f;
const evKeyboard  = 0x0010;
const evMessage   = 0xFF00;

/* Mouse button state masks */

const mbLeftButton  = 0x01;
const mbRightButton = 0x02;

#endif  // __EVENT_CODES


#if defined( Uses_TEvent ) && !defined( __TEvent )
#define __TEvent

struct MouseEventType
{
    uchar buttons;
    Boolean doubleClick;
    TPoint where;
};

class THWMouse
{

protected:

    THWMouse();
    THWMouse( const THWMouse& ) {};
    ~THWMouse();

    static void cdecl show();
    static void cdecl hide();
#ifdef __OS2__
    static void hide( const TRect& rect );
#endif

    static void setRange( ushort, ushort );
    static void getEvent( MouseEventType& );
    static void registerHandler( unsigned, void (far *)() );
    static Boolean present();

    static void suspend();
    static void resume();
    static void inhibit();

protected:

    static uchar near buttonCount;

private:

    static Boolean near handlerInstalled;
    static Boolean near noMouse;

};

inline Boolean THWMouse::present()
{
    return Boolean( buttonCount != 0 );
}

inline void THWMouse::inhibit()
{
    noMouse = True;
}

class TMouse : public THWMouse
{

public:

    TMouse();
    ~TMouse();

    static void show();
    static void hide();
#ifdef __OS2__
    static void hide( const TRect& rect ) { THWMouse::hide( rect ); }
#endif

    static void setRange( ushort, ushort );

    static void getEvent( MouseEventType& );
    static void registerHandler( unsigned, void (far *)() );
    static Boolean present();

    static void suspend() { THWMouse::suspend(); }
    static void resume() { THWMouse::resume(); show(); }

};

inline void TMouse::show()
{
    THWMouse::show();
}

inline void TMouse::hide()
{
    THWMouse::hide();
}

inline void TMouse::setRange( ushort rx, ushort ry )
{
    THWMouse::setRange( rx, ry );
}

inline void TMouse::getEvent( MouseEventType& me )
{
    THWMouse::getEvent( me );
}

inline void TMouse::registerHandler( unsigned mask, void (far *func)() )
{
    THWMouse::registerHandler( mask, func );
}

inline Boolean TMouse::present()
{
    return THWMouse::present();
}

struct CharScanType
{
    uchar charCode;
    uchar scanCode;
};

struct KeyDownEvent
{
    union
        {
        ushort keyCode;
        CharScanType charScan;
        };
};

struct MessageEvent
{
    ushort command;
    union
        {
        void *infoPtr;
        long infoLong;
        ushort infoWord;
        short infoInt;
        uchar infoByte;
        char infoChar;
        };
};

struct TEvent
{
    ushort what;
    union
    {
        MouseEventType mouse;
        KeyDownEvent keyDown;
        MessageEvent message;
    };
    void getMouseEvent();
    void getKeyEvent();
};

#endif  // Uses_TEvent

#if defined( Uses_TEventQueue ) && !defined( __TEventQueue )
#define __TEventQueue

class TEventQueue
{
public:
    TEventQueue();
    ~TEventQueue();

    static void getMouseEvent( TEvent& );
    static void suspend();
    static void resume();

    friend class TView;
    friend void genRefs();
    friend unsigned long getTicks(void);
    friend class TProgram;
    static ushort near doubleDelay;
    static Boolean near mouseReverse;

private:

    static TMouse near mouse;
    static void getMouseState( TEvent& );
#ifdef __DOS32__
    friend void _loadds _far mouseInt(int flag,int buttons,int x,int y);
#   pragma aux mouseInt parm [EAX] [EBX] [ECX] [EDX];
#else
    static void huge mouseInt();
#endif

    static void setLast( TEvent& );

    static MouseEventType near lastMouse;
    static MouseEventType near curMouse;

    static MouseEventType near downMouse;
    static ushort near downTicks;

    static ushort far * near Ticks;
    static TEvent near eventQueue[ eventQSize ];
    static TEvent * near eventQHead;
    static TEvent * near eventQTail;
    static Boolean near mouseIntFlag;
    static ushort near eventCount;

    static Boolean near mouseEvents;

    static ushort near repeatDelay;
    static ushort near autoTicks;
    static ushort near autoDelay;

#ifdef __OS2__
public:
    static void mouseThread(void* arg);
    static void keyboardThread( void * arg );
    static TEvent keyboardEvent;
    static unsigned char shiftState;
#endif

};

inline void TEvent::getMouseEvent()
{
    TEventQueue::getMouseEvent( *this );
}

#endif  // Uses_TEventQueue

#if defined( Uses_TScreen ) && !defined( __TScreen )
#define __TScreen

#ifdef PROTECT

extern ushort monoSeg;
extern ushort colrSeg;
extern ushort biosSeg;

#endif

class TDisplay
{

public:

    friend class TView;

    enum videoModes
        {
        smBW80      = 0x0002,
        smCO80      = 0x0003,
        smMono      = 0x0007,
        smFont8x8   = 0x0100
        };

    static void clearScreen( uchar, uchar );

    static void setCursorType( ushort );
    static ushort getCursorType();

    static ushort getRows();
    static ushort getCols();

    static void setCrtMode( ushort );
    static ushort getCrtMode();

protected:

    TDisplay() { updateIntlChars(); };
    TDisplay( const TDisplay& ) { updateIntlChars(); };
    ~TDisplay() {};

private:

    static void videoInt();
    static void updateIntlChars();

    static ushort far * near equipment;
    static uchar far * near crtInfo;
    static uchar far * near crtRows;

};

class TScreen : public TDisplay
{

public:

    TScreen();
    ~TScreen();

    static void setVideoMode( ushort mode );
    static void clearScreen();

    static ushort near startupMode;
    static ushort near startupCursor;
    static ushort near screenMode;
    static uchar near screenWidth;
    static uchar near screenHeight;
    static Boolean near hiResScreen;
    static Boolean near checkSnow;
    static uchar far * near screenBuffer;
    static ushort near cursorLines;

    static void setCrtData();
    static ushort fixCrtMode( ushort );

    static void suspend();
    static void resume();

};

#endif  // Uses_TScreen

#if defined( Uses_TSystemError ) && !defined( __TSystemError )
#define __TSystemError

class far TDrawBuffer;

class TSystemError
{

public:

    TSystemError();
    ~TSystemError();

    static Boolean near ctrlBreakHit;

    static void cdecl suspend();
    static void cdecl resume();
    static short ( far * cdecl sysErrorFunc )( short, uchar );

private:

    static ushort near sysColorAttr;
    static ushort near sysMonoAttr;
    static Boolean near saveCtrlBreak;
    static Boolean near sysErrActive;

    static void cdecl swapStatusLine( TDrawBuffer far & );
    static ushort selectKey();
    static short cdecl sysErr( short, uchar );

    static Boolean near inIDE;

    static const char *const near errorString[14];
    static const char * near sRetryOrCancel;

    friend class Int11trap;

};

#ifdef __DOS16__

class Int11trap
{

public:

    Int11trap();
    ~Int11trap();

private:

    static void interrupt handler(...);
    static void interrupt (far * near oldHandler)(...);

};

#endif	// __DOS16__

#endif  // Uses_TSystemError

#if defined( Uses_TOS2Handles ) && !defined( __TOS2Handles)
#define __TOS2Handles

#ifdef __OS2__
#define INCL_NOPMAPI
#define INCL_KBD
#define INCL_MOU
#define INCL_VIO
#define INCL_DOSPROCESS
#define INCL_DOSMEMMGR
#define INCL_DOSSEMAPHORES
#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#define INCL_DOSMISC
#include <os2.h>

#pragma pack()
#pragma options -a.

// These must not cross a 64K boundary
struct TiledStruct {
  HMOU mouseHandle;
  MOUEVENTINFO mouseInfo;
  NOPTRRECT ptrArea;
  PTRLOC ptrLoc;
  VIOCURSORINFO cursorInfo;
  VIOMODEINFO modeInfo;
  KBDKEYINFO keyboardInfo;
  KBDINFO    keyboardShiftInfo;
};

class TOS2Handles {
public:
  TOS2Handles();
  ~TOS2Handles();
  static void resume();
  static void suspend();
  static void deadEnd();
  static TiledStruct *tiled; // tiled will be allocated with DosAllocMem
  static volatile TID mouseThreadID;
  static volatile TID keyboardThreadID;
  static volatile SEMRECORD evCombined[2];
  static volatile HEV &hevMouse1;
  static volatile HEV &hevKeyboard1;
  static volatile HEV hevKeyboard2;
  static volatile HEV hevMouse2;
  static volatile HMTX hmtxMouse1;
  static volatile HMUX hmuxMaster;
  static volatile int shutDownFlag;
};

#endif // __OS2__
#endif // Uses_TOS2Handles
  
#include <tvvo2.h>
