
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
        




