head	1.6;
access;
symbols;
locks;
comment	@ * @;


1.6
date	96.09.08.19.05.31;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	96.07.31.07.26.25;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	96.01.22.04.35.34;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	96.01.13.09.48.05;	author dz;	state Exp;
branches;
next	;


desc
@@


1.6
log
@Version 25
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	Approvement system
 *
 *      $Log: Approve.C $
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



@


1.5
log
@Exceptions added, not compiled
@
text
@d8 3
d30 1
d54 1
a54 1
    if( !parse( orig, swho, swhat ) )
d68 1
a68 1
        throw Ex_Errno("approve::load_file","Can't open file",fn );
d71 1
a71 1
    while( l.load( f ) == Ok )
d73 2
d82 1
a82 1
        catch( general_Ex ex ) { ex.print() ; }
d91 1
a91 1
    if( done_load ) return Ok;
d97 1
a97 1
        catch( general_Ex ex ) { ex.print() ; }
@


1.4
log
@Before exceptions.
@
text
@d8 3
d29 1
a29 1
bool approve::insert( const app_elem &ae )
a31 1
    return Ok;
d46 1
a46 1
bool approve::load_line( const string &orig )
a47 2
    debug( "Appr. line: "+orig );
    
d51 3
a53 8
        {
        Error("Syntax error in .ap line: "+orig );
        return Err;
        }
    
    addr   who( swho ), what( swhat );
    
    return insert( app_elem(swho, swhat) );
d58 1
a58 3


bool approve::load_file( const string &fn )
a60 1
    
d64 1
a64 4
        {
        Error( "Can't open "+fn+"!");
        return Err;
        }
a66 1
    
d75 2
a76 6
        if( load_line(l) == Err )
            {
            Error("Error in line "+l+" of "+fn);
            //fclose( bf );
            //return Err;
            }
a79 1
    return Ok;
d83 1
a83 1
bool approve::load()
a86 1
    bool allright = Yes;
d90 2
a91 2
        if( load_file( f.current() ) != Ok )
            allright = No;
a92 6

    if( !allright ) return Err;
    
    done_load = Yes;
    
    return Ok;
@


1.3
log
@Clean up of watcom classlib based code
@
text
@d8 3
@


1.2
log
@C Set & STL support
@
text
@d7 4
a10 1
 *      $Log: Approve.c $
a24 1
#if USE_STL
a26 3
#else
    return set_v.insert( ae ) ? Ok : Err;
#endif
a30 1
#if USE_STL
a31 3
#else
    return set_v.contains( ae ) ? Yes : No;
#endif
a117 14
#if !USE_STL
unsigned app_elem_hash( const app_elem &ae )
    {
    return 
        (ae.who.zone() << 15) +
        (ae.who.node() << 5 ) +
        (ae.who.net() << 9) +
        ae.who.point() +
        (ae.what.zone() << 14)+
        (ae.what.node() << 6)+
        (ae.what.net() << 10) +
        ae.what.point();
    }
#endif
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: Announce.c $
d22 6
a27 1
    return set.insert( ae ) ? Ok : Err;
d32 5
a36 1
    return set.contains( ae ) ? Yes : No;
d123 1
d136 1
@
