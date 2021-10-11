// edits.cpp defines functions previously found in edits.asm

#define Uses_TEditor
#include <tv.h>

char cdecl TEditor::bufChar( ushort p )
{
    return buffer[p + ((p >= curPtr) ? gapLen : 0)];
}

ushort cdecl TEditor::bufPtr( ushort p )
{
    return (p >= curPtr) ? p + gapLen : p;
}

#if 0
void cdecl TEditor::formatLine( void *DrawBuf, ushort LinePtr,
                          int Width, ushort Color )
{
    ushort p = LinePtr;
    while ((p < curPtr) && (buffer[p] != 0x0D) && (p - LinePtr <= Width))
    {
        ((ushort *) DrawBuf) [p - LinePtr] = buffer[p] + ((Color & 0xFF) << 8);
        p++;
    }

    if (p >= curPtr)
    {
    p += gapLen;

//    while ((p < bufLen) && (buffer[p] != 0x0D) &&
    while ((p < bufSize) && (buffer[p] != 0x0D) &&
           (p - gapLen - LinePtr <= Width))
    {
        ((ushort *) DrawBuf) [p - gapLen - LinePtr] =
                                        buffer[p] + ((Color & 0xFF) << 8);
        p++;
    }
    } else 
        p += gapLen;

    while (p - gapLen - LinePtr <= Width)
    {
        ((ushort *) DrawBuf) [p - gapLen - LinePtr] =
                                        ' ' + ((Color & 0xFF) << 8);
        p++;
    }
}
#endif

void cdecl TEditor::formatLine( void *DrawBuf, ushort LinePtr,
                          int Width, ushort Color )
{
  ushort i = 0;       // index in the DrawBuf
  ushort p = LinePtr; // index in the Buffer
  ushort curColor;
  while ((p < curPtr) && (buffer[p] != 0x0D) && (i <= Width)) {
    curColor = (p>=selStart && p<selEnd) ? (Color & 0xFF00) : ((Color & 0xFF) << 8);
    if (buffer[p] == 0x9) {
      do {
        ((ushort *) DrawBuf) [i] = ' ' + curColor;
        i++;
      } while ((i % 8) && (i <= Width));
      p++;
    } else {
      ((ushort *) DrawBuf) [i] = buffer[p] + curColor;
      p++; i++;
    }
  }

  if (p >= curPtr)
  {
    p += gapLen;
  
    while ((p < bufSize) && (buffer[p] != 0x0D) && (i <= Width))
    {
      curColor = (p>=selStart && p<selEnd) ? (Color & 0xFF00) : ((Color & 0xFF) << 8);
      if (buffer[p] == 0x9) {
        do {
          ((ushort *) DrawBuf) [i] = ' ' + curColor;
          i++;
        } while ((i % 8) && (i <= Width));
        p++;
      } else {
        ((ushort *) DrawBuf) [i] = buffer[p] + curColor;
        p++; i++;
      };
    };
  } else
    p += gapLen;

  while (i <= Width)
  {
    curColor = (p>=selStart && p<selEnd) ? (Color & 0xFF00) : ((Color & 0xFF) << 8);
    ((ushort *) DrawBuf) [i] = ' ' + curColor;
    p++; i++;
  }
}

ushort cdecl TEditor::lineEnd( ushort p )
{
/*
    while (p < curPtr)
        if (buffer[p] == 0x0D)
            return p;
        else
            p++;

    if (curPtr == bufLen)
        return curPtr;

    while (p + gapLen < bufLen)
        if (buffer[p + gapLen] == 0x0D)
            return p;
        else
            p++;

    return p;
*/
    if (p < curPtr)
    {
        while (p < curPtr)
            if (buffer[p] == 0x0D)
                return p;
            else
                p++;

        if (curPtr == bufLen)
            return bufLen;
 
 
    }
    else
    {
        if (p == bufLen)
            return bufLen;
    }

    while (p + gapLen < bufSize)
        if (buffer[p + gapLen] == 0x0D)
            return p;
        else
            p++;

    return p;
 
}

ushort cdecl TEditor::lineStart( ushort p )
{
/*
    while (p - gapLen > curPtr)
        if (buffer[--p + gapLen] == 0x0D)
            return p + 2;

    if (curPtr == 0)
        return 0;

    while (p > 0)
        if (buffer[--p] == 0x0D)
            return p + 2;

    return 0;
*/
    while (p > curPtr)
        if (buffer[--p + gapLen] == 0x0D)
            if (p+1 == bufLen || buffer[p+gapLen+1] != 0x0A)
                return p + 1;
            else
                return p + 2;

    if (curPtr == 0)
        return 0;

    while (p > 0)
        if (buffer[--p] == 0x0D)
            if (p+1 == bufLen || p+1 == curPtr || buffer[p+1] != 0x0A)
                return p + 1;
            else
                return p + 2;

    return 0;
}

ushort cdecl TEditor::nextChar( ushort p )
{
    if (p == bufLen)   return p;
    if (++p == bufLen) return p;

    int t = (p >= curPtr) ? p + gapLen : p;

    return ((buffer [t-1] == 0x0D) && (buffer [t] == 0x0A)) ? p + 1 : p;
}

ushort cdecl TEditor::prevChar( ushort p )
{
    if (p == 0)   return p;
    if (--p == 0) return p;

    int t = (p >= curPtr) ? p + gapLen : p;

    return ((buffer [t-1] == 0x0D) && (buffer [t] == 0x0A)) ? p - 1 : p;
}



