head	1.7;
access;
symbols;
locks;
comment	@ * @;


1.7
date	97.03.17.08.03.42;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	97.01.21.08.53.51;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.12.28.13.42.00;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.12.28.03.11.55;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.08.12.17.08.26;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.08.09.16.13.48;	author dz;	state Exp;
branches;
next	;


desc
@@


1.7
log
@fixes, see diff.
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module: msgid base
 *
 *      $Log: msgid.h $
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
@


1.6
log
@*** empty log message ***
@
text
@d8 3
d74 1
d80 2
a81 2
        void read(fstream &) const;
        void write(fstream &);
d85 2
a86 1
        int   version;
d88 8
a95 2
        int   page_size;
        int   pages;
d133 1
a133 1
long crc32( const char *data, int len );
@


1.5
log
@Ver. 27
@
text
@d8 3
d29 2
@


1.4
log
@new string/istring/domain_s comparison ops
@
text
@d8 3
d20 3
d81 1
d83 4
d120 1
@


1.3
log
@Version 25
@
text
@d8 3
d18 1
a18 1
#include <except.h>
@


1.2
log
@*** empty log message ***
@
text
@d7 4
a10 1
 *      $Log: frip.C $
d22 4
a25 1
  // - MSGID length should not be > 64 bytes.
d27 1
d29 7
a35 2
const DEF_PAGESIZE   = 512;
const DEF_PAGES      = (100*1024)/DEF_PAGESIZE;
d92 1
a92 36
        msgid_base( string fn, int pagesize = -1, int pages = -1 )
            {
            is_open = No;
            cur_page = -1;
            cur_page_modified = No;
            buf = NULL;
            pfn = fn;
            
            if( sizeof(header) != 512 )
                throw Ex_Errno( "msgid_base::msgid_base", "sizeof(header) != 512", sizeof(header) );

            try
                {
                  // theoretically close is called from open and
                  // is able to throw something too, but souldn't
                  // in this case 'cause it does nothing if file
                  // is not really open yet.
                open(); 
                if(
                   (h.page_size != pagesize && pagesize != -1 ) ||
                   (h.pages != pages && pages != -1)
                  )
                    throw Ex_Arg("msgid_base::msgid_base", "PageFile will be rebuilt", fn );
                }
            catch( General_Ex ex )
                {
                ex.print();

                if( pagesize == -1 ) pagesize = DEF_PAGESIZE;
                if( pages    == -1 ) pages    = DEF_PAGES;
                
                rebuild( pagesize, pages );
                open();
                }
            }

d103 1
@


1.1
log
@Initial revision
@
text
@d11 6
a16 2
#include "strng.h"
#include "binbuf.h"
d22 2
d25 1
a25 2

class msgid ; public string
d29 3
d43 1
a43 1
    
d60 1
a60 1
        char  reserved[ 512 - offsetof(reserved) ];
d66 1
a66 1
    bool exist();
d70 1
d77 1
a77 1
    bool check_in_buf( const msgid & ) const;
d80 1
a80 1
        msgid_base( string fn, int pagesize, int pages ) 
d86 4
d98 4
a101 1
                if( h.pagesize != pagesize || h.pages != pages )
d108 3
d120 2
@
