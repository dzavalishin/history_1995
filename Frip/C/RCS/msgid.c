head	1.6;
access;
symbols;
locks;
comment	@ * @;


1.6
date	97.03.17.08.00.55;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	97.01.21.08.53.51;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.08.12.22.10.05;	author dz;	state Exp;
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


1.6
log
@header r/w code redone to be structure packing
independent. File version check added.
Version changed to 1st (was zero).
More i/o failure checks.
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module: msgid base
 *
 *      $Log: msgid.c $
 *      Revision 1.5  1997/01/21 08:53:51  dz
 *      Bugfix: didn't flush page on close.
 *
 *      Revision 1.4  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.3  1996/08/12 22:10:05  dz
 *      hash func. gives 120/90 distribution
 *
 *      Revision 1.2  1996/08/12 17:08:26  dz
 *      *** empty log message ***
 *
 *
\*/

#ifdef __OS2__
#define INCL_DOSFILEMGR          /* File Manager values */
#define INCL_DOSERRORS           /* DOS Error values    */
#include <os2.h>
//#include <stdio.h>
#endif

#include <process.h>
#include <io.h>


//#include "conf.h"
#include "msgid.h"


void msgid_base::header::read( fstream &s ) 
    {
    s.seekg( 0 );
    //s.read( (char *)&(this->signature), sizeof(header) );
      //if( s.fail() || s.gcount() != sizeof(header) ) throw Ex_Fail( "header::read", "read error", "" );
    s.read( signature, sizeof(signature) );
    //if( s.fail() || s.gcount() != sizeof(signature) ) throw Ex_Fail( "header::read", "signature read error", "" );
    s.read( (char *)&version, sizeof(version) );
    s.read( (char *)&page_size, sizeof(page_size) );
    s.read( (char *)&pages, sizeof(pages) );
    if( s.fail() ) throw Ex_Errno( "header::read", "I/O error", errno );
    }

void msgid_base::header::write( fstream &s ) const
    {
    s.seekp( 0 );
    //s.write( (char *)&(this->signature), sizeof(header) );
    //if( s.fail() )  throw Ex_Fail( "header::write", "write error", "" );
    s.write( signature, sizeof(signature) );
    s.write( (char *)&version, sizeof(version) );
    s.write( (char *)&page_size, sizeof(page_size) );
    s.write( (char *)&pages, sizeof(pages) );
    s.flush();
    if( s.fail() ) throw Ex_Errno( "header::write", "I/O error", errno );
    }


  // ---------------------------------------------------------------------

const char  msgid_base::signature[10] = "<_MSGID_>";
const int   msgid_base::version       = 1;


//bool msgid_base::exist()   {   }


void msgid_base::open()
    {
    close();
    is_open = No;
    
    pf.open(pfn, ios::binary|ios::in|ios::out|ios::nocreate);
    if( !pf )
        throw Ex_Fail( "msgid_base::open", "Can't open", pfn );
    
    h.read(pf);

        {
        string s; s.print("id base ver %d, %d pages of %d bytes each", h.version, h.pages, h.page_size );
        debug( s );
        }
    
    if( strncmp( h.signature, signature, sizeof(signature) ) )
        {
        pf.close();
        throw Ex_Fail( "msgid_base::open", "Wrong file signature", pfn );
        }

    if( h.version != version )
        {
        pf.close();
        throw Ex_Fail( "msgid_base::open", "Wrong file version", pfn );
        }

    buf = new char[h.page_size];
    if( buf == NULL ) throw Ex_Fail( "msgid_base::open", "Can't allocate buffer", pfn );

    is_open = Yes;
    }


void msgid_base::close()
    {
    if( is_open )
        {
        flush_cur_page();
        pf.flush();
        if( pf.fail() )
            throw Ex_Errno( "msgid_base::close", "I/O error", errno );
        pf.close();
        }
    delete buf;
    buf = NULL;
    }


int msgid_base::page_pos( int p )
    {
    if( p < 0 || p >= h.pages )
        throw Ex_Errno( "msgid_base::read_page", "invalid page number", p );
    return sizeof(header)+(h.page_size * p );
    }


void msgid_base::read_page( int p )
    {
    pf.seekg( page_pos( p ) );
    pf.read( buf, h.page_size );
    if( pf.fail() ) throw Ex_Errno( "msgid_base::write_page", "I/O error", errno );
    }


void msgid_base::write_page( int p )
    {
    pf.seekp( page_pos( p ) );
    pf.write( buf, h.page_size );
    if( pf.fail() ) throw Ex_Errno( "msgid_base::write_page", "I/O error", errno );
    }


void msgid_base::flush_cur_page()
    {
    if( cur_page_modified )
        {
        write_page( cur_page );
        cur_page_modified = No;
        }
    }


void msgid_base::set_cur_page( int p )
    {
    flush_cur_page();
    read_page( p );
    cur_page = p;
    }


  // при помещении в базу можно паковать строки из цифр/хекс-цифр,
  // групп типа фидо-адреса, etc
  // и слова fidonet/fidonet.org персонально

bool msgid_base::check_in_buf( const msgid &id, bool add )
    {
    char *p;
    
    for( p = buf; *p; p += strlen( p ) + 1 )
        {
        if (p >= buf + h.page_size)
            throw Ex_Errno( "msgid_base::check_in_buf", "buffer doesn't end with two zeroes", page_pos(cur_page) );
        if( p == id ) return Yes;
        }

    if( !add )
        return No;


    cur_page_modified = Yes;
    msgid short_id = id.substr(0, MSGID_MAX_ID_LEN);

    memmove( buf+short_id.length()+1, buf, h.page_size-(short_id.length()+1) );
    memmove( buf, short_id.c_str(), short_id.length()+1 );

    p = buf+h.page_size-1;
    if( *p == 0 ) // last byte is zero?
        p--;      // clear previous field
    for( ; *p && p >= buf; p-- )
        *p = 0;

    if( buf[h.page_size-1] != 0 || buf[h.page_size-2] != 0 )
        {
        cur_page_modified = No; // Don't save
        cur_page = -1;          // Force reload
        throw Ex_Fail( "msgid_base::check_in_buf", "buffer doesn't end with two zeroes after msgid add", short_id );
        }

        {
        string msg;
        msg.print("Msgid: add to page %d", cur_page);
        debug( msg );
        }
    return No;
    }


bool msgid_base::is_dupe( const msgid &id )
    {
    set_cur_page( id.hashvalue( h.pages ));
    return check_in_buf( id, No );
    }


bool msgid_base::check( const msgid &id )
    {
    set_cur_page( id.hashvalue( h.pages ));
    return check_in_buf( id, Yes );
    }


int msgid::hashvalue( int modulo ) const
    {
    unsigned int v = 0;

    for( int i = length(); i >= 0; i-- )
        {
        unsigned c = (uint)operator[] (i);

        v ^= c << 13;
        v += c << 7;
        v ^= c;
        v ^= c >> 3;

        unsigned int upper2 = v >> 30;
        v <<= 2;
        v |= upper2;
        }

    int pv = v & 0x7FFFFFFF;

    while( pv >= modulo )
        {
        int a = pv / modulo;
        int b = pv % modulo;
        pv = a+b;
        }

    return pv;
    }



msgid_base::msgid_base( string fn, int pagesize, int pages )
    {
    is_open = No;
    cur_page = -1;
    cur_page_modified = No;
    buf = NULL;
    pfn = fn;
    
    if( pagesize > 0 && pagesize < MSGID_MAX_ID_LEN )
        pagesize = MSGID_MAX_ID_LEN + 2;
    
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
        
        if( pagesize == -1 ) pagesize = MSGID_DEF_PAGESIZE;
        if( pages    == -1 ) pages    = MSGID_DEF_PAGES;
        
        rebuild( pagesize, pages );
        open();
        }
    }


void msgid_base::rebuild( int &pagesize, int &pages )
    {
// BUG!! #error Rebuild of existing dupebase is not written yet
    if( is_open )
        throw Ex_Arg( "msgid_base::rebuild", "Rebuild of existing dupebase is not written yet", "" );

    int newsize = sizeof( header ) + (pagesize*pages);

    // BUG! Use tempFileName here!
    char tempn[15];
    sprintf( tempn, "build%3.3d.$$$", getpid() % 999 );

    chmod( tempn, 0666 );
    unlink( tempn );
    
#ifdef __OS2__
    HFILE  fh;
    ULONG  action;
    APIRET rc;
        
    rc = DosOpen(tempn,                            // File path name
                 &fh,                            // File handle
                 &action,                        // Action taken
                 newsize,                        // File primary allocation
                 FILE_ARCHIVED | FILE_NORMAL,    // File attribute
                 OPEN_ACTION_CREATE_IF_NEW,      // Open function type
                 OPEN_FLAGS_NOINHERIT |
                 OPEN_SHARE_DENYREADWRITE  |
                 OPEN_ACCESS_READWRITE,          // Open mode of the file
                 0L);                            // No extended attribute
        
    if (rc != NO_ERROR)
        throw Ex_Errno("msgid_base::rebuild", "DosOpen error", rc );

    if( action != FILE_CREATED )
        throw Ex_Errno("msgid_base::rebuild", "DosOpen did not created file", action );

    rc = DosClose(fh);

    if (rc != NO_ERROR)
        throw Ex_Errno("msgid_base::rebuild", "DosClose error", rc );
        
    fstream newf(tempn, ios::binary|ios::in|ios::out|ios::nocreate);
#else
    fstream newf(tempn, ios::binary|ios::in|ios::out|ios::noreplace);
#endif

    if( !newf )
        throw Ex_Fail( "msgid_base::rebuild", "Can't create", pfn );
    
    strncpy( h.signature, signature, sizeof(signature) );
    h.page_size = pagesize;
    h.pages = pages;
    h.version = version;
    h.write(newf);

    // clear all the pages
    for( int i = 0; i < pages; i++ )
        {
        char zz[3] = "\0\0";
        newf.seekp( page_pos( i ) );
        newf.write( zz, 2 );
        }

    if( newf.fail() )
        {
        newf.close();
        throw Ex_Fail( "msgid_base::rebuild", "Can't clear", pfn );
        }
    
    newf.close();

    chmod( pfn, 0666 );
    if( unlink( pfn ) )
        Error("Can't delete "+pfn);

    if( rename( tempn, pfn ) )
        {
        unlink( tempn );
        throw Ex_Errno( "msgid_base::rebuild", "Can't rename "+string(tempn) + " to "+pfn, errno );
        }
    }




@


1.5
log
@Bugfix: didn't flush page on close.
@
text
@d8 3
d31 1
d38 1
a38 1
void msgid_base::header::read( fstream &s ) const
d41 8
a48 3
    s.read( (char *)this, sizeof(header) );
    if( s.fail() || s.gcount() != sizeof(header) )
        throw Ex_Fail( "header::read", "read error", "" );
d51 1
a51 1
void msgid_base::header::write( fstream &s )
d54 8
a61 3
    s.write( (char *)this, sizeof(header) );
    if( s.fail() )
        throw Ex_Fail( "header::write", "write error", "" );
d68 1
a68 1

d85 5
d92 1
a92 1
        close();
d96 6
d103 2
d116 1
a116 1
            throw Ex_Fail( "msgid_base::close", "I/O error", pfn );
d136 1
d144 1
d310 3
d352 1
d371 4
d376 2
d379 1
@


1.4
log
@Version 25
@
text
@d8 3
d30 1
d63 1
d78 1
d175 5
a179 2
    cout << "add to " << cur_page << "\n";
    
@


1.3
log
@hash func. gives 120/90 distribution
@
text
@d8 3
a190 1
#if 1
a196 6
        //if( c <= ' ' || c == ':' || c == '/' || c == '.' )
        //    continue;
        
#if 0
        v += c;
#else
a200 1
#endif
a206 3
    // % here? why?
    //return (v & 0x7FFFFFFF) % modulo;

d217 11
d229 29
a257 3
#else
    return  crc32( c_str(), length() ) % modulo;
#endif
a260 1

d263 1
@


1.2
log
@*** empty log message ***
@
text
@d7 4
a10 1
 *      $Log: frip.C $
d134 3
a136 1
    for( char *p = buf; *p; p += strlen( p ) + 1 )
d143 9
a151 4
    if ( add )
        {
        cur_page_modified = Yes;
        msgid short_id = id.substr(0, 64);
d153 5
a157 17
        if( (p + id.length() + 2) > (buf + h.page_size) )
            {
            //for (; p - s + id.length() + 2 <= h.page_size;
            for(
                char *s = buf;
                  //s + id.length() + 2 <= h.page_size;
                (s-buf)+ ((buf+h.page_size) - p) > id.length() + 2;
                 s += strlen( s ) + 1
               )
                ;
            
              //memmove(buf, s, p - s + 1); p -= s - buf;
            memmove(buf, s, h.page_size-(s-buf)); p -= s - buf;
            }
        
        strcpy( p, id );
        p += strlen( p ) + 1;
d159 5
a163 7
        if(p-buf >= h.page_size)
            {
            buf[h.page_size-1] = 0;
            throw Ex_Errno( "msgid_base::check_in_buf", "wrong end pos ", p-buf );
            }
        else
            *p = 0;
d166 2
d188 2
a189 1
    int v = 0;
d192 19
a210 1
        v += operator[] (i);
d213 16
a228 1
    return v % modulo;
@


1.1
log
@Initial revision
@
text
@d11 10
d24 1
a24 1
void header::read( fstream &s ) const
d32 1
a32 1
void header::write( fstream &s );
d43 1
a43 1
const char[]  msgid_base::signature[10] = "<_MSGIDS_>";
d47 1
a47 2
void msgid_base::rebuild( int &pagesize, int &pages );
bool msgid_base::exist();
d66 1
a66 1
    buf = new char[h.pagesize];
d76 1
a76 1
        if( s.fail() )
d89 1
a89 1
    return sizeof(header)+(h.pagesize * p );
d125 4
d134 2
a135 2
            throw Ex_General( "msgid_base::check_in_buf", "buffer doesn't end with two zeroes" );
        if( p == id ) return true;
d140 1
d143 1
a143 1
        if( p + id.length() + 2 > buf + h.page_size )
d145 8
a152 4
            char *s = buf;
            
            for (; p - s + id.length() + 2 <= h.page_size;
                 s += strlen( s ) + 1 );
d154 2
a155 1
            memmove(buf, s, p - s + 1); p -= s - buf;
d160 8
a167 3
        *p = 0;
        
        return true;
d169 2
a170 2
    else
        return false;
d177 1
a177 1
    return check_in_buf( id, false );
d181 1
a181 1
bool check( const msgid &data )
d184 86
a269 1
    return check_in_buf( id, true );
d271 3
@
