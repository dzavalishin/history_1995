head	1.6;
access;
symbols
	Beta_12:1.3;
locks;
comment	@ * @;


1.6
date	96.08.04.17.28.17;	author dz;	state Exp;
branches;
next	1.5;

1.5
date	96.08.04.08.18.43;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	96.01.22.04.35.34;	author dz;	state Exp;
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
date	95.11.16.00.54.30;	author dz;	state Exp;
branches;
next	;


desc
@@


1.6
log
@Exceptions added, not compiled
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	T-Mail support
 *
 *      $Log: TMail.C $
 *      Revision 1.5  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.4  1996/01/22 04:35:34  dz
 *      C Set & STL support
 *
 *      Revision 1.3  1996/01/07 20:21:01  dz
 *      Beta 12
 *
 *
 *
\*/

#include "frip.h"



static void radix32( char *cp, long int i, unsigned int wid )
    {
    static char digit[] = { "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" };

//fprintf( stderr, "cp %s, i %d, wid %d", cp, i, wid );

    i %= 0x7FFF;  // всяко не более 3-х 32-pичных цифp
    
    char out[4], *op = out;
    
    out[3] = 0;
    out[2] = 0;
    
    if(wid > 3 /*|| wid < 1*/) wid = 3;
    
    do
        {
        wid--;
        out[wid] = digit[i%32];
        i /= 32;
        }
    while( wid > 0 );

    strcat( cp, out );
    }

    
void TMailAttachFAT( const string &file, const addr &to, bool hold )
    {
    if( conf.tmail_fileboxes_fat().is_empty() )
        return;
    
    string _lo = conf.tmail_fileboxes_fat();
    mkdir( (char *)(const char *)_lo );

    char subdir[30] = "/";
    
    radix32( subdir, to.zone(), 2 );
    radix32( subdir, to.net(), 3 );
    radix32( subdir, to.node(), 3 );
    strcat( subdir, "." );
    radix32( subdir, to.point(), 2 );
    if( hold ) strcat( subdir, "H" );
    
    _lo += subdir;
    mkdir( (char *)(const char *)_lo );
    
    _lo += "/TOBESENT.$$$";
    
    //debug( "tmail FAT lo name: " + _lo );
    
#ifdef __IBMCPP__
    ofstream f_lo( _lo, ios::app );
#else
    ofstream f_lo( _lo, ios::append );
#endif
    
    if( !f_lo )
        throw Ex_Fail( "TMailAttachFAT", "Can't create TOBESENT.$$$: " + _lo, "left unattached " + file );
    
    f_lo << "^" << ((const char *)file) << "\n";
    }



void TMailAttachHPFS( const string &file, const addr &to, bool hold )
    {
#ifndef __DOS__
    if( conf.tmail_fileboxes_hpfs().is_empty() )
        return;
    
    string _lo = conf.tmail_fileboxes_hpfs();
    mkdir( (char *)(const char *)_lo );
    
    
    char subdir[30];
    
    
    sprintf( subdir, "/%d.%d.%d.%d%s",
            to.zone(), to.net(), 
            to.node(), to.point(),
            hold ? "H" : ""
           );
    
    _lo += subdir;
    mkdir( (char *)(const char *)_lo );
    
    _lo += "/TOBESENT.$$$";
    
#ifdef __IBMCPP__
    ofstream f_lo( _lo, ios::app );
#else
    ofstream f_lo( _lo, ios::append );
#endif
    
    if( !f_lo )
        throw Ex_Fail( "TMailAttachHPFS", "Can't create TOBESENT.$$$: " + _lo, "left unattached " + file );
    
    f_lo << "^" << ((const char *)file) << "\n";
    
#endif // __DOS__
    }








@


1.5
log
@Before exceptions.
@
text
@d8 3
d52 1
a52 1
bool TMailAttachFAT( const string &file, const addr &to, bool hold )
d55 1
a55 1
        return Ok;
d59 1
a59 2
    
    
d83 1
a83 4
        {
        Error( "Can't create TOBESENT.$$$: " + _lo + ", left unattached " + file );
        return Err;
        }
a85 2
    
    return Ok;
d90 1
a90 1
bool TMailAttachHPFS( const string &file, const addr &to, bool hold )
d94 1
a94 1
        return Ok;
a113 2
    //debug( "tmail HPFS lo name: " + _lo );
    
d121 1
a121 4
        {
        Error( "Can't create TOBESENT.$$$: " + _lo + ", left unattached " + file );
        return Err;
        }
a125 1
    return Ok;
a134 27
/*
static void radix32( char *cp, int i, int wid )
    {
    static char digit[] = { "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
    
    i %= 0x7FFF;  // всяко не более 3-х 32-pичных цифp
    
    char out[10], *op = out;
    
    int got_digits = 0;
    while( i )
        {
        *op ++ = digit[i%32];
        i /= 32;
        got_digits++;
        }
    *op = 0;
    
    while( got_digits < wid )
        {
        got_digits++;
        strcat( cp, "0" );
        }
    
    strcat( cp, out );
    }
*/
@


1.4
log
@C Set & STL support
@
text
@d7 4
a10 1
 *      $Log: TMail.c $
@


1.3
log
@Beta 12
@
text
@d7 4
a10 1
 *      $Log:$
d52 1
a52 1
    mkdir( _lo );
d65 1
a65 1
    mkdir( _lo );
d71 3
d75 1
d97 1
a97 1
    mkdir( _lo );
d110 1
a110 1
    mkdir( _lo );
d116 3
d120 1
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
@d3 3
a5 2
/*
static void radix32( char *cp, int i, int wid )
a7 23
    
    i %= 0x7FFF;  // всяко не более 3-х 32-pичных цифp
    
    char out[10], *op = out;
    
    int got_digits = 0;
    while( i )
        {
        *op ++ = digit[i%32];
        i /= 32;
        got_digits++;
        }
    *op = 0;
    
    while( got_digits < wid )
        {
        got_digits++;
        strcat( cp, "0" );
        }
    
    strcat( cp, out );
    }
*/
d9 1
a10 4
static void radix32( char *cp, int i, int wid )
    {
    static char digit[] = { "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
    
d18 1
a18 1
    if(wid > 3 || wid < 1) wid = 3;
d20 1
a20 1
    while( wid-- >= 0 )
d22 1
d26 2
a27 1
    
d32 1
a32 1
bool TMailAttachFAT( const string &file, const addr &to )
d48 1
a48 1
    strcat( subdir, "H" );
d72 1
a72 1
bool TMailAttachHPFS( const string &file, const addr &to )
d74 1
d82 1
a82 1
    char subdir[30] = "/";
d85 1
a85 1
    sprintf( subdir, "/%d_%d_%d_%d", 
d87 3
a89 1
            to.node(), to.point() );
d108 1
d119 27
@
