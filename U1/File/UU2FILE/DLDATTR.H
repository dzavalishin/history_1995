/************************ Letter mgmnt library ***************************\
 *
 *      Copyright (C) 1991 by Infinity Soft
 *
 *      Module  :       Letter disassembler attr module
 *
 *      $Log:   I:/letter/vcs/dldattr.h_v  $
 *      
 *         Rev 1.1   11 Dec 1992 16:45:14   dz
 *      Working version
 *      
 *         Rev 1.0   05 Dec 1992 03:26:10   dz
 *      Initial revision.
 *
 *
\*/

#ifndef	DLDATTR_H
#define	DLDATTR_H

#include	<style.h>
#include	<lread.h>
#include	<ladtypes.h>

#include	<attrib.h>


class DLGenDisasm;
//class dl_attr;


struct dld_attr
	{
	ulong		pos;	// Position in file, relative to our beginning
	uint		len;	// Length of attr excluding its length bytes
//	DLAttr * attr;	// Attribute value, if any

				dld_attr();
			   ~dld_attr();

	const char *tag() { return tag_v; }
	bool		tag( const char *new_tag );

//private:
	char *		tag_v;	// Pre-loaded tag
	};

	// Attributes
class DLDAttrList : public virtual Object
	{
	friend			DLGenDisasm;

	DLReadBase &	reader;

	int				n;			// Total number of
	int				has_read;	// This many attrs scanned
	long			begin;		// Where first one starts
	long			cur;		// Where first not scanned lies
	dld_attr **		array;		// Scanned positions

	bool			scan_to( int index );	// Make sure this one is scanned
	bool			scan_one( int index, long pos );
	bool			load_tag( int index );	// Pre-load tag for this one

protected:
	bool			init();		// Startup - read num of args

public:
					DLDAttrList( DLReadBase &r );
					~DLDAttrList();

	int				no( void ) { return n; }			// Number of
	DLAttr			operator [] ( int index );			// Get by number
	bool			have_attr( const char *tag );		// Do we have an attr
														// with this tag?
	DLAttr			operator [] ( const char *tag );	// Get by tag
//	bool			dispose( int index );				// Forget this one

	bool			get( DLAttrList &to );				// Extract all

	/*----------------------------------------------------------*\
						 Object type methods
	\*----------------------------------------------------------*/

	virtual classType isA() const 		{ return DLDAttrListClass; };
	virtual char _FAR *nameOf() const 	{ return "DLDAttrList"; };
	virtual hashValueType hashValue() const { return 0; };
	virtual int isEqual( const Object _FAR & ) const;
	virtual void printOn( ostream _FAR & ) const;
	};




#endif	DLDATTR_H