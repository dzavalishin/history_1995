/********************** Date/time conversion library *************************\
 *
 *	Copyright (C) 1991 by Infinity Soft
 *
 *	Module 	:	Date/time conversion library header
 *
 *      $Log: DateC.h $
 *      Revision 1.3  1995/11/05 13:52:48  dz
 *      current.
 *
 * Revision 1.2  1995/04/13  11:06:28  dz
 * debug
 *
 * Revision 1.1  1995/03/11  18:07:42  dz
 * Initial revision
 *
 * Revision 1.1  1995/03/11  18:07:42  dz
 * Initial revision
 *
 *      
 *         Rev 1.0   26 Nov 1992 19:32:46   dz
 *      Initial revision.
 *
 *
\*/

#include	<style.h>
#include	<stdlib.h>
#include	<time.h>


/* interface to dateconv() */

time_t _Export	dateconv( struct tm *tm, int zone );

/*
 *  Meridian:  am, pm, or 24-hour style.
 */
#define AM 0
#define PM 1
#define HR24 2



/**
 *	Convert Internet-type date string to time_t
**/

time_t _Export		getindate( const char * line );


/**
 *	Convert time_t to Imternet-style string
**/

const char * _Export	timeline( time_t t, const uchar *tz );
