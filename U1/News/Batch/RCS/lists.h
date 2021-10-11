head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	96.05.21.00.21.33;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.10.02.08.14;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.26.07;	author dz;	state Exp;
branches;
next	;


desc
@@


1.3
log
@cosmetical changes
@
text
@/************************ UU2 News Batcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Article lists iterator.
 *
 *      $Log: lists.h $
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
@


1.2
log
@IBM C Set version
@
text
@d8 5
a12 2
 * Revision 1.1  1995/03/11  18:26:07  dz
 * Initial revision
d26 2
a27 1
    const char	*listdir;                   // Directory where list reside
d29 2
a30 2
    FILE        *ff;                        // Current list file
    int         listno;                     // Number of current list file
d32 7
a38 7
public:
                lists( const char *listdir );
                ~lists();

    void        rewind();                   // Go to first
    FILE *      next();                     // Get next
    void        kill();                     // Delete contents of current list file
d50 2
a51 1
    FILE        *lf;            // List file we iterate through
d53 4
a56 4
public:
                alist( FILE *list_file )
                    { lf = list_file; fseek( lf, 0L, SEEK_SET ); }
                ~alist();
d58 1
a58 1
    bool        next( char *art_file, char *art_group, char *path );
@


1.1
log
@Initial revision
@
text
@d7 3
a9 1
 *      $Log:$
d12 1
d22 2
a23 2
	{
	const char	*listdir;					// Directory where list reside
d25 2
a26 2
	FILE		*ff;						// Current list file
    int			listno;						// Number of current list file
d29 1
a29 1
				lists( const char *listdir );
d32 4
a35 4
    void		rewind();					// Go to first
	FILE *		next();						// Get next
    void		kill();						// Delete contents of current list file
	};
d45 2
a46 2
	{
	FILE		*lf;						// List file we iterate through
d49 2
a50 2
				alist( FILE *list_file )
					{ lf = list_file; fseek( lf, 0L, SEEK_SET ); }
d53 2
a54 2
	bool		next( char *art_file, char *art_group, char *path );		// Get next
	};
@
