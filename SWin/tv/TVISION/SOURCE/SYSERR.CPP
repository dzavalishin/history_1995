/*------------------------------------------------------------*/
/* filename -       syserr.cpp                                */
/*                                                            */
/* function(s)                                                */
/*          TSystemError member functions                     */
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

#if defined(__BORLANDC__) && defined(__MSDOS__)
#pragma inline
#endif

#define Uses_TDrawBuffer
#define Uses_TSystemError
#define Uses_TScreen
#include <tv.h>

#include <Dos.h>
#include <conio.h>
#include <stdio.h>
#include <String.h>
#include <Math.h>

Boolean near TSystemError::ctrlBreakHit = False;
short ( far * cdecl near TSystemError::sysErrorFunc )( short, uchar )
#ifdef __MSDOS__
#ifdef __BORLANDC__
	= (short (far * cdecl near)(short,uchar))TSystemError::sysErr;
#else
	= TSystemError::sysErr;
#endif
#else
	= NULL;		// no error hanlding if not MS DOS
#endif

ushort near TSystemError::sysColorAttr = 0x4E4F;
ushort near TSystemError::sysMonoAttr = 0x7070;
Boolean near TSystemError::saveCtrlBreak = False;
Boolean near TSystemError::sysErrActive = False;
Boolean near TSystemError::inIDE = False;

#ifdef __DOS16__

const SecretWord = 1495;
const productID  =  136;

static void near checkIDE()
{
    Int11trap trap;
    _BX = SecretWord;
    _AX = SecretWord;
    geninterrupt( 0x12 );
}
#endif // __DOS16__

TSystemError::TSystemError()
{
#ifdef __DOS16__
    inIDE = False;
    checkIDE();
#endif // __DOS16__
    resume();
}

TSystemError::~TSystemError()
{
    suspend();
}

#ifdef __DOS16__
static int getakey()
{
    asm {
        MOV AH,1;
        INT 16h;
        JNZ keyWaiting;
        };
    return 0;

keyWaiting:

    asm {
        MOV AH,0;
        INT 16h;
        };
    return _AX;
}

#else // __DOS16__
  
#ifdef __DOS32__

#include <bios.h>

static int getakey() {
  if ( _bios_keybrd(_KEYBRD_READY) == 0 ) return 0;
  return _bios_keybrd(_KEYBRD_READ);
}

typedef void __interrupt __far (*intvec_p)();
static intvec_p old = NULL;

void set_es(void);
#pragma aux set_es = "push ds" "pop es";

static void _interrupt int24handler( union INTPACK r ) {
  _enable();
  set_es();
  uchar code = r.w.di;
  ushort drive = r.w.ax;
  ushort devseg = r.w.bp;
  ushort devoff = r.w.si;
  if ( code == 9 ) drive = 0xFF;	// Printer out of paper
  else if ( drive & 0x8000 ) {
    code = 13;				// Bad memory image of FAT
    char *devhd = (char *)MK_FP(devseg,devoff);
    if ( devhd[5] & 0x80 )		// Block device?
      code++;				// Device access error
  }
  if ( TSystemError::sysErrorFunc(code,drive) == 0 ) r.w.ax = 1; // retry
  else r.w.ax = 3;			// fail
//  REGS rr;
//  rr.w.ax = 0x5400;			// Dummy function call to get
//  int386(0x21,&rr,&rr); 		// DOS into a stable state
}

void cdecl TSystemError::resume() {
   REGS r;
   r.w.ax = 0x3300;
   int386(0x21,&r,&r);
   saveCtrlBreak = Boolean(r.h.dl);
   r.w.ax = 0x3301;
   r.h.dl = 0;
   int386(0x21,&r,&r);
   if ( old == NULL ) {
     old = _dos_getvect(0x24);
     _dos_setvect(0x24,intvec_p(int24handler));
   }
}

void cdecl TSystemError::suspend() {
   REGS r;
   if ( old != NULL ) {
     _dos_setvect(0x24,old);
     old = NULL;
   }
   r.w.ax = 0x3301;
   r.h.dl = saveCtrlBreak;
   int386(0x21,&r,&r);
}

#else	// __DOS32__

void cdecl TSystemError::resume()  {}
void cdecl TSystemError::suspend() {}

#endif	// __DOS32__

#endif	// else __DOS16__

#ifdef __MSDOS__

ushort TSystemError::selectKey()
{
    ushort crsrType = TScreen::getCursorType();

    TScreen::setCursorType( 0x2000 );

    while( getakey() )
        ;
    
    int ch = getakey() & 0xFF;
    while( ch != 13 && ch != 27 )
        ch = getakey() & 0xFF;

    TScreen::setCursorType( crsrType );
    return ch == 27;
}

short cdecl TSystemError::sysErr( short errorCode, uchar drive )
{
    ushort c = ( (TScreen::screenMode & 0x00fF) != TDisplay::smMono  ) ?
                                        sysColorAttr : sysMonoAttr;
#ifdef __DOS32__
    char s[ 63 ];
    char *p;
    TDrawBuffer b;

    if (errorCode < sizeof(errorString)/sizeof(errorString[0]) ) {
      strcpy( s, errorString[ errorCode ] );
      p = strchr(s,'%');
    } else {
      strcpy(s,"Unknown error on drive ");
      p = strchr(s,'\0');
    }
    if ( p != NULL ) {
      *p++ = drive + 'a';
    *p++ = '\0';
    }
#else
    char s[ 63 ];
    TDrawBuffer b;

    if (errorCode < sizeof(errorString)/sizeof(errorString[0]) )
      sprintf( s, errorString[ errorCode ], drive + 'a' );
    else
      sprintf( s, "Unknown error %d on drive %c", errorCode, drive + 'a' );
#endif
    b.moveChar( 0, ' ', c, TScreen::screenWidth );
    b.moveCStr( 1, s, c);
    b.moveCStr( TScreen::screenWidth-1-cstrlen(sRetryOrCancel), sRetryOrCancel, c);
    swapStatusLine(b);
    int res = selectKey();
    swapStatusLine(b);
    return res;
}

#endif	// __MSDOS__

#ifdef __DOS16__

Int11trap::Int11trap()
{
    oldHandler = getvect( 0x11 );
    setvect( 0x11, Int11trap::handler );
}

Int11trap::~Int11trap()
{
    setvect( 0x11, oldHandler );
}

void interrupt (far * near Int11trap::oldHandler)(...) = 0;

#pragma warn -eas

void interrupt Int11trap::handler(...)
{
    if( _AX == SecretWord && _BX == productID )
        TSystemError::inIDE++;
    oldHandler();
}

#pragma warn .eas

#endif // __DOS16__
