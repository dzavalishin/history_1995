/************************ UU2 News Batcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Article lists iterator.
 *
 *      $Log: lists.h $
 *      Revision 1.3  1996/05/21 00:21:33  dz
 *      cosmetical changes
 *
 *      Revision 1.2  1995/04/10  02:08:14  dz
 *      IBM C Set version
 *
 *      Revision 1.1  1995/03/11  18:26:07  dz
 *      Initial revision
 *
 *
 *
\*/

#ifndef	LISTS_H
#define	LISTS_H

#include	<style.h>
#include	<stdio.h>

class lists
    {
    protected:
        const char	*listdir;           // Directory where list reside

        FILE        *ff;                    // Current list file
        int         listno;                 // Number of current list file

    public:
        lists( const char *listdir );
        ~lists();

        void        rewind();               // Go to first
        FILE *      next();                 // Get next
        void        kill();                 // Delete contents of current list file
    };



/**********************************************************************/


#define	AL_MAX_GROUP	3000

class alist
    {
    protected:
        FILE        *lf;            // List file we iterate through
											// Must be open in binary mode!
    public:
        alist( FILE *list_file )
            { lf = list_file; fseek( lf, 0L, SEEK_SET ); }
        ~alist();

        bool        next( char *art_file, char *art_group, char *path );
    };



#endif
