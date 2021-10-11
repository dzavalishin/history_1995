/************************ UUCP interface library ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	UUCP Address class implemenation
 *
 *      $Log:   Q:/lib/uucp/vcs/uucpaddr.c_v  $
 *      
 *         Rev 1.0   07 Jun 1993 18:03:12   dz
 *      Initial revision.
 *
 *
\*/

#include	"uucpaddr.h"
#include	<fidoaddr.h>
#include	<userlist.h>
#include	<log.h>

#include	<stdlib.h>





struct trav_data
	{
	DoubleList	*l;
	};

static bool
uu_traverse( uucp_addr::el_type t, const char *el, int len, void *arg )
	{
	trav_data	*e = (trav_data *)arg;

	if( t == uucp_addr::UU_name )		// No, no... domains only
		return Yes;

    const      bl = 200;
    char       buf[bl];

    if( len > bl )
        {
        error( EI_None, "Address fragment too long: '%*.*s'", len, len, el );
        len = bl-1;
        }

    strncpy( buf, el, len );
    buf[len] = '\0';

    String *s = new String( buf );

    if( s == NULL || s == Object::ZERO )
        fatal( EC_Out_Of_Memory, EI_None, "No RAM :(" );

	e->l->addAtTail( *s );

	return Yes;
	}


DoubleList
uucp_addr::operator DoubleList() const
    {
    DoubleList  d;
    trav_data   td = { &d };

    traverse( uu_traverse, (void *)&td );

    return d;
    }


