/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Approvement system
 *
 *      $Log: Approve.C $
 *      Revision 1.6  1996/09/08 19:05:31  dz
 *      Version 25
 *
 *      Revision 1.5  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.4  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.3  1996/07/31 07:26:25  dz
 *      Clean up of watcom classlib based code
 *
 *      Revision 1.2  1996/01/22 04:35:34  dz
 *      C Set & STL support
 *
 *      Revision 1.1  1996/01/13 09:48:05  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#include "frip.h"
#include "approve.h"
#include "filer.h"


void approve::insert( const app_elem &ae )
    {
    set_v.insert( ae );
    }

bool approve::lookup( const app_elem &ae )
    {
    return set_v.count( ae ) > 0 ? Yes : No;
    }

bool approve::is_approved( const addr &who, const addr &what )
    {
    if( !done_load ) load();
    return lookup( app_elem(who,what) ); 
    }


void approve::load_line( const string &orig )
    {
    string  swho, swhat;
    
    if( !orig.parse( swho, swhat ) )
        throw Ex_Arg("approve::load_line","Syntax error in .ap line",orig);
    //addr   who( swho ), what( swhat );
    insert( app_elem(swho, swhat) );
    }



void approve::load_file( const string &fn )
    {
    debug( "Loading approvements from "+fn );
    FILE *f = fopen( fn, "r" );
    
    if( f == NULL )
        throw Ex_Errno("approve::load_file","Can't open file"+fn, errno );

    string l;
    while( 1 )
        {
        try { l.load( f ); } catch( Ex_EOF ) { break; }
        
        l.strip_ws();
        l.strip_crlf();
        
        if(l[0] == ';' || l.is_empty() )
            continue;

        try { load_line(l); }
        catch( General_Ex ex ) { ex.print() ; }
        }

    fclose( f );
    }


void approve::load()
    {
    if( done_load ) return;
    debug("Loading .ap files...");
    
    for( filer f(conf.dir(),"*.ap"); f.valid(); f.next() )
        {
        try   { load_file( f.current() ); }
        catch( General_Ex ex ) { ex.print() ; }
        }
    }



