head	1.10;
access;
symbols;
locks;
comment	@ * @;


1.10
date	96.07.10.18.27.24;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.07.10.08.19.31;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.07.09.16.03.58;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.07.09.02.12.58;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.07.07.10.01.14;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.07.06.06.55.29;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.07.06.06.42.51;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.07.06.06.37.07;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.07.06.00.46.53;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.07.02.04.33.28;	author dz;	state Exp;
branches;
next	;


desc
@@


1.10
log
@something goes through, framer/frame classes
are work somehow.
@
text
@
#include "MpPacket.h"

#include <ctype.h>

  /**
   *
   * Todo:
   *
   * - See BUGs below.
   *
   * - There is other way of checking CRC32. Need to implement?
   *
  **/


/*
 
 Packet structure:
 
 -1   Start of packet char (MP_Packet_Start)
 
 0    Packet CRC32, hex (incl. type, len & data, not start char)
 
 8    Packet type char     (MP_P_Type)
      If packet is in binary format, packet type has hi bit set.
 
 9    Packet data length, 2-byte int, hex
 
 13   Packet data, uue or btoa if data packet, other packets
      data is in ascii anyway.
 
 
*/

// NB! Ofsets of fields are from  ___Start of packet char___

const crc_offset = 0;
const typ_offset = 8;
const len_offset = 9;
const dat_offset = 13;

const crc_add_len = dat_offset - typ_offset;

// Receive packet from port
MP_Packet *   MP_Packet::receive( MP_Buffer &in )
    {
    const char *cp;
    char        type;

    while(1)
        {

        // Scan for and delete everything else
        in.scan(MP_Packet_Start); // will throw disconnect, if any

        //MP_Message("MP_Packet::receive got MP_Packet_Start");
        
        // wait for header to arrive
        in.wait_for_data( dat_offset );    // will throw disconnect, if any
        cp = (const char *)in;
        type = cp[typ_offset];
        
        if( !valid_type( type&0x7Fu ) )
            continue; // note we didn't eat anything

        bool binary = (type & 0x80u) ? Yes : No;
        type &= 0x7Fu;
        
        int len = hex2bin16( cp+len_offset );
        int crc = hex2bin32( cp+crc_offset );
        //MP_Message("MP_Packet::receive got header, t %c, len %d, crc 0x%x", type, len, crc );
        //MP_Dump("rcvr header", cp, dat_offset );
        
        // we can't receive such a large packets anyway. Either
        // line noise or proto impl. incompatibility?
        // One more note is that 65K buffer will be enough
        // for all possible packets of this format for sure.
        if( dat_offset+len > in.bufsize() )
            {
            MP_Message("MP_Packet::receive packet length error");
            continue; // note we didn't eat anything
            }
        
        // wait for header+data to arrive
        in.wait_for_data( dat_offset+len ); // will throw disconnect, if any
        //MP_Message("MP_Packet::receive got data");

        //MP_Dump("rcvr cp before crc", cp, len+dat_offset );
        long calc_crc = crc32( cp+typ_offset, len + crc_add_len );
        //MP_Message("MP_Packet::receive crc our %x, their %x", calc_crc, crc);
        
        if( calc_crc != crc )
            {
            MP_Message("MP_Packet::receive packet CRC error");
            continue;
            }

        // Here we got it in general form
        //MP_Message("MP_Packet::receive got correct packet '%c'", type);
        
        // go construct correct subtype
        MP_Packet *pkt;
        switch( type )
            {
            case MP_PT_Wakeup:     pkt = new MP_PK_Wakeup  ( cp+dat_offset, len, type ); break;
            case MP_PT_Init:       pkt = new MP_PK_Init    ( cp+dat_offset, len, type ); break;
            case MP_PT_Data:       pkt = new MP_PK_Data    ( cp+dat_offset, len, type, binary ); break;
            //case MP_PT_MiniData:   return MP_PK_MiniData( cp+dat_offset, len, type ); break;
            case MP_PT_Query:      pkt = new MP_PK_Query   ( cp+dat_offset, len, type ); break;
            case MP_PT_EOF:        pkt = new MP_PK_EOF     ( cp+dat_offset, len, type ); break;
            case MP_PT_Ack:        pkt = new MP_PK_Ack     ( cp+dat_offset, len, type ); break;
            case MP_PT_Nak:        pkt = new MP_PK_Nak     ( cp+dat_offset, len, type ); break;
                
            case MP_PT_XOFF:       pkt = new MP_PK_XOFF    (); break;
            case MP_PT_XON:        pkt = new MP_PK_XON     (); break;
            case MP_PT_SStop:      pkt = new MP_PK_SStop   (); break;
            case MP_PT_RStop1:     pkt = new MP_PK_RStop1  (); break;
            case MP_PT_RStop2:     pkt = new MP_PK_RStop2  (); break;
            case MP_PT_HBT:        pkt = new MP_PK_HBT     (); break;
                
                // Got unknown type of packet? 
                // strange, but pretend we didn't seen it
            default: 
                continue;
            }

        //MP_Dump( "MP_Packet::receive", cp, len );
        // eat data
        in += (dat_offset+len);

        //MP_Message("MP_Packet::receive got '%c'", type );
        return pkt;
        
        }


    }





// Send packet to port
void   MP_Packet::send( MP_Port &port ) const
    {
    // This stuff should be redone with some dynamic allocator
    char sb[MP_Packet_Work_Buf];
    
    // Grow stack, %^$%#$%$#...
    for( int i = 0; i < (sizeof(sb)/2048); i++ )
        sb[i*2048] = 0;
    
    bool binary = No;
    
        {
        char c = MP_Packet_Start;
        port.write( &c, 1 );
        }
    
    // Put type here to count crc more easily
    sb[typ_offset] = type_v | (binary ? 0x80 : 0);
    
    int size = fill_data( sb+dat_offset, MP_Packet_Work_Buf-dat_offset, binary );
    
    bin2hex16( sb+len_offset, size );
    
    long crc = crc32( sb+typ_offset, size + crc_add_len );
    //MP_Message("send '%c', len %d", type_v, size );
    
    bin2hex32( sb+crc_offset, crc );
    
    //MP_Dump( "pkt send", sb, dat_offset+size );
    port.write( sb, dat_offset+size );
    
    // BUG!
    // should send crlf here if MP_O_CRLF is turned on.
    }







// --------------------------------------------------------------------
// Packets constructors
// --------------------------------------------------------------------

// Wakeup

MP_PK_Wakeup::MP_PK_Wakeup( const char *data, int len, char type )
    {
    type_v = MP_PT_Wakeup;
    sscanf( data, "%x", &challenge_v );
    }
long MP_PK_Wakeup::fill_data( char *b, long bufsize, bool binary ) const
    {
    // Bug - we do not check bufsize foe speed.
    // It must be big enough anyway
    // note trailing \0 is added and counted (1+)
    return 1 + sprintf( b, "%x %s", challenge_v, MP_Packet_AutoDL_Sequence );
    }


// Init

MP_PK_Init::MP_PK_Init( const char *data, int len, char type )
    {
    type_v = MP_PT_Init; int i;
    sscanf( data, "%x %x", &i, &challenge_v );
    options_v = (MP_Options)i;
    }
long MP_PK_Init::fill_data( char *b, long bufsize, bool binary ) const
    {
    // Bug - we do not check bufsize foe speed.
    // It must be big enough anyway
    // note trailing \0 is added and counted (1+)
    return 1 + sprintf( b, "%x %x", options_v, challenge_v );
    }


// EOF

MP_PK_EOF::MP_PK_EOF( const char *data, int len, char type )
    {
    type_v = MP_PT_EOF;
    sscanf( data, "%x", &eof_pos_v );
    }
long MP_PK_EOF::fill_data( char *b, long bufsize, bool binary ) const
    {
    // Bug - we do not check bufsize foe speed.
    // It must be big enough anyway
    // note trailing \0 is added and counted (1+)
    return 1 + sprintf( b, "%x", eof_pos_v );
    }



// Frame

MP_PK_G_Frame::MP_PK_G_Frame( const char *data, int len, char type )
    {
    type_v = (MP_P_Type)type; int i1, i2;
    sscanf( data, "%x %x", &i1, &i2 );
    f.off = i1; f.size = i2;
    }

long MP_PK_G_Frame::fill_data( char *b, long bufsize, bool binary ) const
    {
    // Bug - we do not check bufsize foe speed.
    // It must be big enough anyway
    // note trailing \0 is added and counted (1+)
    return 1 + sprintf( b, "%x %x", f.off, f.size );
    }
    

//Data

MP_PK_Data::MP_PK_Data( const char *data, int len, char type, bool binary )
    {
    type_v = MP_PT_Data; 
    
    int i1, i2;
    sscanf( data, "%x %x ", &i1, &i2 );
    f.off = i1; f.size = i2;
    
    const char *dbegin = data;
    while( !isspace( *data ) && *data ) data++; // 1st hex
    while(  isspace( *data ) && *data ) data++; // separator
    while( !isspace( *data ) && *data ) data++; // 2nd hex
                                        data++; // one space
    
    if( data - dbegin > len )
        throw Mp_Ex_Fail( "MP_PK_Data::MP_PK_Data", "invalid packet", "" );
    
    int ds = len - (data - dbegin);
    
    if( ds != f.size )
        throw Mp_Ex_Fail( "MP_PK_Data::MP_PK_Data", "packet size mismatch", "" );
    
    d = new char[ds];
    if( binary )
        memcpy( d, (void *)data, ds );
    else
        memcpy( d, (void *)data, ds ); // BUG -- decoder not written
    
    }

long MP_PK_Data::fill_data( char *b, long bufsize, bool binary ) const
    {
    int frame_s = sprintf( b, "%x %x ", f.off, f.size );
    
    if( frame_s + f.size > bufsize )
        throw Mp_Ex_Fail( "MP_PK_Data::fill_data", "can't fit in buffer", "" );
    
    if( binary )
        memcpy( b+frame_s, d, f.size );
    else
        memcpy( b+frame_s, d, f.size ); // BUG - encoder not written
    return f.size + frame_s;
    }















@


1.9
log
@debugging. packetizing, packet queues
and wakeup/init state machine seems to be ok.
@
text
@d132 1
a132 1
        MP_Message("MP_Packet::receive got '%c'", type );
d169 1
a169 1
    MP_Message("send '%c', len %d", type_v, size );
@


1.8
log
@debugging depacketizer
@
text
@d6 10
d36 1
a36 1
// Ofsets of fields from  ___Start of packet char___
d72 2
a73 1
        MP_Message("MP_Packet::receive got header, t %c, len %d, crc 0x%x", type, len, crc );
d89 1
a89 1
        MP_Dump("rcvr cp before crc", cp, len+dat_offset );
d91 1
a91 1
        MP_Message("MP_Packet::receive crc our %x, their %x", calc_crc, crc);
d100 1
a100 1
        MP_Message("MP_Packet::receive got correct packet '%c'", type);
d128 1
a128 1
        MP_Dump( "MP_Packet::receive", cp, len );
d169 1
a169 1
    MP_Message("send crc 0x%x, len %d", crc, size );
d173 1
a173 1
    MP_Dump( "pkt send", sb, dat_offset+size );
@


1.7
log
@Debugging has begone
@
text
@d28 4
a31 4
static const crc_offset = 0;
static const typ_offset = 8;
static const len_offset = 9;
static const dat_offset = 13;
d33 1
a33 1
static const crc_add_len = dat_offset - typ_offset;
d46 2
d52 1
a52 1
        type = cp[crc_offset];
d62 1
d70 1
a70 1
            printf("Packet length error");
d76 1
d78 1
d80 1
d84 1
a84 1
            printf("Packet CRC error");
d89 1
d158 1
d162 1
@


1.6
log
@*** empty log message ***
@
text
@d4 1
a4 1
#include "ctype.h"
d109 2
a110 1
        
d113 2
a114 1
        
@


1.5
log
@*** empty log message ***
@
text
@a5 62
virtual MP_Packet::~MP_Packet() {}


void MP_Packet::throw_fail(const char *name) const 
    { 
    throw(Mp_Ex_Fail("MP_Packet","Wrong method call",name)); 
    }


static const hexchar[16] = "0123456789ABCDEF";

static void         bin2hex16( char *cp, int val )
    {
    for( i = 4; i--; )
        {
        cp[i] = hexchar[val&0x0F];
        val >>.4;
        }
    }
    
static void         bin2hex32( char *cp, int val );
    for( i = 8; i--; )
        {
        cp[i] = hexchar[val&0x0F];
        val >>.4;
        }

inline int hc2i( char c )
    {
    return ( c > '9' ) ? c - ('A' - 10) : c - '0';
    }

static short        hex2bin16( const char *cp )
    {
    int val = 0;
    for( i = 4; i--; )
        {
        val << 4;
        val += hc2i(*cp++);
        }
    }

static int          hex2bin32( const char *cp )
    int val = 0;
    for( i = 4; i--; )
        {
        val << 4;
        val += hc2i(*cp++);
        }
    }


// Packet-type specific methods, overridden in corresponding subtypes

MP_Off      MP_Packet::eof()       const { throw_fail("eof");       return 0; }
long        MP_Packet::challenge() const { throw_fail("challenge"); return 0; }
MP_Frame    MP_Packet::frame()     const { throw_fail("frame");     return MP_Frame(); }
const char *MP_Packet::data()      const { throw_fail("data");      return 0; }
MP_Options  MP_Packet::options()   const { throw_fail("options");   return MP_Options(0); }
//int         MP_Packet::channel()   const { throw_fail("channel"); }

// --------------------------------------------------------------------
d73 1
a73 1
        
d81 1
a81 1
        
@


1.4
log
@*** empty log message ***
@
text
@d6 1
d13 45
a57 1
        
@


1.3
log
@packet ass'y/disass'y written
@
text
@d4 2
d192 1
a192 1
virtual long MP_PK_Wakeup::fill_data( char *b, long bufsize, bool binary ) const
d207 1
a207 1
    options_v = i;
d209 1
a209 1
virtual long MP_PK_Init::fill_data( char *b, long bufsize, bool binary ) const
d225 1
a225 1
virtual long MP_PK_EOF::fill_data( char *b, long bufsize, bool binary ) const;
d239 1
a239 1
    type_v = type; int i1, i2;
d244 1
a244 1
virtual long MP_PK_G_Frame::fill_data( char *b, long bufsize, bool binary ) const
d272 1
a272 1
    int ds = len - data - dbegin;
d285 1
a285 1
virtual long MP_PK_Data::fill_data( char *b, long bufsize, bool binary ) const
@


1.2
log
@ in work
@
text
@d26 1
a26 1
 0    Start of packet char (MP_Packet_Start)
d28 3
a30 1
 1    Packet type char     (MP_P_Type)
d33 1
a33 3
 2    Packet CRC32, hex (incl. type, len & data, not start char)
 
 10   Packet data length, 2-byte int, hex
d35 1
a35 1
 14   Packet data, uue or btoa if data packet, other packets
d41 6
d48 1
d51 1
a51 1
MP_Packet::MP_Packet( MP_Buffer &in )
d53 2
a54 6
    //char rb[MP_Packet_Work_Buf];
    char *cp, type;

    // Grow stack, %^$%#$%$#...
    //for( int i = 0; i < (sizeof(rb)/2048); i++ )
    //    rb[i*2048] = 0;
a58 2
        type_v = MP_PT_None;
        
d61 5
a65 3

        in.wait_for_data( 1 );    // will throw disconnect, if any
        type = *(const char *)in;
d70 26
a95 3
        bool binary = (c & 0x80u) ? Yes : No;
        c &= 0x7Fu;

d97 1
d99 25
d125 2
d128 1
d151 5
a155 1
    sb[0] = MP_Packet_Start;
d157 92
a248 1
    sb[9] = type_v | (binary ? 0x80 : 0);
d250 19
a268 1
    int size = fill_data( sb+14, MP_Packet_Work_Buf-14, binary );
d270 1
a270 1
    bin2hex16( sb+10, size );
d272 2
a273 1
    long crc = crc32( sb+5, size + (14-5) );
d275 5
a279 1
    sb[1] = sb[9]; // move type to its place
d281 5
a285 1
    bin2hex32( sb+2, crc );
d287 8
a294 1
    port.write( sb, 10+size );
d296 11
@


1.1
log
@Initial revision
@
text
@d13 101
a113 6
virtual MP_Off_t    MP_Packet::eof()       const { throw_fail("eof"); }
virtual long        MP_Packet::challenge() const { throw_fail("challenge"); }
virtual int         MP_Packet::channel()   const { throw_fail("channel"); }
virtual MP_Frame    MP_Packet::frame()     const { throw_fail("frame"); }
virtual const char *MP_Packet::data()      const { throw_fail("data"); }
virtual MP_Options  MP_Packet::options()   const { throw_fail("options"); }
@
