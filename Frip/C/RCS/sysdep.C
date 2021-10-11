head	1.9;
access;
symbols
	Beta_12:1.1;
locks;
comment	@ * @;


1.9
date	97.03.22.15.50.37;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	97.03.17.08.02.53;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	97.03.05.20.32.06;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	97.03.05.18.12.38;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.07.31.07.00.23;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	;


desc
@@


1.9
log
@binary!!! in copy :(:(
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	OS Dependent Thingies
 *
 *      $Log: sysdep.C $
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



@


1.8
log
@SYSDEP_FRIP_SLEEP
@
text
@d8 3
d133 1
a133 1
    ifstream i( from );
d136 1
a136 1
    ofstream o( to );
@


1.7
log
@Do_Copy bug fix (NT/DOS versions - watcom bug?)
@
text
@d8 3
d236 1
a236 1

d245 1
a245 2


@


1.6
log
@Ver. 34
@
text
@d8 3
d139 3
a142 1
        
a143 2
        if( i.eof() )  return;
        if( i.bad() )  throw Ex_Fail( "Do_Copy", "Error reading from", from );
@


1.5
log
@Version 25
@
text
@d8 3
d228 11
@


1.4
log
@Exceptions added, not compiled
@
text
@d8 3
d39 1
a39 1
#include "dep_os2.ic"
d43 1
a43 1
#include "dep_nt.ic"
d47 1
a47 1
#include "dep_dos.ic"
d51 1
a51 1
#include "dep_unix.ic"
d176 1
a176 1
        parse( line, ffn, fdesc );
d179 1
a179 1
            return desc;
d182 1
a182 1
    Ex_EOF("find_in_list");
d185 1
a185 1
static bool put_in_list( const string &list, const string& fn, const string &desc )
d202 2
a203 2
    try   { return find_in_list( path+"\\files.bbs", fn );    }
    catch { return find_in_list( path+"\\descript.ion", fn ); }
d216 4
a219 4
    bool ret = Ok;
    try { put_in_list( path+"\\files.bbs", fn, desc )    } catch { ret = Err; }
    try { put_in_list( path+"\\descript.ion", fn, desc ) } catch { ret = Err; }
    try { set_ea_ASCII( file, ".SUBJECT", desc )         } catch { ret = Err; }
d221 1
a221 1
    throw Ex_Fail("set_desc","some method failed","");
@


1.3
log
@Before exceptions.
@
text
@d8 3
d94 1
a94 1
bool  ilock::lock()
a100 1
        //printf("errno = %d\n", errno );
d102 1
a102 3
            Error("Can't create lock file "+fn);
//perror("lock");
//printf("ret = %d, errno = %d\n", handle, errno );
a103 1
        return Err;
d105 2
a106 3
    
    we_own_the_lock_v = Yes;
    return Ok;
d116 1
a116 1
static bool Do_Copy( string from, string to )
d119 1
a119 5
    if( !i )
        {
        Error( "Can't open  "+from );
        return Err;
        }
d122 1
a122 6
    
    if( !o )
        {
        Error( "Can't create "+to );
        return Err;
        }
d132 3
a134 15
        if( o.bad() )
            {
            Error("error writing to "+to);
            return Err;
            }
        
        if( i.eof() )
            return Ok;
        
        if( i.bad() )
            {
            Error("error reading from "+from);
            return Err;
            }
        
a135 1
    
d140 1
a140 1
bool frip_rename( string from, string to )
d144 1
a144 5
        if( Err == Do_Copy( from, to ) )
            {
            Error("Can't rename or copy "+from+" to "+to);
            return Err;
            }
d146 1
a146 4
            {
            Error("Can't delete "+from);
            return Err;
            }
a147 1
    return Ok;
d150 1
a150 1
bool frip_copy( string from, string to )
d152 1
a152 6
    if( Err == Do_Copy( from, to ) )
        {
        Error("Can't copy "+from+" to "+to);
        return Err;
        }
    return Ok;
d161 1
a161 1
static bool find_in_list( const string &list, const string& fn, string &desc )
d164 1
a164 2

    if( !i )  return Err;
d176 1
a176 5
            {
            desc = fdesc;
            //Msg("Found desc "+desc);
            return Ok;
            }
d178 2
a179 1
    return Err;
d185 1
a185 6
    if( !o )
        {
        Error("Can't create or open "+list);
        return Err;
        }

a186 1
    return Ok;
d189 1
a189 1
bool get_desc( string file, string &desc )
d199 2
a200 3
    if( Err != find_in_list( path+"\\files.bbs", fn, desc ) ) return Ok;
    if( Err != find_in_list( path+"\\descript.ion", fn, desc ) ) return Ok;
    return Err;
d203 1
a203 1
bool set_desc( string file, string desc )
d214 5
a218 4
    if( Err == put_in_list( path+"\\files.bbs", fn, desc ) ) ret = Err;
    if( Err == put_in_list( path+"\\descript.ion", fn, desc ) ) ret = Err;
    if( Err == set_ea_ASCII( file, ".SUBJECT", desc ) ) ret = Err;
    return ret;
@


1.2
log
@FripMail written
@
text
@d7 4
a10 1
 *      $Log: sysdep.c $
d225 1
a225 1
    ofstream o( list );
d233 1
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:$
d15 5
d22 7
d34 1
a34 1
//#include "dep_nt.ic"
d38 1
a38 1
//#include "dep_dos.ic"
d42 18
a59 1
//#include "dep_unix.ic"
d61 208
@
