head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	96.07.09.16.24.28;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.07.09.16.03.58;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.07.09.02.12.58;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.07.07.10.01.14;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.07.06.06.56.10;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@hex2bin << replaced with <<= :)
@
text
@
#include "MpPacket.h"

#include <ctype.h>


const char *MP_Packet_AutoDL_Sequence = "<*_MPipe_*>";



MP_Packet::~MP_Packet() 
    {
    }

MP_Packet::MP_Packet() 
    {
    type_v = MP_PT_None;
    }


void MP_Packet::throw_fail(const char *name) const 
    { 
    throw(Mp_Ex_Fail("MP_Packet","Wrong method call",name)); 
    }


static const char hexchar[17] = "0123456789ABCDEF";

void MP_Packet::bin2hex16( char *cp, int val )
    {
    for( int i = 4; i--; )
        {
        cp[i] = hexchar[val&0x0F];
        val >>= 4;
        }
    }
    
void MP_Packet::bin2hex32( char *cp, int val )
    {
    for( int i = 8; i--; )
        {
        cp[i] = hexchar[val&0x0F];
        val >>= 4;
        }
    }

inline int hc2i( char c )
    {
    return ( c > '9' ) ? c - ('A' - 10) : c - '0';
    }

short MP_Packet::hex2bin16( const char *cp )
    {
    int val = 0;
    for( int i = 4; i--; )
        {
        val <<= 4;
        val += hc2i(*cp++);
        }
    return val;
    }

int MP_Packet::hex2bin32( const char *cp )
    {
    int val = 0;
    for( int i = 8; i--; )
        {
        val <<= 4;
        val += hc2i(*cp++);
        }
    return val;
    }


// Packet-type specific methods, overridden in corresponding subtypes

MP_Off      MP_Packet::eof()       const { throw_fail("eof");       return 0; }
long        MP_Packet::challenge() const { throw_fail("challenge"); return 0; }
MP_Frame    MP_Packet::frame()     const { throw_fail("frame");     return MP_Frame(); }
const char *MP_Packet::data()      const { throw_fail("data");      return 0; }
MP_Options  MP_Packet::options()   const { throw_fail("options");   return MP_Options(0); }
//int         MP_Packet::channel()   const { throw_fail("channel"); }



bool MP_Packet::valid_type( char c )
    {
    switch( c )
        {
        case MP_PT_Wakeup:             case MP_PT_Init:
        case MP_PT_Data:               case MP_PT_Query:
        case MP_PT_EOF:                case MP_PT_Ack:
        case MP_PT_Nak:                case MP_PT_XOFF:
        case MP_PT_XON:                case MP_PT_SStop:
        case MP_PT_RStop1:             case MP_PT_RStop2:
        case MP_PT_HBT:        
            return Yes;

        default:
            return No;
        }
    }

// -------------------------------------------------------------------------
// CRC32
// -------------------------------------------------------------------------

const crc32poly =  0xEDB88320L;  // Generator polynomial number
const crc32init =  0xFFFFFFFFL;  // Initial CRC value for calculation
const crc32test =  0xDEBB20E3L;  // Result to test for at receiver

//#define h_crc32test(crc)   (((crc) == CRC32TEST) ? 1 : 0)

class crc32table
    {
    char t[256];
    public:
        crc32table();
        const char *tab() { return t; }
    };

static crc32table  crc32tab;

crc32table::crc32table()
    {
    int   i, j, crc;

    for (i = 0; i <= 255; i++) 
        {
        crc = i;
        for (j = 8; j > 0; j--) 
            {
            if (crc & 1) crc = (crc >> 1) ^ crc32poly;
            else         crc >>= 1;
            }
        t[i] = crc;
        }
    }

inline int crc32upd(const char *crctab, int crc, char c)
    {
    return ((crctab)[((int) (crc) ^ (c)) & 0xff] ^ ((crc) >> 8));
    }

long MP_Packet::crc32( const char *data, int len )
    {
    int crc = crc32init;
    //MP_Dump("counting crc for", data, len );
    while (len--) crc = crc32upd(crc32tab.tab(),crc,*data++);

    //MP_Message("counted crc = 0x%x", ~crc );
    return ~crc;
    }
        




@


1.4
log
@debugging depacketizer
@
text
@d57 1
a57 1
        val << 4;
d68 1
a68 1
        val << 4;
@


1.3
log
@Debugging has begone
@
text
@d66 1
a66 1
    for( int i = 4; i--; )
d108 3
a110 3
static const crc32poly =  0xEDB88320L;  // Generator polynomial number
static const crc32init =  0xFFFFFFFFL;  // Initial CRC value for calculation
static const crc32test =  0xDEBB20E3L;  // Result to test for at receiver
d148 1
d150 2
a151 1
    
@


1.2
log
@*** empty log message ***
@
text
@d4 1
a4 1
#include "ctype.h"
d11 8
a18 2
MP_Packet::~MP_Packet() {}
MP_Packet::MP_Packet() {}
@


1.1
log
@Initial revision
@
text
@d6 7
a12 1
virtual MP_Packet::~MP_Packet() {}
d21 1
a21 1
static const hexchar[16] = "0123456789ABCDEF";
d23 1
a23 1
static void         bin2hex16( char *cp, int val )
d25 1
a25 1
    for( i = 4; i--; )
d28 1
a28 1
        val >>.4;
d32 3
a34 2
static void         bin2hex32( char *cp, int val );
    for( i = 8; i--; )
d37 1
a37 1
        val >>.4;
d39 1
d46 1
a46 1
static short        hex2bin16( const char *cp )
d49 1
a49 1
    for( i = 4; i--; )
d54 1
d57 2
a58 1
static int          hex2bin32( const char *cp )
d60 1
a60 1
    for( i = 4; i--; )
d65 1
d80 67
@
