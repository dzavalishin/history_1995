/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Misc. code
 *
 *      $Log: util.C $
 *      Revision 1.17  1997/03/28 22:04:49  dz
 *      cleanup, cosmetics
 *
 *      Revision 1.16  1997/03/05 18:12:38  dz
 *      Ver. 34
 *
 *      Revision 1.14  1996/12/28 03:11:55  dz
 *      new string/istring/domain_s comparison ops
 *
 *      Revision 1.12  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.11  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.10  1996/07/31 07:00:23  dz
 *      FripMail written
 *
 *      Revision 1.7  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#include "frip.h"
#include "filer.h"
#include "stat.h"
#include "ansi.h"
#include <ctype.h>


#ifdef __IBMCPP__

// No hidden files!
#define ATTRS2FIND	(FILE_READONLY|FILE_SYSTEM|FILE_ARCHIVED)

bool filer::do_restart( string &name )
	{
        ULONG           count = 1;
        APIRET          rc;
	FILEFINDBUF3	ff = { 0 };

	h = HDIR_CREATE; 

	rc = DosFindFirst( mask, &h, ATTRS2FIND, &ff, sizeof(ff), &count, FIL_STANDARD );

	if( rc == NO_ERROR )
		{
		name = ff.achName;
		restart_flag = No;
                opened_flag = Yes;
                valid_v = Yes;
		return Yes;
		}

	if( rc == ERROR_NO_MORE_FILES )
		{
                //close();
                valid_v = No;
		return No;
		}
        
        valid_v = No;
	throw Ex_Errno( "filer::do_restart", "FindFirst error", rc );
	}


bool filer::do_continue( string &name )
	{
        ULONG           count = 1;
        APIRET          rc;
	FILEFINDBUF3	ff = { 0 };

	rc = DosFindNext( h, &ff, sizeof(ff), &count );

	if( rc == NO_ERROR )
		{
                name = ff.achName;
                valid_v = Yes;
		return Yes;
		}
        
        valid_v = No;
        
	if( rc == ERROR_NO_MORE_FILES )
		return No;

        throw Ex_Errno( "filer::do_restart", "FindNext error", rc );
	}


void filer::next()
    {
    switch( restart_flag ? do_restart( result ) : do_continue( result ) )
        {
        case No:                   valid_v = No;    break;
        default:                   valid_v = Yes;   break;
        }
    }


filer::filer( string dir, string mask_a )
    {
    mask = dir + "/" + mask_a;
    valid_v = No;
    restart_flag = Yes;
    opened_flag = No;
    next();
    }

filer::filer()
    {
    valid_v = No;
    restart_flag = Yes;
    opened_flag = No;
    }


bool filer::valid()
    {
    return valid_v;
    }
    

string filer::current()
    {
    return result;
    }
    

filer::~filer()
    {
    APIRET          rc;
    
    if( opened_flag )
        {
        restart_flag = Yes;
    
        if( (rc = DosFindClose( h )) != NO_ERROR )
            Error( "FindClose error" );
        }
    
    }
#else

#include <dos.h>

void filer::next()
    {
    do
        {
        if( dirp != NULL ) direntp = readdir( dirp );
        else
            {
            direntp = NULL;
            return;
            }
        if( direntp == 0 )
            return;

        string attr_fn = dir_v + "/" + string(direntp->d_name);
        unsigned at = 0, r = 0;
        r = _dos_getfileattr( attr_fn, &at );
        //printf("%-30s: r = 0x%x, at = 0x%x\n", attr_fn.c_str(), r, at );
        
        if( r )
            {
            debug( "filer::do_restart _dos_getfileattr failed at "+attr_fn);
            //continue;
            }
        else
            {
            if( (at & _A_HIDDEN) || (at & _A_SUBDIR) )
                {
                //printf("%s: skipped\n", attr_fn.c_str() );
                continue;
                }
            }

        }
    while( !fnmatch( direntp->d_name, mask ) );

    }

filer::filer( string dir, string mask_a )
    {
    dirp = opendir( dir );
    direntp = 0;
    mask = mask_a;
    dir_v = dir;
    next();
    }


bool filer::valid()
    {
    return (dirp == 0 || direntp == 0 ) ? No : Yes;
    }
    

string filer::current()
    {
    return direntp->d_name;
    }
    

filer::~filer()
    {
    closedir( dirp );
    }
#endif




static bool match_one( const char *data, const char *mask )
    {
    const char *mcp = mask, *ncp = data;

    bool match = No;

    while( *mcp && *ncp )
        {
        if(
           (toupper(*mcp) == toupper(*ncp)) ||
           *mcp == '?'
          )
            {
            mcp++, ncp++;
            continue;
            }
    
        if( mcp[0] == '*' && mcp[1] == 0 )
            return Yes;
        if( mcp[0] == '*' )
            throw Ex_Arg("match_one","* is not last in mask",mask);
        return No;
        }
    
    return (*mcp == *ncp) ? Yes : No;
    }



bool filer::fnmatch( const char *name, const char *mask )
    {
    char ndr[_MAX_DRIVE], ndi[_MAX_DIR], nfn[_MAX_FNAME], nex[_MAX_EXT];
    char mdr[_MAX_DRIVE], mdi[_MAX_DIR], mfn[_MAX_FNAME], mex[_MAX_EXT];
    _splitpath( (char *)name, ndr, ndi, nfn, nex );
    _splitpath( (char *)mask, mdr, mdi, mfn, mex );
    
    if(0 == stricmp( nex, mex ) && 0 == strcmp(mfn,"*")) return Yes;

    return
        (match_one(nex,mex) && match_one(nfn,mfn))
        ? Yes : No;
    }





bool our_aka_matches( const addr &a )
    {

    if( conf.aka.count() == 0 )
        throw Ex_Arg( "our_aka_matches", "No our addresses defined", "" );

    for( int i = 0; i < conf.aka.count(); i++ )
        //if( match_addr(conf.aka[i],a) ) return Yes;
        if( conf.aka[i].matches(a) ) return Yes;

    return No;
    }

bool is_our_aka( const addr &a )
    {

    if( conf.aka.count() == 0 ) return No;

    for( int i = 0; i < conf.aka.count(); i++ )
        if( conf.aka[i] == a ) return Yes;

    return No;
    }


  // -----------------------------------------------------------------------

void tempFile( fstream &f, string &outname, const string &mask, int mode )
    {
    int tries = 20;
    do
        {
        if( tries-- < 0 )  throw Ex_Fail("tempFile", "Tired of trying", mask);
        outname = tempFileName(mask);
        f.open( outname, mode );
        }
    while( !f );
    }


string tempFileName( const string &mask )
    {
    long  t = time(NULL);
    int   tries = 100;
    static long int modifier = 0;
    
    modifier += 0x5005;  // Чтобы дважды одно имя не pожать даже если такого файла нет
    
    char   name[600], ts[40];
    do
        {
        t += time(NULL);
        t += modifier;
        
        strncpy( name, mask, 600 );
        sprintf( ts, "%ld", t%99999 );
        
        const char *i = ts;
        char *o = name;
        
        while( *i && *o )
            {
            if( *o == '?' ) *o++ = *i++;
            else o++;
            }
        
        while( *o )
            {
            if( *o == '?' ) *o = '0';
            o++;
            }
        
        if( o==name || (tries-- == 0) )  // No '?' in name? Out of luck?
            throw Ex_Fail( "tempFileName", "I've tried enough!", "" );
        
        }
#ifdef __IBMCPP__
    while( 0 == access(name, 0));
#else
    while( 0 == access(name, F_OK));
#endif
    
    return name;
    }

  // -----------------------------------------------------------------------


string newdir( const char *name, string dir )
    {
    char ndr[_MAX_DRIVE], ndi[_MAX_DIR], nfn[_MAX_FNAME], nex[_MAX_EXT];
    _splitpath( (char *)name, ndr, ndi, nfn, nex );
    
    return dir+"\\"+string(nfn)+string(nex);
    }

void mkpath( const string &path )
    {
    for( int i = 0; i < path.length(); i++ )
        {
        if( path[i] != '/' && path[i] != '\\' )
            continue;
        string dir = path.substr( 0, i );
          //debug("mkpath "+dir );
        
        mkdir( (char *)(const char *)dir );
        }
    
    }



  // -----------------------------------------------------------------------

void statistics::report()
    {
    string st;

    if(no_good_v||new_addr_v||better_repl_v||timeout_repl_v||filtered_v)
        {
        st.print( "RIPs: %3d ignored, %3d new, %3d better, %3d timed out, %3d filtered",
                no_good_v, new_addr_v, better_repl_v, timeout_repl_v, filtered_v );
        BoldMsg( st );
        }

    if(rifs_total_v||rifs_for_us_v||rifs_forw_v||rifs_held_v)
        {
        st.print( "RIFs: %3d total, %3d for us, %3d forwarded, %3d held",
                rifs_total_v, rifs_for_us_v, rifs_forw_v, rifs_held_v );
        BoldMsg( st );
        }
    }

statistics stats;

  // -----------------------------------------------------------------------

void Log( const char *p, const string &s )
    {
    if( !conf.log() ) return;
    conf.log_stream() << p << s << "\n";
    }


void StatLog( const string &s )
    {
    if( !conf.stat_log() ) return;
    conf.stat_log_stream() << s << "\n";
    }



void Error(string s)
    {
    if( conf.ansi() )
        fprintf( stderr,
                ansi_red ansi_intense "Error:" ansi_normal
                " %s\n", (const char *)s );
    else
        fprintf( stderr, "Error: %s\n", (const char *)s );
    
    Log( "Error: ", s );
    }

void Warning(string s)
    {
    if( conf.ansi() )
        fprintf( stderr,
                ansi_magenta ansi_intense "Warning:" ansi_normal
                " %s\n", (const char *)s );
    else
        fprintf( stderr, "Warning: %s\n", (const char *)s );
    
    Log( "Warning: ", s );
    }


void Msg(string s)
    {
    if( conf.ansi() )
        fprintf( stderr,
                ansi_green ansi_intense "*** " ansi_normal
                "%s\n", (const char *)s );
    else
        fprintf( stderr, "*** %s\n", (const char *)s );
    Log( "*** ", s );
    }

void BoldMsg(string s)
    {
    if( conf.ansi() )
        fprintf( stderr,
                ansi_brown ansi_intense "*!* " ansi_normal
                "%s\n", (const char *)s );
    else
        fprintf( stderr, "*!* %s\n", (const char *)s );
    
    Log( "*!* ", s );
    }


void General_Ex::print() const
    {
    Error( where+": "+what+" ("+why+")" );
    }










