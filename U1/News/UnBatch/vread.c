/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1993 by Infinity Soft
 *
 *	Module 	:	Virtual file reader
 *
 *      $Log: vread.c $
 * Revision 1.2  1995/04/12  17:32:19  dz
 * Bug fixes
 *
 * Revision 1.1  1995/03/11  18:32:26  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:32:26  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   05 Mar 1993 00:05:14   dz
 *      Initial revision.
 *
 *
\*/

#include	<stddef.h>
#include	<fcntl.h>

#include	"vread.h"

VReadBase::
~VReadBase( void )
	{}

/*-------------------------------------------------------------------------*\
							  Error view...
\*-------------------------------------------------------------------------*/


int  VReadError::read( void *, int )	{ return -1; }
long VReadError::seekg( long )  		{ return -1L; }
long VReadError::tellg( void )  		{ return -1L; }
long VReadError::size( void )			{ return 0; }
bool VReadError::gets( uchar *, int ) 	{ return Err; }


VReadError VReadError::Error;


/****************************************************************************
							istream interface
****************************************************************************/
/*
int VReadStream::
read( void *buf, int blen )
	{
	is.read( (signed char *)buf, blen );
	return is.gcount();
	}

long VReadStream::
seekg( long pos )
	{
	is.seekg( pos );
	return is.tellg();
	}

long VReadStream::
tellg( void )
	{
	return is.tellg();
	}

VReadStream::
~VReadStream( void )
	{}
*/

