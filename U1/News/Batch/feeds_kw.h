/************************ UU2 News Batcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Feeds file keywords.
 *
 *      $Log: feeds_kw.h $
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
