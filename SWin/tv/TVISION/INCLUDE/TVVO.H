#if defined(__MSDOS__) && !defined(__DOS32__) && !defined(__DOS16__)
#define __DOS16__
#endif
#if !defined(__MSDOS__) && !defined(__OS2__)
#error Unknown target platform!
#endif
#if defined(__MSDOS__) && defined(__BORLANDC__)
#pragma option -Vo-
#if defined( __BCOPT__ )
#pragma option -po-
#endif
#endif // __MSDOS__ && __BORLANDC__
