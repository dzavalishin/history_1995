/************************ UU2 UU2FILE ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Get part with given number
 *
 *      $Log:   Q:/news/snews/vcs/main.c_v  $
 *      
 *
\*/

#include    "uu2file.h"
#include	"part.h"



part *
get_part( Bag &parts, const String &id, int no )
	{
	ContainerIterator	&i = parts.initIterator();

	for( ; i; i++ )
		{
		part &p = (part &)i.current();
		if( p.id_v == id && p.pno == no )
			return &p;
		}

	return NULL;
	}
