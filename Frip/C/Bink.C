/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Binkley interface
 *
 *      $Log: Bink.C $
 *      Revision 1.8  1997/03/05 18:12:38  dz
 *      Ver. 34
 *
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






