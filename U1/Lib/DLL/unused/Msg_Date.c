/************************ FIDO MSG file class ***************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:		Date/time parsing
 *
 *      $Log: Msg_Date.c $
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
 *         Rev 1.4   09 Apr 1993 10:10:20   dz
 *      Reorganized, get_date written
 *      
 *         Rev 1.3   06 Nov 1992 19:19:42   dz
 *      Ver 1.12a save
 *      
 *         Rev 1.2   04 Feb 1992 22:06:10   dz
 *      Using strncpy() to put date string into the
 *      message header to be sure nothing gets overwritten
 *      
 *         Rev 1.1   27 Nov 1991 15:22:30   dz
 *      Bug fixed: date field used to overwrite next one - changed
 *      %d to %u to prevent negative numbers, added test for invalid
 *      (negative) time.
 *      
 *         Rev 1.0   28 Aug 1991 00:59:58   dz
 *      Initial revision.
 *
 *
\*/

#include	<style.h>
#include	<uu2io.h>
#include	<string.h>
#include	<time.h>
#include	<ctype.h>

#include	"fido_msg.h"

#define	BLEN	300


static	char months[][4] =
	{
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
	};

void
fido_msg::set_date( long tmt )
	{
	char    buf[40];

	if( tmt <= 0 )
		tmt = time(NULL);

	tm		*t = localtime( &tmt );

	if( error )		return;

	sprintf( buf,  "%02u %3.3s %02u  %02u:%02u:%02u",
				   t->tm_mday, months[t->tm_mon], t->tm_year,
				   t->tm_hour, t->tm_min, t->tm_sec);

	strncpy( hdr.date, buf, 19 );
	hdr.date[19] = '\0';

	}



void
fido_msg::set_date( char *d )
	{
	sprintf( hdr.date,	"%.19s", d );
	}

#define	WOFC( a, b )	( (uint)a | (((uint)b)<<8) )


long
fido_msg::get_date()
	{
	if( error )		return time(0);

	tm		t;

	char	mon[4];

	sscanf( hdr.date, "%02u %3s %02u  %02u:%02u:%02u",
				   &t.tm_mday, mon, &t.tm_year,
				   &t.tm_hour, &t.tm_min, &t.tm_sec
				   );

	t.tm_isdst = 0;

	switch( WOFC( tolower(mon[0]), tolower(mon[1]) ) )
		{
	case WOFC('j','a'):	t.tm_mon = 1;		break;
	case WOFC('f','e'): t.tm_mon = 2;		break;
	case WOFC('m','a'):
		t.tm_mon = (mon[2] == 'r' || mon[2] == 'R' ? 3 : 5);
		break;
	case WOFC('a','p'): t.tm_mon = 4;		break;
	case WOFC('j','u'):
		t.tm_mon = 6;
		if (mon[2] == 'l' || mon[2] == 'L')
			t.tm_mon++;   /* July */
		break;
	case WOFC('a','u'): t.tm_mon = 8;		break;
	case WOFC('s','e'): t.tm_mon = 9;		break;
	case WOFC('o','c'): t.tm_mon = 10;		break;
	case WOFC('n','o'): t.tm_mon = 11;		break;
	case WOFC('d','e'): t.tm_mon = 12;		break;

		}

	return mktime( &t );
	}