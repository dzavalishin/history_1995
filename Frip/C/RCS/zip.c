head	1.12;
access;
symbols
	Beta_12:1.4;
locks;
comment	@ * @;


1.12
date	97.03.17.08.03.14;	author dz;	state Exp;
branches;
next	1.11;

1.11
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.10;

1.10
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.9;

1.9
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.8;

1.8
date	96.07.31.07.26.25;	author dz;	state Exp;
branches;
next	1.7;

1.7
date	96.01.22.04.35.34;	author dz;	state Exp;
branches;
next	1.6;

1.6
date	96.01.18.00.56.53;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.01.13.09.48.05;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.01.07.20.21.01;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.01.03.20.27.32;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.12.05.04.23.40;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.26.13.58.02;	author dz;	state Exp;
branches;
next	;


desc
@@


1.12
log
@unzip code moved in
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Compression/expansion interface
 *
 *      $Log: zip.c $
 *      Revision 1.11  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.10  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.9  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.8  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.7  1996/01/22 04:35:34  dz
 *      C Set & STL support
 *
 *      Revision 1.6  1996/01/18 00:56:53  dz
 *      Beta 15
 *
 *      Revision 1.5  1996/01/13 09:48:05  dz
 *      *** empty log message ***
 *
 *      Revision 1.4  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#include "zip.h"
#include "frip.h"
#include "filer.h"
#include <process.h>

#if 0
class RipZip
    {
    string                         zipfile_v;
    vector            <string>     ripnames_v;
    addr                           dest_v;
    
public:
    
    RipZip() {}
    RipZip( const addr & );

    
    void                           add( const string &rip_name )
        {
        ripnames_v.push_back( rip_name );
        }
    void                           zip();
    void                           do_zip();
    const string &                 name() const { return zipfile_v; }
    const addr &                   dest() const { return dest_v;    }
    
    int operator ==                ( const RipZip &r ) const
        {
        return dest_v == r.dest_v && ripnames_v == r.ripnames_v
            && zipfile_v == r.zipfile_v;
        }
    };

class zip_list
    {
    private:
        vector  <RipZip>  zips_v;
        
    public:
        void              add( const string &ripfile, const addr &dest );
        void              send();
        
        ~zip_list() { send(); }
        
        
    };




#endif


static void exec_zip( const string & zip, const string & rips )
    {
    bool ret;
    string cmd = conf.zipcmd()+" "+zip+rips;
    
#if defined( __DOS__ ) || defined( __NT__ )
    int l = cmd.length();
    for( int i = 0; i <  l; i++ )
        if( cmd[i] == '/' ) cmd[i] = '\\';
#endif
    
    if( system(cmd) )
        throw Ex_Fail( "exec_zip", "Can't run ", cmd );
    }


#if defined(__OS2__) || defined(__NT__)
void RipZip::do_zip()
    {
    string rips;
    bool  zipit = No;
    int   headlen = zipfile_v.length() + conf.zipcmd().length();
    
    int extra = 0; // at least 1 for prog. name
    string cmd = conf.zipcmd(), _;
    while( cmd.strip_ws(),!cmd.is_empty() )
        {
        cmd.parse( _, cmd );
        extra++;                      // reserve space for one more arg.
        }
    
      // +2 = one for zip name and one for trailing NULL
    const char ** av = new const char *[ripnames_v.size() + extra + 2];
    
    string *shold = new string [extra];
    
    if( av == NULL || shold == NULL ) 
        {
        if( av ) delete av;
        throw Ex_Fail("","nomem",""); // try to consume as little memory as we can
        }

    cmd = conf.zipcmd();
    cmd.strip_ws();
        
    for( int i = 0; i < extra; i++ )
        {
        cmd.parse( shold[i], cmd );
        av[i] = (const char *)shold[i];
        }
        
    av[i++] = zipfile_v;

    for( int j = 0; j < ripnames_v.size(); i++, j++ )
        {
        if(i >= ripnames_v.size() + extra + 1)
            {
            delete [] shold;
            delete [] av;
            throw Ex_Fail( "RipZip::do_zip", "i >= ripnames_v.size() + extra + 1!", zipfile_v );
            }
        av[i] = (const char *) ripnames_v[j];
        }

    av[i] = 0;
        
    bool  err = No;
#ifdef __IBMCPP__
    if( spawnvp( P_WAIT, (char *)av[0], (char **)av ) != 0 )
#else
    if( spawnvp( P_WAIT, av[0], /*(char **)*/av ) != 0 )
#endif
        err = Yes;
        
    delete [] shold;
    delete [] av;

    if( err ) throw Ex_Fail( "RipZip::do_zip", "can't zip", zipfile_v );
    }

#else // defined(__OS2__)||defined(__NT__)

#define ZIP_LIST_FILE 1

void RipZip::do_zip()
    {
#ifdef ZIP_LIST_FILE

    fstream lif;
    string  tname;
    
    tempFile( lif, tname, conf.dir()+"/out/riz?????.lst", ios::noreplace|ios::out );

    for( int i = 0; i < ripnames_v.size(); i++ )
        lif << (ripnames_v[i]).c_str() << "\n";

    if( lif.fail() ) throw Ex_Fail("RipZip::do_zip","Can't write list file", tname );
    lif.close();
    
    string cmd = conf.zipcmd()+" "+zipfile_v+" @@"+tname;
    
#if defined( __DOS__ ) || defined( __NT__ )
    int l = cmd.length();
    for( i = 0; i <  l; i++ )
        if( cmd[i] == '/' ) cmd[i] = '\\';
#endif

    debug("zipping: "+cmd);
    
    if( system(cmd) )
        {
        unlink(tname);
        throw Ex_Fail( "RipZip::do_zip", "Can't run pkzip", cmd );
        }
    unlink(tname);

#else // ZIP_LIST_FILE
    string rips;
    bool  err = No;
    bool  zipit = No;
    int   headlen = zipfile_v.length() + conf.zipcmd().length();

    for( int i = 0; i < ripnames_v.size(); i++ )
        {
        string &r = ripnames_v[i];
        
        if( rips.length() + r.length() + headlen >= conf.cmdlimit() )
            {
            try { exec_zip( zipfile_v, rips ); }
            catch(General_Ex ex) { ex.print(); err = Yes; }
            rips = "";
            zipit = No;
            }
        
        rips += " ";
        rips += r;
        zipit = Yes;
        }
    
    if( zipit )
        {
        try { exec_zip( zipfile_v, rips ); }
        catch(General_Ex ex) { ex.print(); err = Yes; }
        }
#endif // ZIP_LIST_FILE
    }
#endif // defined(__OS2__)||defined(__NT__)



void RipZip::zip()
    {
    try { do_zip(); }
    catch( General_Ex ex )
        {
        ex.print();
        Error("Unable to zip, sending unzipped rips");
        for( int i = 0; i < ripnames_v.size(); i++ )
            {
            debug("Sending "+ripnames_v[i]+" to "+string(dest_v));
            deliver( dest_v, ripnames_v[i] );
            }
        }
    for( int i = 0; i < ripnames_v.size(); i++ )
        ripnames_v.erase(ripnames_v.begin()+i);
    }

RipZip::RipZip( const addr &a )
    {
    zipfile_v = tempFileName(conf.dir()+"/out/riz?????.riz");
    dest_v = a;
    }


void zip_list::add( const string &ripfile, const addr &dest )
    {
    //debug( "ziplist adding "+ripfile+" for "+((string)dest) );
          
    for( int i = 0; i < zips_v.size(); i++ )
        {
        RipZip &r = zips_v[i];
        if( r.dest() == dest )
            {
            r.add( ripfile );
            return;
            }
        }

    RipZip r( dest );
    r.add( ripfile );
    zips_v.push_back( r );
    }

void zip_list::send()
    {
    bool error = No;
    
    for( int i = 0; i < zips_v.size(); i++ )
        {
        RipZip &r = zips_v[i];
        try         { r.zip();      }
        catch(...)  { error = Yes;  }
        
        deliver( r.dest(), r.name() );
        }

    
    
    if( error ) throw Ex_Fail("zip_list::send()", "error in zip", "" );
    else        zips_v.erase(zips_v.begin(),zips_v.end()); // Remove all - we sent 'em anyway
    }




// ------------------------------------------------------------------------- 



void FripApp::unzip_in( const string &inb )
    {
    const string our ( conf.dir() );
    
    for( filer finriz(inb,"*.riz"); finriz.valid(); finriz.next() )
        {
        string fn1=inb+"/"+finriz.current();

        // we need to unzip "rif*.*" too!!
        //string cmd = conf.unzipcmd() +" "+ fn1 + " *.rip";
        string cmd = conf.unzipcmd() +" "+ fn1 + " ri*.ri?";
        
#if defined( __DOS__ ) || defined( __NT__ )
        int l = cmd.length();
        for( int i = 0; i <  l; i++ )
            if( cmd[i] == '/' ) cmd[i] = '\\';
#endif
        debug("Executing "+cmd );
        if( system( cmd ) )
            Error("Error executing "+cmd );
        else
            {
            debug("Deleting "+fn1 );
            chmod( fn1, 0666 );
            if( unlink( fn1 ) )
                {
                char s[30]; sprintf(s,"%d", errno );
                Error("Error deleting "+fn1+": "+string(s));
                //Error("Error deleting "+fn1);
                }
            }
        eat_rips_in( "." );
        if( Stop_Frip ) break;
        }
    }

@


1.11
log
@Version 25
@
text
@d8 3
d38 1
d305 11
d317 27
@


1.10
log
@Exceptions added, not compiled
@
text
@d8 3
d33 1
d37 1
d54 2
a55 1
    bool                           zip();
d66 16
d83 1
a83 1
#define SPAWN_MODE 1
d102 2
a103 2

void RipZip::zip()
a105 1
    bool  rc = Ok;
a108 2
#if defined( SPAWN_MODE ) && (defined(__OS2__)||defined(__NT__))
    
a109 1
    
d113 2
a114 2
        parse( cmd, _, cmd );
        extra++;                      // reserver space fopr one more arg.
d117 1
a117 1
    // +2 = one for zip name and one for trailing NULL
d119 1
a119 1

a123 1
        rc = Err;
d125 1
d127 5
a131 1
    else
d133 3
a135 2
        cmd = conf.zipcmd();
        cmd.strip_ws();
d137 5
a141 1
        for( int i = 0; i < extra; i++ )
d143 3
a145 3
            parse( cmd, shold[i], cmd );
            av[i] = (const char *)shold[i];
            //fprintf( stderr, "av[%d] = %s\n", i, av[i] );
d147 4
d152 1
a152 15
        av[i++] = zipfile_v;
        //fprintf( stderr, "av[%d] = %s\n", i-1, av[i-1] );
        
        for( int j = 0; j < ripnames_v.size(); i++, j++ )
            {
            if(i >= ripnames_v.size() + extra + 1)
                {
                Error("i >= ripnames_v.size() + extra + 1!");
                break;
                }
            av[i] = (const char *) ripnames_v[j];
            }
        
        av[i] = 0;
        
d154 1
a154 1
        if( spawnvp( P_WAIT, (char *)av[0], (char **)av ) != 0 )
d156 1
a156 1
        if( spawnvp( P_WAIT, av[0], /*(char **)*/av ) != 0 )
d158 1
a158 1
            rc = Err;
d160 26
a185 2
        delete [] shold;
        }
d187 7
a193 1
#else
d195 13
d214 2
a215 2
            if( Ok != exec_zip( zipfile_v, rips ) )
                rc = Err;
a225 6
        if( Ok != exec_zip( zipfile_v, rips ) )
            rc = Err;
    
#endif
    
    if( rc != Ok )
d227 16
a242 1
        Error(string("Unable to zip, sending unzipped rips"));
a247 1
        throw Ex_Fail("RipZip::zip()","unable to zip",zipfile_v);
d249 2
a255 7
    /*
    if( zipfile_v=="" )
        {
        throw Ex_Fail( "riz generation aborted - no temp file name" );
        exit( 33 );
        }
        */
d260 1
a260 19
class ZipList
    {
    private:
        vector  <RipZip>  zips_v;

    public:
        void              add( const string &ripfile, const addr &dest );
        void              send();

        ~ZipList() { send(); }
        
    
    };


static ZipList   zlist;


void ZipList::add( const string &ripfile, const addr &dest )
d279 1
a279 1
void ZipList::send()
d286 2
a287 2
        try     { r.zip()       }
        catch   { error = Yes;  }
d291 2
d294 2
a295 1
    if( error ) throw Ex_Fail("ZipList::send()", "error in zip", "" );
a297 5

void ZipList_add( const string &ripfile, const addr &dest )
    {
    zlist.add( ripfile, dest );
    }
@


1.9
log
@Before exceptions.
@
text
@d8 3
d43 1
d64 1
a64 1
bool exec_zip( const string & zip, const string & rips )
d75 2
a76 4
    ret = system(cmd) ? Err : Ok;
    if( ret != Ok )
        Error("Can't run "+cmd );
    return ret;
a78 10
#if 0
bool unzip_all()
    {
    string cmd = conf.unzipcmd() +" *.riz *.rip";
    ret = system(cmd) ? Err : Ok;
    if( ret != Ok )
        Error("Can't run "+cmd );
    return ret;
    }
#endif
d80 2
a81 1
bool RipZip::zip()
d175 2
d178 2
a180 2

    return rc;
d186 1
d189 1
a189 1
        Error( "riz generation aborted - no temp file name" );
d192 1
d204 4
a207 1
        bool              send();
d234 1
a234 1
bool ZipList::send()
d236 1
a236 1
    bool rc = Ok;
d241 2
a242 5
        if( Ok != r.zip() )
            {
            Error( "Can't zip "+r.name() );
            rc = Err;
            }
d247 1
a247 1
    return rc;
a256 4
bool ZipList_send()
    {
    return zlist.send();
    }
@


1.8
log
@Clean up of watcom classlib based code
@
text
@d8 3
d155 1
a155 1
    for( int i = 0; i < ripnames_v.entries(); i++ )
d157 1
a157 1
        string &r = *(ripnames_v[i]);
d190 1
a190 1
    zipfile_v = tempFileName(conf.dir()+"/out/rip?????.riz");
@


1.7
log
@C Set & STL support
@
text
@d7 4
a10 1
 *      $Log: Zip.c $
a29 1
#if USE_STL
a30 3
#else
    WCPtrOrderedVector<string>     ripnames_v;
#endif
a39 1
#if USE_STL
a40 3
#else
        ripnames_v.insert( new string( rip_name ) );
#endif
a103 1
#if USE_STL
d105 1
a105 3
#else
    const char ** av = new const char *[ripnames_v.entries() + extra + 2];
#endif
a127 1
#if USE_STL
a136 14
#else
        for( int j = 0; j < ripnames_v.entries(); i++, j++ )
            {
            
            if(i >= ripnames_v.entries() + extra + 1)
                {
                Error("i >= ripnames_v.entries() + extra + 1!");
                break;
                }
            
            av[i] = (const char *) *(ripnames_v[j]);
            //fprintf( stderr, "av[%d] = %s\n", i, av[i] );
            }
#endif
a177 1
#if USE_STL
a179 7
#else
        for( int i = 0; i < ripnames_v.entries(); i++ )
            {
            string &r = *(ripnames_v[i]);
            deliver( dest_v, r );
            }
#endif
d199 6
a204 8
#if USE_STL
    vector            <RipZip>  zips_v;
#else
    WCPtrOrderedVector<RipZip>  zips_v;
#endif
public:
    void                        add( const string &ripfile, const addr &dest );
    bool                        send();
a215 1
#if USE_STL
a228 17
#else
    for( int i = 0; i < zips_v.entries(); i++ )
        {
        RipZip &r = *(zips_v[i]);
        if( r.dest() == dest )
            {
            r.add( ripfile );
            return;
            }
        }

    //debug("creating new one");
    
    RipZip &r = *new RipZip( dest );
    r.add( ripfile );
    zips_v.insert( &r );
#endif
a234 1
#if USE_STL
a245 15
#else
    for( int i = 0; i < zips_v.entries(); i++ )
        {
        RipZip &r = *(zips_v[i]);
        if( Ok != r.zip() )
            {
            Error( "Can't zip "+r.name() );
            rc = Err;
            }
        
        //debug( "zip delivering "+r.name()+" to "+((string)r.dest()) );
        
        deliver( r.dest(), r.name() );
        }
#endif
@


1.6
log
@Beta 15
@
text
@d8 3
d27 3
d31 1
d36 2
a37 1
                                   RipZip( const addr & );
d41 3
d45 1
d109 3
d113 1
d136 11
d159 1
d163 3
d167 1
d200 5
a204 1
        Error("Unable to zip, sending unzipped rips");
d210 1
d230 3
d234 1
d249 15
d279 1
d286 13
d312 1
@


1.5
log
@*** empty log message ***
@
text
@d8 3
d70 1
a70 1
    string cmd = conf.unzipcmd() /* +" *.riz *.rip" */;
@


1.4
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d132 1
a132 1
        if( spawnvp( P_WAIT, av[0], (char **)av ) != 0 )
@


1.3
log
@before unzip
@
text
@d1 11
d61 10
a70 1

@


1.2
log
@before COW strings
@
text
@d2 1
d29 3
a31 9
/*
#ifdef __DOS__
#define CMDLIMIT 100
//#define ZIP_CMD "pkzip -m -es"
#else
#define CMDLIMIT 480
//#define ZIP_CMD "zip -mj1g "
#endif
*/
d59 58
d137 2
@


1.1
log
@Initial revision
@
text
@d28 2
a29 2

#ifdef __MSDOS__
d36 1
d41 1
a41 1
    string cmd = conf.zipcmd()+zip+rips;
d43 1
a43 1
#ifdef __MSDOS__
d62 1
d68 1
a68 1
        if( rips.length() + r.length() + zipfile_v.length() + 10 >= CMDLIMIT )
@
