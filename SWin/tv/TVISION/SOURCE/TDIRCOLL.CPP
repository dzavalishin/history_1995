/*------------------------------------------------------------*/
/* filename -       tdircoll.cpp                              */
/*                                                            */
/* function(s)                                                */
/*                  TDirCollection member functions           */
/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
/*                                                            */
/*    Turbo Vision -  Version 1.0                             */
/*                                                            */
/*                                                            */
/*    Copyright (c) 1991 by Borland International             */
/*    All Rights Reserved.                                    */
/*                                                            */
/*------------------------------------------------------------*/

#define Uses_TDirCollection
#define Uses_TDirEntry
#define Uses_opstream
#define Uses_ipstream
#include <tv.h>
#include <tvdir.h>

#ifdef __OS2__
#define INCL_DOSFILEMGR
#include <os2.h>
#endif

#include <String.h>
#include <Dos.h>
#include <sys\stat.h>

Boolean driveValid( char drive )
{
#ifdef __MSDOS__
   struct diskfree_t df;
   return Boolean(_dos_getdiskfree(drive-'@',&df) == 0);
#endif
#ifdef __OS2__
  FSALLOCATE a;
  return Boolean(!DosQueryFSInfo(drive - '@', FSIL_ALLOC, &a, sizeof(a) ) );
#endif
}

Boolean isDir( const char *str )
{
    struct stat st;
    return Boolean(stat(str, &st) == 0 && (st.st_mode & S_IFDIR));
}

//#ifdef __BORLANDC__
//    ffblk ff;
//    return Boolean( findfirst( str, &ff, FA_DIREC ) == 0 &&
//                    (ff.ff_attrib & FA_DIREC) != 0 );
//#else
//    struct stat st;
//    if ( stat(str,&st) != 0 ) return False;
//    return Boolean(S_ISDIR(st.st_mode));
//#endif
//}

Boolean pathValid( const char *path )
{
    char expPath[MAXPATH];
    strcpy( expPath, path );
    fexpand( expPath );
    int len = strlen(expPath);
    if( len <= 3 )
        return driveValid(expPath[0]);

    if( expPath[len-1] == '\\' )
        expPath[len-1] = EOS;

    return isDir( expPath );
}

Boolean validFileName( const char *fileName )
{
#ifdef __MSDOS__
    static const char * const illegalChars = ";,=+<>|\"[] \\";
#else
    static const char * const illegalChars = "<>|\"\\";
#endif

    char path[MAXPATH];
    char dir[MAXDIR];
    char name[MAXFILE];
    char ext[MAXEXT];

    ext[1] = 0; // V.Timonin
    fnsplit( fileName, path, dir, name, ext );
    strcat( path, dir );
    if( *dir != EOS && !pathValid( path ) )
	return False;
    if( strpbrk( name, illegalChars ) != 0 ||
	strpbrk( ext+1, illegalChars) != 0 ||
	strchr( ext+1, '.' ) != 0
      )
	return False;
    return True;
}

void getCurDir( char *dir )
{
    getcwd(dir,MAXPATH);
    int len = strlen(dir);
    if (len > 3 && len < MAXPATH - 1) {
      dir[len] = '\\',
      dir[len + 1] = '\0';
    }
}

Boolean isWild( const char *f )
{
    return Boolean( strpbrk( f, "?*" ) != 0 );
}

TStreamable *TDirCollection::build()
{
    return new TDirCollection( streamableInit );
}

void TDirCollection::writeItem( void *obj, opstream& os )
{
    TDirEntry *item = (TDirEntry *)obj;
    os.writeString( item->text() );
    os.writeString( item->dir() );
}

void *TDirCollection::readItem( ipstream& is )
{
    const char *txt = is.readString();
    const char *dir = is.readString();
    return new TDirEntry( txt, dir );
}
