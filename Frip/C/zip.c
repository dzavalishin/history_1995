/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Compression/expansion interface
 *
 *      $Log: zip.c $
 *      Revision 1.12  1997/03/17 08:03:14  dz
 *      unzip code moved in
 *
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
    if( ripnames_v.size() == 0 )
        {
        Warning("Tried to zip nothing!? Will not.");
        return;
        }
#ifdef ZIP_LIST_FILE

    fstream lif;
    string  tname;
    
    tempFile( lif, tname, conf.dir()+"/out/riz?????.lst", ios::noreplace|ios::out );

    for( int i = 0; i < ripnames_v.size(); i++ )
        lif << (ripnames_v[i]).c_str() << "\n";

    if( lif.fail() ) throw Ex_Fail("RipZip::do_zip","Can't write list file", tname );
    lif.close();
    
    string cmd = conf.zipcmd()+" "+zipfile_v+" @"+tname;
    
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
                Error("Error deleting "+fn1+": "+string(s)+", will retry in a 2 seconds.");
                //Error("Error deleting "+fn1);
                frip_sleep( 2 );
                if( unlink( fn1 ) )
                    throw Ex_Errno("FripApp::unzip_in","Error deleting "+fn1, errno);
                }
            }
        eat_rips_in( "." );
        if( Stop_Frip ) break;
        }
    }

