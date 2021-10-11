head	1.5;
access;
symbols;
locks;
comment	@ * @;


1.5
date	95.08.07.13.57.10;	author dz;	state Exp;
branches;
next	1.4;

1.4
date	95.08.07.13.28.26;	author dz;	state Exp;
branches;
next	1.3;

1.3
date	95.08.02.13.35.50;	author dz;	state Exp;
branches;
next	1.2;

1.2
date	95.04.09.23.24.27;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.03.11.18.31.14;	author dz;	state Exp;
branches;
next	;


desc
@@


1.5
log
@Cleanup
@
text
@/****************************** UU2 Snews *********************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	.PKT read functions
 *
 *      $Log: read.c $
 *      Revision 1.4  1995/08/07 13:28:26  dz
 *      Using class App
 *
 *      Revision 1.3  1995/08/02 13:35:50  dz
 *      avoided ungetc where possible, added return code checks
 *      to ungetc's left.
 *
 *      This should be rewritten as packet class, surely
 *
 *      Revision 1.2  1995/04/09  23:24:27  dz
 *      IBM C Set version
 *
 *      Revision 1.1  1995/03/11  18:31:14  dz
 *      Initial revision
 *
 *      
 *         Rev 1.4   07 Jun 1993 17:13:16   dz
 *      Copyright
 *      
 *         Rev 1.3   18 Jun 1992 17:33:38   dz
 *      Typo: was '0' instead of '\0'
 *      
 *         Rev 1.2   18 Jun 1992 17:15:12   dz
 *      Bugfix from /lib/fido/msg_rw.c - previous code would
 *      fall into the infinite loop if a word longer then 80
 *      bytes occured.
 *      
 *         Rev 1.1   18 Jun 1992 11:12:28   dz
 *      Formatter written
 *      
 *         Rev 1.0   07 Apr 1992 20:52:48   dz
 *      Initial revision.
 *
 *
\*/

#include	"snews.h"
#include	"..\..\u1.h"


bool
SnewsApp::readl( char *buf, int len, FILE *fp )
	{
	int		c;

	while( len > 1 )
		{
		if( (c = getc( fp )) == EOF )
			return Err;

		if( c == '\0' )
			{
			*buf = '\0';
			return Ok;
			}

		*buf++ = c;
		len--;
		}

	*buf = '\0';
	return Err;
	}



/**************************************************************************
								Formatter
**************************************************************************/



static bool
readw( char *buf, int len, bool *cr, FILE *fp ) 
	{
	int		c;
	bool	first = Yes;
	bool	have_char = No;

	*cr = No;

	while( 1 ) 
		{							// Skip ws & lf
		if( (c = getc( fp )) == EOF )
			return Err;

		if( c == ' ' || c == '\t' || c == '\n' )
			continue;

		if( c == 0 )
			return Err;

//		if( ungetc( c, fp ) ) fatal( EC_Got_Insane, EI_Full, "Can't ungetc 1!");
		have_char = Yes;
		break;
		}


	while( len > 1 ) 
		{

		if( !have_char )
			{
			if( (c = getc( fp )) == EOF )
				break;
			}
		have_char = No;

		if( c == 0 )
			{
			if( !first )
				if( ungetc( 0, fp ) ) fatal( EC_Got_Insane, EI_Full, "Can't ungetc 2!");
			*cr = Yes;
			break;
			}

		first = No;

		if( ((uchar)c) == 0x0Du || ((uchar)c) == 0x8Du )
			*cr = Yes;

		if(	((uchar)c) <= ' ' ) 
			{
			*buf = '\0';
//			fflush( fp );
			return Ok;
			}

		*buf++ = c;
		len--;
		}

	*buf = '\0';
//	fflush( fp );
	return first ? Err : Ok;
	}


bool
SnewsApp::read_form( char *buf, FILE *fp ) 
	{
	int		c, len;
	bool	first = Yes;
	bool	got_eol = No;
	bool	have_char;
	long	pos;
	char	lbuf[100], *lbp, *bp;

	if( (pos = ftell(fp)) < 0 )
		return Err;


	c = getc( fp );
	have_char = Yes;
	if( c == EOF || c == 0 )
		return No;

//	if( ungetc( c, fp ) ) fatal( EC_Got_Insane, EI_Full, "Can't ungetc 3!");

	lbp = lbuf;
	len = 81;
	while( len > 1 ) 
		{
		if( !have_char )
			{
			if( (c = getc( fp )) == EOF )
				break;
			}
		have_char = No;

		if( c == '\n' )								// Eat LF - allways!
			continue;

		if( ((uchar)c) == 0x0Du || ((uchar)c) == 0x8Du ) 
			{
			got_eol = Yes;
			break;
			}

		if( c == 0 )
			{
			got_eol = Yes;
			if( ungetc( 0, fp ) ) fatal( EC_Got_Insane, EI_Full, "Can't ungetc 4!");
			break;
			}

		*lbp++ = c;
		len--;
		}

	*lbp = '\0';

	if( got_eol ) 
		{
		strcpy( buf, lbuf );
//		fflush( fp );
		return Yes;
		}


	// Line too long - read again and format


	fseek( fp, pos, SEEK_SET );
	first = Yes;
	*buf = '\0';
	bp = buf;

	while( 1 ) 
		{
		bool	cr, result;
		char	word[85], *wp;

		pos		= ftell( fp );
		result	= readw( word, 81, &cr, fp );

		if( result == Err )
			break;

		if( strlen( buf ) + strlen( word ) + 1 > 80 )
			{											// Too long!!
			if( first )										// But first word
				{
				word[80] = '\0';								// Get 80 characters
				cr = Yes;									// Say we done
				fseek( fp, pos+80, SEEK_SET );				// Skip
				}
			else
				{
				fseek( fp, pos, SEEK_SET );
				break;
				}
			}

		first = No;

		if( strlen( word ) ) 
			{
			for( wp = word; *wp; )
				*bp++ = *wp++;
			*bp++ = ' ';
			}
		*bp = '\0';

		if( cr )
			break;

		}


	return first ? No : Yes;
	}



@


1.4
log
@Using class App
@
text
@d8 3
d17 2
a18 2
 * Revision 1.2  1995/04/09  23:24:27  dz
 * IBM C Set version
d20 2
a21 2
 * Revision 1.1  1995/03/11  18:31:14  dz
 * Initial revision
a46 39
//#define		BUFS	500

/*

bool
reads( char *buf, int len, FILE *fp ) 
	{
	int		c;

	while( len > 1 )
		{
		if( (c = getc( fp )) == EOF )
			return Err;

		if( c == '\n' || ((uchar)c) == 0x8Du )	// Eat LF & soft CR - allways!
			continue;

		if( ((uchar)c) == 0x0Du )
			{
			*buf = '\0';
			return Yes;
			}

		if( c == '\0' )
			{
			*buf = '\0';
			return No;
			}

		*buf++ = c;
		len--;
		}

	*buf = '\0';
	return Yes;
	}


*/
@


1.3
log
@avoided ungetc where possible, added return code checks
to ungetc's left.

This should be rewritten as packet class, surely
@
text
@d8 6
d46 1
a46 1

d82 1
a82 1

d85 1
a85 1
readl( char *buf, int len, FILE *fp )
d183 1
a183 1
read_form( char *buf, FILE *fp ) 
@


1.2
log
@IBM C Set version
@
text
@d8 3
d115 1
d130 2
a131 1
		ungetc( c, fp );
d138 7
a144 2
		if( (c = getc( fp )) == EOF )
			break;
d149 1
a149 1
				ungetc( 0, fp );
d162 1
a162 1
			fflush( fp );
d171 1
a171 1
	fflush( fp );
d182 1
d191 1
d195 1
a195 1
	ungetc( c, fp );
d201 6
a206 2
		if( (c = getc( fp )) == EOF )
			break;
d220 1
a220 1
			ungetc( 0, fp );
d233 1
a233 1
		fflush( fp );
@


1.1
log
@Initial revision
@
text
@d7 4
a10 1
 *      $Log:   Q:/news/snews/vcs/read.c_v  $
a32 1
//#include	"..\..\version.h"
a33 1
//#include	"fido_pkt.h"
d40 2
a41 1
reads( char *buf, int len, FILE *fp ) {
d108 2
a109 1
readw( char *buf, int len, bool *cr, FILE *fp ) {
d115 2
a116 1
	while( 1 ) {							// Skip ws & lf
d131 2
a132 1
	while( len > 1 ) {
d149 2
a150 1
		if(	((uchar)c) <= ' ' ) {
d167 2
a168 1
read_form( char *buf, FILE *fp ) {
d187 2
a188 1
	while( len > 1 ) {
d195 2
a196 1
		if( ((uchar)c) == 0x0Du || ((uchar)c) == 0x8Du ) {
d214 2
a215 1
	if( got_eol ) {
d230 2
a231 1
	while( 1 ) {
d258 2
a259 1
		if( strlen( word ) ) {
@
