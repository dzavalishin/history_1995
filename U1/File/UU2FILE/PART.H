/************************ UU2 UU2FILE ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	part class definition
 *
 *      $Log:   Q:/news/snews/vcs/main.c_v  $
 *
 *
\*/

#include	<object.h>
#include	<sys/stat.h>


class part : public Object
	{
public:
	String			fn_v;		// File name
	String			id_v;		// Identifier
	int				pno;		// Part number
	int				pts;		// Parts total
	struct stat 	st_v;		// File info

					part( 	const String &fn, const String &id,
							int part_no, int parts,
							struct stat &st )
						{
						fn_v = fn; id_v = id;
						pno = part_no; pts = parts;
						st_v = st;
						}

					~part() {}


	/*----------------------------------------------------------*\
						 Object type methods
	\*----------------------------------------------------------*/

	virtual classType isA() const 		{ return 0; };
	virtual char _FAR *nameOf() const 	{ return "part"; };
	virtual hashValueType hashValue() const { return 0; };
	virtual int isEqual( const Object _FAR &he ) const
		{ return &he == this; }
	virtual void printOn( ostream _FAR & ) const {}
	};



// get_part.c
part *		get_part( Bag &parts, const String &id, int no );
