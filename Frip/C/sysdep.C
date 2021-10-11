/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	OS Dependent Thingies
 *
 *      $Log: sysdep.C $
 *      Revision 1.9  1997/03/22 15:50:37  dz
 *      binary!!! in copy :(:(
 *
 *      Revision 1.8  1997/03/17 08:02:53  dz
 *      SYSDEP_FRIP_SLEEP
 *
 *      Revision 1.7  1997/03/05 20:32:06  dz
 *      Do_Copy bug fix (NT/DOS versions - watcom bug?)
 *
 *      Revision 1.6  1997/03/05 18:12:38  dz
 *      Ver. 34
 *
 *      Revision 1.5  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.4  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.3  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.2  1996/07/31 07:00:23  dz
 *      FripMail written
 *
 *      Revision 1.1  1996/01/07 20:21:01  dz
 *      Initial revision
 *
 *
 *
\*/

#ifdef __IBMCPP__
#define INCL_DOSPROCESS
#endif


#include <frip.h>

#include <errno.h>

#include "lock.h"
#include "lock.h"



#ifdef __OS2__
#include "dep_os2_i.c"
#endif

#ifdef __NT__
#include "dep_nt_i.c"
#endif

#ifdef __DOS__
#include "dep_dos_i.c"
#endif

#ifdef unix
#include "dep_unix_i.c"
#endif


// Here goes system-independent implementation of
// some functions. It is used only if no system-specific version
// is written in some of the files, included above.

#ifndef SYSDEP_ILOCK
 
#include <fcntl.h> 
#include <sys\stat.h> 
#include <sys\types.h> 
#include <share.h> 

#ifdef __WATCOMC__
// Какой кретин положил сюда sleep?!
#  include <dos.h>
#endif

#ifdef __IBMCPP__
#  include <os2.h>
#endif


#ifndef SYSDEP_ILOCK_WAIT
void  ilock::wait( int time )
    {
#ifdef __IBMCPP__
    DosSleep( time * 1000 );
#else
    sleep(time);
#endif
    }
#endif

void  ilock::release()
    {
    if( we_own_the_lock_v ) 
        {
        we_own_the_lock_v = No;
        close(handle);
        unlink( conf.dir() + "/" + name_v );
        }
    }

void ilock::lock()
    {
    string fn = conf.dir() + "/" + name_v;

    handle = sopen( fn, O_RDWR|O_CREAT|O_NOINHERIT, SH_DENYRW, 0666 );
    if( handle < 0 ) 
        {
        if( errno == EMFILE || errno == ENOENT )
            throw Ex_Errno("ilock::lock()","Can't create lock file "+fn,errno);
        we_own_the_lock_v = No;
        }
    else
        we_own_the_lock_v = Yes;
    }

#endif // ilock



#ifndef SYSDEP_COPYMOVE

#ifndef SYSDEP_DO_COPY
static void Do_Copy( string from, string to )
    {
    ifstream i( from, ios::binary );
    if( !i )           throw Ex_Fail( "Do_Copy", "Can't open file", from );
    
    ofstream o( to,   ios::binary );
    if( !o )           throw Ex_Fail( "Do_Copy", "Can't create file", to );
    
    const bs = 2*1024;
    char buf[bs];
    
    while(1)
        {
        i.read( buf, bs );
        if( i.bad() )  throw Ex_Fail( "Do_Copy", "Error reading from", from );
        if( i.eof() || ( i.gcount() == 0 )  )  return;

        o.write( buf, i.gcount() );
        if( o.bad() )  throw Ex_Fail( "Do_Copy", "Error writing to", to );
        }
    }

#endif //SYSDEP_DO_COPY

void frip_rename( string from, string to )
    {
    if( rename( from, to ) )
        {
        Do_Copy( from, to );
        if( unlink( from ) )
            throw Ex_Fail( "frip_rename", "Can't unlink", from );
        }
    }

void frip_copy( string from, string to )
    {
    Do_Copy( from, to );
    }

#endif // SYSDEP_COPYMOVE



#ifndef SYSDEP_DESCRIPTION

static string find_in_list( const string &list, const string& fn )
    {
    ifstream i( list );
    if( !i )  throw Ex_Fail("find_in_list","Can't open",list);
    
    const ls = 1024;
    char line[ls];
    string ffn, fdesc;

    while( (!i.eof()) && (!i.bad()) )
        {
        i.getline( line, ls );
        string(line).parse( ffn, fdesc );

        if( 0==stricmp( ffn, fn ) )
            return fdesc;
        }

    throw Ex_EOF("find_in_list");
    }

static void put_in_list( const string &list, const string& fn, const string &desc )
    {
    ofstream o( list, ios::out|ios::ate|ios::app );
    if( !o )  throw Ex_Fail("find_in_list","Can't create or open",list);
    o << fn.c_str() << " " << desc.c_str() << "\r\n";
    }

string get_desc( string file )
    {
    char ndr[_MAX_DRIVE], ndi[_MAX_DIR], nfn[_MAX_FNAME], nex[_MAX_EXT];
    _splitpath( (char *)(const char *)file, ndr, ndi, nfn, nex );
    
    string path = string(ndr)+string(ndi);
    string fn = string(nfn)+string(nex);

    if( path.length() == 0 ) path = ".";
    
    try        { return find_in_list( path+"\\files.bbs", fn );    }
    catch(...) { return find_in_list( path+"\\descript.ion", fn ); }
    }

void set_desc( string file, string desc )
    {
    char ndr[_MAX_DRIVE], ndi[_MAX_DIR], nfn[_MAX_FNAME], nex[_MAX_EXT];
    _splitpath( (char *)(const char *)file, ndr, ndi, nfn, nex );
    
    string path = string(ndr)+string(ndi);
    string fn = string(nfn)+string(nex);
    
    if( path.length() == 0 ) path = ".";

    bool err = No;
    try { put_in_list( path+"\\files.bbs", fn, desc );    } catch(...) { err = Yes; }
    try { put_in_list( path+"\\descript.ion", fn, desc ); } catch(...) { err = Yes; }
    try { set_ea_ASCII( file, ".SUBJECT", desc );         } catch(...) { err = Yes; }

    if( err ) throw Ex_Fail("set_desc","some method failed","");
    }

#endif // SYSDEP_DESCRIPTION


#ifndef SYSDEP_FRIP_SLEEP
void frip_sleep( int sec )
    {
#ifdef __IBMCPP__
    DosSleep( sec * 1000 );
#else
    sleep(sec);
#endif
    }
#endif // SYSDEP_FRIP_SLEEP



