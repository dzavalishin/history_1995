head	1.4;
access;
symbols;
locks;
comment	@ * @;


1.4
date	96.05.21.00.21.04;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.07.05.15.02;	author dz;	state Exp;
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


1.4
log
@cosmetical changes
@
text
@/************************ UU2 News Batcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *  Module  :   Compare group name to group names list
 *
 *      $Log: cmpgroup.c $
 *      Revision 1.3  1995/08/07 05:15:02  dz
 *      Using App class
 *
 *      Revision 1.2  1995/04/10  02:08:14  dz
 *      IBM C Set version
 *
 *      Revision 1.1  1995/03/11  18:26:07  dz
 *      Initial revision
 *
 *      Rev 1.1   28 Nov 1992 22:09:18   dz
 *      consts, minor fixes
 *      
 *      Rev 1.0   23 Apr 1992 02:59:08   dz
 *      Initial revision.
 *
 *
\*/

#include    "batch.h"

#include    <ctype.h>

#define MAXSEL  50

static bool     selected( const char *group, char *selector );

bool
BatchApp::group_in_list( const char *group, const char *g_list )
    {
    const char    *s_beg;                     // Selector beginning
    const char    *s_end;                     // Selector end
    char          sel[MAXSEL+1];              // Parsed selector

    s_end = g_list;
    while( *s_end )
        {
        int l;

        while( isspace( *s_end ) )     // Skip spaces
            s_end++;

        s_beg = s_end;                  // Go to next selector

        while( *s_end && !isspace( *s_end ) )
            s_end++;

        if( s_beg == s_end )
            return No;

        if( (l = (int)(s_end - s_beg)) > MAXSEL )
            {
            error( EI_None, "Selector too long: %s.*", s_beg, l );
            continue;
            }

        strncpy( sel, s_beg, l );
        sel[l] = '\0';

        if( selected( group, sel ) == Yes )
            return Yes;
        }

    return No;
    }




static bool selected( const char *group, char *selector )
    {
    char    *esf;                   // End of selector field
    int     l;                      // Length of selector field

    while( *selector && *group )
        {
        esf = strchr( selector, '.' );
        if( esf == NULL )
            l = strlen( selector );
        else
            l = (int) (esf-selector);

        if( (l == 3 && !strnicmp( selector, "all", 3 )) ||
           (l == 1 && *selector == '*' ))      // Next part is eq.
            {
            while( *group && *group != '.' )
                group++;
            if( *group == '.' )
                group++;

            if( esf == NULL )               // End of selector
                return Yes;

            selector = esf+1;
            continue;
            }

        if( strnicmp( selector, group, l ) )
            return No;

        selector += l;
        group += l;

        if( *selector == '.' )
            selector++;
        else if( *selector != '\0')
            {
            error( EI_None, "Can't parse selector - program error!" );
            // printf("Can't parse selector - program error!\n");
            return Err;
            }
        else if( *group == '\0' )
            return Yes;

        if( *group == '.' )
            group++;
        }

    return *selector == *group ? Yes : No;
    }



@


1.3
log
@Using App class
@
text
@d8 2
a9 2
 * Revision 1.2  1995/04/10  02:08:14  dz
 * IBM C Set version
d11 2
a12 2
 * Revision 1.1  1995/03/11  18:26:07  dz
 * Initial revision
d14 4
a17 2
 *      
 *         Rev 1.1   28 Nov 1992 22:09:18   dz
d20 1
a20 1
 *         Rev 1.0   23 Apr 1992 02:59:08   dz
d36 3
a38 3
	{
	const char    *s_beg;                     // Selector beginning
	const char    *s_end;                     // Selector end
d43 1
a43 1
		{
d57 1
a57 1
		if( (l = (int)(s_end - s_beg)) > MAXSEL )
d59 2
a60 4
			error( EI_None, "Selector too long: %s.*", s_beg, l );
//            printf("Selector too long: %s.*\n", s_beg, l );

			continue;
d66 1
a66 1
		if( selected( group, sel ) == Yes )
d71 1
a71 1
	}
d76 1
a76 2
static bool
selected( const char *group, char *selector )
d87 1
a87 1
			l = (int) (esf-selector);
d90 1
a90 1
            (l == 1 && *selector == '*' ))      // Next part is eq.
d114 2
a115 2
			error( EI_None, "Can't parse selector - program error!" );
//            printf("Can't parse selector - program error!\n");
a128 12
/*

void
main( int, char **av )
    {

    printf("%s\n",
    group_in_list( av[1], av[2] ) ? "Yes" : "No" );

    }

/**/
@


1.2
log
@IBM C Set version
@
text
@d8 3
d25 1
d33 1
a33 1
group_in_list( const char *group, const char *g_list )
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/batch/vcs/cmpgroup.c_v  $
d86 1
a86 1
        if( (l == 3 && !strncmpi( selector, "all", 3 )) ||
d101 1
a101 1
        if( strncmpi( selector, group, l ) )
@
