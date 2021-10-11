
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















