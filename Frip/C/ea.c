/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	OS/2 EAs
 *
 *      $Log: ea.c $
 *      Revision 1.5  1996/09/08 19:05:31  dz
 *      Version 25
 *
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





