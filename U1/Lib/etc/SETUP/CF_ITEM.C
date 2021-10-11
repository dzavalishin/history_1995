/************************ UUCP to FIDO gate ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Implementation of config_item class
 *
 *      $Log: CF_ITEM.C $
 * Revision 1.1  1995/03/11  18:14:11  dz
 * Initial revision
 *
 *      
 *         Rev 1.1   07 Jun 1993 17:42:52   dz
 *      
 *         Rev 1.0   14 May 1993 13:41:50   dz
 *      Initial revision.
 *
 *
\*/

#include	"config.h"
#include	<ctype.h>
#include	<strstream.h>
#include	<fstream.h>
#include	<assoc.h>
#include	<sys/stat.h>

#ifdef	TEST
#include	"./test.h"
#endif

	/*************************************************************
						   C'tor/d'tor
	*************************************************************/


config_item::
config_item(
		const char *	Keyword,
		bool			Obligatory,
		const char *	Default_Value
		) : list( (DoubleList &)NOOBJECT )
	{
	keyword = Keyword;
	obligatory = Obligatory;
	default_value = Default_Value;

	has_value = No;
	is_default = No;

	}

config_item::
config_item(
		config_list			&	List,
		const char *	Keyword,
		bool			Obligatory,
		const char *	Default_Value
		) : list( /*(DoubleList &)*/List.items )
	{
	keyword = Keyword;
	obligatory = Obligatory;
	default_value = Default_Value;

	has_value = No;
	is_default = No;

	List.add( *this );

	}


config_item::
~config_item()
	{
	if( list != NOOBJECT )
		list.detach( *(Object *)this );
	}




	/*************************************************************
						   Methods
	*************************************************************/


bool
config_item::LoadText( const config_text & text )
	{
	String v;
	bool	b;

	b = text.find( keyword, v );

	if( b == Err )		return Err;
	if( b == Yes )
		{
		if( putAsText( v ) == Err )
			return Err;
		}
	return Ok;
	}







/****************************************************************************
						Object
****************************************************************************/



classType config_item::
isA() const
	{
	return 6435;	// Bullshit
	}

char _FAR * config_item::
nameOf() const
	{
	return "config_item";
	}


hashValueType config_item::
hashValue() const
	{
	return 0;
	};


int config_item::
isEqual( const Object _FAR &o ) const
	{
	return &o == this;
	}



void config_item::
printOn( ostream _FAR &o ) const
	{
	StoreText( o );
	}

