head	1.2;
access;
symbols;
locks;
comment	@ *      @;


1.2
date	95.11.06.02.33.43;	author dz;	state Exp;
branches;
next	1.1;

1.1
date	95.11.05.17.45.44;	author dz;	state Exp;
branches;
next	;


desc
@@


1.2
log
@Fixes & cleanups caused by FTN_Msg introduction
@
text
@/************************ FIDO MSG file class ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:		Text/kludge lines read/write
 *
 *      $Log: FTN_Rw.c $
 *      Revision 1.1  1995/11/05 17:45:44  dz
 *      Initial revision
 *
 *      Revision 1.1  1995/11/05 17:37:36  dz
 *      Initial revision
 *
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:00:54  dz
 * Initial revision
 *
 *      
 *         Rev 1.7   07 Jun 1993 17:38:06   dz
 *      io.h --> uu2io.h
 *      
 *         Rev 1.6   06 Nov 1992 19:20:06   dz
 *      Ver 1.12a save
 *      
 *         Rev 1.5   18 Jun 1992 17:30:42   dz
 *      Typo: was '0' instead of '\0'
 *      
 *         Rev 1.4   17 Jun 1992 20:50:42   dz
 *      Bug in formatter fixed: formatter wold fall into the
 *      infinite loop when longer then 80 characters word
 *      occured in the input .MSG
 *      
 *         Rev 1.3   04 Feb 1992 22:08:46   dz
 *      Added const modifier in appropriate places
 *      
 *         Rev 1.2   11 Oct 1991 05:07:10   dz
 *      Text formatting written
 *      
 *         Rev 1.1   12 Sep 1991 01:40:56   dz
 *      Reads() instead of fgets().
 *      
 *         Rev 1.0   28 Aug 1991 01:01:10   dz
 *      Initial revision.
 *
 *
\*/

#include	<style.h>
#include	<uu2io.h>
#include	<string.h>
#include	<time.h>

#include	"ftn_msg.h"

//#define	BLEN	300


/****************************************************************************
							Text management
****************************************************************************/



bool
FTN_Msg::rewind( void )
	{
	if( error )		return Err;
        
        cursor.setToFirst();
        
	return Ok;
	}


bool
FTN_Msg::clear_text( void )
	{
	if( error )		return Err;
        text.removeAll();
	return Ok;
	}

/*
static bool
reads( char *buf, int len, FILE *fp )
	{
	int		c;
	bool	first = Yes;

	while( len > 1 )
		{
		if( (c = getc( fp )) == EOF )
			break;

		if( c == '\n' )								// Eat LF - allways!
			continue;

		first = No;

		if( ((uchar)c) == 0x0Du || ((uchar)c) == 0x8Du )
			{
			*buf = '\0';
			return Ok;
			}

		*buf++ = c;
		len--;
		}

	*buf = '\0';
	return first ? Err : Ok;
	}


static bool
readw( char *buf, int len, bool *cr, FILE *fp )
	{
	int		c;
	bool	first = Yes;

	*cr = No;

	while( 1 )
		{									// Skip ws & lf
		if( (c = getc( fp )) == EOF )
			return Err;

		if( c == ' ' || c == '\t' || c == '\n' )
			continue;

		ungetc( c, fp );
		break;
		}


	while( len > 1 )
		{
		if( (c = getc( fp )) == EOF )
			break;

		first = No;

		if( ((uchar)c) == 0x0Du || ((uchar)c) == 0x8Du )
			*cr = Yes;

		if(	((uchar)c) <= ' ' )
			{
			*buf = '\0';
			fflush( fp );
			return Ok;
			}

		*buf++ = c;
		len--;
		}

	*buf = '\0';
	fflush( fp );
	return first ? Err : Ok;
	}


static bool
readl( char *buf, FILE *fp )
	{
	int		c, len;
	bool	first = Yes;
	bool	got_eol = No;
	long	pos;
	char	lbuf[100], *lbp, *bp;

	if( (pos = ftell(fp)) < 0 )
		return Err;


	lbp = lbuf;
	len = 81;
	while( len > 1 )
		{
		if( (c = getc( fp )) == EOF )
			break;

		if( c == '\n' )								// Eat LF - allways!
			continue;

		if( ((uchar)c) == 0x0Du || ((uchar)c) == 0x8Du )
			{
			got_eol = Yes;
			break;
			}

		*lbp++ = c;
		len--;
		}

	*lbp = '\0';

	if( got_eol )
		{
		strcpy( buf, lbuf );
		fflush( fp );
		return Ok;
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
			{												// Too long!!
			if( first )										// But first word
				{
				word[80] = '\0';							// Get 80 characters
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

		if( strlen( word ) ) {
			for( wp = word; *wp; )
				*bp++ = *wp++;
			*bp++ = ' ';
			}
		*bp = '\0';

		if( cr )
			break;

		}


	return first ? Err : Ok;
	}






static void
seek_end( FILE *fp )
	{
	if( (fseek( fp, 0L, SEEK_END ) != 0) ||
	  (ftell(fp) < sizeof(struct fido_msg_hdr)))

		fseek( fp, sizeof(struct fido_msg_hdr), SEEK_SET );
	}
*/

        /*************************************************************
                                    R/W msg
        *************************************************************/


bool
FTN_Msg::gets( string &s )
    {
    if( error )  return Err;

    do
        {
        if( !cursor.isValid() )
            return Err;
        s = cursor.element();
        cursor.setToNext();
        } while( s[0] == '\1' ); // Клуджи по боку

    return Ok;
    }



bool
FTN_Msg::geta( string &s )
    {
    if( error )  return Err;

    do
        {
        if( !cursor.isValid() )
            return Err;
        s = cursor.element();
        cursor.setToNext();
        } while( s[0] != '\1' ); // Hе клуджи по боку
    
    s = s.substr( 1 );           // Выкинем ^A
    
    return Ok;
    }

bool
FTN_Msg::puts( const string &s )
    {
    text.add( s );
    rewrite = Yes;
    return Ok;
    }


bool
FTN_Msg::puta( const string &s )
    {
    text.add( string("\1") + s );       // Добавим ^A
    rewrite = Yes;
    return Ok;
    }


/*
bool
fido_msg::getl( char *s )
	{
	char	buf[100];

	if( error )		return Err;


	do	{
		if( readl( buf, fp ) == Err )
			return Err;

		} while( buf[0] == 1 );

	strncpy( s, buf, 80 );
	s[80] = '\0';
	fflush( fp );

	return Ok;
	}
*/
        
        
/*
bool
fido_msg::fputs( const char *s )
    {
    if( error )
        return Err;

//	rewrite	= Yes;

//	while( *s == 1 )                   // It can't be ^A!!!
//			s++;

//	seek_end( fp );

    if( fputs( s + string("\r\n"), fp ) == EOF )
        return Err;

    return Ok;
    }
*/

         /*************************************************************
                        R/W msg attribute strings
	 *************************************************************/

/*
bool
fido_msg::geta( char *s )
	{
	char	buf[BLEN];
	char	*cp;

	if( error )		return Err;

	do	{
		if( reads( buf, BLEN, fp ) == Err )
			return Err;

		} while( buf[0] != 1 );

	strncpy( s, buf+1, BLEN );

	if( (cp = strpbrk( s, "\r\n\x8D" )) != NULL )
		*cp = '\0';

	return Ok;
	}
*/

/*
bool
fido_msg::fputa( const char *s )
	{
	if( error )		return Err;

//	seek_end( fp );     Slo-o-ow!

	if( *s != 1 ) 					// It must be ^A!!!
		{
		if( fputs( "\1", fp ) == EOF )
				return Err;
		}

	if( fputs( s, fp ) == EOF )
			return Err;

	if( fputs( "\r\n", fp ) == EOF )
			return Err;

	return Ok;
	}

*/



@


1.1
log
@Initial revision
@
text
@d8 3
a72 3
//    for( tc.setToFirst(); tc.isValid(); tc.setToNext() )
//        if( !is_known_kludge( tc.element() ) )
//           fprintf( fp, "%s\r\n", (const char *)(tc.element()) );
d323 1
d332 1
@
