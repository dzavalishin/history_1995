/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1993 by Infinity Soft
 *
 *	Module 	:	Virtual file reader - view
 *
 *      $Log: vread_wi.c $
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
 *         Rev 1.1   05 Mar 1993 03:12:36   dz
 *      Bug fix in gets & seekg
 *      
 *         Rev 1.0   05 Mar 1993 00:06:00   dz
 *      Initial revision.
 *
 *
\*/

#include	"vread.h"

VReadView::
VReadView( void )
	{
	view = &VReadError::Error;
	}


VReadView::
~VReadView( void )
	{
	close();
	}


	/*************************************************************
						   Open/Close
	*************************************************************/


bool VReadView::
open( VReadBase * a_view, long a_begin, long a_len )
	{
	view = a_view;
	begin = a_begin;
	len = a_len;

	if( view == NULL )
		{
		view = &VReadError::Error;
		return Err;
		}

	return Ok;
	}

bool VReadView::
close( void )
	{
	view = &VReadError::Error;
	return Ok;
	}


	/*************************************************************
						   Access
	*************************************************************/


int VReadView::
read( void *buf, int blen )
	{
	long	cp = view->tellg() - begin;

	if( cp < 0L || cp >= len )
		return -1;

	if( cp + blen > len )
		blen = (int)(len - cp);

	return view->read( buf, blen );
	}

bool VReadView::
gets( uchar *buf, int blen )
	{
	long	cp = view->tellg() - begin;

	if( cp < 0L || cp >= len )
		return Err;

	if( cp + (blen-1) > len )			// -1 = Place for trailing zero!
		blen = (int)(len - cp) + 1;

	return view->gets( buf, blen );
	}


long VReadView::
seekg( long pos )
	{
//	if( pos > len || pos < 0 )
	if( pos < 0 )
		return -1L;

	return view->seekg( pos + begin );
	}

long VReadView::
tellg( void )
	{
	long pos = view->tellg() - begin;
	if( pos < 0 )
		return -1L;

	return pos;
	}


long VReadView::
size( void )
	{
	return len;
	}


bool VReadView::
eof()
	{
	if( view->eof() )
		return Yes;

	if( tellg() >= len )
		return Yes;

	return No;
	}

