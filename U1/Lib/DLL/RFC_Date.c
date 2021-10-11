/********************** Date/time conversion library *************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	time_t to Internet-style lime line conversion.
 *
 *      $Log: RFC_Date.c $
 *      Revision 1.3  1995/11/05 13:52:48  dz
 *      current.
 *
 *      Revision 1.2  1995/04/13  11:06:28  dz
 *      debug
 *
 *      Revision 1.1  1995/03/11  18:07:42  dz
 *      Initial revision
 *
 *      Revision 1.1  1995/03/11  18:07:42  dz
 *      Initial revision
 *
 *         Rev 1.1   26 Nov 1992 19:32:24   dz
 *      const
 *      
 *         Rev 1.0   11 Oct 1992 04:30:50   dz
 *      Initial revision.
 *
 *
\*/

#include	"log.h"
#include	"datec.h"
#include	<stdio.h>

	/*************************************************************
                      Make time/date string withno '\n'
	*************************************************************/

static char *s_week[] =
    { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

static char *s_month[] =
    {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

const char * timeline( time_t t, const uchar *tz )
    {
    static char		tl[50];
    struct tm 		*tb;
    
    if( t == -1 )
        {
        ::error( EI_None, "timeline( %ld ), reverting to time()", t );
        t = time(NULL);
        }

    tb = localtime(&t);
    
    //*   [Weekday,] dd Mmm [19]yy hh:mm[:ss] Timezone
    
    sprintf( tl, "%s, %2d %.3s %d %02d:%02d:%02d %s",
            s_week[ (tb->tm_wday) % 7 ],
            tb->tm_mday,
            s_month[ (tb->tm_mon) % 12 ],
            tb->tm_year+1900,
            tb->tm_hour,
            tb->tm_min,
            tb->tm_sec,
            tz
           );
    
    return tl;
    }
