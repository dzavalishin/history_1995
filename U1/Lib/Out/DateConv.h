#include	<time.h>



/* interface to dateconv() */

time_t _Export	dateconv( struct tm *tm, int zone );



/*
 *  Meridian:  am, pm, or 24-hour style.
 */
#define AM 0
#define PM 1
#define HR24 2

