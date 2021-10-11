// os2handl.cpp
//
// - Functions that are not supplied by icc or emx but are used by TVision.
// - Threads and Semaphores
//
// There are a lot of #ifs in this file. It took a long time to get the thread stuff
// working, I kept all the unsuccessful attempts.
//
// Copyright 1993 by J”rn Sierwald

#define Uses_TEventQueue
#define Uses_TScreen
#define Uses_TOS2Handles
#include <tv.h>
#include <tvdir.h>

#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <process.h>
#include <string.h>
#include <dos.h>

#ifdef __OS2__

//HKBD TOS2Handles::keyboardHandle;     // Keyboardhandle is not neccessary.
TiledStruct *
  TOS2Handles::tiled;                   // tiled will be allocated with DosAllocMem. The TiledStruct will
                                        // not cross a 64K boundary, therefore the members can be used by
                                        // 16Bit functions.
volatile TID  
  TOS2Handles::mouseThreadID;           //
volatile TID  
  TOS2Handles::keyboardThreadID;        //
volatile SEMRECORD  
  TOS2Handles::evCombined[2];           // Two Event Semaphores, one for the Mouse and one for Keyboard.
                                        // The main thread will do a MuxWait for both.
volatile HEV &
  TOS2Handles::hevMouse1   = (HEV&) TOS2Handles::evCombined[0].hsemCur;
volatile HEV &
  TOS2Handles::hevKeyboard1= (HEV&) TOS2Handles::evCombined[1].hsemCur;
                                        // Just an alias.
volatile HEV  
  TOS2Handles::hevMouse2;               // _beginthread can't create a suspended thread, so
                                        // we keep it waiting with this one
volatile HEV  
  TOS2Handles::hevKeyboard2;            // Signals that the keypress is received.
volatile HMTX 
  TOS2Handles::hmtxMouse1;              // Access to the internal mouse queue
volatile HMUX 
  TOS2Handles::hmuxMaster;              // MuxWait for evCombined.
volatile int  
  TOS2Handles::shutDownFlag=0;

void mouseThread( void * arg ) { TEventQueue::mouseThread(arg); };
void keyboardThread( void * arg ) { TEventQueue::keyboardThread(arg); };
                                        // ICC doesn't want to call member functions as threads
void TOS2Handles::deadEnd() {
#if 0
  // this function captures a thread as long the shutDownFlag is set.
  // this function is reentrant.
  DosSetPriority(PRTYS_THREAD,PRTYC_IDLETIME,0,0);
  while (shutDownFlag);
  DosSetPriority(PRTYS_THREAD,PRTYC_REGULAR,0,0);
#endif
#if 1
  // Kill The current thread
//  cerr << "Thread " 
#ifdef __TV4ICC__
//       << *_threadid
#endif
//       << " is ending.\n";
#endif
  _endthread();
};

TOS2Handles::TOS2Handles() {
};

TOS2Handles::~TOS2Handles() {
};

void TOS2Handles::resume() {
#if 1
//  cerr << "TOS2Handles::resume\n";
#ifdef __TV4EMX__
  if (_osmode == OS2_MODE) {
#endif
  shutDownFlag=0;
  assert(! DosAllocMem((void**)&tiled,sizeof(TiledStruct),fALLOC | OBJ_TILE)    );
  tiled->modeInfo.cb = (unsigned short) sizeof(VIOMODEINFO);
  assert(! MouOpen ((PSZ) 0, &tiled->mouseHandle)                               );

  assert(! DosCreateEventSem( NULL, (PHEV) &hevMouse1, 0, 0 )      );
  assert(! DosCreateEventSem( NULL, (PHEV) &hevMouse2, 0, 0 )      );
  assert(! DosCreateEventSem( NULL, (PHEV) &hevKeyboard1, 0, 0 )   );
  assert(! DosCreateEventSem( NULL, (PHEV) &hevKeyboard2, 0, 0 )   );
  assert(! DosCreateMutexSem( NULL, (PHMTX) &hmtxMouse1, 0, 0 )     );
//  cerr << hevMouse1 << " " << evCombined[0].hsemCur << endl;

  APIRET rc;
  evCombined[0].ulUser=0;
  evCombined[1].ulUser=1;
  rc=(DosCreateMuxWaitSem( NULL, (PHMUX) &hmuxMaster, 2, (PSEMRECORD) evCombined, DCMW_WAIT_ANY ) );
//  cerr << rc << endl;
  assert(!rc);


#ifdef __BORLANDC__
  mouseThreadID = _beginthread(mouseThread,16384,NULL);
#else
//  cerr << "_beginthread mouse...\n";
  mouseThreadID = _beginthread(mouseThread,NULL,16384,NULL);
//  cerr << "_beginthread - ok\n";
#endif
//  DosSuspendThread(mouseThreadID);
  DosPostEventSem(TOS2Handles::hevMouse2);
#ifdef __BORLANDC__
  keyboardThreadID = _beginthread(keyboardThread,16384,NULL);
#else
//  cerr << "_beginthread keyboard...\n";
  keyboardThreadID = _beginthread(keyboardThread,NULL,16384,NULL);
//  cerr << "_beginthread - ok\n";
#endif

//  DosSuspendThread(keyboardThreadID);
//  assert(! DosResumeThread( keyboardThreadID ) );
//  KbdOpen( &keyboardHandle ); no keyboard handle is used

#ifdef __TV4EMX__
  };
#endif
#endif
};

void TOS2Handles::suspend() {
//  cerr << "TOS2Handles::suspend1..\n";
//  DosSuspendThread( keyboardThreadID ); // this will probably not work,
                                        // because the keyboardThread calls a 16Bit function.
//  APIRET rc;
#ifdef __TV4EMX__
  if (_osmode == OS2_MODE) {
#endif

#if 1
//  DosSleep(3000);
//  cerr << "Set shutdownflag\n";
  shutDownFlag=1;
#if 0
  // stuff the Mouse Queue
  // nice try, doesn't work in fullscreen mode.
  assert(! MouDrawPtr (TOS2Handles::tiled->mouseHandle)  );
  DosSleep(1);
  tiled->ptrLoc.row=0;
  tiled->ptrLoc.col=0;
  MouSetPtrPos(&tiled->ptrLoc,tiled->mouseHandle);
  DosSleep(1);
  tiled->ptrLoc.row=1;
  tiled->ptrLoc.col=1;
  MouSetPtrPos(&tiled->ptrLoc,tiled->mouseHandle);
#endif
  TID localTID=mouseThreadID;
//  cerr << "Now wait\n";
//  DosSleep(1000);
  /*cerr <<*/ 
    DosWaitThread(&localTID,DCWW_WAIT);
//  cerr << " MouseThread has ended\n";
  ULONG count;
  assert(! DosQueryEventSem(TOS2Handles::hevKeyboard2,&count) );
  if ( !count )
    assert(! DosPostEventSem(TOS2Handles::hevKeyboard2) ); // Make sure the thread is running.
  localTID=keyboardThreadID;
  /*cerr << */
    DosWaitThread(&localTID,DCWW_WAIT);
//  cerr << " KeyboardThread has ended\n";
#if 0
  {
    NOPTRRECT *ptrArea = &TOS2Handles::tiled->ptrArea;
    ptrArea->row = ptrArea->col = 0;
    ptrArea->cRow = TScreen::screenHeight-1;
    ptrArea->cCol = TScreen::screenWidth-1;
    APIRET rc = MouRemovePtr (ptrArea, TOS2Handles::tiled->mouseHandle);
    if (rc) {
      cerr << rc << endl;
    };
    assert(!rc);
  };
#endif
#endif

#if 1 
  assert(! DosCloseMutexSem(hmtxMouse1)  );
  assert(! DosCloseEventSem(hevKeyboard2) );
  assert(! DosCloseEventSem(hevKeyboard1) );
  assert(! DosCloseEventSem(hevMouse2) );
  assert(! DosCloseEventSem(hevMouse1) );
  assert(!DosCloseMuxWaitSem(hmuxMaster) );

  MouClose(tiled->mouseHandle); // This doesn't work, the mouseThread uses the handle.
  assert(! DosFreeMem(tiled) ); // Better not, dito
#endif

//  KbdClose( keyboardHandle );
#ifdef __TV4EMX__
  }
#endif

//  cerr << "TOS2Handles::suspend2..\n";
}

#endif	// __OS2__

#ifndef __BORLANDC__

inline char *my_strncpy (char *string1, const char *string2, size_t size)
{
  if ( size != 0 ) {
    strncpy(string1,string2,size);
    string1[size-1] = '\0';
  }
  return string1;
}

/////* This function Copyright (c) 1992-1993 by Eberhard Mattes */
////static int _my_splitpath2 (const char *src, char *drive, char *dir,
////			   char *fname, char *ext)
////{
////  int retVal=0;
////  int i, j;
////
////  i = 0;
////  while (i < MAXDRIVE-2 && src[i] != 0 && src[i] != ':')
////    ++i;
////  if (i > 0 && src[i] == ':')
////    {
////      if (drive != NULL)
////        my_strncpy (drive, src, i+2);
////      src += i+1;
////      retVal |= DRIVE;
////    }
////  else if (drive != NULL)
////    *drive = 0;
////
////  i = 0;
////  for (j = 0; src[j] != 0; ++j)
////    if (src[j] == '/' || src[j] == '\\')
////      i = j + 1;
////  if (dir != NULL) {
////    if (i>0) {
////      my_strncpy (dir, src, min (MAXDIR, i + 1));
////      retVal |= DIRECTORY;
////    }
////    else
////      *dir = 0;
////  }
////  src += i;
////
////  i = 0;
////  for (j = 0; src[j] != 0; ++j)
////    if (src[j] == '.')
////      i = j;
////  if (i == 0)
////    i = j;
////  if (fname != NULL) {
////    my_strncpy (fname, src, min (MAXFILE, i + 1));
////    retVal |= FILENAME;
////  }
////  src += i;
////
////  if (ext != NULL) {
////    my_strncpy (ext, src, MAXEXT);
////    if (strlen(ext)>0)
////      retVal |= EXTENSION;
////  }
////  return retVal;
////}

int fnsplit(const char *__path,
                            char *__drive,
                            char *__dir,
                            char *__name,
                            char *__ext)
{
  _splitpath(__path,__drive,__dir,__name,__ext);
  int code = 0;
  if ( __drive != NULL && __drive[0] != '\0' ) code |= DRIVE;
  if ( __dir   != NULL && __dir  [0] != '\0' ) code |= DIRECTORY;
  if ( __name  != NULL && __name [0] != '\0' ) code |= FILENAME;
  if ( __ext   != NULL && __ext  [0] != '\0' ) code |= EXTENSION;
  return code;
}

/* makepath -- Copyright (c) 1993 by Eberhard Mattes */

static void my_makepath (char *dst, const char *drive, const char *dir,
                const char *fname, const char *ext)
{
  int n;
  char slash;

  n = 0; slash = '/';
  if (drive != NULL && *drive != 0)
    {
      dst[n++] = *drive;
      dst[n++] = ':';
    }
  if (dir != NULL && *dir != 0)
    {
      while (n < MAXPATH - 1 && *dir != 0)
        {
          if (*dir == '\\')
            slash = '\\';
          dst[n++] = *dir++;
        }
      if (dst[n-1] != '\\' && dst[n-1] != '/' && n < MAXPATH - 1)
        dst[n++] = slash;
    }
  if (fname != NULL)
    {
      while (n < MAXPATH - 1 && *fname != 0)
        dst[n++] = *fname++;
    }
  if (ext != NULL && *ext != 0)
    {
      if (*ext != '.' && n < MAXPATH - 1)
        dst[n++] = '.';
      while (n < MAXPATH - 1 && *ext != 0)
        dst[n++] = *ext++;
    }
  dst[n] = 0;
}

void fnmerge(char *__path,
                            const char *__drive,
                            const char *__dir,
                            const char *__name,
                            const char *__ext)
{
  my_makepath(__path,__drive,__dir,__name,__ext);
};
#endif

#ifndef __BORLANDC__
int getdisk(void) 
{
#ifdef __WATCOMC__
  unsigned int mydrive;
  _dos_getdrive(&mydrive);
  return mydrive-1;
#endif
#if defined(__TV4ICC__) || defined(__TV4ICC__)
  unsigned long d,map;
  DosQueryCurrentDisk(&d,&map);
  return (int) d-1;
#endif
}
#endif

// getcurdir should fill buffer with the current directory without
// drive char and without a leading backslash.

#ifndef __BORLANDC__ // ig
int getcurdir(int __drive, char *buffer)
{
#define MAXPATHLEN MAXPATH
  long size=MAXPATHLEN;
  char tmp[MAXPATHLEN+1];
  tmp[0]='\\';
#ifdef __DOS32__
  unsigned int old, dummy;
  _dos_getdrive(&old);
  _dos_setdrive(__drive,&dummy);
  if (getcwd (tmp, size) == 0) { // getcwd returns a leading backslash
    _dos_setdrive(old,&dummy);
    return 0;
  }
  _dos_setdrive(old,&dummy);
  strcpy(buffer,&tmp[3]);
  return 1;
#endif
#if defined(__OS2__)
  size_t len;
  char *p;
  unsigned long l = MAXPATHLEN;
  if (DosQueryCurrentDir(__drive,tmp+1,&l)) return NULL;
  len = strlen (tmp) + 2;
  if (buffer == NULL)
    {
      if (size < len) size = len;
      buffer = (char*) malloc (size);
    }
  if (buffer == NULL)
    {
      errno = ENOMEM;
      return (0);
    }
  for (p = tmp; *p != 0; ++p) // no forward slashes please.
    if (*p == '/') *p = '\\';
  strcpy (buffer, tmp+1);
  return 1;
#endif
}

#endif // !__BORLANDC__ ig

#ifdef __OS2__

unsigned long getTicks() {
// return a value that can be used as a substitute for the DOS Ticker at [0040:006C]
#ifdef __TV4EMX__
static int isReady;
static volatile unsigned long *ticker;
  if (_osmode == OS2_MODE) {
    unsigned long m;
    DosQuerySysInfo( 14, 14, &m, sizeof(m));
    return m/52;
  } else {
    if (isReady) {
      return *ticker;
    } else {
      ticker=(volatile unsigned long*) (((char*)_memaccess(0,0xFFF,0))+0x46C);
      isReady=1;
      return *ticker;
    };
  };
  return 0; // turn warning off
#endif
#if defined(__TV4ICC__) || defined(__WATCOMC__) ||  defined(__BORLANDC__)
    unsigned long m;
    DosQuerySysInfo( 14, 14, &m, sizeof(m));
    return m/52;
#endif
#ifdef __TV4TS__
    return *((unsigned long*) 0x0040006C);
#endif
};

unsigned char getShiftState() {
// returns a value that can be used as a substitute for the shift state at [0040:0017]
#ifdef __TV4EMX__
static int isReady;
static volatile unsigned char *shiftState;
  if (_osmode == OS2_MODE) {
    TOS2Handles::tiled->keyboardShiftInfo.cb = 10;
    KbdGetStatus(&TOS2Handles::tiled->keyboardShiftInfo, 0 );
    return TOS2Handles::tiled->keyboardShiftInfo.fsState & 0xFF;
  } else {
    if (isReady) {
      return *shiftState;
    } else {
      shiftState=(volatile unsigned char*) (((char*)_memaccess(0,0xFFF,0))+0x17C);
      isReady=1;
      return *shiftState;
    };
  };
  return 0; // turn warning off
#endif
#if defined(__TV4ICC__) || defined(__WATCOMC__) ||  defined(__BORLANDC__)
    TOS2Handles::tiled->keyboardShiftInfo.cb = 10;
    KbdGetStatus(&TOS2Handles::tiled->keyboardShiftInfo, 0 );
    return TOS2Handles::tiled->keyboardShiftInfo.fsState & 0xFF;
#endif
#ifdef __TV4TS__
    return *((unsigned char*) 0x00400017);
#endif

}

#endif
