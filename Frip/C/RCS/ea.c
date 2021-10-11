head	1.5;
access;
symbols;
locks;
comment	@ * @;


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
date	96.07.22.02.48.05;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@Version 25
@
text
@/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	OS/2 EAs
 *
 *      $Log: ea.C $
 *      Revision 1.4  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.3  1996/08/04 08:18:43  dz
 *      Before exceptions.
 *
 *      Revision 1.2  1996/07/31 07:00:23  dz
 *      FripMail written
 *
 *      Revision 1.1  1996/07/22 02:48:05  dz
 *      Initial revision
 *
 *
 *
 *
\*/

#include "frip.h"
#include "binbuf.h"
#include "except.h"
//#include "ea.h"

#ifdef __OS2__

#define INCL_DOSFILEMGR   /* File Manager values */
#define INCL_DOSERRORS    /* DOS error values    */
#include <os2.h>
//#include <stdio.h>
//#include <string.h>

#pragma pack(4)

static void set_ea( const char *file_name, const char *ea_name, const char *ea_data, int ea_data_len )
    {
    APIRET       rc            = NO_ERROR;      /* Return code                 */
    EAOP2        op;

    int  ea_name_len = strlen( ea_name );
    if( ea_name_len > 255 )
        throw Ex_Arg("set_ea","EA name too long",string(ea_name));
    
    binbuf databuf((64*2*1024)+1024); // twice 64K for EA data + 1024 for any case
    
    op.fpGEA2List = (PGEA2LIST)0;
    op.fpFEA2List = (PFEA2LIST)databuf.b;

    char *databufp = databuf.b + sizeof(long);
    *((long*)databufp) = 0; // Next field offset is zero - just one field here
    databufp += sizeof(long);
    *databufp++ = 0; // not critical
    *databufp++ = (char)ea_name_len;
    *((short*)databufp) = ea_data_len;
    databufp += sizeof(short);
    memcpy( databufp, ea_name, ea_name_len+1 ); // with trailing zero
    databufp += ea_name_len+1;
    memcpy( databufp, ea_data, ea_data_len ); // with trailing zero
    databufp += ea_data_len;
    
    *((long*)(databuf.b)) = databufp-(databuf.b); // Size of all that stuff
    
    rc = DosSetPathInfo( file_name, FIL_QUERYEASIZE, &op, sizeof(op), 0);

    if (rc != NO_ERROR)
        throw Ex_Errno( "set_ea", "DosSetPathInfo error", rc );
    }

#endif // __OS2__



void set_ea_ASCII( const char *fn, const char* ea_name, string data )
    {
#ifdef __OS2__
    binbuf b(64*1024);

    char *buf = b.b;

    *((short*)buf) = EAT_ASCII;
    buf += sizeof(short);
    *((short*)buf) = data.length();
    buf += sizeof(short);
    strcpy( buf, data.c_str() );
    
    set_ea( fn, ea_name, b.b, data.length() + 4 );
#endif // __OS2__
    }


void set_ea_MVMT_ASCII( const char *fn, const char* ea_name, const vector <string> &data )
    {
#ifdef __OS2__
    binbuf b(64*1024);
    
    char *buf = b.b;
    
    *((short*)buf) = EAT_MVMT;       buf += sizeof(short);
    *((short*)buf) = 0;              buf += sizeof(short); // Default CodePage == 0
    *((short*)buf) = data.size();    buf += sizeof(short);

    int len = data.size();
    for( int i = 0; i < len; i++ )
        {
        if( (64*1024-1) < ((buf-b.b) + data[i].length() + 4) )
            throw Ex_Arg("set_ea_MVMT_ASCII","vector too big to fit in EA","");
        
        *((short*)buf) = EAT_ASCII;          buf += sizeof(short);
        *((short*)buf) = data[i].length();   buf += sizeof(short);
        strcpy( buf, data[i].c_str() );      buf += data[i].length();
        }
    
    set_ea( fn, ea_name, b.b, buf-b.b );
#endif // __OS2__
    }

void set_ea_MVST_ASCII( const char *fn, const char* ea_name, const vector <string> &data )
    {
#ifdef __OS2__
    binbuf b(64*1024);
    
    char *buf = b.b;
    
    *((short*)buf) = EAT_MVST;       buf += sizeof(short);
    *((short*)buf) = 0;              buf += sizeof(short); // Default CodePage == 0
    *((short*)buf) = data.size();    buf += sizeof(short);
    *((short*)buf) = EAT_ASCII;      buf += sizeof(short);
    
    int len = data.size();
    for( int i = 0; i < len; i++ )
        {
        if( (64*1024-1) < ((buf-b.b) + data[i].length() + 4) )
            throw Ex_Arg("set_ea_MVST_ASCII","vector too big to fit in EA","");
        
        *((short*)buf) = data[i].length();   buf += sizeof(short);
        strcpy( buf, data[i].c_str() );      buf += data[i].length();
        }
    
    set_ea( fn, ea_name, b.b, buf-b.b );
#endif // __OS2__
    }





@


1.4
log
@Exceptions added, not compiled
@
text
@d8 3
d27 1
d52 1
a52 1
    op.fpFEA2List = (PFEA2LIST)databuf;
d66 1
a66 1
    *((long*)databuf) = databufp-(databuf.b); // Size of all that stuff
d96 1
a96 1
bool set_ea_MVMT_ASCII( const char *fn, const char* ea_name, const vector <string> &data )
d122 1
a122 1
bool set_ea_MVST_ASCII( const char *fn, const char* ea_name, const vector <string> &data )
@


1.3
log
@Before exceptions.
@
text
@d8 3
d23 1
d36 1
a36 1
static bool set_ea( const char *file_name, const char *ea_name, const char *ea_data, int ea_data_len )
a38 1
    
d40 4
d45 1
a45 1
    char       * databuf = new char[(64*2*1024)+1024]; // twice 64K for EA data + 1024 for any case
d49 2
a50 14
    
    //char *attname = ".SUBJECT";
    int  ea_name_len = strlen( ea_name );
    if( ea_name_len > 255 )
        {
        Error("EA name too long: "+string(ea_name));
        return Err;
        }
    
    //char datname[] = "\xFD\xFF\x14\x00More Stupid Subject!\x0"; // FFFD, 2-byte len, text
    //char  datlen = sizeof( datname );
    
    
    char *databufp = databuf + sizeof(long);
d62 1
a62 1
    *((long*)databuf) = databufp-databuf; // Size of all that stuff
d65 1
d67 1
a67 8
        {
        Error("DosSetPathInfo error");
        return Err;
        }
    
    delete [] databuf;
    
    return Ok;
a72 9
class binbuf
    {
    public:
        char *b;
        
        binbuf( int size ) { b = new char[size]; }
        ~binbuf() { delete [] b; }
    };

d74 1
a74 1
bool set_ea_ASCII( const char *fn, const char* ea_name, string data )
d87 1
a87 3
    return set_ea( fn, ea_name, b.b, data.length() + 4 );
#else // __OS2__
    return Ok;
d100 1
a100 2
    // Default CodePage == 0
    *((short*)buf) = 0;              buf += sizeof(short);
a105 1

d107 1
a107 4
            {
            Error("vector too big to fit in EA, cut it off :(");
            break;
            }
d114 1
a114 3
    return set_ea( fn, ea_name, b.b, buf-b.b );
#else // __OS2__
    return Ok;
d126 1
a126 2
      // Default CodePage == 0
    *((short*)buf) = 0;              buf += sizeof(short);
a132 1
        
d134 1
a134 4
            {
            Error("vector too big to fit in EA, cut it off :(");
            break;
            }
d140 1
a140 3
    return set_ea( fn, ea_name, b.b, buf-b.b );
#else // __OS2__
    return Ok;
@


1.2
log
@FripMail written
@
text
@d8 3
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log: util.C $
d17 1
a17 1
#include "ea.h"
d29 1
a29 1
bool set_ea( const char *file_name, const char *ea_name, const char *ea_data, int ea_data_len )
d44 1
a44 1
        Error("EA name too long: "+ea_name);
d98 4
a101 2
    *((short*)buf)++ = EAT_ASCII;
    *((short*)buf)++ = data.length();
d105 2
d111 1
a111 1
bool set_ea_MVMT_ASCII( const char *fn, const char* ea_name, vector <string> data )
d113 1
d118 4
a121 3
    *((short*)buf)++ = EAT_MVMT;
    *((short*)buf)++ = 0; // CodePage - default
    *((short*)buf)++ = data.size();
d123 1
d133 3
a135 4
        *((short*)buf)++ = EAT_ASCII;
        *((short*)buf)++ = data[i].length();
        strcpy( buf, data[i].c_str() );
        buf += data[i].length();
d139 3
d144 32
@
