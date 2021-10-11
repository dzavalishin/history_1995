/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	T-Mail support
 *
 *      $Log: TMail.C $
 *      Revision 1.6  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
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








