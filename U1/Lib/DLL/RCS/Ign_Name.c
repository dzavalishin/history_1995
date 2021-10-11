head	1.2;
access;
symbols;
locks;
comment	@ * @;


1.2
date	95.04.13.11.06.28;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.04.17;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@debug
@
text
@/************************ UU2 Conf. ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Ignored names list
 *
 *      $Log: Ign_Name.c $
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   07 Jun 1993 17:50:00   dz
 *      Initial revision.
 *
 *
\*/


#include	<string.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<ctype.h>

#include	"setup.h"
#include	"log.h"

#include	<bag.h>

/***************************************************************************\
							  Definitions
\***************************************************************************/


static Bag	namesbag;				// Here names to ignore will be stored

class BadName: public Object
	{
	virtual classType isA() const 						{ return 0; };
	virtual char _FAR *nameOf() const 					{ return "BadName"; };
	virtual hashValueType hashValue() const 			{ return 0; };
	virtual int isEqual( const Object _FAR & ) const 	{ return 0; };
	virtual void printOn( ostream _FAR & ) const		{};

public:
	char	*name;
	bool	set( char *s );
	};

bool
BadName::set( char *s )
	{
	name = strdup( s );
	if( name == NULL )
		return Err;
	return Ok;
	}


/***************************************************************************\
							Add name to bag
\***************************************************************************/

bool
add_ignname( char *name )
	{
	BadName		*bn = new BadName;

	if( bn == NULL )
		return Err;

	if( bn->set(name) == Err )
		{
		delete bn;
		return Err;
		}

	namesbag.add( *(Object*)bn );
	return Ok;
	}



/***************************************************************************\
						Search bag for a given name
\***************************************************************************/

bool
check_ignname( char *name )
	{
	ContainerIterator	&it = namesbag.initIterator();

	for(; (int)it; it++)
		{
		BadName	&bn = (BadName&)it.current();
		if( stricmp( name, bn.name ) == 0 )
			return Yes;

printf("IgtnName='%s'\n", bn.name );

		}
	return No;
	}

@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/uu2/vcs/ign_name.c_v  $
@
