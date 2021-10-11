head	1.8;
access;
symbols
	Beta_12:1.3;
locks;
comment	@ * @;


1.8
date	97.03.05.18.12.38;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.01.22.04.35.34;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.01.13.09.48.05;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.11.26.13.58.02;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.16.00.54.44;	author dz;	state Exp;
branches;
next	;


desc
@@


1.8
log
@Ver. 34
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Binkley interface
 *
 *      $Log: Bink.C $
 *      Revision 1.7  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.6  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.5  1996/01/22 04:35:34  dz
 *      C Set & STL support
 *
 *      Revision 1.4  1996/01/13 09:48:05  dz
 *      *** empty log message ***
 *
 *      Revision 1.3  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#include "frip.h"
#include <fcntl.h>
#include <share.h>
#include <errno.h>


class bsy_flag
    {
    string fn_v;
    public:
        bsy_flag( const string name );
        ~bsy_flag();
    
    };

bsy_flag::bsy_flag( const string fn_base )
    {
    int handle;
    bool asleep = No;
    string fn1 = fn_base + ".b$y";
    string fn2 = fn_base + ".bsy";

    fn_v = fn2;

    while(!Stop_Frip)
        {
          // BSY exist. We need to check it here because some NFS mounts do not support locking :(:(
        if(0 != access(fn2, 0))
            {

            if( (handle = sopen( fn1, O_RDWR|O_CREAT|O_EXCL|O_NOINHERIT, SH_DENYRW, 0666 )) >= 0 )
                {
                close(handle);

                if( 0 == rename( fn1, fn2 ) )
                    {
                    if(asleep)   Msg("Awaken (lock file "+fn2+")");
                    return;
                    }
                unlink( fn1 );
                }

            if( errno == EMFILE || errno == ENOENT )
                throw Ex_Errno("bsy_flag::bsy_flag","Can't create lock file "+fn2,errno);
            }

        if( !asleep )        Msg("Sleeping on lock file "+fn2);
        asleep = Yes;
        
        frip_sleep( 4 ); // 4 seconds between tries
        }

    throw Ex_Fail("bsy_flag::bsy_flag","Interrupted while trying to create lock",fn2);
    }

bsy_flag::~bsy_flag()
    {
    chmod(fn_v, 0666);
    unlink(fn_v);
    }


static string short_dom( const string &s )
    {
    
    for( int i = 0; i < s.length(); i++ )
        if( s[i] == '.' )
            return s.substr( 0, i );
    
    return s;
    }

void BinkAttach( const string &file, const addr &to, bool hold )
    {
    if( conf.bink_outbound().is_empty() || conf.bink_outbound_default().is_empty() )
        return;
    
    mkdir((char *)(const char *)conf.bink_outbound());
    string _lo_base = conf.bink_outbound() + "/";
    
    bool do_zone = No;
    
    if( to.domain() != conf.aka[0].domain() )
        {
        _lo_base += short_dom(to.domain());
        do_zone = Yes;
        }
    else
        _lo_base += conf.bink_outbound_default();
    
    if( do_zone || (to.zone() != conf.aka[0].zone()) )
        {
        char zs[30];
        sprintf( zs, ".%03X", to.zone() % 0xFFF );
        _lo_base += zs;
        }
    
    mkdir( (char *)(const char *)_lo_base );
    
    char nn[60];
    sprintf( nn, "/%04X%04X", to.net(), to.node() );
    _lo_base += nn;
    
    if( to.point() )
        {
        char p[40];
        sprintf( p, ".pnt", to.point() );
        _lo_base += p;
        mkdir( (char *)(const char *)_lo_base );
        sprintf( p, "/0000%04X", to.point() );
        _lo_base += p;
        }
    
    char  flavor = hold ? 'H' : 'D';
    char ext[40];
    sprintf( ext, ".%cLO", flavor );
    string  _lo = _lo_base + ext;

    /*+ ".Bsy" */
    bsy_flag bsy( _lo_base );
    
    // debug( "lo name: " + _lo_base );
    
//#ifdef __IBMCPP__
    ofstream f_lo( _lo, ios::app );
//#else
//    ofstream f_lo( _lo, ios::append );
//#endif
    
    if( !f_lo )
        throw Ex_Fail("BinkAttach","Can't create ?LO, left unattached", _lo_base );
    
    f_lo << "^" << ((const char *)file) << "\n";
    }



void HPFSAttach( const string &file, const addr &to, bool hold )
    {
    if( conf.hpfs_outbound().is_empty() )
        return;
    
#ifdef __DOS__
    Error("Can't use HPFS outbound in DOS!");
#else
    mkdir( (char *)(const char *)conf.hpfs_outbound() );
    
    string _lo_base = conf.hpfs_outbound() + "/";
    
    _lo_base += short_dom(to.domain());
    
    char cc[80];
    sprintf( cc, ".%d.%d.%d", to.zone(), to.net(), to.node() );
    _lo_base += cc;
    
    //if( to.point() )
        {
        sprintf( cc, ".%d", to.point() );
        _lo_base += cc;
        }
    
    char  flavor = hold ? 'H' : 'D';
    sprintf( cc, ".%cLO", flavor );
    string _lo = _lo_base + cc;

    bsy_flag bsy( _lo_base ); /*+ ".Bsy"*/
    
    // debug( "lo name: " + _lo );
    
//#ifdef __IBMCPP__
    ofstream f_lo( _lo, ios::app );
//#else
//    ofstream f_lo( _lo, ios::append );
//#endif
    
    if( !f_lo )
        throw Ex_Fail("HPFSAttach","Can't create ?LO, left unattached", _lo );
    
    f_lo << "^" << ((const char *)file) << "\n";
    
#endif // __DOS__
    }






@


1.7
log
@Exceptions added, not compiled
@
text
@d8 3
d28 60
d105 1
a105 1
    string _lo = conf.bink_outbound() + "/";
d111 1
a111 1
        _lo += short_dom(to.domain());
d115 1
a115 1
        _lo += conf.bink_outbound_default();
d121 1
a121 1
        _lo += zs;
d124 1
a124 1
    mkdir( (char *)(const char *)_lo );
d128 1
a128 1
    _lo += nn;
d134 2
a135 2
        _lo += p;
        mkdir( (char *)(const char *)_lo );
d137 1
a137 1
        _lo += p;
d143 4
a146 1
    _lo += ext;
d148 1
a148 1
    // debug( "lo name: " + _lo );
d150 1
a150 1
#ifdef __IBMCPP__
d152 3
a154 3
#else
    ofstream f_lo( _lo, ios::append );
#endif
d157 1
a157 1
        throw Ex_Fail("BinkAttach","Can't create ?LO, left unattached", _lo );
a165 1
#ifndef __DOS__
d169 3
d174 1
a174 1
    string _lo = conf.hpfs_outbound() + "/";
d176 1
a176 1
    _lo += short_dom(to.domain());
d180 1
a180 1
    _lo += cc;
d185 1
a185 1
        _lo += cc;
d190 3
a192 1
    _lo += cc;
d196 1
a196 1
#ifdef __IBMCPP__
d198 3
a200 3
#else
    ofstream f_lo( _lo, ios::append );
#endif
@


1.6
log
@Before exceptions.
@
text
@d8 3
d36 1
a36 1
bool BinkAttach( const string &file, const addr &to, bool hold )
d38 2
a39 3
    if( conf.bink_outbound().is_empty() ||
       conf.bink_outbound_default().is_empty() )
        return Ok;
a41 1
    
a46 1
    //if( !addr::dom_eq( to.domain(), conf.aka[0].domain()) )
d91 1
a91 4
        {
        Error( "Can't create ?LO: " + _lo + ", left unattached " + file );
        return Err;
        }
a93 2
    
    return Ok;
d98 1
a98 1
bool HPFSAttach( const string &file, const addr &to, bool hold )
d102 1
a102 1
        return Ok;
d133 1
a133 4
        {
        Error( "Can't create ?LO: " + _lo + ", left unattached " + file );
        return Err;
        }
a137 1
    return Ok;
@


1.5
log
@C Set & STL support
@
text
@d7 4
a10 1
 *      $Log: Bink.c $
@


1.4
log
@*** empty log message ***
@
text
@d8 3
d36 1
a36 1
    mkdir(conf.bink_outbound());
d58 1
a58 1
    mkdir( _lo );
d69 1
a69 1
        mkdir( _lo );
d81 3
d85 1
d106 1
a106 1
    mkdir( conf.hpfs_outbound() );
d128 3
d132 1
@


1.3
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d39 2
a40 2
    //if( to.domain() != conf.aka[0].domain() )
    if( !addr::dom_eq( to.domain(), conf.aka[0].domain()) )
@


1.2
log
@beta 9
@
text
@d1 11
@


1.1
log
@Initial revision
@
text
@d13 1
a13 1
bool BinkAttach( const string &file, const addr &to )
d19 2
d57 1
a57 1
    char  flavor = 'D';
d79 1
a79 1
bool HPFSAttach( const string &file, const addr &to )
d81 1
d101 1
a101 1
    char  flavor = 'D';
d117 1
@
