/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	:	
 *
 *      $Log: MpPacket.h $
 *      Revision 1.7  1996/07/09 02:12:58  dz
 *      Debugging has begone
 *
 *      Revision 1.6  1996/07/07 10:01:14  dz
 *      *** empty log message ***
 *
 *      Revision 1.5  1996/07/06 00:46:10  dz
 *      in work
 *
 *      Revision 1.4  1996/07/02 04:33:28  dz
 *      writing
 *
 *      Revision 1.3  1996/07/02 01:55:57  dz
 *      writing
 *
 *      Revision 1.2  1996/06/30 22:58:25  dz
 *      update
 *
 *      Revision 1.1  1996/06/30 16:11:38  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#ifndef MPPACKET_H
#define MPPACKET_H

#include "MpOptions.h"
#include "MpUtil.h"
#include "MpWin.h"
#include "MpPort.h"
#include "MpBuff.h"



// NB! No types with hi bit set!

enum MP_P_Type
    {
    MP_PT_None     =  0,      // No packet!
    
    MP_PT_Wakeup   = 'W',     // B I'm alive
    MP_PT_Init     = 'I',     // B Parameters exchange
    MP_PT_Data     = 'D',     // S Data block
    MP_PT_MiniData = 'd',     // S Data block
    MP_PT_XOFF     = '-',     // S Please stop
    MP_PT_XON      = '+',     // S Please start again
    MP_PT_Query    = 'Q',     // S Data range query (is it received, or not)
    MP_PT_EOF      = 'E',     // S EOF position
    MP_PT_SStop    = 'S',     // S Sender stop
    MP_PT_Ack      = 'A',     // R Ack
    MP_PT_Nak      = 'N',     // R Negative Ack
    MP_PT_Nack     = 'N',     // R Negative Ack
    MP_PT_RStop1   = '1',     // R receiver stopping
    MP_PT_RStop2   = '2',     // R receiver stoped
    
    MP_PT_HBT      = 'H'      // I Heart Beat
    
    };

const char MP_Packet_Start   = 0x18; // Good old ^X
//const char MP_Packet_Escape  = 0x0B;
extern const char *MP_Packet_AutoDL_Sequence;

// Here we support packets of 16K max, so 24K buff. should be ok
const MP_Packet_Max_Data = 16*1024;
const MP_Packet_Work_Buf = 24*1024;


class MP_Packet
    {
    protected:
        MP_P_Type type_v;
        
        void                throw_fail(const char *name) const;
        virtual long        fill_data( char *, long bufsize, bool binary ) const = 0;
        
        static bool         valid_type( char c );
        
        static long         crc32( const char *data, int len );
        //static bool         crc32test( const char *data, int len );
        
        static void         bin2hex16( char *, int val );
        static void         bin2hex32( char *, int val );
        
        static short        hex2bin16( const char * );
        static int          hex2bin32( const char * );
        
    public:
        //void *              operator new( MP_port & );  // Receive packet
        
        void                send( MP_Port & ) const;      // Send packet
        static MP_Packet *  receive( MP_Buffer &in );     // Receive packet
        
        MP_Packet();
        virtual ~MP_Packet();
        
        MP_P_Type           type() const { return type_v; }

        // Packet-type specific methods, overridden in corresponding subtypes
        virtual MP_Off      eof() const;
        virtual long        challenge() const;
        virtual MP_Frame    frame() const;
        virtual const char *data() const;
        virtual MP_Options  options() const;
    };



// --------------------------------------------------------------------
// Packets with data




struct MP_PK_Wakeup  : public MP_Packet
    {
    // Sent by called side only, used to speedup things
    // and can be completely ignored and taken from init.
    long                challenge_v;
    virtual long        challenge() const { return challenge_v; }
    virtual long        fill_data( char *, long bufsize, bool binary ) const;
    MP_PK_Wakeup::MP_PK_Wakeup( const char *data, int len, char type );
    MP_PK_Wakeup::MP_PK_Wakeup( long chal )
        {
        challenge_v = chal;
        type_v = MP_PT_Wakeup;
        }
    };

struct MP_PK_Init    : public MP_Packet
    {
    long    challenge_v;
    virtual long        challenge() const { return challenge_v; }
    virtual long        fill_data( char *, long bufsize, bool binary ) const;
    // Sent by called side only, returned back by calling side.
    // Caller hangs up immediately if values differ.
    // This feature is taken from zmodem and used to fight
    // trojan-horses in e-mail or files, viewed via terminal
    // emulator with auto-download feature
    // long             challenge; is in wakeup
    MP_Options       options_v;
    virtual MP_Options  options()   const { return options_v; }
    MP_PK_Init::MP_PK_Init( const char *data, int len, char type );    
    MP_PK_Init::MP_PK_Init( MP_Options options, long chal )
        {
        options_v = options;
        challenge_v = chal;
        type_v = MP_PT_Init;
        }
    };

struct MP_PK_Data    : public MP_Packet
    {
    MP_Frame         f;
    char *           d;
    virtual MP_Frame    frame()     const  { return f; }
    virtual const char *data()      const  { return d; }
    virtual long        fill_data( char *, long bufsize, bool binary ) const;
    
    // lower 4 bytes of data block offset. Window can't be > maxlong!
    //unsigned long    off_v;
    // data packet can't be > 64K.
    //unsigned short   size_v;
    MP_PK_Data::MP_PK_Data( const char *data, int len, char type, bool binary );
    MP_PK_Data::MP_PK_Data( const MP_Frame &ff, const char *data )
        {
        type_v = MP_PT_Data;
        f = ff;
        d = new char[ff.size];
        memcpy( d, (void *)data, ff.size );
        }
    MP_PK_Data::~MP_PK_Data()
        {
        delete [] d;
        }
    };

# if 0
struct MP_PK_MiniData: public MP_Packet
    {
    MP_Frame         v;
    // data block _offset_. Not truncated.
    unsigned long    off_v;   
    // data packet can't be > 64K.
    unsigned short   size_v;
    };
#endif

struct MP_PK_EOF     : public MP_Packet
    {
    MP_Off_t            eof_pos_v;
    virtual MP_Off_t    eof() const { return eof_pos_v; }
    virtual long        fill_data( char *, long bufsize, bool binary ) const;
    MP_PK_EOF::MP_PK_EOF( const char *data, int len, char type );    
    MP_PK_EOF::MP_PK_EOF(MP_Off eof_pos)
        { type_v = MP_PT_EOF; eof_pos_v = eof_pos; }
    };




// --------------------------------------------------------------------
// Packets withno data


struct MP_PK_G_Empty : public MP_Packet
    {
    virtual long        fill_data( char *, long bufsize, bool binary ) const { return 0; }
    };

struct MP_PK_HBT     : public MP_PK_G_Empty
    {
    MP_PK_HBT::MP_PK_HBT() { type_v = MP_PT_HBT; }
    };

struct MP_PK_SStop   : public MP_PK_G_Empty
    {
    MP_PK_SStop::MP_PK_SStop() { type_v = MP_PT_SStop; }
    };

struct MP_PK_RStop1  : public MP_PK_G_Empty
    {
    MP_PK_RStop1::MP_PK_RStop1() { type_v = MP_PT_RStop1; }
    };

struct MP_PK_RStop2  : public MP_PK_G_Empty
    {
    MP_PK_RStop2::MP_PK_RStop2() { type_v = MP_PT_RStop2; }
    };

struct MP_PK_XOFF    : public MP_PK_G_Empty
    {
    MP_PK_XOFF::MP_PK_XOFF()     { type_v = MP_PT_XOFF; }
    };

struct MP_PK_XON     : public MP_PK_G_Empty
    {
    MP_PK_XON::MP_PK_XON()       { type_v = MP_PT_XON; }
    };



// --------------------------------------------------------------------
// Packets, that carry frame data only

struct MP_PK_G_Frame : public MP_Packet
    {
    MP_Frame            f;
    MP_PK_G_Frame::MP_PK_G_Frame( const char *data, int len, char type );
    MP_PK_G_Frame::MP_PK_G_Frame( const MP_Frame & _f) : f(_f) {}
    virtual MP_Frame    frame()     const  { return f; }
    virtual long        fill_data( char *, long bufsize, bool binary ) const;
    };

struct MP_PK_Query   : public MP_PK_G_Frame
    {
    MP_PK_Query::MP_PK_Query( const char *data, int len, char type )
        : MP_PK_G_Frame( data, len, type ) {}
    MP_PK_Query::MP_PK_Query( const MP_Frame & _f) : MP_PK_G_Frame(_f)
        { type_v = MP_PT_Query; }
    };

struct MP_PK_Ack     : public MP_PK_G_Frame
    {
    MP_PK_Ack::MP_PK_Ack( const char *data, int len, char type )
        : MP_PK_G_Frame( data, len, type ) {}
    MP_PK_Ack::MP_PK_Ack( const MP_Frame & _f) : MP_PK_G_Frame(_f)
        { type_v = MP_PT_Ack; }
    };

struct MP_PK_Nak     : public MP_PK_G_Frame
    {
    MP_PK_Nak::MP_PK_Nak( const char *data, int len, char type )
        : MP_PK_G_Frame( data, len, type ) {}
    MP_PK_Nak::MP_PK_Nak( const MP_Frame & _f) : MP_PK_G_Frame(_f)
        { type_v = MP_PT_Nak; }
    };

#define MP_PK_Nack MP_PK_Nak





#endif // MPPACKET_H


