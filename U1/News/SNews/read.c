/****************************** UU2 Snews *********************************\
 *
 *  Copyright (C) 1991-1993 by Infinity Soft
 *
 *	Module 	:	.PKT read functions
 *
 *      $Log: read.c $
 *      Revision 1.5  1995/08/07 13:57:10  dz
 *      Cleanup
 *
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



