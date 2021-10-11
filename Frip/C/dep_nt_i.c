/*\
 *        The software included, file formats and basic algorithms are
 *      copyright (C) 1995,96 by Dmitry Zavalishin. All rights reserved.
 *
 *	Module:	OS Dependent Thingies - Win NT/ Win 95
 *
 *      $Log: dep_nt_i.c $
 *      Revision 1.2  1996/08/04 17:28:17  dz
 *      Exceptions added, not compiled
 *
 *      Revision 1.1  1996/08/04 08:18:43  dz
 *      Initial revision
 *
 *      Revision 1.2  1996/01/14 01:06:31  dz
 *      beta 14
 *
 *      Revision 1.1  1996/01/07 20:21:01  dz
 *      Initial revision
 *
 *
 *
\*/

#include "frip.h"

extern "C"
    {
#pragma pack(2);
#   include <windows.h>
    }

#if 0
// from okhapkin
int
windows_95 ()
{
    DWORD version = GetVersion ();
    if ((version & 0x80000000) && (version & 0xff) > 3)
        return 1;
    return 0;
}

#endif


OS_Type::OS_Type( void )
    {
    OSVERSIONINFO VerInfo;
    string info;

    v = Win32_95;
    name_v = "Win95";
    if( FALSE == GetVersionEx( &VerInfo ) )
        {
        name_v = "Unknown Win32";
        info.print("GetVersionEx failed: %d", GetLastError());
        Error(info);
        return;
        }

    info.print(
               "Win32: %d.%d build %d, platform %d, comment '%.127s'",
               (int)VerInfo.dwMajorVersion,
               (int)VerInfo.dwMinorVersion,
               (int)VerInfo.dwBuildNumber,
               (int)VerInfo.dwPlatformId,
               VerInfo.szCSDVersion
              );
    Error(info);

    if( VerInfo.dwPlatformId == VER_PLATFORM_WIN32_NT )
        {
        v = Win32_NT;
        name_v = "WinNT";
        }

    }




#define SYSDEP_DO_COPY
static void Do_Copy( string from, string to )
    {
    // LPCTSTR  lpExistingFileName, // address of name of an existing file
    // LPCTSTR  lpNewFileName,      // address of filename to copy to    
    // BOOL  bFailIfExists          // flag for operation if file exists 
    BOOL rc = CopyFile( from, to, TRUE );
    if( TRUE != rc  ) throw Ex_Errno( "Do_Copy", "CopyFile error", GetLastError() );
    }









