head	1.3;
access;
symbols;
locks;
comment	@ * @;


1.3
date	95.11.05.13.52.48;	author dz;	state Exp;
branches;
next	1.2;

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


1.3
log
@current.
@
text
@/************************ UU2 Library ***************************\
 *
 *	Copyright (C) 1991,1992 by Infinity Soft
 *
 *	Module 	:	Code table convertor header
 *
 *      $Log: Recode.h $
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

@


1.2
log
@debug
@
text
@d8 3
d53 2
a54 1
	void	rs( uchar * );					// Recode string
@


1.1
log
@Initial revision
@
text
@d7 7
a13 1
 *      $Log:   Q:/lib/uu2/vcs/recode.h_v  $
d35 1
a35 1
class recoder
@
