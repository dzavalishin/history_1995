/*------------------------------------------------------------*/
/* filename -       tevent.cpp                                */
/*                                                            */
/* function(s)                                                */
/*                  TEvent member functions                   */
/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/*                                                            */
/*    Turbo Vision -  Version 1.0                             */
/*                                                            */
/*                                                            */
/*    Copyright (c) 1991 by Borland International             */
/*    All Rights Reserved.                                    */
/*                                                            */
/*------------------------------------------------------------*/

#define Uses_TEventQueue
#define Uses_TEvent
#define Uses_TScreen
#define Uses_TKeys
#define Uses_TOS2Handles
#include <tv.h>

#ifdef __OS2__
#include <stdio.h>
#include <assert.h>
#endif

TEvent near TEventQueue::eventQueue[ eventQSize ] = { {0} };
TEvent *near TEventQueue::eventQHead = TEventQueue::eventQueue;
TEvent *near TEventQueue::eventQTail = TEventQueue::eventQueue;
Boolean near TEventQueue::mouseIntFlag = False;

#ifdef __OS2__
TEvent TEventQueue::keyboardEvent;
unsigned char TEventQueue::shiftState;
#endif

#ifdef __MSDOS__

#include <dos.h>
#include <bios.h>

ushort *TEventQueue::Ticks = (ushort *)MK_FP( 0x000, 0x046c );
unsigned long getTicks(void) { return *(TEventQueue::Ticks); }

#endif	// __MSDOS__

ushort near TEventQueue::eventCount = 0;
Boolean near TEventQueue::mouseEvents = False;
Boolean near TEventQueue::mouseReverse = False;
ushort near TEventQueue::doubleDelay = 8;
ushort near TEventQueue::repeatDelay = 8;
ushort near TEventQueue::autoTicks = 0;
ushort near TEventQueue::autoDelay = 0;

MouseEventType near TEventQueue::lastMouse;
MouseEventType near TEventQueue::curMouse;
MouseEventType near TEventQueue::downMouse;
ushort near TEventQueue::downTicks = 0;

TEventQueue::TEventQueue()
{
#ifdef __MSDOS__
    resume();
#endif
}

void TEventQueue::resume()
{
    if( mouse.present() == False )
        mouse.resume();
    if( mouse.present() == False )
        return;
    mouse.getEvent( curMouse );
    lastMouse = curMouse;
    mouse.registerHandler( 0xFFFF, (void (*)(void))mouseInt );
    mouseEvents = True;
    mouse.show();
    TMouse::setRange( TScreen::screenWidth-1, TScreen::screenHeight-1 );
}

void TEventQueue::suspend()
{
    mouse.suspend();
}

TEventQueue::~TEventQueue()
{
#ifdef __MSDOS__
    suspend();
#endif
}

void TEventQueue::getMouseEvent( TEvent& ev )
{
    if( mouseEvents == True )
        {

        getMouseState( ev );

        if( ev.mouse.buttons == 0 && lastMouse.buttons != 0 )
            {
            ev.what = evMouseUp;
            lastMouse = ev.mouse;
            return;
            }

        if( ev.mouse.buttons != 0 && lastMouse.buttons == 0 )
            {
            if( ev.mouse.buttons == downMouse.buttons &&
                ev.mouse.where == downMouse.where &&
                ev.what - downTicks <= doubleDelay )
                    ev.mouse.doubleClick = True;

            downMouse = ev.mouse;
            autoTicks = downTicks = ev.what;
            autoDelay = repeatDelay;
            ev.what = evMouseDown;
            lastMouse = ev.mouse;
            return;
            }

        ev.mouse.buttons = lastMouse.buttons;

        if( ev.mouse.where != lastMouse.where )
            {
            ev.what = evMouseMove;
            lastMouse = ev.mouse;
            return;
            }

        if( ev.mouse.buttons != 0 && ev.what - autoTicks > autoDelay )
            {
            autoTicks = ev.what;
            autoDelay = 1;
            ev.what = evMouseAuto;
            lastMouse = ev.mouse;
            return;
            }
        }

    ev.what = evNothing;
}

void TEventQueue::getMouseState( TEvent & ev )
{
#ifdef __OS2__
    assert(! DosRequestMutexSem(TOS2Handles::hmtxMouse1,SEM_INDEFINITE_WAIT) );
#else
    _disable();
#endif
    if( eventCount == 0 )
        {
        ev.what = getTicks();
        ev.mouse = curMouse;
        }
    else
        {
        ev = *eventQHead;
        if( ++eventQHead >= eventQueue + eventQSize )
            eventQHead = eventQueue;
        eventCount--;
        }
#ifdef __OS2__
    if (eventCount==0) {
      ULONG dummy;
      assert(! DosResetEventSem( TOS2Handles::hevMouse1, &dummy ) );
    };
    assert(! DosReleaseMutexSem(TOS2Handles::hmtxMouse1) );
#else
    _enable();
#endif
    if( mouseReverse != False && ev.mouse.buttons != 0 && ev.mouse.buttons != 3 )
        ev.mouse.buttons ^= 3;
}

#ifdef __DOS16__
#pragma saveregs

void huge TEventQueue::mouseInt()
{
    unsigned flag = _AX;
    MouseEventType tempMouse;

    tempMouse.buttons = _BL;
    tempMouse.doubleClick = False;
    tempMouse.where.x = _CX >> 3;
    tempMouse.where.y = _DX >> 3;

    if( (flag & 0x1e) != 0 && eventCount < eventQSize )
        {
        eventQTail->what = *Ticks;
        eventQTail->mouse = curMouse;
        if( ++eventQTail >= eventQueue + eventQSize )
            eventQTail = eventQueue;
        eventCount++;
        }

    curMouse = tempMouse;
    mouseIntFlag = True;
}

void TEvent::getKeyEvent()
{
    asm {
        MOV AH,1;
        INT 16h;
        JNZ keyWaiting;
        };
    what = evNothing;
    return;

keyWaiting:
    what = evKeyDown;
    asm {
        MOV AH,0;
        INT 16h;
        };
    keyDown.keyCode = _AX;
    return;
}

#endif // __DOS16__
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
const unsigned char scSpaceKey          = 0x39;
const unsigned char scInsKey            = 0x52;
const unsigned char scDelKey            = 0x53;
const unsigned char scGreyEnterKey      = 0xE0;

#ifdef __DOS32__
void _loadds _far mouseInt(int flag,int buttons,int x,int y)
{ 
    MouseEventType tempMouse;

    tempMouse.buttons = buttons;
    tempMouse.doubleClick = False;
    tempMouse.where.x = x >> 3;
    tempMouse.where.y = y >> 3;

    if( (flag & 0x1e) != 0 && TEventQueue::eventCount < eventQSize )
        {
        TEventQueue::eventQTail->what = getTicks();
        TEventQueue::eventQTail->mouse.buttons     = TEventQueue::curMouse.buttons    ;
        TEventQueue::eventQTail->mouse.doubleClick = TEventQueue::curMouse.doubleClick;
        TEventQueue::eventQTail->mouse.where.x     = TEventQueue::curMouse.where.x    ;
        TEventQueue::eventQTail->mouse.where.y     = TEventQueue::curMouse.where.y    ;
        if( ++TEventQueue::eventQTail >= TEventQueue::eventQueue + eventQSize )
            TEventQueue::eventQTail = TEventQueue::eventQueue;
        TEventQueue::eventCount++;
        }

    TEventQueue::curMouse.buttons     = tempMouse.buttons    ;
    TEventQueue::curMouse.doubleClick = tempMouse.doubleClick;
    TEventQueue::curMouse.where.x     = tempMouse.where.x    ;
    TEventQueue::curMouse.where.y     = tempMouse.where.y    ;
    TEventQueue::mouseIntFlag = True;
}

void TEvent::getKeyEvent() {
  if ( _bios_keybrd(_KEYBRD_READY) == 0 ) {
    what = evNothing;
  } else {
    what = evKeyDown;
    keyDown.keyCode = _bios_keybrd(_KEYBRD_READ);
    switch ( keyDown.charScan.scanCode ) {
      case scSpaceKey:
        if ( getShiftState() & kbAltShift ) keyDown.keyCode = kbAltSpace;
	break;
      case scInsKey:
	{
	  unsigned char state = getShiftState();
          if ( state & kbCtrlShift ) keyDown.keyCode = kbCtrlIns;
          else if ( state & (kbLeftShift|kbRightShift) ) keyDown.keyCode = kbShiftIns;
	}
	break;
      case scDelKey:
	{
	  unsigned char state = getShiftState();
          if ( state & kbCtrlShift ) keyDown.keyCode = kbCtrlDel;
          else if ( state & (kbLeftShift|kbRightShift) ) keyDown.keyCode = kbShiftDel;
	}
	break;
    }
  }
}

#endif // __DOS32__
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#ifdef __OS2__

void huge TEventQueue::mouseInt() { /* no mouse... :( */ }

// These variables are used to reduce the CPU time used by the
// two threads if they don't receive any input from the user for some time.
volatile int mousePollingDelay = 0;
volatile int keyboardPollingDelay = 0;

#define jsSuspendThread if (TOS2Handles::shutDownFlag) break;

void TEventQueue::mouseThread(void* arg) {
  arg = arg;
  assert(! DosWaitEventSem(TOS2Handles::hevMouse2, SEM_INDEFINITE_WAIT) ); 
  MouseEventType tempMouse;
  MOUEVENTINFO *info = &TOS2Handles::tiled->mouseInfo;

  while (1) {
    do {
      jsSuspendThread
      USHORT type = 0; // non-blocking read
      MouReadEventQue(info, &type, TOS2Handles::tiled->mouseHandle);
      if (info->time==0) {
        DosSleep(mousePollingDelay);
        if (mousePollingDelay < 500) mousePollingDelay += 5;
      } else {
        mousePollingDelay=0;
      }
    } while (info->time==0);
    tempMouse.buttons = ((info->fs & 06)  != 0) +
               (((info->fs & 030) != 0) << 1)+
               (((info->fs & 0140) != 0) << 2);
    tempMouse.where.x = info->col;
    tempMouse.where.y = info->row;
    tempMouse.doubleClick = False;

    jsSuspendThread
    DosRequestMutexSem(TOS2Handles::hmtxMouse1,SEM_INDEFINITE_WAIT);
    if( (tempMouse.buttons!=curMouse.buttons) && eventCount < eventQSize ) {
      eventQTail->what = info->time/52; //*Ticks;
      eventQTail->mouse = tempMouse; // curMouse;
      if( ++eventQTail >= eventQueue + eventQSize ) eventQTail = eventQueue;
      eventCount++;
    }

    curMouse = tempMouse;

    APIRET rc = DosPostEventSem(TOS2Handles::hevMouse1);
    assert( rc==0 || rc==ERROR_ALREADY_POSTED );
    assert(! DosReleaseMutexSem(TOS2Handles::hmtxMouse1));
  }
  TOS2Handles::deadEnd();
}


void TEventQueue::keyboardThread( void * arg ) {
  arg = arg;
  KBDKEYINFO *info = &TOS2Handles::tiled->keyboardInfo;
  {
    KBDINFO ki;
    ki.cb = sizeof(ki);
    assert(!KbdGetStatus(&ki,0));
    ki.fsMask &= ~KEYBOARD_ASCII_MODE;
    ki.fsMask |= KEYBOARD_BINARY_MODE;
    assert(!KbdSetStatus(&ki,0));
  }

  while (1) {
    jsSuspendThread
    USHORT errCode = KbdCharIn( info, IO_NOWAIT, 0 );
    jsSuspendThread
    if ( errCode || (info->fbStatus & 0xC0)!=0x40 || info->fbStatus & 1) { // no char
      keyboardEvent.what = evNothing;
      DosSleep(keyboardPollingDelay);
      if (keyboardPollingDelay < 500) keyboardPollingDelay += 5;
    } else {
    keyboardEvent.what = evKeyDown;

    if ((info->fbStatus & 2) && (info->chChar==0xE0)) info->chChar=0; // OS/2 cursor keys.
    keyboardEvent.keyDown.charScan.charCode=info->chChar;
    keyboardEvent.keyDown.charScan.scanCode=info->chScan;
    shiftState = info->fsState & 0xFF;

    jsSuspendThread

    assert(! DosPostEventSem(TOS2Handles::hevKeyboard1) );

    jsSuspendThread
    assert(! DosWaitEventSem(TOS2Handles::hevKeyboard2, SEM_INDEFINITE_WAIT) );
    keyboardEvent.what = evNothing;
    ULONG dummy;
    jsSuspendThread
    assert(! DosResetEventSem(TOS2Handles::hevKeyboard2, &dummy) );
    keyboardPollingDelay=0;
    }
  }
  {
    KBDINFO ki;
    ki.cb = sizeof(ki);
    assert(!KbdGetStatus(&ki,0));
    ki.fsMask |= KEYBOARD_ASCII_MODE;
    ki.fsMask &= ~KEYBOARD_BINARY_MODE;
    assert(!KbdSetStatus(&ki,0));
  }
  TOS2Handles::deadEnd();
}

// Some Scancode conversion.. in OS/2 ShiftIns/ShiftDel only
// I got understand the CtrlDel/Ins Conversion. Maybe this is
// XT-Keyboard only

static unsigned char translateTable1[] =
  { scSpaceKey,
    scInsKey, scDelKey,
    scInsKey, scDelKey  };

static unsigned char translateTable2[] =
  { kbAltShift,
    kbCtrlShift, kbCtrlShift,
    kbLeftShift+kbRightShift, kbLeftShift+kbRightShift };

static unsigned short translateTable3[]=
  { kbAltSpace,
    kbCtrlIns,  kbCtrlDel,
    kbShiftIns, kbShiftDel };

const unsigned translateTableLength = 5;


void TEvent::getKeyEvent()
{
  unsigned char shiftState = 0;
  ULONG dummy;
  assert(! DosResetEventSem(TOS2Handles::hevKeyboard1, &dummy) );

  *this = TEventQueue::keyboardEvent;
  shiftState = TEventQueue::shiftState;
  assert(! DosPostEventSem(TOS2Handles::hevKeyboard2) );

    for (int i=0; i<translateTableLength; i++) {
      if ((translateTable1[i]==    keyDown.charScan.scanCode) &&
          (translateTable2[i] & shiftState))
      {
            keyDown.keyCode = translateTable3[i];
        break;
      };
    };
    if (     keyDown.charScan.scanCode == scGreyEnterKey )
          keyDown.keyCode = kbEnter;

  return;

}

#endif	// __OS2__
