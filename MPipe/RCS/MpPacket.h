head	1.7;
access;
symbols;
locks;
comment	@ * @;


1.7
date	96.07.09.02.12.58;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.07.07.10.01.14;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.07.06.00.46.10;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.07.02.04.33.28;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.07.02.01.55.57;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.06.30.22.58.25;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.06.30.16.11.38;	author dz;	state Exp;
branches;
next	;


desc
@@


1.7
log
@Debugging has begone
@
text
@/************************ MPipe Protocol Library ***************************\
 *
 *	Copyright (C) 1991-1996 by Infinity Soft
 *
 *	Module 	:	
 *
 *      $Log: MpPacket.h $
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


@


1.6
log
@*** empty log message ***
@
text
@d8 3
d82 1
a82 1
        virtual long        fill_data( char *, long bufsize, bool binary ) const;
@


1.5
log
@in work
@
text
@d8 3
d35 1
d66 1
d81 10
a90 4
        bool                valid_type( char c ) const;
        void                bin2hex16( char *, int val ) const;
        void                bin2hex32( char *, int val ) const;
        long                crc32( const char *data, int len ) const;
d96 1
a96 1
        MP_Packet( MP_Buffer &in );                       // Receive packet
a98 1
    
d111 8
d126 1
d147 1
a147 1
    
d168 1
d198 1
d253 2
d261 4
a264 2
    MP_PK_Query::MP_PK_Query( const MP_Frame & _f) 
        { type_v = MP_PT_Query; f = _f; }
d269 4
a272 2
    MP_PK_Ack::MP_PK_Ack( const MP_Frame & _f)
        { type_v = MP_PT_Ack; f = _f; }
d277 4
a280 2
    MP_PK_Nak::MP_PK_Nak( const MP_Frame & _f) 
        { type_v = MP_PT_Nak; f = _f; }
@


1.4
log
@writing
@
text
@d8 3
d33 3
a35 1
// Как-то это все убого, но пока не знаю, как именно и где лечить.
d60 2
d63 3
a67 7
struct MP_PK_header
    {
    long     crc32;
    char     channel; 
    char     type;
    };

d73 7
a79 1
        void throw_fail(const char *name) const { throw(Mp_Ex_Fail("MP_Packet","Wrong method call",name)); }
d82 10
a91 2
        MP_P_Type type() const { return type_v; }

d94 5
a98 13

        virtual MP_Off_t    eof()       const { throw_fail("eof"); }

        virtual long        challenge() const { throw_fail("challenge"); }
        virtual int         channel()   const { throw_fail("channel"); }

        //virtual MP_Off_t    off()       const { throw_fail("off"); }
        //virtual MP_Off_t    size()      const { throw_fail(""); }
        virtual MP_Frame    frame()     const { throw_fail("frame"); }

        virtual const char *data()      const { throw_fail("data"); }

        virtual MP_Options  options()   const { throw_fail("options"); }
d105 1
a105 1
    long    challenge_v;
d107 1
d119 1
d143 1
d173 1
a173 1
struct MP_PK_XOFF    : public MP_Packet
d175 5
a179 14
    // channel to stop
    char             channel_v;
    virtual int         channel()   const { return channel_v; }
    MP_PK_XOFF::MP_PK_XOFF( char ch )
        { type_v = MP_PT_XOFF; channel_v = ch; }
    };

struct MP_PK_XON     : public MP_Packet
    {
    // channel to continue
    char             channel_v;
    virtual int         channel()   const { return channel_v; }
    MP_PK_XON::MP_PK_XON( char ch )
        { type_v = MP_PT_XON; channel_v = ch; }
d182 13
a194 1
struct MP_PK_EOF     : public MP_Packet
d196 1
a196 4
    MP_Off_t         eof_pos_v;
    virtual MP_Off_t eof() const { return eof_pos_v; }
    MP_PK_EOF::MP_PK_EOF(MP_Off eof_pos)
        { type_v = MP_PT_EOF; eof_pos_v = eof_pos; }
d199 1
a199 1
struct MP_PK_SStop   : public MP_Packet
d204 1
a204 1
struct MP_PK_RStop1  : public MP_Packet
d209 1
a209 1
struct MP_PK_RStop2  : public MP_Packet
d214 1
a214 1
struct MP_PK_Ack     : public MP_Packet
d216 1
a216 4
    MP_Frame         f;
    virtual MP_Frame    frame()     const  { return f; }
    MP_PK_Ack::MP_PK_Ack( const MP_Frame & _f)
        { type_v = MP_PT_Ack; f = _f; }
d219 1
a219 1
struct MP_PK_Nak     : public MP_Packet
d221 1
a221 4
    MP_Frame         f;
    virtual MP_Frame    frame()     const  { return f; }
    MP_PK_Nak::MP_PK_Nak( const MP_Frame & _f) 
        { type_v = MP_PT_Nak; f = _f; }
a223 1
#define MP_PK_Nack MP_PK_Nak
d225 5
a229 1
struct MP_PK_Query   : public MP_Packet
d231 1
a231 1
    MP_Frame         f;
d233 5
d242 1
a242 1
struct MP_PK_HBT     : public MP_Packet
d244 2
a245 1
    MP_PK_HBT::MP_PK_HBT() { type_v = MP_PT_HBT; }
d248 5
d254 1
a254 4




@


1.3
log
@writing
@
text
@d8 3
d30 2
d67 5
a71 3
    private:
        //MP_PK_header     h;  // what for? one static will do for all

d73 1
a73 1
        MP_P_Type type() const;
d78 1
a78 1
        virtual MP_Off_t    eof()       const = 0;
d80 2
a81 2
        virtual long        challenge() const = 0;
        virtual int         channel()   const = 0;
d83 3
a85 3
        virtual MP_Off_t    off()       const = 0;
        virtual MP_Off_t    size()      const = 0;
        virtual MP_Frame    frame()     const = 0;
d87 1
a87 2
        virtual const char *data()      const = 0;
        virtual int         len()       const = 0;
d89 1
a89 1
        virtual MP_Options  options()   const = 0;
d97 6
d105 1
a105 2
// NB! based on MP_PK_Wakeup!
struct MP_PK_Init    : public MP_PK_Wakeup
d107 2
d116 8
d128 5
d134 1
a134 1
    unsigned long    off_v;   
d136 12
a147 1
    unsigned short   size_v;
d150 1
d153 1
d159 1
d165 3
d174 3
a180 2
    //int              len;
    // len characters of a hex number follow
a181 1
    
d183 2
d189 1
d194 1
d199 1
d204 4
a207 4
    //int              len_v;
    // len characters of a (hex number, space, hex number) follow
    MP_Off_t         off_v;
    MP_Off_t         size_v;
d212 4
a215 4
    //int              len;
    // len characters of a (hex number, space, hex number) follow
    MP_Off_t         off_v;
    MP_Off_t         size_v;
d218 1
a218 3
struct MP_PK_Nack    : public MP_PK_Nak
    {
    };
d222 4
a225 4
    //int              len_v;
    // len characters of a (hex number, space, hex number) follow
    MP_Off_t         off_v;
    MP_Off_t         size_v;
d230 1
a236 40
// ------------------------------------------------------------------------

class MP_PAssembler
    {
    private:
        MP_Port &p;
    
    public:
        MP_PAssembler( MP_Port & _p ) : p(_p) {}
        
        // Sender
        void P_Send_Init( long challenge );
        void P_Send_Wakeup( long challenge );

        void P_Send_Data( const MP_Frame &, const char *data, int len );
        void P_Send_Query( const MP_Off_t & pos, const MP_Off_t & size );
        void P_Send_SEOF( const MP_Off_t & s_eof_pos );
        
        void P_Send_SStop();
        
        // Receiver
        void P_Send_Ack( const MP_Off_t & pos, const MP_Off_t & size );
        void P_Send_NAck( const MP_Off_t & pos, const MP_Off_t & size );
        
#define P_Send_Nack P_Send_NAck
#define P_Send_Nak P_Send_NAck
        
        void P_Send_XOFF( int channel );
        void P_Send_XON( int channel );
    
        void P_Send_RStop1();
        void P_Send_RStop2();
        
        // General
        void P_Send_HBT();

#define P_Send_HeartBeat P_Send_HBT
        
        
    };
@


1.2
log
@update
@
text
@d8 3
d25 1
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: Recode.c $
d16 6
d58 22
a79 3
    MP_PK_header     h;
    
    MP_P_Type type() const;
d86 1
a86 1
    long    challenge; 
d98 1
a98 1
    MP_Options       options;
d104 1
a104 1
    unsigned long    off;   
d106 1
a106 1
    unsigned short   size;
d112 1
a112 1
    unsigned long    off;   
d114 1
a114 1
    unsigned short   size;
d120 1
a120 1
    char             channel;
d126 1
a126 1
    char             channel;
d131 1
a131 1
    int              len;
d133 3
a135 1
    MP_Off_t         eof_pos;
d152 1
a152 1
    int              len;
d154 2
a155 2
    MP_Off_t         off;
    MP_Off_t         size;
d160 1
a160 1
    int              len;
d162 2
a163 2
    MP_Off_t         off;
    MP_Off_t         size;
d172 1
a172 1
    int              len;
d174 2
a175 2
    MP_Off_t         off;
    MP_Off_t         size;
d182 51
@
