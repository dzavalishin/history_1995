/************************ UU2 Library ***************************\
 *
 *	Copyright (C) 1991,1992 by Infinity Soft
 *
 *	Module 	:	Code table convertor header
 *
 *      $Log: Recode.h $
 *      Revision 1.3  1995/11/05 13:52:48  dz
 *      current.
 *
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:04:17  dz
 * Initial revision
 *
 *      
 *         Rev 1.1   07 Jun 1993 17:58:30   dz
 *      update
 *      
 *         Rev 1.0   04 Feb 1992 22:15:50   dz
 *      Initial revision.
 *
 *
 *
\*/


#ifndef	RECODE_H
#define	RECODE_H

#ifndef	STYLE_H
#	include	<style.h>
#endif

#pragma pack(1)

class _Export recoder
	{

private:
	char	*tab;
	bool	alloced;
	bool	bad;

public:
	recoder( const char *tabname );
	~recoder();

	bool	valid();						// This instnce was constructed

	uchar	rc( uchar );					// Recode character
	void	rs( uchar * );                 // Recode string
        void    recoder::rs( string &s );      // Recode string
	};

#pragma pack()

#endif

