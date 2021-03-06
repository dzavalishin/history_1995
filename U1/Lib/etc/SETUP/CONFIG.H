/************************ Config Library ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Main header
 *
 *      $Log: CONFIG.H $
 * Revision 1.1  1995/03/11  18:14:11  dz
 * Initial revision
 *
 *      
 *         Rev 1.1   14 May 1993 13:42:54   dz
 *
 *
\*/

#include	<style.h>
#include	<ctype.h>
#include	<values.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<iostream.H>

#include	<strng.h>
#include	<dict.h>
#include	<dbllist.h>



class config_item;

class config_list
	{
	DoubleList	items;

	friend	config_item;
public:
			config_list() { items.ownsElements( 0 ); }

	void	add( config_item &item ) { items.addAtTail( (Object&)item ); }
	bool	setup( const char * config_file_name, const char * cache_file_name );

	};


class config_text
	{
	Dictionary	lines;
	bool		bad;

public:
			config_text( istream &file );

	bool	error() { return bad; }
	bool	find( const String &keyw, String &value ) const;
	};


class config_item : public Object
	{
	const signed char *		keyword;					// From c'tor
	bool					obligatory;					// ...
	const signed char *		default_value;				// ...

	bool					has_value;
	bool					is_default;

	DoubleList			&	list;						// That contains us, or NOOBJECT

	/*************************************************************
					istream/ostream interface
	*************************************************************/
protected:

	bool		push_bool( ostream &s, bool val ) const
		{
		char	cc = val;
		s.write( (signed char *) &cc, 1 );
		if( s.fail() )	return Err;
		return Ok;
		}

	bool		pop_bool( istream &s, bool &val)
		{
		int i = s.get();
		if( i == EOF )	return Err;
		val = i ? Yes : No;
		return Ok;
		}

	bool		push( ostream &s, void *data, int size ) const
		{
		s.write( (signed char *)data, size );
		if( s.fail() )	return Err;
		return Ok;
		}


	bool		pop( istream &s, void *data, int size )
		{
		s.read( (signed char *)data, size );
		if( s.fail() )	return Err;
		return Ok;
		}

	/*************************************************************
					  Derived classes interface
	*************************************************************/
private:

	bool		PushGenericData( ostream &cc ) const
			{ if( push_bool( cc, has_value ) == Err ) return Err;  return push_bool( cc, is_default ); }

	bool		PopGenericData( istream &cc )
			{ if( pop_bool( cc, is_default ) == Err ) return Err;  return pop_bool( cc, has_value ); }



	// Next 4 must be re-implemented in subclasses

	virtual bool		PushSpecificData( ostream & )	const	{ return Ok; }
	virtual bool		PopSpecificData( istream & )			{ return Ok; }

	virtual bool		DoGetAsText( String &s )			const	{ s = (const signed char *)""; return Ok;}
	virtual bool		DoPutAsText( const String & )				{ return Ok; }

public:

		/*********************************************
						Constructors
		*********************************************/

				config_item(
					const char *	Keyword,
					bool					Obligatory = No,
					const char *	Default_Value = NULL
					);

				config_item(
					config_list			&	List,
					const char *	Keyword,
					bool			Obligatory = No,
					const char *	Default_Value = NULL
					);

				~config_item();


	// Operations

			// Loads ftom text, stores to cache
	bool		LoadText( const config_text & text );

	bool		StoreText( ostream & text ) const
			{
			text << keyword << " ";
			String v;
			if( getAsText( v ) == Err )
				return Err;
			text << v << "\n";
			return Ok;
			}

	bool		Push( ostream &cc )	const	{ if( PushGenericData( cc ) == Err ) return Err; return PushSpecificData( cc ); }
	bool		Pop( istream &cc )			{ if( PopGenericData( cc ) == Err ) return Err; return PopSpecificData( cc );   }


			// Fill self with default value, if still empty
	bool		SetDefault()
			{
			bool r = Ok;
			if( !has_value )
				{
				r = putAsText( (const signed char *)default_value );
				is_default = Yes;
				}
			return r;
			}

			// Get/set value as text string
	bool		getAsText( String &s )			const	{ return DoGetAsText(s); }
	bool		putAsText( const String &s )			{ has_value = Yes; return DoPutAsText(s); }

	// State query

	bool		isEmpty() const				// No value
			{ return (bool)!has_value; };

	bool		isError() const				// No value for obligatory one
			{ return (bool) ((!has_value) && obligatory); }

	bool		isDefault() const			// Current value is default one
			{ return is_default; }


/****************************************************************************
						Object methods
****************************************************************************/

	virtual classType isA() const;
	virtual char _FAR *nameOf() const;
	virtual hashValueType hashValue() const;
	virtual int isEqual( const Object _FAR & ) const;
	virtual void printOn( ostream _FAR & ) const;

	};


class cfShort : public config_item
	{
	short		val;

	virtual bool		PushSpecificData( ostream &s ) const
			{ return push( s, (void *)&val, sizeof( val ) ); }

	virtual bool		PopSpecificData( i��������  �� ��
                