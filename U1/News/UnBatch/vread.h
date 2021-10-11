/************************ UU2 News Unbatcher ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Virtual file reader
 *
 *      $Log: vread.h $
 *      Revision 1.4  1995/08/07 14:55:01  dz
 *      Using class App
 *
 * Revision 1.3  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.3  1995/04/09  18:25:15  dz
 * IBM C Set version. Phew, it was not a piece of cake to get here...
 *
 * Revision 1.2  1995/04/09  10:45:11  dz
 * rewriting for C Set
 *
 * Revision 1.1  1995/03/11  18:32:26  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   05 Mar 1993 00:15:54   dz
 *      Initial revision.
 *
 *
\*/


#ifndef	VREAD_H
#define	VREAD_H

#define	VREAD_H_MARK	"$Header: D:/U1/News/UnBatch/RCS/vread.h 1.4 1995/08/07 14:55:01 dz Exp $"


#include	<style.h>
#include	<stdio.h>
#include	<io.h>



class VReadBase
	{

protected:
	long			gpos;

public:
					VReadBase( void ) {}
	virtual			~VReadBase( void );

	virtual int		read( void *buf, int blen ) = 0;
	virtual bool	gets( uchar *buf, int blen ) = 0;
	virtual bool	eof() = 0;
	virtual	bool	ferr( void ) = 0;
	virtual long	seekg( long pos ) { return gpos = pos; }
	virtual long	tellg( void )     { return gpos; }
	virtual long	size( void ) = 0;

	};








class VReadFile : public VReadBase
	{
	int				xfd;
	bool			do_close;

public:
					VReadFile( void );
	virtual			~VReadFile( void );

	bool			open( int fd );
	bool			open( const char *name );
	bool			close( void );

	virtual int		read( void *buf, int blen );
	virtual long	seekg( long pos );
	virtual long	tellg( void );
	virtual bool	eof() { return ::eof( xfd ) ? Yes : No; }
	virtual long	size( void );

	};




class VReadBFile : public VReadBase
	{
	FILE *			xfp;
	bool			do_close;

public:
					VReadBFile( void );
	virtual			~VReadBFile( void );

	bool			open( FILE * fp );
	bool			open( const char *name );
	bool			close( void );

	virtual int		read( void *buf, int blen );
	virtual bool	gets( uchar *buf, int blen );
	virtual long	seekg( long pos );
	virtual long	tellg( void );
	virtual	bool	eof( void )	{ return feof( xfp ) ? Yes : No; }
	virtual	bool	ferr( void )	{ return ferror( xfp ) ? Yes : No; }
	virtual long	size( void );

	};






//Û 	Used to create virtual view into the
//Û 	file (or something alike, such as another view)

class VReadView : public VReadBase
	{
	VReadBase *	view;
	long			begin;
	long			len;

public:
					VReadView( void );
	virtual			~VReadView( void );

	virtual bool	open( VReadBase	*view, long begin, long len );
	virtual bool	close();

	virtual int		read( void *buf, int blen );
	virtual bool	gets( uchar *buf, int blen );
	virtual long	seekg( long pos );
	virtual long	tellg();
	virtual bool	eof();
	virtual	bool	ferr()  { return view->ferr(); }
	virtual long	size();

	};






//Û 	Used in VReadView as a default reader,
//Û 	which only returns errors...

class VReadError : public VReadBase
	{
public:
	virtual int		read( void *buf, int blen );
	virtual bool	gets( uchar *buf, int blen );
	virtual long	seekg( long pos );
	virtual long	tellg( void );
	virtual bool	eof() { return Yes; }
	virtual	bool	ferr( void ) { return Yes; }
	virtual long	size( void );

	static VReadError	Error;

	};


#if 0
/****************************************************************************
						istream interface
****************************************************************************/


class VReadStream :	public VReadBase
	{
	istream	&		is;

public:
					VReadStream( istream &i ) : is( i ) {}
	virtual			~VReadStream( void );

	virtual int		read( void *buf, int blen );
	virtual long	seekg( long pos );
	virtual long	tellg( void );
	virtual long	size( void );


	};
#endif


#endif //	VREAD_H
