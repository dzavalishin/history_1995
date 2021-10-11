/*\
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


//#include "conf.h"
#include "msgid.h"


void msgid_base::header::read( fstream &s ) const
    {
    s.seekg( 0 );
    s.read( (char *)this, sizeof(header) );
    if( s.fail() || s.gcount() != sizeof(header) )
        throw Ex_Fail( "header::read", "read error", "" );
    }

void msgid_base::header::write( fstream &s )
    {
    s.seekp( 0 );
    s.write( (char *)this, sizeof(header) );
    if( s.fail() )
        throw Ex_Fail( "header::write", "write error", "" );
    }


  // ---------------------------------------------------------------------

const char  msgid_base::signature[10] = "<_MSGID_>";



//bool msgid_base::exist()   {   }


void msgid_base::open()
    {
    close();
    is_open = No;
    
    pf.open(pfn, ios::binary|ios::in|ios::out|ios::nocreate);
    if( !pf )
        throw Ex_Fail( "msgid_base::open", "Can't open", pfn );
    
    h.read(pf);

    if( strncmp( h.signature, signature, sizeof(signature) ) )
        {
        close();
        throw Ex_Fail( "msgid_base::open", "Wrong file signature", pfn );
        }

    buf = new char[h.page_size];
    is_open = Yes;
    }


void msgid_base::close()
    {
    if( is_open )
        {
        flush_cur_page();
        pf.flush();
        if( pf.fail() )
            throw Ex_Fail( "msgid_base::close", "I/O error", pfn );
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
    }


void msgid_base::write_page( int p )
    {
    pf.seekp( page_pos( p ) );
    pf.write( buf, h.page_size );
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

    if( rename( tempn, pfn ) )
        throw Ex_Errno( "msgid_base::rebuild", "Can't rename "+string(tempn) + " to "+pfn, errno );
    }




