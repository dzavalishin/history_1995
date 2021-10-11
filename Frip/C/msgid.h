/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module: msgid base
 *
 *      $Log: msgid.h $
 *      Revision 1.7  1997/03/17 08:03:42  dz
 *      fixes, see diff.
 *
 *      Revision 1.6  1997/01/21 08:53:51  dz
 *      *** empty log message ***
 *
 *      Revision 1.5  1996/12/28 13:42:00  dz
 *      Ver. 27
 *
 *      Revision 1.4  1996/12/28 03:11:55  dz
 *      new string/istring/domain_s comparison ops
 *
 *      Revision 1.3  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.2  1996/08/12 17:08:26  dz
 *      *** empty log message ***
 *
 *
\*/

#ifndef MSGID_H
#define MSGID_H

#include <strng.h>
#include "except.h"
#include <binbuf.h>

#include "conf.h"

#include <fstream.h>
#include <stddef.h>

  // Limitations:
  // - MSGID length should not be > MSGID_MAX_ID_LEN bytes.


const MSGID_DEF_PAGESIZE   = 512;

#define SMALL_BASE 1

#if SMALL_BASE
const MSGID_DEF_PAGES      = ((100*1024)/MSGID_DEF_PAGESIZE)+7;
#else
const MSGID_DEF_PAGES      = ((2000*1024)/MSGID_DEF_PAGESIZE)+7;
#endif

const MSGID_MAX_ID_LEN     = 128;

class msgid : public string
    {
    public:
        int hashvalue(int modulo) const;

        msgid( const msgid& m ) : string( (string&)m ) {}
        msgid( const string& m ) : string( m ) {}
    };

class msgid_base
    {
    fstream      pf;
    string       pfn;
    bool         is_open;

    char         *buf;
    int          cur_page;
    bool         cur_page_modified;

    static const char signature[10];
    static const int  version;
    
    struct header
        {
        header()   { memset( this, 0, sizeof(header) );  }

        void read(fstream &);
        void write(fstream &) const;
        
        char  signature[10];

#ifdef __DOS__
        long int   version;
        
        long int   page_size;
        long int   pages;
#else
        int        version;
        
        int        page_size;
        int        pages;
#endif

#ifdef __IBMCPP__
        char  reserved[ 512 - (((sizeof(int))*3)+10) - 4];
#else
        char  reserved[ 512 - (((sizeof(int))*3)+10)];
#endif
        
        };

    header h;
    
    void rebuild( int &pagesize, int &pages );
    //bool exist();
    void open();
    void close();

    int  page_pos( int p );
    void read_page( int );
    void write_page( int );

    void flush_cur_page();
    void set_cur_page( int );

    bool check_in_buf( const msgid &id, bool add );
    
    public:
        msgid_base( string fn, int pagesize = -1, int pages = -1 );
        ~msgid_base() { close(); }

        bool is_dupe( const msgid &data ); // yes = dupe.
        bool check( const msgid &data ); // yes = dupe. msgid goes to base anyway

        void flush() { flush_cur_page(); pf.flush(); }
    };



//long crc32( const char *data, int len );


#endif // MSGID_H
