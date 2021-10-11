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
 *	Module 	:	Feeds file keywords.
 *
 *      $Log: feeds_kw.h $
 *      Revision 1.2  1995/04/10  02:08:14  dz
 *      IBM C Set version
 *
 *      Revision 1.1  1995/03/11  18:26:07  dz
 *      Initial revision
 *
 *
 *
\*/

#ifndef	FEEDS_KW_H
#define	FEEDS_KW_H

#include	<style.h>
#include	"feeds.h"


struct f_mode_kw
    {
    feed::f_mode		fmode;
    char *				fkeyw;
    } modetab[] =
        {
        { feed::ViaUux,        "viauux"	},
        { feed::ViaMail,        "viamail"	},
        { feed::ViaRsh,        "viarsh"	},
        { feed::ViaFilt,        "viafilt",	}
        };

const modetab_s = sizeof( modetab ) / sizeof( f_mode_kw );

struct f_comp_kw
    {
    feed::f_comp		fcomp;
    char *				fkeyw;
    } comptab[] =
    	{
        { feed::Batch,	"batch"		},
        { feed::Comp12,	"comp12"	},
        { feed::Comp16,	"comp16"	},
//        { feed::Zip,	"zip"		},
        };

const comptab_s = sizeof( comptab ) / sizeof( f_comp_kw );


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
d37 1
a37 1
	{
d44 3
a46 3
		{ feed::Comp16,	"comp16"	},
//		{ feed::Zip,	"zip"		},
		};
@
