/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   TEXTVIEW.H                                                            */
/*                                                                         */
/*   Copyright (c) Borland International 1991                              */
/*   All Rights Reserved.                                                  */
/*                                                                         */
/*   defines the classes TTextDevice and TTerminal                         */
/*                                                                         */
/* ------------------------------------------------------------------------*/

#include <tvvo.h>

#if defined( Uses_TTextDevice ) && !defined( __TTextDevice )
#define __TTextDevice

#include <iostream.h>

#include <tvvo.h>

class far TRect;
class far TScrollBar;

class TTextDevice : public TScroller, public streambuf
{

public:

    TTextDevice( const TRect& bounds,
                 TScrollBar *aHScrollBar,
                 TScrollBar *aVScrollBar
               );

    virtual int do_sputn( const char *s, int count ) = 0;
#if defined(__OS2__) && defined(__BORLANDC__)
    virtual int _RTLENTRY overflow(int = EOF); // flush buffer and make more room
#else
    virtual int cdecl overflow( int = EOF );
#endif
#if defined(__WATCOMC__)
    virtual int underflow(void) { return 0; }
#endif

};

#endif  // Uses_TTextDevice

#if defined( Uses_TTerminal ) && !defined( __TTerminal )
#define __TTerminal

class far TRect;
class far TScrollBar;

class TTerminal: public TTextDevice
{

public:

    friend void genRefs();

    TTerminal( const TRect& bounds,
               TScrollBar *aHScrollBar,
               TScrollBar *aVScrollBar,
               ushort aBufSize
             );
    ~TTerminal();

    virtual int do_sputn( const char *s, int count );

    void bufInc( ushort& val );
    Boolean canInsert( ushort amount );
    short calcWidth();
    virtual void draw();
    ushort nextLine( ushort pos );
    ushort cdecl prevLines( ushort pos, ushort lines );
    Boolean queEmpty();

protected:

    unsigned int bufSize;
    char *buffer;
    unsigned int queFront, queBack;
    void bufDec(ushort& val);
};

#endif  // Uses_TTerminal

#if defined( Uses_otstream ) && !defined( __otstream )
#define __otstream

#include <tvvo.h>

class otstream : public ostream
{

public:

    otstream( TTerminal * );

};


#endif

#include <tvvo2.h>
